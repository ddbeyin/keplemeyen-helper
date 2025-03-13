// Copyright (c) 2025 ddbeyin
// MIT License - See LICENSE file for details

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

// Main application window with support for file analysis
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor
    MainWindow(QWidget *parent = nullptr);

    // Destructor
    ~MainWindow();

protected:
    // Handle file drag events
    void dragEnterEvent(QDragEnterEvent *event) override;

    // Process dropped files
    void dropEvent(QDropEvent *event) override;

private slots:
    // Toggle window always-on-top state
    void on_actionAlways_on_Top_triggered(bool checked);

    // Open file dialog for file selection
    void onLoadButtonClicked();

    // Process a loaded or dropped file
    void handleDroppedFile(const QString &filePath);

    // Handle module selection change
    void onModuleSelected(const QString &text);

    // Run time discrepancy analysis
    void analyzeTimeDiscrepancy();

    // Perform analysis with selected module
    void onAnalyzeButtonClicked();

    // Reset application state
    void onResetButtonClicked();

    // Display application information
    void showAboutDialog();

    // Display version information
    void showVersionDialog();

private:
    Ui::MainWindow *ui;                 // UI components
    QString currentFilePath;            // Current file path
    QString lastDirectory;              // Last used directory
    CsvParser *m_csvParser;             // CSV parser

    // Parse timestamps from CSV file
    bool readCSVFile(const QString &filePath,
                     QList<QDateTime> &eventTimes,
                     QList<QDateTime> &processTimes);
};

#endif // MAINWINDOW_H