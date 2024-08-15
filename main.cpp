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
#include <QIcon>

#define WINDOWSCALE 2

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QMainWindow mainWindow;
    mainWindow.setWindowIcon(QIcon(":./logo.png"));
    quickfit *fitnesswidget = new quickfit(&mainWindow);
    mainWindow.setCentralWidget(fitnesswidget);

    // Set window properties based on screen resolution
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenSize = screen->geometry();
    int height = screenSize.height();
    int width = screenSize.width();

    mainWindow.setWindowTitle("QuickFit");
    mainWindow.resize(width / WINDOWSCALE, height / WINDOWSCALE);
    mainWindow.show();

    return app.exec();
}
