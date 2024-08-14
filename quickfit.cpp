#include "quickfit.h"

quickfit::quickfit(QWidget *parent) : QWidget(parent) {
    QGroupBox *groupboxcal = new QGroupBox("", this);
    calwidget = new QCalendarWidget(this);
    connect(calwidget, &QCalendarWidget::selectionChanged, this, &quickfit::onSelectedDate);

    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(calwidget);
    groupboxcal->setLayout(vbox1);

    QLabel *activityLabel = new QLabel("Activity:", this);
    activityLabel->setFixedSize(60, 25);
    activityInput = new QLineEdit;
    activityInput->setFixedSize(200, 20);

    QLabel *setsLabel = new QLabel("Sets:", this);
    setsLabel->setFixedSize(60, 30);
    setsInput = new QLineEdit;
    setsInput->setFixedSize(60, 20);

    QLabel *repsLabel = new QLabel("Reps:", this);
    repsLabel->setFixedSize(60, 30);
    repsInput = new QLineEdit;
    repsInput->setFixedSize(60, 20);

    QPushButton *logbutton = new QPushButton("Log Activity", this);

    connect(logbutton, &QPushButton::clicked, [=]() {
        logActivity(calwidget->selectedDate(), activityInput->text(), setsInput->text(), repsInput->text());
    });

    QFormLayout *formLayout = new QFormLayout;
    QGroupBox *formContainer = new QGroupBox;
    formLayout->setFormAlignment(Qt::AlignHCenter);
    formLayout->addRow(activityLabel, activityInput);
    formLayout->addRow(setsLabel, setsInput);
    formLayout->addRow(repsLabel, repsInput);
    formLayout->addWidget(logbutton);
    formContainer->setLayout(formLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(groupboxcal);
    mainLayout->addWidget(formContainer);
    setLayout(mainLayout);

    initDatabase();
}

quickfit::~quickfit() {

}

void quickfit::viewData(const QDate &date) {

}

void quickfit::onSelectedDate() {
    QDate selectedDate = calwidget->selectedDate();
    QMessageBox::information(this, "Date Selected", "Selected Date: " + selectedDate.toString());

    QSqlTableModel *model = new QSqlTableModel(this, QSqlDatabase::database());
    model->setTable("fitness_log");
    model->setFilter(QString("Date = '%1'").arg(selectedDate.toString(Qt::ISODate)));
    model->select();

    QTableView *workoutdata = new QTableView;
    workoutdata->setModel(model);
    workoutdata->hideColumn(0);
    workoutdata->show();
}

void quickfit::logActivity(const QDate &date, const QString &activity, const QString &sets, const QString &reps) {
    if (activity.isEmpty() || sets.isEmpty() || reps.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter all data.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO fitness_log (date, activity, sets, reps) VALUES (?, ?, ?, ?)");
    query.addBindValue(date.toString(Qt::ISODate));
    query.addBindValue(activity);
    query.addBindValue(sets.toInt());
    query.addBindValue(reps.toInt());

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Activity logged successfully.");
    } else {
        QMessageBox::critical(this, "Database Error", "Failed to log activity: " + query.lastError().text());
    }
}

void quickfit::initDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("fitness.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to open database.");
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS fitness_log ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "Date TEXT, "
               "Activity TEXT, "
               "Sets INTEGER, "
               "Reps INTEGER)");
}
