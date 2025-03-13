#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QMainWindow>
#include <QDateTime>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lastDirectory("")
    , m_csvParser(new CsvParser(this))
{
    ui->setupUi(this);

    // Drag & drop for easy file loading
    ui->frame->setAcceptDrops(true);
    this->setAcceptDrops(true);

    // Initialize available analysis modules
    ui->moduleComboBox->addItem("Time Discrepancy");

    // Connect window controls
    connect(ui->actionAlways_on_Top, &QAction::triggered, this, &MainWindow::on_actionAlways_on_Top_triggered);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(ui->actionVersion, &QAction::triggered, this, &MainWindow::showVersionDialog);

    // Connect file operations
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onLoadButtonClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);
    connect(ui->actionReset, &QAction::triggered, this, &MainWindow::onResetButtonClicked);

    // Connect analysis controls
    connect(ui->moduleComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onModuleSelected);
    connect(ui->analyzeButton, &QPushButton::clicked, this, &MainWindow::onAnalyzeButtonClicked);

    // Start with analysis controls disabled until file is loaded
    ui->analyzeButton->setEnabled(false);
    ui->moduleComboBox->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urls = event->mimeData()->urls();
        // Only allow single file drops
        if (urls.count() == 1) {
            QString filePath = urls.first().toLocalFile();
            QFileInfo fileInfo(filePath);
            QString extension = fileInfo.suffix().toLower();

            // Accept these file formats
            if (extension == "csv" || extension == "xlsx" || extension == "xls") {
                event->acceptProposedAction();
                // Highlight drop zone
                ui->frame->setStyleSheet("QFrame { border: 2px dashed #000000; }");
                return;
            }
        }
    }
    event->ignore();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    // Reset drop zone appearance
    ui->frame->setStyleSheet("");

    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urls = mimeData->urls();
        if (urls.count() == 1) {
            handleDroppedFile(urls.first().toLocalFile());
            event->acceptProposedAction();
        }
    }
}

void MainWindow::handleDroppedFile(const QString &filePath)
{
    if (!filePath.isEmpty()) {
        // Update file tracking
        lastDirectory = QFileInfo(filePath).path();
        currentFilePath = filePath;

        // Update UI state
        QFileInfo fileInfo(filePath);
        ui->loadButton->setText("Loaded: " + fileInfo.fileName());
        ui->analyzeButton->setEnabled(true);
        ui->moduleComboBox->setEnabled(true);

        // Clear any previous analysis results
        ui->resultsTextBox->clear();
        ui->warningsTextBox->clear();
        ui->repliesTextBox->clear();
    }
}

void MainWindow::on_actionAlways_on_Top_triggered(bool checked)
{
    // Toggle window's always-on-top state
    setWindowFlags(checked ?
                       windowFlags() | Qt::WindowStaysOnTopHint :
                       windowFlags() & ~Qt::WindowStaysOnTopHint);
    show();
}

void MainWindow::onLoadButtonClicked()
{
    // Configure file dialog to show supported formats
    QString filters = "Supported Files (*.csv *.xlsx *.xls);;CSV Files (*.csv);;Excel Files (*.xlsx *.xls);;All Files (*)";
    QString startingDir = lastDirectory.isEmpty() ? QDir::homePath() : lastDirectory;

    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        startingDir,
        filters
        );

    if (!filePath.isEmpty()) {
        handleDroppedFile(filePath);
    }
}

void MainWindow::onModuleSelected(const QString &text)
{
    // Run analysis when module is selected (can be expanded for other modules)
    if (text == "Time Discrepancy") {
        analyzeTimeDiscrepancy();
    }
}

bool MainWindow::readCSVFile(const QString &filePath, QList<QDateTime> &eventTimes, QList<QDateTime> &processTimes)
{
    bool success = m_csvParser->parseTimestamps(
        filePath, 
        "event_time",     // Name of event time column
        "process_time",   // Name of process time column
        eventTimes, 
        processTimes
    );
    
    if (!success) {
        QMessageBox::warning(this, "Error", m_csvParser->errorMessage());
    }
    
    return success;
}

void MainWindow::analyzeTimeDiscrepancy()
{
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please load a file first.");
        return;
    }

    QList<QDateTime> eventTimes, processTimes;
    if (!readCSVFile(currentFilePath, eventTimes, processTimes)) {
        return;
    }

    // Look for cases where event time is ahead of the process time
    // This could indicate time manipulation
    QString results;
    bool discrepanciesFound = false;

    for (int i = 0; i < eventTimes.size(); i++) {
        if (eventTimes[i] > processTimes[i]) {
            discrepanciesFound = true;
            results += QString("The player's event_time is ahead of the process_time on %1. The player made life hack.\n")
                           .arg(eventTimes[i].toString("yyyy-MM-dd HH:mm:ss"));
        }
    }

    if (!discrepanciesFound) {
        results = "No time discrepancies found.";
    }

    ui->resultsTextBox->setPlainText(results);
}

void MainWindow::onAnalyzeButtonClicked()
{
    // Run appropriate analysis based on selected module
    QString selectedModule = ui->moduleComboBox->currentText();
    if (selectedModule == "Time Discrepancy") {
        analyzeTimeDiscrepancy();
    }
}

void MainWindow::onResetButtonClicked()
{
    // Clear file selection
    currentFilePath.clear();
    ui->loadButton->setText("Load");

    // Reset all result displays
    ui->resultsTextBox->clear();
    ui->warningsTextBox->clear();
    ui->repliesTextBox->clear();

    // Disable analysis controls
    ui->analyzeButton->setEnabled(false);
    ui->moduleComboBox->setEnabled(false);

    // Reset module selection
    if (ui->moduleComboBox->count() > 0) {
        ui->moduleComboBox->setCurrentIndex(0);
    }
}

void MainWindow::showAboutDialog()
{
    QMessageBox about(this);
    about.setWindowTitle("About Keplemeyen Helper");
    about.setTextFormat(Qt::RichText);

    QFont messageFont("MS Sans Serif", 10);
    about.setFont(messageFont);

    about.setText(
        "<div style='font-family: \"MS Sans Serif\"; font-size: 10pt;'>"
        "<b>Keplemeyen Helper</b>"
        "<p>A comprehensive toolkit for game support agents.</p>"
        "<p><b>Version:</b> " APP_VERSION_STR "</p>"
        "<p><b>Developer:</b> ddbeyin</p>"
        "<p><b>Contact:</b> ddbeyin@gmail.com</p>"
        "<p><b>GitHub:</b> <a href='https://github.com/ddbeyin/keplemeyen-helper'>github.com/ddbeyin/keplemeyen-helper</a></p>"
        "<p>Copyright &copy; 2025 ddbeyin</p>"
        "<p><b>License:</b> MIT License</p>"
        "<p>This software is provided 'as is', without warranty of any kind.</p>"
        "<p><b>Third-Party Licenses:</b></p>"
        "<p>Qt 6 - <a href='https://www.gnu.org/licenses/lgpl-3.0.en.html'>LGPL v3</a></p>"
        "</div>"
        );
    about.setIcon(QMessageBox::Information);
    
    // Add detail button to show full license
    about.setStandardButtons(QMessageBox::Ok);
    QPushButton* licenseButton = about.addButton("View License", QMessageBox::ActionRole);
    
    about.exec();
    
    if (about.clickedButton() == licenseButton) {
        // Read the license file contents
        QFile licenseFile(":/LICENSE");
        QString licenseText;
        
        if (licenseFile.exists() && licenseFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            licenseText = licenseFile.readAll();
            licenseFile.close();
        } else {
            // Fallback if file can't be opened
            licenseText = "MIT License\n\n"
                "Copyright (c) 2025 ddbeyin\n\n"
                "Permission is hereby granted, free of charge, to any person obtaining a copy "
                "of this software and associated documentation files (the \"Software\"), to deal "
                "in the Software without restriction, including without limitation the rights "
                "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell "
                "copies of the Software, and to permit persons to whom the Software is "
                "furnished to do so, subject to the following conditions:\n\n"
                "The above copyright notice and this permission notice shall be included in all "
                "copies or substantial portions of the Software.\n\n"
                "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
                "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
                "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE "
                "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
                "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
                "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE "
                "SOFTWARE.";
        }
        
        // License dialog
        QDialog licenseDialog(this);
        licenseDialog.setWindowTitle("License Information");
        licenseDialog.setMinimumSize(500, 400);
        
        QVBoxLayout *layout = new QVBoxLayout(&licenseDialog);
        
        QTextBrowser *textBrowser = new QTextBrowser(&licenseDialog);
        textBrowser->setFont(messageFont);
        textBrowser->setPlainText(licenseText);
        textBrowser->setLineWrapMode(QTextBrowser::WidgetWidth);
        
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, &licenseDialog);
        connect(buttonBox, &QDialogButtonBox::accepted, &licenseDialog, &QDialog::accept);
        
        layout->addWidget(textBrowser);
        layout->addWidget(buttonBox);
        
        licenseDialog.exec();
    }
}

void MainWindow::showVersionDialog()
{
    QMessageBox version(this);
    version.setWindowTitle("Version Information");

    QFont messageFont("MS Sans Serif", 10);
    version.setFont(messageFont);

    version.setText(
        QString("<div style='font-family: \"MS Sans Serif\"; font-size: 10pt;'>"
                "Keplemeyen Helper Version %1</div>")
            .arg(APP_VERSION_STR)
        );
    version.setTextFormat(Qt::RichText);
    version.setIcon(QMessageBox::Information);
    version.exec();
}
