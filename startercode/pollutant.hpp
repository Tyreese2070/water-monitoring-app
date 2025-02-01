
#pragma once

#include <string>
#include <iostream>



class Pollutant
{
  public:
    Pollutant(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, double, double, double, double, double, int, int);
    std::string getTime() const { return date_time; }
    std::string getSite() const { return site_name; }
    std::string getPollutant() const { return pollutant_name; }
    std::string getUnit() const { return unit; }
    std::string getType() const { return type; }    
    std::string  getQualifier() const { return result_qualifier; }
    double getEasting() const { return easting; }
    double getNorthing() const { return northing; }
    double getResult() const { return result; }
    double getLimit() const { return limit; }
    double getNearLimit() const { return near_limit; }
    int getCategory() const {return category; }
    int getCompliance() const { return compliance; }

    void setLimit(double limit) { this->limit = limit; }
    void setNearLimit(double near_limit) { this->near_limit = near_limit; }
    void setCategory(int category) { this->category = category; }
    void setCompliance(int compliance) { this->compliance = compliance; }

    bool operator<(const Pollutant& other) const {
        return pollutant_name < other.pollutant_name;
    }
    
  private:
    std::string site_name;
    std::string date_time;
    std::string pollutant_name;
    std::string unit;
    std::string type;
    std::string result_qualifier;
    double result;
    double easting;
    double northing;
    double limit;
    double near_limit;
    int category;
    //0 = pop, 1 = common, 2 = pfa, 3 = litter
    int compliance;
    //0 = non compliant, 1 = near compliant, 2 = compliant
};

std::ostream& operator << (std::ostream&, const Pollutant&);