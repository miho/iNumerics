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

#ifndef PROBLEM_H
#define	PROBLEM_H

#include <iostream>

#include "Types.h"



namespace iNumerics {
    
    typedef void (*RHS_FUNC)( const DVec &x , DVec &dxdt , const double t );
    
    /*class RHS {
    public:
        virtual void operator() ( const DVec &x , DVec &dxdt , const double t ){};
    };*/

    // forward declaration
    class ODESolver;
    
    class Problem {
        
        friend class ODESolver;
        
    public:
        Problem();
//        Problem(const Problem& orig);
        virtual ~Problem();

        Problem& setRhs(RHS_FUNC rhs);
        
        Problem& setInitialValue(DVec init);
    
        Problem& setTimeRange(double t0, double tn);
        
        Problem& setPrecision(double absError, double relError, double h = 0.1);

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

