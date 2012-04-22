/// \file   instopwatch.cpp
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the definition of a stopwatch.

#define INSTOPWATCH_HPP

#include "instopwatch.h"

namespace iNumerics
{

	StopWatch::StopWatch()
	{
		running = 0;
		last_time = 0;
		total_time = 0;
	}

	void StopWatch::reset()
	{
		running = 0;
		last_time = 0;
		total_time = 0;
	}

	void StopWatch::start()
	{
		if ( !running )
		{
			last_time = clock();
			running = true;
		}
	}

	double StopWatch::stop()
	{
		if ( running )
		{
			double s = ( double ) ( clock() - last_time ) / ( double ) CLOCKS_PER_SEC;
			while ( s<0 ) s+= ( double ) ( ( clock_t ) ( -1 ) ) / ( double ) CLOCKS_PER_SEC;
			total_time+=s;
			running = 0;
		}
		return total_time;
	}

	double StopWatch::read() const
	{
		if ( running ) return -1;
		return total_time;
	}
}




