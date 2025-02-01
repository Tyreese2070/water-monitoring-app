// COMP2811 Coursework 2: application entry point

#include <QtWidgets>
#include "window.hpp"
#include <QLocale>

int checkFirstTimeUse() {
    QSettings settings("YourOrganization6", "YourAppName");
    bool isFirstTime = settings.value("firstUse", true).toBool();

    if (isFirstTime) {
      settings.setValue("firstUse", false);
      return 1;
    }
  return 0;
}

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  
  int new_user = checkFirstTimeUse();
  QString style = R"(
  QWidget {
      background-color: #fefef2; 
      font-family: Arial, sans-serif;
      font-size: 12pt;
  }

  QTabWidget::pane {
      border: 1px solid #B0C4DE; 
      background: #F0F8FF; 
  }
  QTabBar::tab {
      background: #4682B4; 
      color: #FFFFFF; 
      padding: 5px 15px;
      border-radius: 5px;
      margin: 2px;
  }
  QTabBar::tab:selected {
      background: #2a4e6c; 
      font-weight: bold;
      color: #FFFFFF;
  }

  QPushButton {
      background-color: #4682B4; 
      border: 1px solid #5F9EA0; 
      padding: 8px 16px;
      color: #FFFFFF;
      font-weight: bold;
  }
  QPushButton:hover {
      background-color: #6495ED; 
  }
  QPushButton:pressed {
      background-color: #4682B4;
  }


  QLabel {
      font-size: 14pt; 
      color: #2C3E50;
      padding: 5px;
  }


  QComboBox {
      background-color: #e5e5da; 
      color: #000000;
      border: 1px solid #B0C4DE; 
      padding: 5px;
  }

  QMenu::item {
    color: #000000; /* Dark text color */
    padding: 5px 20px; /* Padding for better spacing */
  }

  QMenu::item::hover {
    background-color: blue; /* Change the background color of the selected item */
  }




  QScrollArea {
      border: none;
  }
  QScrollBar:vertical {
      border: none;
      background: #F5F5F5; 
      width: 10px;
  }
  QScrollBar::handle:vertical {
      background: #4682B4; 
      border-radius: 5px;
  }


  QTableView {
      gridline-color: #B0C4DE;
      background-color: #FFFFFF; 
      border: 1px solid #4682B4; 
  }
  QHeaderView::section {
      background-color: #2a4e6c; 
      color: #FFFFFF; 
      padding: 4px;
      font-weight: bold;
      border: 1px solid #B0C4DE;
  }

  QChartView {
      border: 1px solid #B0C4DE; 
      background: #F0F8FF; 
  }

  )";
  app.setStyleSheet(style  
  );



    QLocale userLocale = QLocale::system();
    QString language = userLocale.name();  

    QTranslator trn;

    if (language.startsWith("de") && trn.load("pollutanttool_en_de.qm", ":/i18n")) {
        app.installTranslator(&trn);
    } 

  PollutantWindow window = PollutantWindow(new_user);
  window.showMaximized();
  

  return app.exec();
}
