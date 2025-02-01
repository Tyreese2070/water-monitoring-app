// COMP2811 Coursework 2: data model

#include "tablemodel.hpp"
#include <iostream>
#include <set>
#include <QtCharts>




void PollutantModel::updateFromFile(const QString& filename, const PollutantFilter& filter, QProgressBar* progressBar)
{
  beginResetModel();
  dataset.loadData(filename.toStdString(), filter, progressBar);
  endResetModel();
  
}

// Function to get unique locations
std::set<std::string> PollutantModel::getUniqueLocations() const
{
    std::set<std::string> uniqueLocations;  // Set to store unique locations

    // Iterate through the dataset and insert each location into the set
    for (int i = 0; i < dataset.size(); ++i) {
        Pollutant q = dataset[i];
        std::string location = q.getSite();  // Get the location from the pollutant

        // Insert the location into the set (duplicates are ignored by the set)
        uniqueLocations.insert(location);
    }

    return uniqueLocations;  // Return the set of unique locations
}

std::set<Pollutant> PollutantModel::getUniquePollutants() const
{
    std::set<Pollutant> uniquePollutants;  // Set to store unique pollutant

    // Iterate through the dataset and insert each pollutant into the set
    for (int i = 0; i < dataset.size(); ++i) {
        uniquePollutants.insert(dataset[i]);
    }

    return uniquePollutants;
}

std::vector<Pollutant> PollutantModel::getAllPollutants() const {
    return dataset.getData();
}

QVariant PollutantModel::data(const QModelIndex& index, int role) const
{
  std::vector<int> cats = getCategories();

  if (!index.isValid()) {
    qDebug() << "Invalid index:" << index;
    return QVariant();
  }
  
  if (role == Qt::TextAlignmentRole) {
    return int(Qt::AlignRight | Qt::AlignVCenter);
  }
  else if (role == Qt::DisplayRole) {
    Pollutant q = dataset[index.row()];
    

    
    switch (index.column()) {
      case 0: return QVariant(q.getTime().c_str());
      case 1: return QVariant(q.getSite().c_str());
      case 2: return QVariant(q.getPollutant().c_str());
      case 3: return QVariant(q.getUnit().c_str());
      case 4: return QVariant(q.getType().c_str());
      case 5: return QVariant(q.getResult());
      case 6: return QVariant(q.getQualifier().c_str());
      case 7: return QVariant(q.getEasting());
      case 8: return QVariant(q.getNorthing());
    }
  }

  return QVariant();
}


QVariant PollutantModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (orientation == Qt::Vertical) {
    return QVariant(section + 1);
  }

  switch (section) {
    case 0: return QString("Date and Time");
    case 1: return QString("Site Name");
    case 2: return QString("Pollutant Name");
    case 3: return QString("Unit of Measurement");
    case 4: return QString("Type");
    case 5: return QString("Result");
    case 6: return QString("Qualifier");
    case 7: return QString("Easting");
    case 8: return QString("Northing");
    default: return QVariant();
  }
}


// basic chart to show a barchart of records per location
QChart* PollutantModel::createChart() const
{
  if (dataset.getIsDataLoaded() == false){
    QChart* emptyChart = new QChart();
    emptyChart->setTitle(tr("Please load the data to view the chart."));
    
    QFont titleFont = emptyChart->titleFont();
    titleFont.setPointSize(24); 
    emptyChart->setTitleFont(titleFont);

    return emptyChart;
  }

    QChart* chart = new QChart();

    // Map to count records per location
    std::map<std::string, int> locationCounts;
    for (int i = 0; i < dataset.size(); ++i) {
        const std::string& location = dataset[i].getSite();
        locationCounts[location]++;
    }

    // Create bar set
    QBarSet* barSet = new QBarSet("Records");

    

    // Create categories for X-axis
    QStringList categories;
    for (const auto& [location, count] : locationCounts) {
        categories << QString::fromStdString(location);
        *barSet << count; // Add count to the bar set
    }

    // Create bar series and add the bar set
    QBarSeries* series = new QBarSeries();
    series->append(barSet);

    // Add series to the chart
    chart->addSeries(series);

    // Create and set the X-axis (categories)
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Create and set the Y-axis (values)
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, std::max_element(locationCounts.begin(), locationCounts.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; })->second);
    axisY->setTitleText("Number of Records");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Set chart title
    chart->setTitle("Records per Location");

    return chart;
}


QChart* PollutantModel::createChartPOPS(int cat) const
{
  if (dataset.getIsDataLoaded() == false){
    QChart* emptyChart = new QChart();
    emptyChart->setTitle(tr("Please load the data to view the chart."));
    
    QFont titleFont = emptyChart->titleFont();
    titleFont.setPointSize(24); 
    emptyChart->setTitleFont(titleFont);

    return emptyChart;
  }

    QChart* chart = new QChart();

    // Create Line and Scatter Series
    QLineSeries* series = new QLineSeries();
    QScatterSeries* scatterSeries = new QScatterSeries();
    series->setName(tr("Result over Time"));
    scatterSeries->setName(tr("Data Points"));

    // Customize scatter series
    scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    scatterSeries->setMarkerSize(8.0);

    bool hasData = false;
    qint64 minTime = std::numeric_limits<qint64>::max();
    qint64 maxTime = std::numeric_limits<qint64>::min();
    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::min();

    // Iterate over the dataset to populate the series
    for (int i = 0; i < dataset.size(); ++i) {
        std::string rawTime = dataset[i].getTime();
        qDebug() << "Raw time string:" << QString::fromStdString(rawTime);

        // Parse the datetime
        QDateTime time = QDateTime::fromString(QString::fromStdString(rawTime), "yyyy-MM-dd'T'HH:mm:ss");
        if (!time.isValid()) {
            qDebug() << "Failed to parse time string:" << QString::fromStdString(rawTime);
            continue;
        }

        // Filter by category
        if (dataset[i].getCategory() != cat) {
            continue;
        }

        if (dataset[i].getResult() > 0.2){
          //consider this a outlier
          continue;
        }

        

        double result = dataset[i].getResult();
        qint64 timeMs = time.toMSecsSinceEpoch();

        // Add to both series
        series->append(timeMs, result);
        scatterSeries->append(timeMs, result);

        // Update axis range variables
        minTime = std::min(minTime, timeMs);
        maxTime = std::max(maxTime, timeMs);
        minValue = std::min(minValue, result);
        maxValue = std::max(maxValue, result);

        hasData = true;

        qDebug() << "Parsed time:" << time.toString("yyyy-MM-dd HH:mm:ss") << "Result:" << result;
    }

    // if one location show the line char too, if many just show points
    if (getUniqueLocations().size() == 1) {
      series->setVisible(true);
    } else {
      series->setVisible(false);
    }


    // If no data, return an empty chart
    if (!hasData) {
        chart->setTitle(tr("No Data Available"));
        return chart;
    }

    // Add series to the chart
    chart->addSeries(series);
    chart->addSeries(scatterSeries);

    // Create and configure X axis
    QDateTimeAxis* axisX = new QDateTimeAxis();
    axisX->setFormat("yyyy-MM-dd HH:mm");
    axisX->setTitleText(tr("Date and Time"));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    scatterSeries->attachAxis(axisX);

    // Create and configure Y axis
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText(tr("Result Value"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    scatterSeries->attachAxis(axisY);

    // Handle single-point cases for X and Y axis
    if (minTime == maxTime) {
        // Add padding for single datetime
        minTime -= 3600 * 1000; // Subtract 1 hour
        maxTime += 3600 * 1000; // Add 1 hour
    }
    if (minValue == maxValue) {
        // Add padding for single value
        minValue -= 1.0; // Subtract 1 unit
        maxValue += 1.0; // Add 1 unit
    }

    // Set the axis ranges
    axisX->setRange(QDateTime::fromMSecsSinceEpoch(minTime - 604800), QDateTime::fromMSecsSinceEpoch(maxTime + 604800));
    axisY->setRange(minValue - (maxValue - minValue) * 0.1, maxValue + (maxValue - minValue) * 0.1);

    // Make the line series invisible
    //series->setVisible(false);

    // Configure the chart
    chart->setTitle(tr("Result Over Time"));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    return chart;
}




QChart* PollutantModel::createPieChartPOPS(int cat) const
{
  if (dataset.getIsDataLoaded() == false){
    QChart* emptyChart = new QChart();
    emptyChart->setTitle(tr("Please load the data to view the chart."));
    
    QFont titleFont = emptyChart->titleFont();
    titleFont.setPointSize(24); 
    emptyChart->setTitleFont(titleFont);

    return emptyChart;
  }

  QChart* chart = new QChart();

  QPieSeries *series = new QPieSeries();

  int withinLimitsCount = 0;
  int nearLimitsCount = 0;
  int outsideLimitsCount = 0;
  // now iterate over the dataset to populate the series
  for (int i = 0; i < dataset.size(); ++i) {
    //makes sure the category is for POPs only but only when cat is set to zero
    // otherwise show all pollutants
    if (cat == 0 && dataset[i].getCategory() != 0)  {
      continue;
    }

    if (dataset[i].getResult() > dataset[i].getLimit()){
      outsideLimitsCount++;
    } else if (dataset[i].getResult() >= dataset[i].getNearLimit()){
      nearLimitsCount++;
    } else {
      withinLimitsCount++;
    }
  }

  // Add slices to the series
  series->append("Within Limits", withinLimitsCount);
  series->append("Near Limits", nearLimitsCount);
  series->append("Over Limits", outsideLimitsCount);

  

  // Customize within limits slice
  QPieSlice *slice = series->slices().at(0); 
  if (withinLimitsCount >0){
    slice->setLabelVisible();
    slice->setExploded();
  } 
  slice->setBrush(Qt::green);
  slice->setExplodeDistanceFactor(0.1);

  // Customize near limits slice
  slice = series->slices().at(1);
  if (nearLimitsCount >0){
    slice->setLabelVisible();
    slice->setExploded();
  }
  slice->setBrush(QColor("#FFA500")); 
  slice->setExplodeDistanceFactor(0.1);

  // Customize over limits slice
  slice = series->slices().at(2);
  if (outsideLimitsCount >0){
    slice->setLabelVisible();
    slice->setExploded();
  }
  slice->setBrush(Qt::red);
  slice->setExplodeDistanceFactor(0.1);

  chart->addSeries(series);
  if (cat == 0){
    chart->setTitle("POP Compliance");
  } else {
    chart->setTitle("All Pollutants Compliance");
  }
  


  return chart;
}

QChart* PollutantModel::createPieChartFluorination() const
{
  if (dataset.getIsDataLoaded() == false){
    QChart* emptyChart = new QChart();
    emptyChart->setTitle(tr("Please load the data to view the chart."));
    
    QFont titleFont = emptyChart->titleFont();
    titleFont.setPointSize(24); 
    emptyChart->setTitleFont(titleFont);

    return emptyChart;
  }

  QChart* chart = new QChart();

  QPieSeries *series = new QPieSeries();

  int withinLimitsCount = 0;
  int nearLimitsCount = 0;
  int outsideLimitsCount = 0;
  // now iterate over the dataset to populate the series
  for (int i = 0; i < dataset.size(); ++i) {
    //makes sure the category is for PFAs only (i think this is floruinated compounds)
    if (dataset[i].getCategory() != 2){
      continue;
    }
    if (dataset[i].getResult() > dataset[i].getLimit()){
      outsideLimitsCount++;
    } else if (dataset[i].getResult() >= dataset[i].getNearLimit()){
      nearLimitsCount++;
    } else {
      withinLimitsCount++;
    }
  }

  // Add slices to the series
  series->append("Within Limits", withinLimitsCount);
  series->append("Near Limits", nearLimitsCount);
  series->append("Over Limits", outsideLimitsCount);


  // Customize within limits slice
  QPieSlice *slice = series->slices().at(0); 
  if (withinLimitsCount >0){
    slice->setLabelVisible();
    slice->setExploded();
  } 
  slice->setBrush(Qt::green);
  slice->setExplodeDistanceFactor(0.1);

  // Customize near limits slice
  slice = series->slices().at(1);
  if (nearLimitsCount >10){
    slice->setLabelVisible();
    slice->setExploded();
  } else if (nearLimitsCount >0){
    slice->setExploded();
  }
  slice->setBrush(QColor("#FFA500"));
  slice->setExplodeDistanceFactor(0.1);

  // Customize over limits slice
  slice = series->slices().at(2);
  if (outsideLimitsCount >0){
    slice->setLabelVisible();
    slice->setExploded();
  }
  slice->setBrush(Qt::red);
  slice->setExplodeDistanceFactor(0.1);

  chart->addSeries(series);
  chart->setTitle("Fluorinated Compounds Compliance");


  return chart;
}

QChart* PollutantModel::createLitterBarChart(const QString& litterType) const
{
  if (dataset.getIsDataLoaded() == false){
    QChart* emptyChart = new QChart();
    emptyChart->setTitle(tr("Please load the data to view the chart."));
    
    QFont titleFont = emptyChart->titleFont();
    titleFont.setPointSize(24); 
    emptyChart->setTitleFont(titleFont);

    return emptyChart;
  }

    QChart* chart = new QChart();

    std::map<std::string, std::pair<double, int>> waterBodyLitterData;

    // Iterate over the dataset and accumulate litter data based on litter type
    for (int i = 0; i < dataset.size(); ++i) {
        const std::string& pollutant = dataset[i].getPollutant();

        if (pollutant != litterType.toStdString()) {
            continue;
        }

        const std::string& waterBodyType = dataset[i].getType();
        double result = dataset[i].getResult();

        // Accumulate total litter data for each water body type
        if (waterBodyLitterData.find(waterBodyType) == waterBodyLitterData.end()) {
            waterBodyLitterData[waterBodyType] = {result, 1};  // Initialize with first result
        } else {
            waterBodyLitterData[waterBodyType].first += result;  // Accumulate results
            waterBodyLitterData[waterBodyType].second++;          // Increment count
        }
    }

    // Create a bar set to hold the data
    QBarSet* barSet = new QBarSet(litterType);

    // Create categories for the x-axis
    QStringList categories;
    
    // Populate the bar set and categories
    for (const auto& [waterBody, data] : waterBodyLitterData) {
        double average = data.first / data.second;  // Calculate average litter level
        categories << QString::fromStdString(waterBody);  // Add water body type to categories
        *barSet << average;  // Add average value to the bar set
    }

    // Create bar series and add the bar set
    QBarSeries* series = new QBarSeries();
    series->append(barSet);

    // Add series to the chart
    chart->addSeries(series);

    // Create and set the X-axis (water body types)
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Create and set the Y-axis (Garber C scale)
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Garber C Scale");
    axisY->setRange(0, 10);  // Set a reasonable range for the Garber C scale
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Set the chart title and enable the legend
    chart->setTitle("Litter Levels by Water Body Type (" + litterType + ")");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    return chart;
}

QChart* PollutantModel::createLitterPieChart() const
{
  if (dataset.getIsDataLoaded() == false){
    QChart* emptyChart = new QChart();
    emptyChart->setTitle(tr("Please load the data to view the chart."));
    
    QFont titleFont = emptyChart->titleFont();
    titleFont.setPointSize(24); 
    emptyChart->setTitleFont(titleFont);

    return emptyChart;
  }

  QChart* chart = new QChart();
  QPieSeries *series = new QPieSeries();

  int excellentCount = 0;
  int goodCount = 0;
  int poorCount = 0;

  for (int i = 0; i < dataset.size(); ++i) {
    if (dataset[i].getUnit() == "garber c"){
      if (dataset[i].getResult() >= dataset[i].getLimit()){ 
        poorCount += 1;
      }
      else if(dataset[i].getResult() < dataset[i].getLimit() && dataset[i].getResult() > dataset[i].getNearLimit()){
        goodCount += 1;
      }
      else{
        excellentCount +=1;
      }
    }
  }

  // Add slices to the series
  series->append("Excellent", excellentCount);
  series->append("Good", goodCount);
  series->append("Poor", poorCount);

  // Customize within limits slice
  QPieSlice *slice = series->slices().at(0); 
  if (excellentCount >0){
    slice->setLabelVisible();
    slice->setExploded();
  } 
  slice->setBrush(Qt::green);
  slice->setExplodeDistanceFactor(0.1);

  // Customize near limits slice
  slice = series->slices().at(1);
  if (goodCount >0){
    slice->setLabelVisible();
    slice->setExploded();
  }
  slice->setBrush(QColor("#FFA500"));
  slice->setExplodeDistanceFactor(0.1);

  // Customize over limits slice
  slice = series->slices().at(2);
  if (poorCount >0){
    slice->setLabelVisible();
    slice->setExploded();
  }
  slice->setBrush(Qt::red);
  slice->setExplodeDistanceFactor(0.1);

  chart->addSeries(series);
  chart->setTitle("Litter Compliance");


  return chart;
}


DashboardInfo PollutantModel::getDashboardInfo(int type) const // 0 for pops, 1 fo common pollutants, 3 for sewage and liter
{
  // do common pollutants
    int count = 0;
    int over_limits = 0;
    for (int i = 0; i < dataset.size(); i++) {
      if (dataset[i].getCategory() == type){
        if (dataset[i].getResult() > dataset[i].getLimit()){
          over_limits++;
        }
        count++;
      }
    }
    if (count == 0){
      return {0.0, 0};
    }
    return {float(over_limits)/count * 100, count};

}

LitterInfo PollutantModel::getLitterInfo() const
{
  int sewage = 0;
  int litter = 0;
  std::set<std::string> locations;
  for (int i = 0; i < dataset.size(); i++) {
    if (locations.find(dataset[i].getSite()) == locations.end()) {
      if (dataset[i].getCategory() == 3) {
        if (dataset[i].getPollutant() == "Bathing Water Profile : Other Litter (incl. plastics)") {
          litter++;
        } else if (dataset[i].getPollutant() == "Sewage debris") {
          sewage++;
        }
        locations.insert(dataset[i].getSite());
      } 
    }
  }

  return {sewage, litter};
}

QChart* PollutantModel::createPieChartCommon() const
{
  if (dataset.getIsDataLoaded() == false){
    QChart* emptyChart = new QChart();
    emptyChart->setTitle(tr("Please load the data to view the chart."));
    
    QFont titleFont = emptyChart->titleFont();
    titleFont.setPointSize(24); 
    emptyChart->setTitleFont(titleFont);

    return emptyChart;
  }

  
  QChart* chart = new QChart();

  QPieSeries *series = new QPieSeries();

  int withinLimitsCount = 0;
  int nearLimitsCount = 0;
  int outsideLimitsCount = 0;
  // now iterate over the dataset to populate the series
  for (int i = 0; i < dataset.size(); ++i) {
    //makes sure the category is for commmon
    if (dataset[i].getCategory() != 1){
      continue;
    }
    if (dataset[i].getResult() > dataset[i].getLimit()){
      outsideLimitsCount++;
    } else if (dataset[i].getResult() >= dataset[i].getNearLimit()){
      nearLimitsCount++;
    } else {
      withinLimitsCount++;
    }
  }

  // Add slices to the series
  series->append("Within Limits", withinLimitsCount);
  series->append("Near Limits", nearLimitsCount);
  series->append("Over Limits", outsideLimitsCount);


  

  // Customize within limits slice
  QPieSlice *slice = series->slices().at(0); 
  if (withinLimitsCount >0){
    slice->setLabelVisible();
    slice->setExploded();
  } 
  slice->setBrush(Qt::green);
  slice->setExplodeDistanceFactor(0.1);

  // Customize near limits slice
  slice = series->slices().at(1);
  if (nearLimitsCount >10){
    slice->setLabelVisible();
    slice->setExploded();
  } else if (nearLimitsCount >0){
    slice->setExploded();
  }
  slice->setBrush(QColor("#FFA500"));
  slice->setExplodeDistanceFactor(0.1);

  // Customize over limits slice
  slice = series->slices().at(2);
  if (outsideLimitsCount >0){
    slice->setLabelVisible();
    slice->setExploded();
  }
  slice->setBrush(Qt::red);
  slice->setExplodeDistanceFactor(0.1);

  chart->addSeries(series);
  chart->setTitle(tr("Common Pollutant Compliance"));


  return chart;
}


QChart* PollutantModel::createCommonBarChart(const QString& commonType) const
{
  if (dataset.getIsDataLoaded() == false){
    QChart* emptyChart = new QChart();
    emptyChart->setTitle(tr("Please load the data to view the chart."));
    
    QFont titleFont = emptyChart->titleFont();
    titleFont.setPointSize(24); 
    emptyChart->setTitleFont(titleFont);

    return emptyChart;
  }
  
    QChart* chart = new QChart();

    std::map<std::string, std::pair<double, int>> waterBodyCommonData;

    // Iterate over the dataset and accumulate litter data based on litter type
    if (commonType.toStdString() == "Select Pollutant"){
      for (int i = 0; i < dataset.size(); ++i) {
        const std::string& waterBodyType = dataset[i].getType();
        double result = dataset[i].getResult();

        // Accumulate total litter data for each water body type
        if (waterBodyCommonData.find(waterBodyType) == waterBodyCommonData.end()) {
            waterBodyCommonData[waterBodyType] = {result, 1};  // Initialize with first result
        } else {
            waterBodyCommonData[waterBodyType].first += result;  // Accumulate results
            waterBodyCommonData[waterBodyType].second++;          // Increment count
        }
     }
    }
    else{
      for (int i = 0; i < dataset.size(); ++i) {
        const std::string& pollutant = dataset[i].getPollutant();

        if (pollutant != commonType.toStdString()) {
            continue;
        }

        const std::string& waterBodyType = dataset[i].getType();
        double result = dataset[i].getResult();

        // Accumulate total litter data for each water body type
        if (waterBodyCommonData.find(waterBodyType) == waterBodyCommonData.end()) {
            waterBodyCommonData[waterBodyType] = {result, 1};  // Initialize with first result
        } else {
            waterBodyCommonData[waterBodyType].first += result;  // Accumulate results
            waterBodyCommonData[waterBodyType].second++;          // Increment count
        }
      }
    }

    // Create a bar set to hold the data
    QBarSet* barSet = new QBarSet(commonType);

    // Create categories for the x-axis
    QStringList categories;
    
    // Populate the bar set and categories
    for (const auto& [waterBody, data] : waterBodyCommonData) {
        double average = data.first / data.second;  // Calculate average litter level
        categories << QString::fromStdString(waterBody);  // Add water body type to categories
        *barSet << average;  // Add average value to the bar set
    }

    // Create bar series and add the bar set
    QBarSeries* series = new QBarSeries();
    series->append(barSet);

    // Add series to the chart
    chart->addSeries(series);

    // Create and set the X-axis (water body types)
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Create and set the Y-axis (µg/l)
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("µg/l");
    axisY->setRange(0, 3);  
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Set the chart title and enable the legend
    if (commonType.toStdString() == "Select Pollutant"){
      chart->setTitle(tr("Common Pollutant levels"));
    }
    else{
      chart->setTitle("Pollutant Levels by Pollutant (" + commonType + ")");
    }
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    return chart;
}
