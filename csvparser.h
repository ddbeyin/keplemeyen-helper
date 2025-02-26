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

/**
 * @brief The CsvParser class provides robust CSV file parsing capabilities
 *
 * This class handles CSV files with support for:
 * - Fields containing commas inside quotes
 * - Different line endings (CR, LF, CRLF)
 * - UTF-8 encoding
 * - Header detection and mapping
 */
class CsvParser : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a CSV parser
     * @param parent The parent QObject
     */
    explicit CsvParser(QObject *parent = nullptr);

    /**
     * @brief Parses a CSV file and extracts datetime values from specified columns
     * @param filePath Path to the CSV file
     * @param eventTimeColumn Name of the column containing event times
     * @param processTimeColumn Name of the column containing process times
     * @param eventTimes List to store parsed event times
     * @param processTimes List to store parsed process times
     * @param delimiter Character used as field delimiter (default: ,)
     * @return true if parsing was successful, false otherwise
     */
    bool parseTimestamps(const QString &filePath, 
                         const QString &eventTimeColumn,
                         const QString &processTimeColumn,
                         QList<QDateTime> &eventTimes,
                         QList<QDateTime> &processTimes,
                         QChar delimiter = ',');
                         
    /**
     * @brief Gets the error message if parsing failed
     * @return The error message
     */
    QString errorMessage() const;

private:
    /**
     * @brief Parses a single CSV line respecting quoted fields
     * @param line The CSV line to parse
     * @param delimiter Character used as field delimiter
     * @return List of fields from the parsed line
     */
    QStringList parseLine(const QString &line, QChar delimiter);
    
    /**
     * @brief Attempts to parse a datetime string in various formats
     * @param dateTimeStr The datetime string to parse
     * @return A valid QDateTime if parsing succeeded, invalid otherwise
     */
    QDateTime parseDateTime(const QString &dateTimeStr);
    
    QString m_errorMessage;  ///< Stores the most recent error message
};

#endif // CSVPARSER_H