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

#include "ODESolver.h"

#include <boost/numeric/odeint.hpp>

namespace iNumerics {

    class _StepObserver {
    public:
        Trajectory& _trajectory;

        _StepObserver(Trajectory& trajectory)
        : _trajectory(trajectory) {
        }

        void operator()(const DVec &x, double t) {
            _trajectory(x, t);
        }
    };
    
//    class _RHSImpl {
//    public:
//        RHS& _rhs;
//        
//        _RHSImpl(RHS& rhs)
//        : _rhs(rhs) {
//        }
//        
//        void operator() ( const DVec &x , DVec &dxdt , const double t ) {
//            _rhs(x, dxdt, t);
//        }
//    };

    ODESolver::ODESolver() {
    }

    ODESolver::ODESolver(const ODESolver& orig) {
    }

    ODESolver::~ODESolver() {
    }

    void ODESolver::solve(Problem& problem, Trajectory& trajectory) {

        using namespace boost::numeric::odeint;

        typedef runge_kutta_cash_karp54< DVec > error_stepper_type; // may change

        integrate_adaptive(
                make_controlled< error_stepper_type > (problem._absError, problem._relError),
                problem._rhs,
                problem._init,
                problem._t0,
                problem._tn,
                problem._h,
                _StepObserver(trajectory));
    }
}