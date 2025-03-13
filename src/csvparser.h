// Copyright (c) 2025 ddbeyin
// MIT License - See LICENSE file for details

#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QHash>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

// CSV parser with support for quoted fields and various date formats
class CsvParser : public QObject
{
    Q_OBJECT

public:
    // Constructor
    explicit CsvParser(QObject *parent = nullptr);

    // Parse timestamps from specified columns in a CSV file
    bool parseTimestamps(const QString &filePath, 
                         const QString &eventTimeColumn,
                         const QString &processTimeColumn,
                         QList<QDateTime> &eventTimes,
                         QList<QDateTime> &processTimes,
                         QChar delimiter = ',');
                         
    // Get the last error message
    QString errorMessage() const;

private:
    // Parse a CSV line respecting quotes
    QStringList parseLine(const QString &line, QChar delimiter);
    
    // Try to parse a datetime string using various formats
    QDateTime parseDateTime(const QString &dateTimeStr);
    
    QString m_errorMessage;  // Last error message
};

#endif // CSVPARSER_H