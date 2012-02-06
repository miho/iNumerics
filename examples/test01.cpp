/* 
 * File:   main.cpp
 * Author: dbf
 *
 * Created on 4. Februar 2012, 17:56
 */

#include <cstdlib>
#include <iostream>

#include "iNumerics.h"

using namespace std;
using namespace iNumerics;


void rhs(const DVec &x, DVec &dxdt, const double t) {
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - 0.15 * x[1];
}


/*
 * 
 */
int main(int argc, char** argv) {


    DVec x(2);
    x[0] = 1.0; // start at x=1.0, p=0.0
    x[1] = 0.0;


//    harm_osc rhs(0.15);

    Problem p;

    p.setInitialValue(x).
            setRhs(&rhs).
            setTimeRange(0.0, 10.0).
            setPrecision(1.0e-10, 1.0e-8);

    ODESolver solver;

    Trajectory t;

    solver.solve(p, t);

    for (size_t i = 0; i < t.size(); i++) {
        
        cout << t.getTime(i) << '\t' << t.getState(i)[0] << '\t' << t.getState(i)[1] << '\n';
    }

    return 0;
}

