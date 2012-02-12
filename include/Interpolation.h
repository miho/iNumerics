/* 
 * File:   Interpolation.h
 * Author: dbf
 * 
 * IDea from: http://www.bnikolic.co.uk/blog/cpp-map-interp.html
 *
 * Created on 6. Februar 2012, 18:19
 */

#ifndef INTERPOLATION_H
#define	INTERPOLATION_H

#include <map>

namespace iNumerics {

    class Interpolation {
    public:
        Interpolation(const TimeSeries& data);
//        Interpolation(const Interpolation& orig);
        virtual ~Interpolation();

        const double operator()(double t) const;

    private:

        std::map<double,double> _data;
    };

}

#endif	/* INTERPOLATION_H */

