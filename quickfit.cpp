#include "quickfit.h"

quickfit::quickfit(QWidget *parent) : QWidget(parent) {
    //Create necessary calendar objects and default date to today
    calwidget = new QCalendarWidget(this);
    calwidget->setSelectedDate(QDate::currentDate());
    //Create all appropriate labels/inputs for features and logging pushbutton
    QLabel *activityLabel = new QLabel("Activity:", this);
    QLabel *setsLabel = new QLabel("Sets:", this);
    QLabel *repsLabel = new QLabel("Reps:", this);
    QLabel *weightsLabel = new QLabel("Weight:", this);
    QPushButton *logbutton = new QPushButton("Log Activity", this);
    activityInput = new QLineEdit;
    setsInput = new QLineEdit;
    repsInput = new QLineEdit;
    weightsInput = new QLineEdit;

    //Containers to hold calendar widget
    QGroupBox *groupboxcal = new QGroupBox("", this);
    QVBoxLayout *calcontainer = new QVBoxLayout;
    calcontainer->addWidget(calwidget);
    groupboxcal->setLayout(calcontainer);

    activityLabel->setFixedSize(60, 25);
    weightsLabel->setFixedSize(60, 30);
    setsLabel->setFixedSize(60, 30);
    repsLabel->setFixedSize(60, 30);
    activityInput->setFixedSize(200, 20);
    setsInput->setFixedSize(60, 20);
    repsInput->setFixedSize(60, 20);
    weightsInput->setFixedSize(60, 20);

    //Form layout allows a LABEL: INPUT style format
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(activityLabel, activityInput);
    formLayout->addRow(weightsLabel, weightsInput);
    formLayout->addRow(setsLabel, setsInput);
    formLayout->addRow(repsLabel, repsInput);

    //Form layout is added to the container followed by the log button to maintain proper order
    QVBoxLayout *dataLayout = new QVBoxLayout;
    dataLayout->addLayout(formLayout);
    dataLayout->addWidget(logbutton);
    //Stretch allows the appropriate widget to stretch to container size
    dataLayout->setStretch(dataLayout->count() - 1, 1);
    dataLayout->addWidget(workoutdata);
    dataLayout->setStretch(dataLayout->count() - 1, 1);

    //Merged form and button layout are added to a container to allow them to be displayed on the main window
    QGroupBox *formContainer = new QGroupBox;
    formContainer->setLayout(dataLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(groupboxcal);
    mainLayout->addWidget(formContainer);
    setLayout(mainLayout);

    //Binds widget and a signal ("sender") to a slot  (function which does the required action)
    connect(calwidget, &QCalendarWidget::selectionChanged, this, &quickfit::onSelectedDate);
    //[=]() creates a copy of variable in current scope for lambda function to access
    connect(logbutton, &QPushButton::clicked, [=]() {
        logActivity(calwidget->selectedDate(), activityInput->text(), setsInput->text(), repsInput->text(), weightsInput->text());
    });

    initDatabase();
    //Sets initial date
    onSelectedDate();
}

quickfit::~quickfit() {

}

void quickfit::viewData(const QDate &date) {

}

void quickfit::onSelectedDate() {
    QDate selectedDate = calwidget->selectedDate();
//  QMessageBox::information(this, "Date Selected", "Selected Date: " + selectedDate.toString());

    //https://doc.qt.io/qt-6/qsqltablemodel.html - allow access to database contents for manipulation
    QSqlTableModel *model = new QSqlTableModel(this, QSqlDatabase::database());
    model->setTable("fitness_log");
    /*"QString("Date = '%1'").arg" was provided by ChatGPT - I'm unsure why my "selectedDate.toString(Qt::ISODate)" did not
    allow me to parse database contents*/
    model->setFilter(QString("Date = '%1'").arg(selectedDate.toString(Qt::ISODate)));
    model->select();

    //Updates a "QTableView" object which allows viewing of data with the data of selected date
    workoutdata->setModel(model);
    //Hide ID numbers and hide date column
    workoutdata->hideColumn(0);
    workoutdata->hideColumn(1);
    //Resize header to fit the formContainer box
    workoutdata->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //Hides vertical header with entry id's
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
    onSelectedDate();
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
