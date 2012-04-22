/// \file   instopwatch.h
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the declaration of a stopwatch.

#ifndef INSTOPWATCH_H
#define INSTOPWATCH_H

#include <time.h>

/**
 * \brief iNumerics Standard Namespace
 */
namespace inumerics
{
	/**
	 * \brief Is a stopwatch.
	 */
	class StopWatch
	{
		protected:
			bool     running;
			clock_t  last_time;
			double   total_time;


		public:
			StopWatch();

			void  reset();
			void  start();
			double stop() ;
			double read() const;
	};
}
#ifndef INSTOPWATCH_HPP
//#define INCLUDED_IN_SNSTOPWATCH_H
#include "instopwatch.hpp"
//#undef INCLUDED_IN_SNSTOPWATCH_H
#endif /*INSTOPWATCH_HPP*/
#endif /*INSTOPWATCH_H*/
