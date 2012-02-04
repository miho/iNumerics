/* 
 * File:   ODESolver.h
 * Author: dbf
 *
 * Created on 4. Februar 2012, 16:51
 */

#ifndef ODESOLVER_H
#define	ODESOLVER_H

#include "iNumerics.h"
#include "Trajectory.h"
#include "Problem.h"

namespace iNumerics {
    
    // forward declaration
    class Problem;

    class ODESolver {
        
    public:
        ODESolver();
        ODESolver(const ODESolver& orig);
        virtual ~ODESolver();

        void solve(Problem& problem, Trajectory& trajectory);

    private:

    };

}

#endif	/* ODESOLVER_H */

