#ifndef SRC_DUMPER_STATISTICS_HPP
#define SRC_DUMPER_STATISTICS_HPP

#include <map>
#include <ostream>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>


class Statistics {
    using Data = std::map<std::string, unsigned>;

    Data currentData, globalData;
    std::ostream& file;
    bool headerPrinted = false;
    boost::posix_time::ptime startTime, nextPrintTime;
    boost::posix_time::time_duration printInterval;

    void print(boost::posix_time::ptime time, const Data& statistics);

public:

    Statistics(std::ostream& file, boost::posix_time::time_duration printInterval):
        file(file), printInterval(printInterval)
    {}
    void init();
    void update(const std::string& key);
    void printGlobal();
};


#endif /* SRC_DUMPER_STATISTICS_HPP */
