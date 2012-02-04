/* 
 * File:   ODESolver.cpp
 * Author: dbf
 * 
 * Created on 4. Februar 2012, 16:51
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
