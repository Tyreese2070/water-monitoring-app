#include "userguide.hpp"

UserGuide::UserGuide(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("User Guide");

    QVBoxLayout* layout = new QVBoxLayout(this);

    guideLabel = new QLabel(tr("Insert Text"));
    guideLabel->setWordWrap(true);  
    guideLabel->setOpenExternalLinks(true);

    layout->addWidget(guideLabel);

    closeButton = new QPushButton(tr("Close"));
    backButton = new QPushButton(tr("Back"));
    nextButton = new QPushButton(tr("Next"));

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(nextButton);

    layout->addLayout(buttonLayout);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
    connect(backButton, &QPushButton::clicked, this, &UserGuide::back);
    connect(nextButton, &QPushButton::clicked, this, &UserGuide::next);

    backButton->hide();

    updateText();
    updateTitle();
}

void UserGuide::close()
{
    this->accept();
}

void UserGuide::next()
{
    index++;
    updateText();
    updateTitle();
    updateButton();
}

void UserGuide::back()
{
    index--;
    updateText();
    updateTitle();
    updateButton();
}

void UserGuide::updateText()
{
    switch(index){
        case 0:
            guideLabel->setText(tr("<center><h1>Getting Started</h1></center>"
                                "<center><b>You can return to this tutorial at any time by selecting User Guide at the bottom of the application.</b></center>"
                                "<br>"
                                "Thank you for downloading the Water Quality Monitoring tool.<br>"
                                "<br>"
                                "&nbsp;&nbsp;1. Download the dataset you would like to explore from <a href=\"https://environment.data.gov.uk/water-quality/view/download\">the Environment Agency</a>.<br>"
                                "<br>"
                                "&nbsp;&nbsp;2. Select 'Import Dataset' at the top left of the app and navigate to the dataset you have downloaded.<br>"
                                "<br>"
                                "The dataset will now be loaded across every page of the application.<br>"
                                "<br>"
                                "To specify the <b>time period</b> or the <b>location</b> you would like to see in the dataset, use the filters on the left of the application.<br>"
                                "<br>"
                                "Click between pages at the top of the application or press Ctrl+Tab to automatically switch between pages.<br>"
                                "<center><b>Select Next to continue to the next page.</b></center>"
                                "<center><b>Select Close to end the tutorial.</b></center>"));
            
            QObject::connect(guideLabel, &QLabel::linkActivated, [](const QUrl &url){
                QDesktopServices::openUrl(url);
            });
            this->adjustSize();
            break;
        case 1:
            guideLabel->setText(tr("<center><h1>Dashboard</h1></center><br>"
                                "The dashboard displays a summary of the dataset which you have imported.<br>"
                                "<br>"
                                "The pie chart shows the proportion of measurements which are non-compliant, dangerously high or safely compliant.<br>"
                                "<br>"
                                "Information about the dataset is shown to the left of the pie chart.<br>"
                                "<br>"
                                "A breakdown of the dataset by pollutant group is shown beneath the pie chart.<br>"
                                "<br>"
                                "<center><b>Select Back to return to the previous page.</b></center>"
                                "<center><b>Select Next to continue to the next page.</b></center>"));
            this->adjustSize();
            break;
        case 2:
            guideLabel->setText(tr("<center><h1>Common Pollutants</h1></center><br>"
                                "This page provides information and analytics about the most common water pollutants.<br>"
                                "<br>"
                                "&nbsp;&nbsp;1. Select a pollutant from the dropdown tool to the left of the bar chart.<br>"
                                "<br>"
                                "The bar chart shows the average measurement of the pollutant by different types of water body.<br>"
                                "<br>"
                                "The pie chart shows the proportion of measurements which are non-compliant, dangerously high or safely compliant.<br>"
                                "<br>"
                                "<center><b>Select Back to return to the previous page.</b></center>"
                                "<center><b>Select Next to continue to the next page.</b></center>"));
            this->adjustSize();
            break;
        case 3:
            guideLabel->setText(tr("<center><h1>POPs</h1></center><br>"
                                "You can learn more about Persistent Organic Pollutants (POPs) using the buttons above the pie chart.<br>"
                                "<br>"
                                "<b>It is recommended that you select a location using the filters on the left of the application to display a more useful visualisation on the graph.</b><br>"
                                "<br>"
                                "The time-series chart shows how measurements of POPs have changed over time.<br>"
                                "<br>"
                                "The pie chart shows the proportion of measurements which are non-compliant, dangerously high or safely compliant.<br>"
                                "<br>"
                                "<center><b>Select Back to return to the previous page.</b></center>"
                                "<center><b>Select Next to continue to the next page.</b></center>"));
            this->adjustSize();
            break;
        case 4:
            guideLabel->setText(tr("<center><h1>Environmental Litter</h1></center><br>"
                                "This page provides information and analytics about environmental litter in water.<br>"
                                "<br>"
                                "&nbsp;&nbsp;1. Select a pollutant from the dropdown tool to the left of the bar chart.<br>"
                                "<br>"
                                "The bar chart shows the average measurement of litter by different types of water body.<br>"
                                "<br>"
                                "The pie chart shows the proportion of measurements which are non-compliant, dangerously high or safely compliant.<br>"
                                "<br>"
                                "<center><b>Select Back to return to the previous page.</b></center>"
                                "<center><b>Select Next to continue to the next page.</b></center>"));
            break;
        case 5:
            guideLabel->setText(tr("<center><h1>Fluorinated Compounds</h1></center><br>"
                                "You can learn more about Fluorinated Compounds using the buttons above the pie chart.<br>"
                                "<br>"
                                "<b>It is recommended that you select a location using the filters on the left of the application to display a more useful visualisation on the graph.</b><br>"
                                "<br>"
                                "The time-series chart shows how measurements of Fluorinated Compounds have changed over time.<br>"
                                "<br>"
                                "The pie chart shows the proportion of measurements which are non-compliant, dangerously high or safely compliant.<br>"
                                "<br>"
                                "<center><b>Select Back to return to the previous page.</b></center>"
                                "<center><b>Select Next to continue to the next page.</b></center>"));
            this->adjustSize();
            break;
        case 6:
            guideLabel->setText(tr("<center><h1>Compliance</h1></center><br>"
                                "This page shows you locations which exceed the legal compliance limit by pollutant or group of pollutants.<br>"
                                "<br>"
                                "&nbsp;&nbsp;1. Select a pollutant from the dropdown tool on the left or select a pollutant group from the buttons on the right.<br>"
                                "<br>"
                                "Below your selection you will see lists of areas where your chosen pollutant has been measured as non-compliant, dangerously high or safely compliant.<br>"
                                "<br>"
                                "<center><b>Select Back to return to the previous page.</b></center>"
                                "<center><b>Select Next to continue to the next page.</b></center>"));
            this->adjustSize();
            break;
        case 7:
            guideLabel->setText(tr("<center><h1>Table</h1></center><br>"
                                "This page allows you to explore the dataset as a table. You can find specific data using the filters on the left of the application.<br>"
                                "<br>"
                                "<center><b>You have now completed the tutorial. You can return to this tutorial at any time by selecting User Guide at the bottom of the application.</b></center>"
                                "<br>"
                                "<center><b>Select Close to end the tutorial.</b></center>"));
            this->adjustSize();
            break;

    }
}

void UserGuide::updateTitle()
{
    this->setWindowTitle("User Guide");
}

void UserGuide::updateButton()
{
    if (index == 0) {
        backButton->hide(); // Hide Back button on the first page
    } else {
        backButton->show(); // Show Back button on other pages
    }

    if (index == 7){
        nextButton->setText("Close");
        connect(nextButton, &QPushButton::clicked, this, &UserGuide::close);

        closeButton->hide();
    }
}
