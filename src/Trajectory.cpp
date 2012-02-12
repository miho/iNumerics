/*
 * Copyright 2012 Michael Hoffer <info@michaelhoffer.de>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY Michael Hoffer <info@michaelhoffer.de> "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Michael Hoffer <info@michaelhoffer.de> OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of Michael Hoffer <info@michaelhoffer.de>.
 */

#include "Trajectory.h"

#include "iostream"

namespace iNumerics {

    Trajectory::Trajectory() {
    }

//    Trajectory::Trajectory(const Trajectory& orig) {
//        this->_times = orig._times;
//        this->_states = orig._states;
//    }

    Trajectory::~Trajectory() {
    }

    void Trajectory::operator()(const DVec& x, const double t) {
        _states.push_back(x);
        _times.push_back(t);
    }

    double Trajectory::getTime(std::size_t i) const {
        return _times[i];
    }

    const DVec& Trajectory::getState(std::size_t i) const {
        return _states[i];
    }

    size_t Trajectory::size() const {
        return _times.size();
    }

    double Trajectory::getMinTime() const {
        
        if (_times.size() == 0) {
            return 0;
        }

        return *std::min_element(_times.begin(), _times.end());
        
    }

    double Trajectory::getMaxTime() const {
        
        if (_times.size() == 0) {
            return 0;
        }

        return *std::max_element(_times.begin(), _times.end());
        
    }

    double Trajectory::getMinState(size_t i) const {
        
        if (_states.size() == 0) {
            return 0;
        }
        
        double result = _states[0][i];
        
        for (size_t j = 0; j < _states.size();j++) {
            if (_states[j][i] < result) {
                result = _states[j][i];
            }
        }
        
        return result;
    }

    double Trajectory::getMaxState(size_t i) const {
        
        if (_states.size() == 0) {
            return 0;
        }
        
        double result = _states[0][i];
        
        for (size_t j = 0; j < _states.size();j++) {
            if (_states[j][i] > result) {
                result = _states[j][i];
            }
        }
        
        return result;
    }

}