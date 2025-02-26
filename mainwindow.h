#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include "version.h"
#include "csvparser.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief The MainWindow class provides the primary user interface for the Keplemeyen Helper application
 *
 * This class handles the main application window and implements core functionality including:
 * - File loading and drag-drop support for CSV and Excel files
 * - Analysis module selection and execution
 * - Result display and management
 * - Window state management
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window
     * @param parent The parent widget (default: nullptr)
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroys the main window and frees allocated resources
     */
    ~MainWindow();

protected:
    /**
     * @brief Handles drag enter events for file dropping
     * @param event The drag enter event to handle
     *
     * Validates incoming drag events to ensure they contain supported file types
     * (CSV, XLS, XLSX)
     */
    void dragEnterEvent(QDragEnterEvent *event) override;

    /**
     * @brief Handles drop events for file loading
     * @param event The drop event to handle
     *
     * Processes dropped files and initiates their analysis if they are valid
     */
    void dropEvent(QDropEvent *event) override;

private slots:
    /**
     * @brief Toggles the always-on-top window state
     * @param checked The new state of the always-on-top setting
     */
    void on_actionAlways_on_Top_triggered(bool checked);

    /**
     * @brief Handles the load button click event
     *
     * Opens a file dialog for selecting input files
     */
    void onLoadButtonClicked();

    /**
     * @brief Processes a file that has been loaded or dropped
     * @param filePath The path to the file to process
     */
    void handleDroppedFile(const QString &filePath);

    /**
     * @brief Handles selection changes in the module combo box
     * @param text The text of the newly selected module
     */
    void onModuleSelected(const QString &text);

    /**
     * @brief Performs time discrepancy analysis on the loaded file
     *
     * Analyzes timestamps to detect potential time manipulation
     */
    void analyzeTimeDiscrepancy();

    /**
     * @brief Handles the analyze button click event
     *
     * Initiates analysis using the currently selected module
     */
    void onAnalyzeButtonClicked();

    /**
     * @brief Handles the reset button click event
     *
     * Clears all fields and resets the application state
     */
    void onResetButtonClicked();

    /**
     * @brief Shows the about dialog
     *
     * Displays information about the application, developer, and contact details
     */
    void showAboutDialog();

    /**
     * @brief Shows the version dialog
     *
     * Displays the current version of the application
     */
    void showVersionDialog();

private:
    Ui::MainWindow *ui;                 ///< Pointer to the UI form
    QString currentFilePath;            ///< Path to the currently loaded file
    QString lastDirectory;              ///< Last directory used for file operations
    CsvParser *m_csvParser;             ///< CSV parser for handling file data

    /**
     * @brief Reads and parses a CSV file containing timestamp data
     * @param filePath Path to the CSV file
     * @param eventTimes List to store parsed event timestamps
     * @param processTimes List to store parsed process timestamps
     * @return true if file was successfully parsed, false otherwise
     */
    bool readCSVFile(const QString &filePath,
                     QList<QDateTime> &eventTimes,
                     QList<QDateTime> &processTimes);
};

#endif // MAINWINDOW_H