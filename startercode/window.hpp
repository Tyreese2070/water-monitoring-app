// COMP2811 Coursework 2 sample solution: main window

#pragma once

#include <QMainWindow>
#include <QThread>
#include <QDesktopServices>
#include <QUrl>
#include "tablemodel.hpp"
#include "userguide.hpp"

class QString;
class QDate;
class QComboBox;
class QLabel;
class QPushButton;
class QTableView;
class StatsDialog;
class QDateEdit;
class QLineEdit;

class PollutantWindow : public QMainWindow
{
  Q_OBJECT

public:
  PollutantWindow(int firstTimeUser);
  

private:
  void createMainWidget();
  void createButtons();
  void createToolBar();
  void createStatusBar();
  void addFileMenu();
  void createNavbar();
  void createFilterMenu();
  void createFooter();
  void updateCompliancePage();
  void filterPollutantsByPollutant(const QString& selectedPollutant);
  void filterPollutantsByCategory(int category);
  QProgressBar* progressBar;
  QWidget* tableWidget();
  QWidget* dashboardWidget();
  QWidget* overviewWidget();
  QWidget* POPsWidget();
  QWidget* litterWidget();
  QWidget* fluorinationWidget();
  QWidget* complianceWidget();
  QWidget* hotspotsWidget();
  QListWidget* nonCompliantList;
  QListWidget* cautiousList;
  QListWidget* compliantList;
  QDialog* loadingDialog;

  PollutantModel model;         // data model used by table
  QString dataLocation;         // location of CSV data files
  QPushButton *loadButton;      // button to load a new CSV file
  QTableView *table;            // table of pollutant data
  QLabel *fileInfo;             // status bar info on current file
  StatsDialog *statsDialog;     // dialog to display stats
  QPushButton *dashboardButton; // returns the user to the dashboard

  // new filter widgets
  QDateEdit *dateFilterStart;
  QDateEdit *dateFilterEnd;
  QComboBox*locationFilter;
  QLineEdit *complianceFilter;
  QLineEdit *levelFilter;
  
  QComboBox* litterDropdown;          // Dropdown for litter type selection
  QChartView* litterChartView;        // Bar chart view for litter
  QChartView* litterPieChartView;     // Pie chart view for litter


  // graph which can be updated - this is the graph in the overview tab
  QChartView* overviewChartView;
  // graph in the POPs tab
  QChartView* POPsChartView;
  // pie chart in the POPs tab
  QChartView* POPsPieChartView;
  // pie chart in fluorination tab
  QChartView* fluorinationPieChartView;
  // Graph in fluorination tab
  QChartView* fluorinationChartView;
  // graph in dashboard for all compliance
  QChartView* allPollutantsChartView;
  // graph in dashboard for common pollutants
  QChartView* commonChartView;
  QChartView* commonPieChartView;
  QComboBox* dropDown;
  QComboBox* pollutantDropdown;

  // text box labels for dashboard
  QLabel* locationLabel;
  QLabel* yearLabel;
  QLabel* recordsLabel;
  QLabel* subLocationLabel;

  // for dashboard bttom row
  QLabel* commonCount;
  QLabel* commonPercent;
  QLabel* popsCount;
  QLabel* popsPercent;
  QLabel* pfaCount;
  QLabel* pfaPercent;
  QLabel* sewage;
  QLabel* litter;
  
  // for starter guide
  int firstTimeUser;

  // for the filter buttons
  QPushButton* lastWeekButton;
  QPushButton* lastMonthButton;
  QPushButton* lastSixMonthsButton;
  QPushButton* lastYearButton;
  QPushButton* allTimeButton;
  
signals:
  void csvLoadComplete();  

private slots:
  void setDataFile();
  void openCSV();
  void about();
  void references();
  void userGuide();
  void loadingMessage();
};