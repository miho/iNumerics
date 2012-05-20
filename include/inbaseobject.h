/// \file   invector.h
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the declaration of the vector class.

#ifndef INBASEOBJECT_H
#define INBASEOBJECT_H

#include <iostream>
#include <string>
// #include <iomanip>
// #include <sstream>
// #include <fstream>
#include <cmath>

#include "intypes.h"

namespace iNumerics {

    /**
     *	\author Michael Hoffer, 2012
     *	\brief BaseObject-Class
     *	
     *	This class has no real functionality. It only exists to ensure that all template
     *	instances of Vector and Matrix use the same static members ::_objCounter and ::_objIDCounter.
     */
    class BaseObject {
    protected:
        static inULong _objCounter;
        static inULong _objIDCounter;
    };
}

#endif /*INBASEOBJECT_H*/
