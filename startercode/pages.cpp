#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include <QtCharts>
#include <QtWebView>
#include <QPixmap>
#include <QFrame>
#include <regex>
#include "window.hpp"


QWidget* PollutantWindow::tableWidget()
{
    std::vector<int> cats = {0};
    model.setCategories(cats);
    table = new QTableView();
    table->setModel(&model);

    QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    table->setFont(tableFont);

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setCentralWidget(table);
    return table;
}

QWidget* PollutantWindow::dashboardWidget()
{
    QWidget* temp = new QWidget(this);

    // Create a scroll area
    QScrollArea* scrollArea = new QScrollArea(temp);
    scrollArea->setWidgetResizable(true); // Ensures the content adjusts to the scroll area

    // Create a container widget for the scroll area
    QWidget* scrollContent = new QWidget();

    // Set up a layout to hold the chartView in the temp widget
    QVBoxLayout* layout = new QVBoxLayout();

    // Make a QHBoxLayout for the first row
    QVBoxLayout* row1 = new QVBoxLayout();

    // Make three boxes within this row
    // r1c1 standing for row 1 column 1
    QHBoxLayout* r1c1 = new QHBoxLayout();
    QVBoxLayout* r1c2 = new QVBoxLayout();
    QVBoxLayout* r1c3 = new QVBoxLayout();

    // Populate the three boxes
    locationLabel = new QLabel("Dataset Location: ");
    locationLabel->setWordWrap(true);
    subLocationLabel = new QLabel("Sub Location:");
    subLocationLabel->setWordWrap(true);
    yearLabel = new QLabel(tr("Year:"));
    recordsLabel = new QLabel(tr("Number of Records:"));
    r1c1->addWidget(locationLabel);
    r1c1->addWidget(yearLabel);
    r1c1->addWidget(subLocationLabel);
    r1c1->addWidget(recordsLabel);



    // make the graph to add to r1c3
    QChart* pieChart = model.createPieChartPOPS(1);
    allPollutantsChartView = new QChartView(pieChart);
    allPollutantsChartView->setRenderHint(QPainter::Antialiasing);
    allPollutantsChartView->setMinimumHeight(200);
    allPollutantsChartView->setMaximumHeight(500);
    allPollutantsChartView->setMaximumWidth(800);
    allPollutantsChartView->setToolTip(tr("Use the filters on the left to change the data displayed in this chart"));

    r1c3->addWidget(allPollutantsChartView);

    

    // Add row 1 to layout
    layout->addLayout(r1c1);

    // now for row 2
    QVBoxLayout* row2 = new QVBoxLayout();

    // now make three columns
    QVBoxLayout* r2c1 = new QVBoxLayout();
    QVBoxLayout* r2c2 = new QVBoxLayout();
    QVBoxLayout* r2c3 = new QVBoxLayout();

    QGroupBox *col1 = new QGroupBox(tr("Common Pollutants"));
    QGroupBox *col2 = new QGroupBox(tr("POPs"));
    QGroupBox *col3 = new QGroupBox(tr("Fluorinated Compounds"));

    col1->setStyleSheet("QGroupBox::title {"
                        "font-size: 20px;"
                        "subcontrol-origin: margin;"
                        "subcontrol-position: top center;"
                        "padding: 10px;"
                        "}");
    col2->setStyleSheet("QGroupBox::title {"
                        "font-size: 20px;"
                        "subcontrol-origin: margin;"
                        "subcontrol-position: top center;"
                        "padding: 10px;"
                        "}");
    col3->setStyleSheet("QGroupBox::title {"
                        "font-size: 20px;"
                        "subcontrol-origin: margin;"
                        "subcontrol-position: top center;"
                        "padding: 10px;"
                        "}");

    commonCount = new QLabel(tr("Data is not loaded."));
    commonPercent = new QLabel(tr(""));
    r2c1->addWidget(commonCount);
    r2c1->addWidget(commonPercent);
    popsCount = new QLabel(tr("Data is not loaded."));
    popsPercent = new QLabel(tr(""));
    r2c2->addWidget(popsCount);
    r2c2->addWidget(popsPercent);
    pfaCount = new QLabel(tr("Data is not loaded."));
    pfaPercent = new QLabel(tr(""));
    r2c3->addWidget(pfaCount);
    r2c3->addWidget(pfaPercent);

    commonCount->setStyleSheet("color: red;");
    commonPercent->setStyleSheet("color: red;");
    popsCount->setStyleSheet("color: red;");
    popsPercent->setStyleSheet("color: red;");
    pfaCount->setStyleSheet("color: red;");
    pfaPercent->setStyleSheet("color: red;");

    // add the columns to the group boxes
    col1->setLayout(r2c1);
    col2->setLayout(r2c2);
    col3->setLayout(r2c3);

    // add cols to row 2
    row2->addWidget(col1);
    row2->addWidget(col2);
    row2->addWidget(col3);

    sewage = new QLabel(tr("Data is not loaded."));
    litter = new QLabel(tr("Data is not loaded."));

    QString sewagePath = "../icons/sewage.png";
    QString litterPath = "../icons/litter.png";
    QPixmap sewagePixmap(sewagePath);
    QPixmap litterPixmap(litterPath);

    QLabel* sewageIcon = new QLabel();
    sewageIcon->setPixmap(sewagePixmap.scaled(64, 64, Qt::KeepAspectRatio));
    QLabel* litterIcon = new QLabel();
    litterIcon->setPixmap(litterPixmap.scaled(64, 64, Qt::KeepAspectRatio));

    QFrame* sewageFrame = new QFrame();
    QFrame* litterFrame = new QFrame();

    QVBoxLayout* sewageLitter = new QVBoxLayout();
    QHBoxLayout* sewageLayout = new QHBoxLayout();
    QHBoxLayout* litterLayout = new QHBoxLayout();

    sewageLayout->addWidget(sewageIcon);
    sewageLayout->addWidget(sewage);
    sewageLayout->setSpacing(5);

    litterLayout->addWidget(litterIcon);
    litterLayout->addWidget(litter);
    litterLayout->setSpacing(5);

    sewageFrame->setLayout(sewageLayout);
    sewageFrame->setFrameShape(QFrame::Box);
    litterFrame->setLayout(litterLayout);
    litterFrame->setFrameShape(QFrame::Box);

    sewageLitter->addWidget(sewageFrame);
    sewageLitter->addWidget(litterFrame);


    QVBoxLayout* right = new QVBoxLayout();
    right->addLayout(r1c3);
    right->addLayout(sewageLitter);

    QHBoxLayout* main = new QHBoxLayout();
    main->addLayout(row2);
    main->addLayout(right);


    layout->addLayout(main);

    // Set the scroll content layout
    scrollContent->setLayout(layout);

    // Add the scroll content to the scroll area
    scrollArea->setWidget(scrollContent);

    // Create the main layout for the temp widget
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(scrollArea);

    temp->setLayout(mainLayout);
    return temp;
}



QWidget* PollutantWindow::overviewWidget()
{
    QWidget* temp = new QWidget(this);
    // Create a scroll area
    QScrollArea* scrollArea = new QScrollArea(temp);
    scrollArea->setWidgetResizable(true); // Ensures the content adjusts to the scroll area

    // Create a container widget for the scroll area
    QWidget* scrollContent = new QWidget();

    QLabel* title = new QLabel(tr("Common Pollutants"));
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    QLabel* description = new QLabel(tr("This page shows an overview of some of the most common water pollutants, such as Endrin, Mercury and Benzene."));
    description->setWordWrap(true);


    QStringList pollutants = {"Endrin","Mercury,Dissolved","Naphthalene","Tetrachloroethylene :- {Perchloroethylene}","Lead, Dissolved","Benzene","Cadmium, Dissolved","Atrazine","Phenol"};
    QComboBox* dropDown = new QComboBox();
    dropDown->addItem("All Pollutants");
    dropDown->addItems(pollutants);

    QLabel* dropDownLabel = new QLabel(tr("Select a pollutant:"));
    
    QVBoxLayout* dropDownLayout = new QVBoxLayout();
    dropDownLayout->addWidget(dropDownLabel);
    dropDownLayout->addWidget(dropDown);
    dropDownLayout->setAlignment(Qt::AlignTop);


    QChart* pieChart = model.createPieChartCommon();
    commonPieChartView = new QChartView(pieChart);
    commonPieChartView->setRenderHint(QPainter::Antialiasing);
    commonPieChartView->setMinimumHeight(400);
    commonPieChartView->setFixedWidth(500);
    commonPieChartView->setToolTip(tr("Use the filters on the left to change the data displayed in this chart"));
    if (dropDown->currentIndex() != -1) {
        QString text = dropDown->currentText();
    }
    commonChartView = new QChartView(model.createCommonBarChart(dropDown->currentText()));  
    commonChartView->setRenderHint(QPainter::Antialiasing);
    commonChartView->setMinimumHeight(400);
    commonChartView->setMaximumWidth(1200);
    commonChartView->setToolTip(tr("Use the filters on the left to change the data displayed in this chart"));

    QGroupBox* paragraphGroupBox = new QGroupBox(tr("Impact of Common Water Pollutants"));
    paragraphGroupBox->setStyleSheet("font-size: 16px; font-weight: bold;");
    QVBoxLayout* paragraphLayout = new QVBoxLayout();
    QLabel* paragraph = new QLabel(tr("Common water pollutants such as endrin, nitrogen compounds, mercury and benzene can severely impact water quality and aquatic life. "
                                      "These pollutants can come from various sources, including agricultural runoff, industrial discharges and improper waste disposal.\n\n"
                                      "Chemicals like endrin and atrazine are harmful to both ecosystems and human health, while heavy metals such as mercury and lead can accumulate in organisms, causing long term damage. "
                                      "Organic compounds like naphthalene and benzene contribute to water contamination, posing risks to wildlife and human populations.\n\n"
                                      "Despite regulatory measures, these pollutants remain a significant concern, as they persist in the environment and continue to affect water safety. "
                                      "Efforts to reduce their impact are ongoing but remain a challenge in maintaining clean and safe water resources."));
    paragraph->setWordWrap(true);
    paragraph->setStyleSheet("font-size: 16px; font-weight: normal;");
    paragraphLayout->addWidget(paragraph);
    paragraphGroupBox->setLayout(paragraphLayout);

    QFrame* barChart = new QFrame();
    barChart->setFrameShape(QFrame::Box);


    // Set up a layout to hold the chartView in the temp widget
    QVBoxLayout* chartslayout = new QVBoxLayout();
    QHBoxLayout* box1 = new QHBoxLayout();
    QHBoxLayout* box2 = new QHBoxLayout();
    box2->setAlignment(Qt::AlignTop);
    box1->addLayout(dropDownLayout);
    box1->addWidget(commonChartView);
    box2->addWidget(commonPieChartView);
    box2->addWidget(paragraphGroupBox);

    barChart->setLayout(box1);
    chartslayout->addWidget(barChart);
    chartslayout->addLayout(box2);

    QVBoxLayout* header = new QVBoxLayout();
    header->addWidget(title);
    header->addWidget(description);

    QVBoxLayout* layout = new QVBoxLayout(temp);
    layout->addLayout(header);
    layout->addLayout(chartslayout);



    
    connect(dropDown, &QComboBox::currentTextChanged, this, [this, dropDown]() {
        if (commonChartView) {
            // Update the bar chart dynamically based on the selected common type
            QChart* updatedChart = model.createCommonBarChart(dropDown->currentText());
            if (updatedChart){
                commonChartView->setChart(updatedChart);
            }
        }
    });

    scrollContent->setLayout(layout);
    scrollArea->setWidget(scrollContent);
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(scrollArea);
    temp->setLayout(mainLayout);

    return temp;
}


QWidget* PollutantWindow::POPsWidget()
{
    QWidget* temp = new QWidget(this);
    // Create a scroll area
    QScrollArea* scrollArea = new QScrollArea(temp);
    scrollArea->setWidgetResizable(true); // Ensures the content adjusts to the scroll area

    // Create a container widget for the scroll area
    QWidget* scrollContent = new QWidget(scrollArea);

    // create the label at top of the page explaining POPs
    QLabel* title = new QLabel(tr("Persistent Organic Pollutants"));
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    QLabel* description = new QLabel(tr("Persistent Organic Pollutants (POPs) are toxic chemicals that persist in water, accumulate in aquatic ecosystems, and harm wildlife and human health. They disrupt food chains, cause bioaccumulation in fish and marine life, and are linked to health issues like cancers and hormone disrupion in humans."));
    description->setWordWrap(true);
    // Create the chart using the model
    QChart* chart = model.createChartPOPS(0);

    // Create a QChartView to display the chart
    POPsChartView = new QChartView(chart);  // Assign to the member variable
    POPsChartView->setRenderHint(QPainter::Antialiasing);

    POPsChartView->setMinimumHeight(400);
    POPsChartView->setToolTip(tr("Use the filters on the left to change the data displayed in this chart"));

    // Set up a layout to hold the chartView in the temp widget
    QVBoxLayout* layout = new QVBoxLayout(temp);
    layout->addWidget(title);
    layout->addWidget(description);
    // now make a new qhboxlayout inside the layer to hold te charts and widgets next to each other
    QHBoxLayout* chartLayout = new QHBoxLayout();
    
    

    // make buttons to go in this layout
    // make a vertical layout to hold the buttons
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    QPushButton* button1 = new QPushButton(tr("What are POPs?"));
    QPushButton* button2 = new QPushButton(tr("Where do POPs come from?"));
    QPushButton* button3 = new QPushButton(tr("Why are POPs harmful to aquatic life?"));
    QPushButton* button4 = new QPushButton(tr("What is the risk to humans?"));
    buttonLayout->addWidget(button1);
    connect(button1, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, tr("Information"), tr("Persistent Organic Pollutants (POPs) are long-lasting toxic chemicals that degrade slowly in the environment. They can accumulate in soil, water, and living organisms, traveling long distances through natural processes. Common POPs include industrial chemicals like PCBs and pesticides like DDT."));
    });
    buttonLayout->addWidget(button2);
    connect(button2, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, tr("Information"), tr("POPs originate from industrial processes, agricultural runoff, waste incineration, and improper disposal of hazardous materials. They can also be released through accidents or leaks in chemical storage and transport. Atmospheric deposition and wastewater discharge are key pathways into aquatic systems."));
    });
    buttonLayout->addWidget(button3);
    connect(button3, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, tr("Information"), tr("POPs accumulate in aquatic ecosystems, entering the food chain and causing bioaccumulation in fish and other organisms. They can disrupt reproduction, growth, and immune systems in aquatic species, leading to population declines. Their persistence means that even small amounts can cause long-term harm."));
    });
    buttonLayout->addWidget(button4);
    connect(button4, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, tr("Information"), tr("Humans can be exposed to POPs through contaminated drinking water and seafood. POPs are linked to serious health issues, including cancer, hormonal disruptions, neurological damage, and developmental problems. Their accumulation in the body over time amplifies these risks, particularly in vulnerable populations like children and pregnant women."));
    });
    chartLayout->addLayout(buttonLayout);
    chartLayout->addWidget(POPsChartView);

    // now add the pie chart
    QChart* pieChart = model.createPieChartPOPS(0);
    POPsPieChartView = new QChartView(pieChart);
    POPsPieChartView->setRenderHint(QPainter::Antialiasing);

    POPsPieChartView->setMinimumHeight(400);
    POPsPieChartView->setFixedWidth(500);
    POPsPieChartView->setToolTip(tr("Use the filters on the left to change the data displayed in this chart"));

    // make another horizontal widget to hold pie chart and other text
    QHBoxLayout* pieLayout = new QHBoxLayout();
    pieLayout->addWidget(POPsPieChartView);

    // make a new layout to go next to pie char for text
    QGroupBox* textGroupBox = new QGroupBox(tr("How significant is the risk of Persistent Organic Pollutants?"));
    textGroupBox->setStyleSheet("font-size: 16px; font-weight: bold;");
    QVBoxLayout* textLayout = new QVBoxLayout();
    //textLayout->setAlignment(Qt::AlignTop);
    QLabel* answer = new QLabel(tr("The risk posed by Persistent Organic Pollutants (POPs) is significant due to their toxicity, persistence, and ability to bioaccumulate in food chains, threatening ecosystems and human health worldwide.\n\nDespite regulatory improvements, these chemicals remain in the environment for decades, with hotspots of contamination still posing risks to water quality, wildlife, and communities. To address this, international efforts like the Stockholm Convention have been pivotal, aiming to phase out or restrict the use of POPs globally. \n\nNational regulations and cleanup initiatives complement these efforts, alongside advancements in monitoring and the development of safer alternatives. Together, these measures work to mitigate the lasting impact of POPs on the environment and public health."));
    answer->setStyleSheet("font-size: 16px; font-weight: normal;");
    answer->setWordWrap(true);
    textLayout->addWidget(answer);
    textGroupBox->setLayout(textLayout);
    pieLayout->addWidget(textGroupBox);

    // now add these to layout
    layout->addLayout(chartLayout);
    layout->addLayout(pieLayout);
    
    // Set the scroll content layout
    scrollContent->setLayout(layout);

    // Add the scroll content to the scroll area
    scrollArea->setWidget(scrollContent);

    // Create the main layout for the temp widget
    QVBoxLayout* mainLayout = new QVBoxLayout(temp);
    mainLayout->addWidget(scrollArea);

    temp->setLayout(mainLayout);
    return temp;
}

QWidget* PollutantWindow::litterWidget()
{
    // Create a main container widget for the scrollable content
    QWidget* container = new QWidget(this);

    // Environmental litter explanation text + title
    QLabel* title = new QLabel(tr("Environmental Litter Indicators"));
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    QLabel* description = new QLabel(tr("Litter in water threatens ecosystems, wildlife, and human health. It disrupts habitats, endangers species through ingestion and entanglement, and breaks into microplastics that enter the food chain. For humans, this pollution contaminates drinking water, harms fisheries, and reduces the economic and recreational value of water bodies."));
    description->setWordWrap(true);

    // Set up a layout to hold the chartView in the container widget
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->addWidget(title);
    layout->addWidget(description);

    // Add a horizontal layout for charts and widgets
    QHBoxLayout* chartLayout = new QHBoxLayout();

    // Dropdown menu (QComboBox) for selecting litter type
    QVBoxLayout* dropdownMenu = new QVBoxLayout();
    QLabel* dropdownLabel = new QLabel(tr("Select Litter Type:"));
    QFont dropdownFont = dropdownLabel->font();
    dropdownFont.setPointSize(14);
    dropdownLabel->setFont(dropdownFont);

    litterDropdown = new QComboBox();
    litterDropdown->addItem(tr("Bathing Water Profile : Other Litter (incl. plastics)"));
    litterDropdown->addItem(tr("Sewage debris"));
    dropdownMenu->setAlignment(Qt::AlignTop);

    // Chart based on selected litter type
    litterChartView = new QChartView(model.createLitterBarChart(litterDropdown->currentText()));
    litterChartView->setRenderHint(QPainter::Antialiasing);
    litterChartView->setToolTip(tr("Use the filters on the left to change the data displayed in this chart"));

    dropdownMenu->addWidget(dropdownLabel);
    dropdownMenu->addWidget(litterDropdown);
    chartLayout->addLayout(dropdownMenu);
    chartLayout->addWidget(litterChartView);

    QFrame* chartFrame = new QFrame();
    chartFrame->setFrameShape(QFrame::Box);
    chartFrame->setLayout(chartLayout);

    // Connect the dropdown's signal to update the chart directly
    connect(litterDropdown, &QComboBox::currentTextChanged, this, [this]() {
        if (litterChartView) {
            // Update the bar chart dynamically based on the selected litter type
            QChart* updatedChart = model.createLitterBarChart(litterDropdown->currentText());
            litterChartView->setChart(updatedChart);
        }
    });

    // Add the pie chart
    litterPieChartView = new QChartView(model.createLitterPieChart());
    litterPieChartView->setRenderHint(QPainter::Antialiasing);
    litterPieChartView->setFixedWidth(500);

    litterPieChartView->setToolTip(tr("Use the filters on the left to change the data displayed in this chart"));

    QHBoxLayout* pieLayout = new QHBoxLayout();
    pieLayout->addWidget(litterPieChartView);

    // add text section to pie layout
    QGroupBox* textGroupBox = new QGroupBox(tr("Bathing Water Standards"));
    textGroupBox->setStyleSheet("font-size: 16px; font-weight: bold;");        
    QVBoxLayout* textLayout = new QVBoxLayout();
    //textLayout->setAlignment(Qt::AlignTop);
    QLabel* answer = new QLabel(tr("The graph on the left illustrates the quality of bathing waters within the dataset, providing a clear picture of how bathing waters meet UK and EU standards.\n\n Although the UK is no longer a member of the European union, it must adhere to The Bathing Water Regulations 2013, which was created from an EU directive. The Environment Agency monitors water quality to report on public health and environmental standards, and in turn to inform researchers, the government, and the public.\n\n Monitoring bathing water quality is crucial to safeguard public health, protect aquatic systems, and promote tourism and recreation. High-quality bathing waters reduce the risk of exposure to harmful bacteria and pollutants, which cause illness in swimmers. Futhermore, consistent monitoring helps identify and address pollution sources, ensuring sustainable management of coastal and inland water resources."));
    answer->setStyleSheet("font-size: 16px; font-weight: normal;");    
    answer->setWordWrap(true);
    //textLayout->setAlignment(Qt::AlignTop);
    textLayout->addWidget(answer);
    textGroupBox->setLayout(textLayout);
    pieLayout->addWidget(textGroupBox);

    layout->addWidget(chartFrame);
    layout->addLayout(pieLayout);

    container->setLayout(layout);

    // Create a scroll area to make the content scrollable
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);

    return scrollArea;
}


QWidget* PollutantWindow::fluorinationWidget()
{
    QWidget *temp = new QWidget(this);
    // Create a scroll area
    QScrollArea* scrollArea = new QScrollArea(temp);
    scrollArea->setWidgetResizable(true); // Ensures the content adjusts to the scroll area

    // Create a container widget for the scroll area
    QWidget* scrollContent = new QWidget(scrollArea);
    QVBoxLayout* layout = new QVBoxLayout(temp);

    // add the title and description
    QLabel* title = new QLabel(tr("Fluorinated Compounds"));
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    QLabel* description = new QLabel(tr("Fluorinated compounds are synthetic chemicals that resist breaking down, accumulating in water and ecosystems. Used in various products, they harm wildlife, disrupt ecosystems, and pose health risks like immune supression, developmental issues and cancer. These compounds also degrade water quality and safety."));
    description->setWordWrap(true);

    // add tese to laout
    layout->addWidget(title);
    layout->addWidget(description);

    // make a horizontal layer to hold he map and buttons
    QHBoxLayout* mapLayout = new QHBoxLayout();
    // make a vertical layout to hold the buttons
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    QPushButton* button1 = new QPushButton(tr("What are Fluorinated Compounds?"));
    QPushButton* button2 = new QPushButton(tr("Where do Fluorinated Compounds come from?"));
    QPushButton* button3 = new QPushButton(tr("Why are Fluorinated Compounds harmful to aquatic life?"));
    QPushButton* button4 = new QPushButton(tr("What is the risk to humans?"));
    buttonLayout->addWidget(button1);
    connect(button1, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, tr("Information"), tr("Fluorinated compounds are synthetic chemicals that contain carbon-fluorine bonds, making them highly stable and resistant to degradation. They are widely used in industrial applications and consumer products such as non-stick cookware, water-repellent fabrics, and firefighting foam. These compounds can persist in the environment for long periods, accumulating in water and ecosystems."));
    });
    buttonLayout->addWidget(button2);
    connect(button2, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, tr("Information"), tr("Fluorinated compounds are primarily released through industrial activities, including manufacturing processes, waste disposal, and the use of products like firefighting foam and stain-resistant fabrics. They can also be introduced through contaminated wastewater and runoff from industrial sites. Improper disposal and spills can contribute to their presence in the environment."));
    });
    buttonLayout->addWidget(button3);
    connect(button3, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, tr("Information"), tr("Fluorinated compounds accumulate in aquatic environments, entering the food chain through water and organisms. These chemicals can disrupt the growth, reproduction, and immune systems of aquatic species. Over time, they can cause declines in biodiversity and harm to sensitive species, such as fish and amphibians."));
    });
    buttonLayout->addWidget(button4);
    connect(button4, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, tr("Information"), tr("Humans can be exposed to fluorinated compounds through contaminated drinking water, food, and air. These chemicals are linked to health risks, including immune system suppression, developmental problems, and increased cancer risk. Long-term exposure can lead to the accumulation of these compounds in the body, potentially causing chronic health issues."));
    });
    mapLayout->addLayout(buttonLayout);

    // add a graph to maplayout
    QChart* chart = model.createChartPOPS(2);
    fluorinationChartView = new QChartView(chart);
    fluorinationChartView->setRenderHint(QPainter::Antialiasing);
    fluorinationChartView->setToolTip(tr("Use the filters on the left to change the data displayed in this chart"));

    fluorinationChartView->setMinimumHeight(400);

    mapLayout->addWidget(fluorinationChartView);
    

    // make a new horizonal layout to hold pie char and text
    QHBoxLayout* pieLayout = new QHBoxLayout();
    // now add the pie chart
    QChart* pieChart = model.createPieChartPOPS(0);
    fluorinationPieChartView = new QChartView(pieChart);
    fluorinationPieChartView->setRenderHint(QPainter::Antialiasing);
    fluorinationPieChartView->setMinimumHeight(400);
    fluorinationPieChartView->setFixedWidth(500);
    // Add a tooltip to explain the pie chart
    fluorinationPieChartView->setToolTip(tr("Use the filters on the left to change the data displayed in this chart"));

    // add to layout
    pieLayout->addWidget(fluorinationPieChartView);

    // add text section to pie layout
    QGroupBox* textGroupBox = new QGroupBox(tr("How significant is the risk of Flourinated Compounds?"));
    textGroupBox->setStyleSheet("font-size: 16px; font-weight: bold;");
    QVBoxLayout* textLayout = new QVBoxLayout();
    //textLayout->setAlignment(Qt::AlignTop);
    QLabel* answer = new QLabel(tr("These chemicals, often used in industrial and consumer products, do not break down easily, leading to long-term contamination of water sources and ecosystems. They accumulate in aquatic organisms and enter the food chain, causing ecological imbalances and reducing biodiversity.\n\nDespite regulatory efforts, their presence in water supplies and the environment remains a challenge, requiring ongoing monitoring and stricter controls. The U.S. Environmental Protection Agency (EPA) has introduced guidelines to limit exposure to certain fluorinated compounds, but more stringent global measures are still needed to address the widespread contamination."));
    answer->setStyleSheet("font-size: 16px; font-weight: normal;");
    answer->setWordWrap(true);
    textLayout->addWidget(answer);
    textGroupBox->setLayout(textLayout);   
    pieLayout->addWidget(textGroupBox);

    layout->addLayout(mapLayout);
    layout->addLayout(pieLayout);
    temp->setLayout(layout);

    
    // Set the scroll content layout
    scrollContent->setLayout(layout);

    // Add the scroll content to the scroll area
    scrollArea->setWidget(scrollContent);

    // Create the main layout for the temp widget
    QVBoxLayout* mainLayout = new QVBoxLayout(temp);
    mainLayout->addWidget(scrollArea);

    temp->setLayout(mainLayout);

    return temp;
}

QWidget* PollutantWindow::complianceWidget() {
    QWidget* container = new QWidget(this);

    QLabel* titleLabel1 = new QLabel(tr("Compliance Overview"));
    titleLabel1->setStyleSheet("font-size: 20px; font-weight: bold;");
    QLabel* titleLabel = new QLabel(tr("To begin, select a specific group of pollutants below."));
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleLabel->setFont(titleFont);

    QHBoxLayout* topLayout = new QHBoxLayout();

    // Dropdown for unique pollutants
    QVBoxLayout* dropdownLayout = new QVBoxLayout();
    QLabel* dropdownLabel = new QLabel(tr("Pollutants:"));
    QFont dropdownLabelFont = dropdownLabel->font();
    dropdownLabelFont.setPointSize(14);
    dropdownLabel->setFont(dropdownLabelFont);
    dropdownLayout->setAlignment(Qt::AlignTop);

    // Dropdown list
    pollutantDropdown = new QComboBox();
    pollutantDropdown->addItem(tr("Select pollutant"));
    dropdownLayout->addWidget(dropdownLabel);
    dropdownLayout->addWidget(pollutantDropdown);

    // Buttons for pollutant groups
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    QPushButton* commonButton = new QPushButton(tr("Common Pollutants"));
    QPushButton* popsButton = new QPushButton(tr("POPs"));
    QPushButton* litterButton = new QPushButton(tr("Litter"));
    QPushButton* pfasButton = new QPushButton(tr("PFAS"));

    buttonLayout->addWidget(commonButton);
    buttonLayout->addWidget(popsButton);
    buttonLayout->addWidget(litterButton);
    buttonLayout->addWidget(pfasButton);

    topLayout->addLayout(dropdownLayout);
    topLayout->addLayout(buttonLayout);

    QHBoxLayout* listLayout = new QHBoxLayout();

    // Non-compliant section
    QPixmap nonCompliantImage("../icons/non-compliant.png");    
    QLabel* nonCompliantImageLabel = new QLabel();
    nonCompliantImageLabel->setPixmap(nonCompliantImage);
    nonCompliantImageLabel->setScaledContents(true);
    nonCompliantImageLabel->setFixedSize(100, 100);

    nonCompliantList = new QListWidget();
    nonCompliantList->addItem(tr("No data to display"));
    QVBoxLayout* nonCompliantLayout = new QVBoxLayout();
    nonCompliantLayout->addWidget(nonCompliantImageLabel);
    nonCompliantLayout->addWidget(new QLabel(tr("Non-Compliant Areas (Above Limit)")));
    nonCompliantLayout->addWidget(nonCompliantList);

    // Cautious section
    QPixmap warningImage("../icons/near-limit.png");    
    QLabel* warningImageLabel = new QLabel();
    warningImageLabel->setPixmap(warningImage);
    warningImageLabel->setScaledContents(true);
    warningImageLabel->setFixedSize(100, 100);

    cautiousList = new QListWidget();
    cautiousList->addItem(tr("No data to display"));
    QVBoxLayout* cautiousLayout = new QVBoxLayout();
    cautiousLayout->addWidget(warningImageLabel);
    cautiousLayout->addWidget(new QLabel(tr("Cautious Areas (Near Limit)")));
    cautiousLayout->addWidget(cautiousList);

    // Compliant section
    QPixmap compliantImage("../icons/compliant.png");
    QLabel* compliantImageLabel = new QLabel();
    compliantImageLabel->setPixmap(compliantImage);
    compliantImageLabel->setScaledContents(true);
    compliantImageLabel->setFixedSize(100, 100);

    compliantList = new QListWidget();
    compliantList->addItem(tr("No data to display"));
    QVBoxLayout* compliantLayout = new QVBoxLayout();
    compliantLayout->addWidget(compliantImageLabel);
    compliantLayout->addWidget(new QLabel(tr("Compliant Areas (Below Limit)")));
    compliantLayout->addWidget(compliantList);

    // Add all sections to the list layout
    listLayout->addLayout(nonCompliantLayout);
    listLayout->addLayout(cautiousLayout);
    listLayout->addLayout(compliantLayout);

    //Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    mainLayout->addWidget(titleLabel1);
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(listLayout);

    container->setLayout(mainLayout);

    // Connect buttons and dropdown
    connect(commonButton, &QPushButton::clicked, this, [this]() { filterPollutantsByCategory(1); });
    connect(popsButton, &QPushButton::clicked, this, [this]() { filterPollutantsByCategory(0); });
    connect(litterButton, &QPushButton::clicked, this, [this]() { filterPollutantsByCategory(3); });
    connect(pfasButton, &QPushButton::clicked, this, [this]() { filterPollutantsByCategory(2); });
    connect(pollutantDropdown, &QComboBox::currentTextChanged, this, &PollutantWindow::filterPollutantsByPollutant);

    return container;
}


void PollutantWindow::filterPollutantsByCategory(int category) {
    nonCompliantList->clear();
    cautiousList->clear();
    compliantList->clear();

    // Sets to track already added sites
    std::set<std::string> nonCompliantSites;
    std::set<std::string> cautiousSites;
    std::set<std::string> compliantSites;

    std::vector<Pollutant> pollutants = model.getAllPollutants();

    for (const Pollutant& pollutant : pollutants) {
        if (pollutant.getCategory() == category) {
            std::string site = pollutant.getSite();
            if (pollutant.getCompliance() == 0 && nonCompliantSites.find(site) == nonCompliantSites.end()) {  // Non-Compliant
                nonCompliantList->addItem(QString::fromStdString(site));
                nonCompliantSites.insert(site);
            } else if (pollutant.getCompliance() == 1 && cautiousSites.find(site) == cautiousSites.end()) {  // Cautious
                cautiousList->addItem(QString::fromStdString(site));
                cautiousSites.insert(site);
            } else if (pollutant.getCompliance() == 2 && compliantSites.find(site) == compliantSites.end()) {  // Compliant
                compliantList->addItem(QString::fromStdString(site));
                compliantSites.insert(site);
            }
        }
    }
}

void PollutantWindow::filterPollutantsByPollutant(const QString& selectedPollutant) {
    nonCompliantList->clear();
    cautiousList->clear();
    compliantList->clear();

    // Sets to track already added sites
    std::set<std::string> nonCompliantSites;
    std::set<std::string> cautiousSites;
    std::set<std::string> compliantSites;

    std::vector<Pollutant> pollutants = model.getAllPollutants();

    for (const Pollutant& pollutant : pollutants) {
        if (selectedPollutant == tr("Select pollutant") || selectedPollutant == QString::fromStdString(pollutant.getPollutant())) {
            std::string site = pollutant.getSite();
            if (pollutant.getCompliance() == 0 && nonCompliantSites.find(site) == nonCompliantSites.end()) {  // Non-Compliant
                nonCompliantList->addItem(QString::fromStdString(site));
                nonCompliantSites.insert(site);
            } else if (pollutant.getCompliance() == 1 && cautiousSites.find(site) == cautiousSites.end()) {  // Cautious
                cautiousList->addItem(QString::fromStdString(site));
                cautiousSites.insert(site);
            } else if (pollutant.getCompliance() == 2 && compliantSites.find(site) == compliantSites.end()) {  // Compliant
                compliantList->addItem(QString::fromStdString(site));
                compliantSites.insert(site);
            }
        }
    }
}



QWidget* PollutantWindow::hotspotsWidget()
{
    QWidget *temp = new QWidget(this);
    return temp;
}