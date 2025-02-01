#pragma once

#include <QAbstractTableModel>
#include "dataset.hpp"
#include <set> // for storing unique locations
#include <QtCharts>

struct DashboardInfo {
  float value;
  int count;
};

struct LitterInfo{
  int sewage;
  int litter;
};

class PollutantModel : public QAbstractTableModel
{
  public:
    PollutantModel(QObject* parent = nullptr): QAbstractTableModel(parent) {}
    void updateFromFile(const QString&, const PollutantFilter& filter, QProgressBar* progressBar);
    bool hasData() const { return dataset.size() > 0; }
    int rowCount(const QModelIndex& index) const { return dataset.size(); }
    int columnCount(const QModelIndex& index) const { return 9; }
    QVariant data(const QModelIndex&, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;
    void setCategories(std::vector<int>& categories) { this->categories = categories; }
    std::vector<int> getCategories() const { return categories; }
    void setIsDataLoaded(bool state) { dataset.setIsDataLoaded(state); }
    QChart* createChart() const;
    QChart* createChartPOPS(int cat) const; 
    QChart* createPieChartPOPS(int cat) const;
    QChart* createPieChartFluorination() const;
    QChart* createLitterPieChart() const;
    QChart* createLitterBarChart(const QString& litterType) const;
    QChart* createPieChartCommon() const;
    QChart* createCommonBarChart(const QString&) const;
    QComboBox* CommonPollutantsDropDown() const;
    // New function to get unique locations
    std::set<std::string> getUniqueLocations() const;
    std::set<Pollutant> getUniquePollutants() const;
    std::vector<Pollutant> getAllPollutants() const;

    // New function to get info to dashboard
    DashboardInfo getDashboardInfo(int type) const;
    LitterInfo getLitterInfo() const;

  private:
    PollutantDataset dataset;
    std::vector<int> categories;
};