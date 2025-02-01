// COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include <QtCharts>
#include <QSettings>
#include <QTimer>
#include "window.hpp"
#include "pages.cpp"


static const int MIN_WIDTH = 1200;

PollutantWindow::PollutantWindow(int firstTimeUser) : QMainWindow(), statsDialog(nullptr)
{
  createButtons();
  createToolBar();
  createNavbar();
  createFooter();
  setMinimumWidth(MIN_WIDTH);
  setWindowTitle("Water Quality Monitor");
    if (firstTimeUser == 1) {
        QTimer::singleShot(0, this, &PollutantWindow::userGuide); // Delayed call to userGuide()
    }
}



// Modify the code in this function to change the widget in the main body
void PollutantWindow::createMainWidget()
{
  tableWidget(); //change this to dashboardWidget to get dashboard working
}

void PollutantWindow::createButtons()
{
  //loadButton = new QPushButton(tr("Load"));

  //connect(loadButton, SIGNAL(clicked()), this, SLOT(openCSV()));
}

void PollutantWindow::createFooter()
{
  // About menu
  QPushButton *aboutButton = new QPushButton(tr("About"));
  statusBar()->addWidget(aboutButton);

  QMenu *aboutMenu = new QMenu(this);

  QAction *aboutAction = new QAction("&About", this);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  QAction *aboutQtAction = new QAction("About &Qt", this);
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  aboutMenu->addAction(aboutAction);
  aboutMenu->addAction(aboutQtAction);

  aboutButton->setMenu(aboutMenu);

  // User guide
  QPushButton *userGuideButton = new QPushButton(tr("User Guide"));
  statusBar()->addWidget(userGuideButton);
  connect(userGuideButton, &QPushButton::clicked, this, &PollutantWindow::userGuide);

  // References menu
  QPushButton *referencesButton = new QPushButton(tr("References"));
  statusBar()->addWidget(referencesButton);
  connect(referencesButton, &QPushButton::clicked, this, &PollutantWindow::references);

  progressBar = new QProgressBar(this);
  progressBar->setRange(0, 100);
  progressBar->setValue(0);
  progressBar->hide();

}

void PollutantWindow::createNavbar()
{
  QTabWidget* pages = new QTabWidget(this);
  pages->addTab(dashboardWidget(), "Dashboard");
  pages->addTab(overviewWidget(), "Common Pollutants");
  pages->addTab(POPsWidget(), "Persistent Organic Pollutants");
  pages->addTab(litterWidget(), "Environmental Litter");
  pages->addTab(fluorinationWidget(), "Fluorinated Compounds");
  pages->addTab(complianceWidget(), "Compliance");
  pages->addTab(tableWidget(), "Table");
  setCentralWidget(pages);
}

// edited by noah - assign variables to each input so can be accessed later
// Need to change the time filter so it has a start + end
void PollutantWindow::createToolBar()
{
  QToolBar *toolBar = new QToolBar();
  QLabel *filtersLabel = new QLabel(tr("Filters: "), this);

  // Create QPushButton for setting the data location
  QPushButton *setDataLocationButton = new QPushButton("Import Dataset");
  setDataLocationButton->setToolTip("Set the location of the data file");
  toolBar->addWidget(setDataLocationButton);  // Add the button to the status bar

  // Connect the button's click signal to the respective slots
  connect(setDataLocationButton, &QPushButton::clicked, this, &PollutantWindow::setDataFile);
  connect(setDataLocationButton, &QPushButton::clicked, this, &PollutantWindow::openCSV);

  // Assign filter widgets to member variables
  dateFilterStart = new QDateEdit(this);
  dateFilterStart->setToolTip(tr("Enter date in YYYY-MM-DD format"));
  dateFilterStart->setDisplayFormat("yyyy-MM-dd");
  dateFilterStart->setCalendarPopup(true);
  dateFilterStart->setDate(QDate::currentDate());
  connect(dateFilterStart, &QDateEdit::dateChanged, this, &PollutantWindow::openCSV);


  // Assign filter widgets to member variables
  dateFilterEnd = new QDateEdit(this);
  dateFilterEnd->setToolTip(tr("Enter date in YYYY-MM-DD format"));
  dateFilterEnd->setDisplayFormat("yyyy-MM-dd");
  dateFilterEnd->setCalendarPopup(true);
  dateFilterEnd->setDate(QDate(2000,01,01));
  connect(dateFilterEnd, &QDateEdit::dateChanged, this, &PollutantWindow::openCSV);


  locationFilter = new QComboBox(this);
  connect(locationFilter, &QComboBox::currentIndexChanged, this, &PollutantWindow::openCSV);

  

  complianceFilter = new QLineEdit(this);
  complianceFilter->setPlaceholderText(tr("Compliance"));
  connect(complianceFilter, &QLineEdit::textChanged, this, &PollutantWindow::openCSV);

  levelFilter = new QLineEdit(this);
  levelFilter->setPlaceholderText(tr("Level"));
  connect(levelFilter, &QLineEdit::textChanged, this, &PollutantWindow::openCSV);

  // add new time filtering buttons
  lastWeekButton = new QPushButton(tr("Last Week"));
  connect(lastWeekButton, &QPushButton::clicked, this, [this]() {
    dateFilterEnd->setDate(QDate::currentDate().addDays(-7));
  });

  lastMonthButton = new QPushButton(tr("Last Month"));
  connect(lastMonthButton, &QPushButton::clicked, this, [this]() {
    dateFilterEnd->setDate(QDate::currentDate().addMonths(-1));
  });

  lastSixMonthsButton = new QPushButton(tr("Last 6 Months"));
  connect(lastSixMonthsButton, &QPushButton::clicked, this, [this]() {
    dateFilterEnd->setDate(QDate::currentDate().addMonths(-6));
  });

  lastYearButton = new QPushButton(tr("Last Year"));
  connect(lastYearButton, &QPushButton::clicked, this, [this]() {
    dateFilterEnd->setDate(QDate::currentDate().addYears(-1));
  });

  allTimeButton = new QPushButton(tr("All Time"));
  connect(allTimeButton, &QPushButton::clicked, this, [this]() {
    dateFilterEnd->setDate(QDate(2000,01,01));
  });

  //toolBar->addWidget(loadButton);
  toolBar->addWidget(filtersLabel);
  toolBar->addWidget(dateFilterStart);
  toolBar->addWidget(dateFilterEnd);
  toolBar->addWidget(locationFilter);
  toolBar->addWidget(lastWeekButton);
  toolBar->addWidget(lastMonthButton);
  toolBar->addWidget(lastSixMonthsButton);
  toolBar->addWidget(lastYearButton);
  toolBar->addWidget(allTimeButton);

  toolBar->setMovable(false);

  addToolBar(Qt::LeftToolBarArea, toolBar);
}

void PollutantWindow::createStatusBar()
{

    

}



void PollutantWindow::setDataFile()
{
    QString file = QFileDialog::getOpenFileName(
        this, "Data File", ".",
        "CSV Files (*.csv)");

    if (!file.isEmpty())
    {

      model.setIsDataLoaded(false);
      
        dataLocation = file;
        QString loc = dataLocation;

        // Extract the base name of the file (e.g., "Y-2024.csv" -> "Y-2024")
        QFileInfo fileInfo(loc);
        QString baseName = fileInfo.baseName(); // Removes path and extension

        // Initialize variables for letters and year
        std::string letters = "";
        QString year = "";

        // split the filename into letters and year
        QRegularExpression regex("^([A-Za-z]*)(?:[-_])?([0-9]{4})(-M|-C)?$");
        QRegularExpressionMatch match = regex.match(baseName);

        if (match.hasMatch())
        {
            letters = match.captured(1).toStdString(); // Captures the letters part
            year = match.captured(2);   // Captures the year part
        }
        std::string location;
        if (letters == ""){
          location = "United Kingdom";
        } else if (letters == "CB"){
          location = "Cambridgeshire and Berkshire";
        } else if (letters == "Y"){
          location = "Yorkshire";
        } else if (letters == "CL"){
          location = "Cumbria and Lancashire";
        } else if (letters == "DNL"){
          location = "Derbyshire, Nottinghamshire, and Leicestershire";
        } else if (letters == "DC"){
          location = "Devon and Cornwall";
        } else if (letters == "ENS"){
          location = "Essex, Norfolk, and Sussex";
        } else if (letters == "GMMC"){
          location = "Greater Manchester, Merseyside, and Cheshire";
        } else if (letters == "HNL"){
          location = "Hertfordshire and North London";
        } else if (letters == "KSL"){
          location = "Kent and South London";
        } else if (letters == "LN"){
          location = "Lincolnshire and Northamptonshire";
        } else if (letters == "NDT"){
          location = "Northumberland, Durham, and Tees";
        } else if (letters == "SHWG"){
          location = "Shropshire, Herefordshire, Worcestershire, and Gloucestershire";
        } else if (letters == "SSD"){
          location = "Solent and South Downs";
        } else if (letters == "SWWM"){
          location = "Staffordshire, Warwickshire, and West Midlands";
        } else if (letters == "W"){
          location = "Wessex";
        } else if (letters == "WT"){
          location = "West Thames";
        } else {
          location = "Unknown";
        }
        QString qText = QString("Dataset Location: %1").arg(QString::fromStdString(location));
        locationLabel->setText(qText);
        QString qText2 = QString("Year: %1").arg(year);
        yearLabel->setText(qText2);
    }
    
}


void PollutantWindow::openCSV()
{
  if (dataLocation == "")
  {
    QMessageBox::critical(this, "No dataset loaded",
                          "Data file has not been set!\n\n"
                          "You can select a file by selecting 'Import Dataset'.");
    return;
  }

  auto path = dataLocation;
  progressBar->setValue(0);

  
  // edit by noah - retrieve and apply the filters to the model
  QDate selctedDateStart = dateFilterStart->date();
  QDate selectedDateEnd = dateFilterEnd->date();
  QString location = locationFilter->currentText();
  QString compliance = complianceFilter->text().trimmed();
  QString level = levelFilter->text().trimmed();

  // put into PollutantFilter struct to be sent to loadCSV
  PollutantFilter filter;
  filter.location = location.toStdString();
  filter.compliance = compliance.toStdString();
  filter.level = level.toStdString();
  filter.dateStart = selctedDateStart.toString("yyyy-MM-dd").toStdString();
  filter.dateEnd = selectedDateEnd.toString("yyyy-MM-dd").toStdString();
  try
  {
    model.updateFromFile(path, filter, progressBar);
    emit csvLoadComplete();
  }
  catch (const std::exception &error)
  {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }
  // need to clear all the inputs from 
  locationFilter->blockSignals(true);
  std::set<std::string> uniqueLocations = model.getUniqueLocations();
  locationFilter->clear();
  if (filter.location != ""){
    locationFilter->addItem(QString::fromStdString(filter.location));
  } else {
    locationFilter->addItem(QString::fromStdString("ALL LOCATIONS"));
  }
  if (filter.location != "ALL LOCATIONS"){
    locationFilter->addItem(QString::fromStdString("ALL LOCATIONS"));
  }
  
  for (const auto& location : uniqueLocations) {
    if (location == filter.location) continue;
      locationFilter->addItem(QString::fromStdString(location));  // Add each location as a new item
  }
  locationFilter->blockSignals(false);

  QFileInfo filename(path);
  table->resizeColumnsToContents();

  // update the graphs when data is loaded
  // Update the chart in the overview widget
  if (commonChartView)
  {
      QString tempText = "Select Pollutant";
      QChart* updatedChart = model.createCommonBarChart(tempText);
      commonChartView->setChart(updatedChart);
  }
  if (commonPieChartView)
  {
      QChart* updatedChart = model.createPieChartCommon();
      commonPieChartView->setChart(updatedChart);
  }
  // update the graph in POPs
  if (POPsChartView)
  {
      QChart* updatedChart = model.createChartPOPS(0);
      POPsChartView->setChart(updatedChart);
  }
  // update the pie chart in POPs
  if (POPsPieChartView)
  {
      QChart* updatedChart = model.createPieChartPOPS(0);
      POPsPieChartView->setChart(updatedChart);
  }
  // update the pie chart in fluorination
  if (fluorinationPieChartView)
  {
      QChart* updatedChart = model.createPieChartFluorination();
      fluorinationPieChartView->setChart(updatedChart);
  }
  if (fluorinationChartView){
    QChart* updatedChart = model.createChartPOPS(2);
    fluorinationChartView->setChart(updatedChart);
  }

  if (litterPieChartView){
    QChart* updatedChart = model.createLitterPieChart();
    litterPieChartView->setChart(updatedChart);
  }

  if (litterChartView){
    QChart* updatedChart = model.createLitterBarChart(litterDropdown->currentText());
    litterChartView->setChart(updatedChart);
  }

  if (allPollutantsChartView){
    QChart* updatedChart = model.createPieChartPOPS(1);
    allPollutantsChartView->setChart(updatedChart);
  }
  progressBar->setValue(100);

  // update number of records in the dashboard
  int rows = model.rowCount(QModelIndex());
  QString qText = QString("Number of records: %1").arg(rows);  
  recordsLabel->setText(qText);

  // update the sub location in the dashboard
  if (filter.location == ""){
    filter.location = "ALL";
  }
  QString qText2 = QString("Sub Location: %1").arg(QString::fromStdString(filter.location));
  subLocationLabel->setText(qText2);

  // update the bottom row of the dashboard
  DashboardInfo commonInfo = model.getDashboardInfo(1);
  QString qText3 = QString("<b>%1</b> measurements").arg(commonInfo.count);
  commonCount->setText(qText3);
  QString qText4 = QString("<b>%1%</b> over legal limit").arg(QString::number(commonInfo.value, 'f', 1));
  commonPercent->setText(qText4);

  DashboardInfo popsInfo = model.getDashboardInfo(0);
  QString qText5 = QString("<b>%1</b> measurements").arg(popsInfo.count);
  popsCount->setText(qText5);
  QString qText6 = QString("<b>%1%</b> over legal limit").arg(QString::number(popsInfo.value, 'f', 1));
  popsPercent->setText(qText6);

  DashboardInfo pfaInfo = model.getDashboardInfo(2);
  QString qText7 = QString("<b>%1</b> measurements").arg(pfaInfo.count);
  pfaCount->setText(qText7);
  QString qText8 = QString("<b>%1%</b> over legal limit").arg(QString::number(pfaInfo.value, 'f', 1));
  pfaPercent->setText(qText8);

  LitterInfo litterInfo = model.getLitterInfo();
  QString qText9 = QString("<b>%1 sites</b> with sewage debris").arg(litterInfo.sewage);
  sewage->setText(qText9);
  QString qText10 = QString("<b>%1 sites</b> with litter").arg(litterInfo.litter);
  litter->setText(qText10);

  updateCompliancePage();
}

// update the compliance page
void PollutantWindow::updateCompliancePage(){
  if (pollutantDropdown){
    std::set<Pollutant> uniquePollutants = model.getUniquePollutants();
    for (const Pollutant& pollutant : uniquePollutants) {
      pollutantDropdown->addItem(QString::fromStdString(pollutant.getPollutant()));
    }
  }}

void PollutantWindow::about()
{
  QMessageBox::about(this, "About Pollutant Tool",
                     "This Application was created for COMP2811 Final Coursework.\n"
                     "Models data collected by the UK Government Environment Agency.");
}

void PollutantWindow::references()
{
    QDialog *references = new QDialog(this);
    references->setWindowTitle("References");

    QLabel *text = new QLabel("<center><h1>References:</h1></center><br>"
                              "<a href=\"https://echa.europa.eu\">https://echa.europa.eu</a><br>"
                              "<a href=\"https://environment.data.gov.uk\">https://environment.data.gov.uk</a><br>"
                              "<a href=\"https://theriverstrust.org\">https://theriverstrust.org</a>");

    QObject::connect(text, &QLabel::linkActivated, [](const QUrl &url){
      QDesktopServices::openUrl(url);
    });

    QVBoxLayout *layout = new QVBoxLayout(references);
    layout->addWidget(text);

    references->adjustSize();

    references->exec();
}

void PollutantWindow::loadingMessage()
{
  // Create and show the loading dialog
  loadingDialog = new QDialog(this);
  loadingDialog->setWindowTitle("Loading");
  //loadingDialog->setWindowModality(Qt::ApplicationModal);
  loadingDialog->setFixedSize(300, 100);

  QVBoxLayout *layout = new QVBoxLayout(loadingDialog);
  QLabel *loadingLabel = new QLabel("Data is loading, please wait...", loadingDialog);
  loadingLabel->setAlignment(Qt::AlignCenter);
  layout->addWidget(loadingLabel);

  loadingDialog->show();

  QTimer::singleShot(0, this, [this]() {
    openCSV();
    loadingDialog->close();
  });
}




void PollutantWindow::userGuide()
{
  UserGuide* dialog = new UserGuide(this);
  dialog->exec();
}
