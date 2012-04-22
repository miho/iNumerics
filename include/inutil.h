/// \file   inutil.h
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains macros for better Debug-Output.

#include <iostream>

#ifndef IN_UTIL
#define IN_UTIL
/**
* Macros for better Debug-Output.
* \author Michael Hoffer, 2012
*/

#ifdef NDEBUG
#define IN_ASSERT(x,y)
#elif DEBUG == 0
#define IN_ASSERT(x,y) \
if ( y == 0 ) \
{ \
	if( !( x ) ) \
{ \
		std::cout << "\nDEBUG-Level: 0\n"; \
		std::cout << "\nERROR: Assertion (" << #x << ") failed\n"; \
		std::cout << "\t>> in line " << __LINE__ << "\n"; \
		std::cout << "\t>> in File " << __FILE__ << "\n\n"; \
} \
}
#elif DEBUG == 1
#define IN_ASSERT(x,y) \
if ( y <= 1 ) \
{ \
	if( !( x ) ) \
	{ \
		std::cout << "\nDEBUG-Level: 1\n"; \
		std::cout << "\nERROR: Assertion (" << #x << ") failed\n"; \
		std::cout << "\t>> in line " << __LINE__ << "\n"; \
		std::cout << "\t>> in File " << __FILE__ << "\n\n"; \
	} \
}
#elif DEBUG == 2
#define IN_ASSERT(x,y) \
if ( y <= 2 ) \
{ \
	if( !( x ) ) \
	{ \
		std::cout << "\nDEBUG-Level: 2\n"; \
		std::cout << "\nERROR: Assertion (" << #x << ") failed\n"; \
		std::cout << "\t>> in line " << __LINE__ << "\n"; \
		std::cout << "\t>> in File " << __FILE__ << "\n\n"; \
	} \
}
#elif DEBUG == 3
#define IN_ASSERT(x,y) \
if ( y <= 3 ) \
{ \
	if( !( x ) ) \
	{ \
		std::cout << "\nDEBUG-Level: 3\n"; \
		std::cout << "\nERROR: Assertion (" << #x << ") failed\n"; \
		std::cout << "\t>> in line " << __LINE__ << "\n"; \
		std::cout << "\t>> in File " << __FILE__ << "\n\n"; \
	} \
	else \
	{ \
		std::cout << "\nDEBUG-Level: 3\n"; \
		std::cout << "Assertion (" << #x << ") OK\n\n"; \
	} \
}
#endif

#ifdef NDEBUG
#define IN_DISPLAY( x, y )
#elif DEBUG == 0
#define IN_DISPLAY( x, y ) \
	if ( y == 0) std::cout << x << std::endl;
#elif DEBUG == 1
#define IN_DISPLAY( x, y ) \
	if ( y <= 1) std::cout << x << std::endl;
#elif DEBUG == 2
#define IN_DISPLAY( x, y ) \
	if ( y <= 2) std::cout << x << std::endl;
#elif DEBUG == 3
#define IN_DISPLAY( x, y ) \
	if ( y <= 3) std::cout << x << std::endl;
#endif

#define REACHED printf("REACHED %s:%d\n", __FILE__, __LINE__);

/**
 * \brief iNumerics Standard Namespace
 */
namespace iNumerics
{
	/**
	 * Returns the absolute value of number.
	 *
	 * @param number 	Skalar or something equal
	 * @return 		absolute value
	 */
	template <class T>
	inline T absValue ( const T& number )
	{
		T abs = 0;

		if ( number < abs )
		{
			return -1*number;
		}
		else
		{
			return number;
		}
	}

}
#endif /*IN_UTIL*/
