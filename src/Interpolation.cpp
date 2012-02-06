/* 
 * File:   Interpolation.cpp
 * Author: dbf
 * 
 * Created on 6. Februar 2012, 18:19
 */

#include "Trajectory.h"


#include "Interpolation.h"

namespace iNumerics {

    Interpolation::Interpolation(const TimeSeries& data) {
        for (size_t i = 0; i < data.size(); i++) {
            _data[data[i].first] = data[i].second;
        }
    }

    Interpolation::Interpolation(const Interpolation& orig) {
    }

    Interpolation::~Interpolation() {
    }

    const double Interpolation::operator()(double t) const {

        typedef std::map<double, double>::const_iterator i_t;

        i_t i = _data.upper_bound(t);
        if (i == _data.end()) {
            return (--i)->second;
        }
        if (i == _data.begin()) {
            return i->second;
        }
        i_t l = i;
        --l;

        const double delta = (t - l->first) / (i->first - l->first);
        return delta * i->second + (1 - delta) * l->second;
    }

}
