/* 
 * File:   Trajectory.cpp
 * Author: dbf
 * 
 * Created on 4. Februar 2012, 16:58
 */

#include "Trajectory.h"

#include "iostream"

namespace iNumerics {

    Trajectory::Trajectory() {
    }

    Trajectory::Trajectory(const Trajectory& orig) {
    }

    Trajectory::~Trajectory() {
    }

    void Trajectory::operator()(const DVec& x, const double t) {
        _states.push_back(x);
        _times.push_back(t);
    }

    double Trajectory::getTime(std::size_t i) {
        return _times[i];
    }
    
    const DVec& Trajectory::getState(std::size_t i) {
        return _states[i];
    }
    
    size_t Trajectory::size() const {
        return _times.size();
    }
    
}