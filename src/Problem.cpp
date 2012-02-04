/* 
 * File:   Problem.cpp
 * Author: dbf
 * 
 * Created on 4. Februar 2012, 17:22
 */

#include "Problem.h"


namespace iNumerics {

    Problem::Problem() {
        _absError = 1.e-10;
        _relError = 1.e-6;
    }

    Problem::Problem(const Problem& orig) {
    }

    Problem::~Problem() {
    }
    
    Problem& Problem::setInitialValue(DVec init) {
        _init = init;
        
        return *this;
    }
    
    Problem& Problem::setRHS(RHS_FUNC rhs) {
        _rhs = rhs;
        
        return *this;
    }
    
    Problem& Problem::setTimeRange(double t0, double tn) {
        _t0 = t0;
        _tn = tn;
        
        return *this;
    }
    
    Problem& Problem::setPrecition(double absError, double relError, double h) {
        
        _absError = absError;
        _relError = relError;
        _h = h;
        
        return *this;
    }
    
}

