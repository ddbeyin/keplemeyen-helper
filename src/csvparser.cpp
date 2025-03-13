// Copyright (c) 2025 ddbeyin
// MIT License - See LICENSE file for details

#include "csvparser.h"
#include <QRegularExpression>
#include <QStringConverter>
#include <QDebug>

CsvParser::CsvParser(QObject *parent)
    : QObject(parent)
    , m_errorMessage("")
{
}

bool CsvParser::parseTimestamps(const QString &filePath,
                                const QString &eventTimeColumn,
                                const QString &processTimeColumn,
                                QList<QDateTime> &eventTimes,
                                QList<QDateTime> &processTimes,
                                QChar delimiter)
{
    // Clear any previous error message
    m_errorMessage.clear();
    
    // Clear output lists
    eventTimes.clear();
    processTimes.clear();
    
    // Open the file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_errorMessage = "Could not open the file.";
        return false;
    }
    
    // Set up text stream with UTF-8 encoding
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    
    // Read header line
    if (in.atEnd()) {
        m_errorMessage = "File is empty.";
        file.close();
        return false;
    }
    
    QString headerLine = in.readLine();
    QStringList headers = parseLine(headerLine, delimiter);
    
    // Find column indices
    int eventTimeIndex = -1;
    int processTimeIndex = -1;
    
    for (int i = 0; i < headers.size(); i++) {
        QString header = headers[i].trimmed();
        if (header.compare(eventTimeColumn, Qt::CaseInsensitive) == 0) {
            eventTimeIndex = i;
        } else if (header.compare(processTimeColumn, Qt::CaseInsensitive) == 0) {
            processTimeIndex = i;
        }
    }
    
    // Validate column indices
    if (eventTimeIndex == -1 || processTimeIndex == -1) {
        m_errorMessage = QString("Required columns '%1' and '%2' not found. Found columns: %3")
                             .arg(eventTimeColumn, processTimeColumn, headers.join(", "));
        file.close();
        return false;
    }
    
    // Process data rows
    int lineNumber = 1; // Header was line 1
    while (!in.atEnd()) {
        lineNumber++;
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) {
            continue;
        }
        
        QStringList fields = parseLine(line, delimiter);
        
        // Check if we have enough fields
        if (fields.size() <= qMax(eventTimeIndex, processTimeIndex)) {
            qDebug() << "Line" << lineNumber << "has insufficient fields:" << fields.size() 
                     << "fields, need index" << qMax(eventTimeIndex, processTimeIndex);
            continue;
        }
        
        // Parse timestamps
        QString eventTimeStr = fields[eventTimeIndex].trimmed();
        QString processTimeStr = fields[processTimeIndex].trimmed();
        
        QDateTime eventTime = parseDateTime(eventTimeStr);
        QDateTime processTime = parseDateTime(processTimeStr);
        
        if (eventTime.isValid() && processTime.isValid()) {
            eventTimes.append(eventTime);
            processTimes.append(processTime);
        } else {
            // Debug invalid timestamps
            if (!eventTime.isValid()) {
                qDebug() << "Invalid event time format at line" << lineNumber << ":" << eventTimeStr;
            }
            if (!processTime.isValid()) {
                qDebug() << "Invalid process time format at line" << lineNumber << ":" << processTimeStr;
            }
        }
    }
    
    file.close();
    
    // Check if we parsed any valid data
    if (eventTimes.isEmpty()) {
        m_errorMessage = "No valid data rows found in the file.";
        return false;
    }
    
    return true;
}

QStringList CsvParser::parseLine(const QString &line, QChar delimiter)
{
    QStringList fields;
    bool inQuote = false;
    QString field;
    
    for (int i = 0; i < line.length(); i++) {
        QChar c = line[i];
        
        // Handle quotes
        if (c == '"') {
            if (i < line.length() - 1 && line[i+1] == '"') {
                // Double quotes - add a single quote to the field
                field.append('"');
                i++; // Skip the next quote
            } else {
                // Toggle quote state
                inQuote = !inQuote;
            }
        }
        // Handle delimiters
        else if (c == delimiter && !inQuote) {
            fields.append(field);
            field.clear();
        }
        // Add character to the current field
        else {
            field.append(c);
        }
    }
    
    // Add the last field
    fields.append(field);
    
    // Remove quotes from fields if present
    for (int i = 0; i < fields.size(); i++) {
        QString &f = fields[i];
        f = f.trimmed();
        if (f.startsWith('"') && f.endsWith('"') && f.length() >= 2) {
            f = f.mid(1, f.length() - 2);
        }
    }
    
    return fields;
}

QDateTime CsvParser::parseDateTime(const QString &dateTimeStr)
{
    // Try different date-time formats
    QList<QString> formats = {
        "yyyy-MM-dd HH:mm:ss",
        "yyyy-MM-dd HH:mm:ss.zzz", // With milliseconds
        "yyyy/MM/dd HH:mm:ss",
        "dd-MM-yyyy HH:mm:ss",
        "dd/MM/yyyy HH:mm:ss",
        "MM/dd/yyyy HH:mm:ss",
        "yyyy-MM-ddTHH:mm:ss",     // ISO format
        "yyyy-MM-dd"               // Date only
    };
    
    foreach (const QString &format, formats) {
        QDateTime dt = QDateTime::fromString(dateTimeStr, format);
        if (dt.isValid()) {
            return dt;
        }
    }
    
    // As a last resort, try Qt::ISODate format
    QDateTime dt = QDateTime::fromString(dateTimeStr, Qt::ISODate);
    if (dt.isValid()) {
        return dt;
    }
    
    // Return invalid datetime if all parsing attempts failed
    return QDateTime();
}

QString CsvParser::errorMessage() const
{
    return m_errorMessage;
}