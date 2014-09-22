#ifndef TIMEMETER_H_
#define TIMEMETER_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <ctime>

class TimeMeter {
	std::clock_t beginProcessorTime_;
	boost::posix_time::ptime beginRealTime_;
public:
	TimeMeter():
		beginProcessorTime_(std::clock()),
		beginRealTime_(boost::posix_time::microsec_clock::universal_time())
	{}
	boost::posix_time::time_duration processorTime()
	{
		std::clock_t value = std::clock() - beginProcessorTime_;
		return boost::posix_time::millisec(value / (CLOCKS_PER_SEC / 1000));
	}
	boost::posix_time::time_duration realTime()
	{
		return boost::posix_time::microsec_clock::universal_time() - beginRealTime_;
	}
};



#endif /* PROCESSORTIMECALCULATOR_H_ */
