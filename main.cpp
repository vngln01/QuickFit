#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QLabel>
#include <QCalendarWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDate>
#include <QMessageBox>
#include "quickfit.h"

#define WINDOWSCALE 2

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QMainWindow mainWindow;

    quickfit *fitnesswidget = new quickfit(&mainWindow);
    mainWindow.setCentralWidget(fitnesswidget);

    // Set window properties
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenSize = screen->geometry();
    int height = screenSize.height();
    int width = screenSize.width();

    mainWindow.setWindowTitle("QuickFit");
    mainWindow.resize(width / WINDOWSCALE, height / WINDOWSCALE);
    mainWindow.show();

    return app.exec();
}
