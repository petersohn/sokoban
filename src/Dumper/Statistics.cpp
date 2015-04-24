#include "Dumper/Statistics.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/map.hpp>

void Statistics::update(const std::string& key)
{
    if (startTime == boost::posix_time::not_a_date_time) {
        init();
    }

    ++globalData[key];
    ++currentData[key];

    auto currentTime = boost::posix_time::microsec_clock::universal_time();
    if (currentTime > nextPrintTime) {
        print(nextPrintTime, currentData);
        nextPrintTime += printInterval;

        for (auto& value: currentData) {
            value.second = 0;
        }
    }
}

void Statistics::print(boost::posix_time::ptime time, const Data& statistics)
{
    if (!headerPrinted) {
        file << "time;" <<
            boost::algorithm::join(statistics | boost::adaptors::map_keys, ";") <<
            std::endl;

        headerPrinted = true;
    }

    if (time != boost::posix_time::not_a_date_time) {
        file << time - startTime;
    }

    file << ";" << boost::algorithm::join(statistics | boost::adaptors::map_values |
                boost::adaptors::transformed(
                    boost::lexical_cast<std::string, unsigned>), ";") <<
            std::endl;
}

void Statistics::printGlobal()
{
    print(boost::posix_time::not_a_date_time, globalData);
}

void Statistics::init()
{
    startTime = boost::posix_time::microsec_clock::universal_time();
    nextPrintTime = startTime + printInterval;
}
