/// \file   intypes.h
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains typedefs.

typedef long int inInt; //this one is used for Fortran functions. On 32-bit Sytems this is equal to int but may differ on 64-bit systems. That is the reason why there are inInt and inLong even if they are equal.
typedef long int inLong;
typedef unsigned long int inULong;
typedef double inDouble;
