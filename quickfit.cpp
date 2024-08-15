#include "quickfit.h"

quickfit::quickfit(QWidget *parent) : QWidget(parent) {
    QGroupBox *groupboxcal = new QGroupBox("", this);
    calwidget = new QCalendarWidget(this);
    calwidget->setSelectedDate(QDate::currentDate());
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

    QLabel *weightsLabel = new QLabel("Weight:", this);
    weightsLabel->setFixedSize(60, 30);
    weightsInput = new QLineEdit;
    weightsInput->setFixedSize(60, 20);

    QPushButton *logbutton = new QPushButton("Log Activity", this);

    connect(calwidget, &QCalendarWidget::selectionChanged, this, &quickfit::onSelectedDate);
    connect(logbutton, &QPushButton::clicked, [=]() {
        logActivity(calwidget->selectedDate(), activityInput->text(), setsInput->text(), repsInput->text(), weightsInput->text());
    });

    QFormLayout *formLayout = new QFormLayout;
    QVBoxLayout *dataLayout = new QVBoxLayout;
    QGroupBox *formContainer = new QGroupBox;
   // formLayout->setFormAlignment(Qt::AlignHCenter);
    formLayout->addRow(activityLabel, activityInput);
    formLayout->addRow(weightsLabel, weightsInput);
    formLayout->addRow(setsLabel, setsInput);
    formLayout->addRow(repsLabel, repsInput);
    dataLayout->addLayout(formLayout);
    dataLayout->addWidget(logbutton);
    dataLayout->setStretch(dataLayout->count() - 1, 1);
    dataLayout->addWidget(workoutdata);
    dataLayout->setStretch(dataLayout->count() - 1, 1);
    formContainer->setLayout(dataLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(groupboxcal);
    mainLayout->addWidget(formContainer);
    setLayout(mainLayout);

    initDatabase();
    onSelectedDate();
}

quickfit::~quickfit() {

}

void quickfit::viewData(const QDate &date) {

}

void quickfit::onSelectedDate() {
    QDate selectedDate = calwidget->selectedDate();
  //  QMessageBox::information(this, "Date Selected", "Selected Date: " + selectedDate.toString());

    QSqlTableModel *model = new QSqlTableModel(this, QSqlDatabase::database());
    model->setTable("fitness_log");
    model->setFilter(QString("Date = '%1'").arg(selectedDate.toString(Qt::ISODate)));
    model->select();

    workoutdata->setModel(model);
    //Hide ID numbers and hide date column
    workoutdata->hideColumn(0);
    workoutdata->hideColumn(1);
    //workoutdata->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //Resize header to fit the formContainer box
    workoutdata->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //hides vertical header with entry id's
    workoutdata->verticalHeader()->setVisible(false);
}

void quickfit::logActivity(const QDate &date, const QString &activity, const QString &sets, const QString &reps, const QString &weights) {
    if (activity.isEmpty() || sets.isEmpty() || reps.isEmpty() || weights.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter all data.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO fitness_log (date, activity, weight, sets, reps) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(date.toString(Qt::ISODate));
    query.addBindValue(activity);
    query.addBindValue(weights.toInt());
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
               "Weight INTEGER, "
               "Sets INTEGER, "
               "Reps INTEGER)");
}
