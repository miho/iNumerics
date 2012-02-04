/* 
 * File:   Problem.h
 * Author: dbf
 *
 * Created on 4. Februar 2012, 17:22
 */

#ifndef PROBLEM_H
#define	PROBLEM_H

#include "Types.h"

#include "ODESolver.h"

#include <iostream>

namespace iNumerics {
    
    typedef void (*RHS_FUNC)( const DVec &x , DVec &dxdt , const double t );
    
    /*class RHS {
    public:
        virtual void operator() ( const DVec &x , DVec &dxdt , const double t ){};
    };*/

    class Problem {
        
        friend class ODESolver;
        
    public:
        Problem();
        Problem(const Problem& orig);
        virtual ~Problem();

        Problem& setRHS(RHS_FUNC rhs);
        
        Problem& setInitialValue(DVec init);
    
        Problem& setTimeRange(double t0, double tn);
        
        Problem& setPrecition(double absError, double relError, double h = 0.1);
        
        Problem& setTrajectory(Trajectory& trajectory);
        
        void operator() ( const DVec &x , DVec &dxdt , const double t );

    private:

        RHS_FUNC _rhs;
        
        double _t0;
        double _tn;
        
        double _absError;
        double _relError;
        
        double _h;
        
        DVec _init;
        
    };

}

#endif	/* PROBLEM_H */

