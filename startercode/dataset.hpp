// COMP2811 Coursework 1 sample solution: PollutantDataset class

#pragma once

#include <vector>
#include "pollutant.hpp"
#include <string>
#include <QProgressBar>

struct PollutantFilter {
    std::string location;  
    std::string compliance;      
    std::string level;      
    std::string dateEnd;  
    std::string dateStart;      
};

class PollutantDataset
{
  public:
    PollutantDataset() {}
    PollutantDataset(const std::string& filename, const PollutantFilter& filter, QProgressBar* progressBar) { loadData(filename, filter, progressBar); }
    void loadData(const std::string& filename, const PollutantFilter& filter, QProgressBar* progressBar);
    int size() const { return data.size(); }
    Pollutant operator[](int index) const { return data.at(index); }
    void filter(const PollutantFilter& filter);
    bool getIsDataLoaded() const { return isDataLoaded; }
    void setIsDataLoaded(bool state) { this->isDataLoaded = state; }
    const std::vector<Pollutant>& getData() const { return data; }

  private:
    std::vector<Pollutant> data;
    std::vector<Pollutant> unfilteredData;
    bool isDataLoaded = false;
    void checkDataExists() const;
};