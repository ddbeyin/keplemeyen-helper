// Keplemeyen Helper
// A comprehensive toolkit for game support agents
// More features to be added

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    // Initialize Qt application
    MainWindow w;                  // Create main window
    w.show();                     // Display window
    return a.exec();              // Start event loop
}
