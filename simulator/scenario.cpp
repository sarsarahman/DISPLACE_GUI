#include "scenario.h"
#include <tseries/timeseriesmanager.h>
#include <tseries/timeseries.h>

#include <dirent.h>

#include <string>
#include <iostream>
#include <fstream>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace displace::simulation;

Scenario *Scenario::mInstance = 0;

Scenario::Scenario()
    : mTsManager(new TimeSeriesManager)
{

}

Scenario *Scenario::instance()
{
    if (mInstance == 0)
        mInstance = new Scenario();
    return mInstance;
}

bool Scenario::loadTimeSeries(std::string path, std::string tsscenario)
{
    std::string suffix = tsscenario + ".dat";

    DIR *dir;
    dirent *dirent;

    int nr = 0;
    dir = opendir(path.c_str());
    if (dir) {
        while ((dirent = readdir(dir))) {
            bool isfile = true;
#ifndef __WIN32
            isfile = dirent->d_type == DT_REG;
#endif
            std::string name = std::string(dirent->d_name);

//            if (isfile && name.length() >= suffix.length() && 0 == name.compare (name.length() - suffix.length(), suffix.length(), suffix)) {
            if (isfile && boost::algorithm::ends_with(name, suffix)) {
                bool res = false;
                try {
                    res = readTsFile (path + "/" + name);
                } catch (std::exception &x) {
                    std::cerr << "Exception thrown form readFile(" << path << "/" << name << ") : " << x.what() << std::endl;
                    res = false;
                }

                if (res) {
                    ++nr;
                } else {
                    std::cerr << "Can't read " << dirent->d_name << std::endl;
                }
            }
        }

        closedir(dir);
    } else {
        std::cerr << "Can't open " << path << std::endl;
        return false;
    }

    return true;
}

bool Scenario::readTsFile (std::string filename) throw (boost::bad_lexical_cast)
{
    std::cout << "@DEBUG: Reading TimeSeries file " << filename << std::endl;

    int f = filename.find_last_of("/");
    int e = filename.find_last_of(".");
    std::string name = filename.substr(f+1, e-f-1);

    // parse name to extract TS parameters
    std::vector<std::string> fields;
    boost::split(fields, name, boost::is_any_of("-"));

    std::cout << "@DEBUG: " << fields.size();
    for (int i = 0; i < fields.size(); ++i)
        std::cout << " " << fields[i];
    std::cout << std::endl;

    displace::simulation::TimeSeriesManager::Variables var;
    if (fields[0] == "fishprice")
        var = displace::simulation::TimeSeriesManager::Fishprice;
    else if (fields[0] == "fuelprice")
        var = displace::simulation::TimeSeriesManager::Fuelprice;
    else if (fields[0] == "wspeed")
        var = displace::simulation::TimeSeriesManager::WSpeed;
    else {
        std::cerr << "Cannot parse file: variable field unknown " << fields[0] << std::endl;
        return false;
    }

    int zone;
    if (fields[1] == "all_area") {
        zone = displace::simulation::TimeSeriesManager::ALL_ZONES;
    } else {
        zone = boost::lexical_cast<int>(fields[1]);
    }

    int adim = boost::lexical_cast<int>(fields[2]);

    std::ifstream stream;
    stream.open(filename.c_str(), std::ios_base::in);
    if (stream.fail()) {
        return false;
    }

    int val_line = 0;

    boost::shared_ptr<TimeSeries> ts (new TimeSeries);

    while (!stream.eof()) {
        std::string line;
        std::getline(stream, line);
        if (stream.eof())
            break;

        if (boost::algorithm::starts_with(line, "#Version")) {
        } else if (boost::algorithm::starts_with(line, "#") || boost::algorithm::starts_with(line, "\"")) {
            // ignore
        } else {
            // it's a valid line
            if (val_line <= 2) {
                if (boost::algorithm::starts_with(line, "NA")) {
                    ++val_line;
                    continue;   // skip if NA.
                }
            }

            double v = boost::lexical_cast<double>(line);

            switch (val_line) {
            case 0: // threshold 0
                ts->setThreshold(0, v);
                break;
            case 1:
                ts->setThreshold(1, v);
                break;
            case 2:
                ts->setThreshold(2, v);
                break;
            default:
                ts->appendValue(v);
                break;
            }

            ++val_line;
        }
    }

    mTsManager->addTimeSerie(var, zone, adim, ts);

    return true;
}
