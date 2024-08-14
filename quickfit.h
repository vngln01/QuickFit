#ifndef QUICKFIT_H
#define QUICKFIT_H

#include <QMainWindow>
#include <QObject>
#include <QQuickItem>
#include <QSharedDataPointer>
#include <QWidget>
#include <QDate>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QtSql/QSqlDatabase>
#include <QCalendarWidget>
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QFormLayout>
#include <QTableView>
#include <QtSql/QSqlTableModel>

class quickfit : public QWidget {
    Q_OBJECT

public:
    quickfit(QWidget *parent = nullptr);
    ~quickfit();

private slots:
    void onSelectedDate();
    void logActivity(const QDate &date, const QString &activity, const QString &sets, const QString &reps);
    void viewData(const QDate &date);

private:
    QCalendarWidget *calwidget;
    QLineEdit *activityInput;
    QLineEdit *setsInput;
    QLineEdit *repsInput;
    void initDatabase();
};

#endif // QUICKFIT_H
