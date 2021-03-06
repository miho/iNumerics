/* 
 * File:   main.cpp
 * Author: dbf
 *
 * Created on 4. Februar 2012, 17:56
 */

#include <cstdlib>
#include <iostream>

#include "iNumerics.h"

#include "inmatrix.h"
#include "invector.h"

using namespace std;
using namespace iNumerics;

class TestModel : public Model {
    
public:

    void rhs (const DVec &y, DVec &dydt, const double t) {
        dydt[0] = y[1];
        dydt[1] = -y[0] - 0.15 * y[1];
    }
    
    void step(const DVec &x, double t) {
        //
    }
};

/*
 * 
 */
int main(int argc, char** argv) {

    Matrix<inDouble>::memCheck.initialize(MByte(1.0), Byte(0));
    Vector<inDouble>::memCheck.initialize(MByte(1.0), Byte(0));


    DVec x(2);
    x[0] = 1.0; // start at x=1.0, p=0.0
    x[1] = 0.0;


    //    harm_osc rhs(0.15);

    TestModel model;
    
    Problem p(model);

    p.setInitialValue(x).
            // setRhs(&rhs).
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



//int main(int argc, char** argv) {
//
//
//    Matrix<inDouble>::memCheck.initialize ( MByte ( 1.0 ), Byte ( 0 ) );
//    Vector<inDouble>::memCheck.initialize ( MByte ( 1.0 ), Byte ( 0 ) );
//    Matrix<inDouble> A(2,2); // row-col
//    
//    A(0,0) = 2;
//    A(1,0) = 1;
//    A(0,1) = 3;
//    A(1,1) = 2;
//    
//    Vector<inDouble> b(2);
//    
//    b(0)=4;
//    b(1)=3;
//    
//    cout << "\n>> Solve A*x=b\n" << endl;
//    
//    cout << "A:\n" << A << endl;  
//    
//    cout << "b:\n" << b << endl; 
//    
//    Vector<inDouble> x = A | b;
//    
//    cout << "\nx:\n" << x << endl << endl;
//    
//    cout << "b (again):\n" << b << endl; 
//
//    return 0;
//}

