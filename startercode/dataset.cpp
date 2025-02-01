// COMP2811 Coursework 1 sample solution: PollutantDataset class

#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "dataset.hpp"
#include "csv.hpp"
#include <iostream>

using namespace std;


void PollutantDataset::loadData(const string& filename, const PollutantFilter& filter, QProgressBar* progressBar)
{

  if (isDataLoaded){
    cout << "test" << endl;
    this->filter(filter);
  }
  else{
    csv::CSVReader reader(filename);
    csv::CSVReader PopLimits("../POP_limits.csv");
    csv::CSVReader CommonLimits("../Common_limits.csv");
    csv::CSVReader PfaLimits("../PFA_limits.csv");
    csv::CSVReader LitterLimits("../litter_limits.csv");
    
    data.clear();
    unfilteredData.clear();

    unordered_map<string, pair < double, double> > PopPollutants;
    for (const auto& row: PopLimits){
      PopPollutants[row["determinand.definition"].get<>()] = 
        make_pair(row["limit"].get<double>(), row["near_limit"].get<double>());
    } 

    unordered_map<string, pair < double, double> > CommonPollutants;
    for (const auto& row: CommonLimits){
      CommonPollutants[row["determinand.definition"].get<>()] = 
        make_pair(row["limit"].get<double>(), row["near_limit"].get<double>());
    } 

    unordered_map<string, pair < double, double> > PfaPollutants;
    for (const auto& row: PfaLimits){
      PfaPollutants[row["determinand.definition"].get<>()] = 
        make_pair(row["limit"].get<double>(), row["near_limit"].get<double>());
    } 

    unordered_map<string, pair < double, double> > LitterPollutants;
    for (const auto& row: LitterLimits){
      LitterPollutants[row["determinand.definition"].get<>()] = 
        make_pair(row["limit"].get<double>(), row["near_limit"].get<double>());
    } 

    int processedRows = 0;
    int rowCount = 0;

    for (const auto& row : reader) {
      rowCount ++;
      Pollutant pollutant{
        row["sample.samplingPoint.label"].get<>(),
        row["sample.sampleDateTime"].get<>(),
        row["determinand.definition"].get<>(),
        row["determinand.unit.label"].get<>(),
        row["sample.sampledMaterialType.label"].get<>(),
        row["resultQualifier.notation"].get<>(),
        row["result"].get<double>(),
        row["sample.samplingPoint.easting"].get<double>(),
        row["sample.samplingPoint.northing"].get<double>(),
        0,
        0,
        -1,
        -1
      };
      if (PopPollutants.find(row["determinand.definition"].get<>()) != PopPollutants.end()) {
        pollutant.setLimit(PopPollutants[row["determinand.definition"].get<>()].first);
        pollutant.setNearLimit(PopPollutants[row["determinand.definition"].get<>()].second);
        pollutant.setCategory(0);
        if (pollutant.getResult() > pollutant.getLimit()){
          pollutant.setCompliance(0);
        }
        else if (pollutant.getResult() > pollutant.getNearLimit()){
          pollutant.setCompliance(1);
        }
        else{
          pollutant.setCompliance(2);
        }
      }
      else if (CommonPollutants.find(row["determinand.definition"].get<>()) != CommonPollutants.end()) {
        pollutant.setLimit(CommonPollutants[row["determinand.definition"].get<>()].first);
        pollutant.setNearLimit(CommonPollutants[row["determinand.definition"].get<>()].second);
        pollutant.setCategory(1);
        if (pollutant.getResult() > pollutant.getLimit()){
          pollutant.setCompliance(0);
        }
        else if (pollutant.getResult() > pollutant.getNearLimit()){
          pollutant.setCompliance(1);
        }
        else{
          pollutant.setCompliance(2);
        }
      }
      else if (PfaPollutants.find(row["determinand.definition"].get<>()) != PfaPollutants.end()) {
        pollutant.setLimit(PfaPollutants[row["determinand.definition"].get<>()].first);
        pollutant.setNearLimit(PfaPollutants[row["determinand.definition"].get<>()].second);
        pollutant.setCategory(2);
        if (pollutant.getResult() > pollutant.getLimit()){
            pollutant.setCompliance(0);
        }
        else if (pollutant.getResult() > pollutant.getNearLimit()){
          pollutant.setCompliance(1);
        }
        else{
          pollutant.setCompliance(2);
        }
      }
      else if (LitterPollutants.find(row["determinand.definition"].get<>()) != LitterPollutants.end()) {
        pollutant.setLimit(LitterPollutants[row["determinand.definition"].get<>()].first);
        pollutant.setNearLimit(LitterPollutants[row["determinand.definition"].get<>()].second);
        pollutant.setCategory(3);
        if (pollutant.getResult() > pollutant.getLimit()){
            pollutant.setCompliance(0);
        }
        else if (pollutant.getResult() > pollutant.getNearLimit()){
          pollutant.setCompliance(1);
        }
        else{
          pollutant.setCompliance(2);
        }
      };

      unfilteredData.push_back(pollutant);

      if (progressBar) {
        processedRows++;
        int progress = static_cast<int>((static_cast<double>(processedRows) / rowCount) * 100);
        QMetaObject::invokeMethod(progressBar, "setValue", Qt::QueuedConnection, Q_ARG(int, progress));
        }
      
    }
    isDataLoaded = true;
    this->filter(filter);
  }
}

void PollutantDataset::filter(const PollutantFilter &filter)
{
  std::vector<Pollutant> filteredData;

    for (const auto& q : unfilteredData)
    {
        bool matchesFilter = true;

        if (filter.location != "ALL LOCATIONS")
        {
            if (!filter.location.empty() && filter.location != q.getSite())
            {
              matchesFilter = false;
            }
        }

        if (!filter.compliance.empty() && filter.compliance != q.getQualifier())
        {
          matchesFilter = false;
        }

        if (!filter.level.empty())
        {
            // need to make result numeric as is currently a string
            double levelFilter = stod(filter.level);
            if (q.getResult() != levelFilter)
            {
              matchesFilter = false;
            }
        }

        //now deal with dates
        // convert the datetime string to just a date
        std::string dateStr = q.getTime().substr(0, 10);
        if (dateStr < filter.dateEnd || dateStr > filter.dateStart)
        {
          matchesFilter = false;
        }

        if (q.getCategory() == -1)
        {
          matchesFilter = false;
        }

        if (matchesFilter)
        {
          filteredData.push_back(q);
        }
    }
  data = std::move(filteredData);
}


void PollutantDataset::checkDataExists() const
{
  if (size() == 0) {
    throw std::runtime_error("Dataset is empty!");
  }
}