#include <stdexcept>
#include <sstream>
#include "pollutant.hpp"

using namespace std;

Pollutant::Pollutant(const string& snm, const string& dt, const string& pnm, const string& un, const string& tp, const string& rq, double rs, double ea, double no, double lim, double nlim, int cat, int com):
  site_name(snm), date_time(dt), pollutant_name(pnm), result_qualifier(rq), result(rs), unit(un), type(tp), easting(ea), northing(no), limit(lim), near_limit(nlim), category(cat), compliance(com)
{
    ostringstream error;

    // if (result < 0.0) {
    //     error << "Invalid result: " << result;
    //     throw out_of_range(error.str());
    // }

    // if (easting < 0.0) {
    //     error << "Invalid easting: " << easting;
    //     throw out_of_range(error.str());
    // }

    // if (northing < 0.0) {
    //     error << "Invalid northing: " << northing;
    //     throw out_of_range(error.str());
    // }
}