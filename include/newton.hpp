
#include <string>
#include <sstream>

#include "invector.h"
#include "inmatrix.h"
#include "inbyte.h"

#include "newton.h"

using namespace inumerics;
using namespace std;

typedef Matrix<inDouble> DMat;
typedef Vector<inDouble> DVec;

DMat Jacobian( const DVec& x )
{
	// create temporary matrix
	DMat J( 2, 2, true );
    
	// compute...
	J( 0, 0 ) = 6 * x( 0 ) * x( 0 );
	J( 0, 1 ) = -2 * x( 1 );
	J( 1, 0 ) = x( 1 ) * x( 1 ) * x( 1 );
	J( 1, 1 ) = 3 * x( 0 ) * x( 1 ) * x( 1 ) - 1;
    
	return J;
}

DVec f( const DVec& x )
{
	// create temporary vector
	DVec y( 2, true );
	
	// compute...
	y( 0 ) = 2 * x( 0 ) * x( 0 ) * x( 0 ) - x( 1 ) * x( 1 ) - 1;
	y( 1 ) = x( 0 ) * x( 1 ) * x( 1 ) * x( 1 ) - x( 1 ) - 4;
    
	return y;
}

std::string newton()
{
    stringstream ss;
    
	// initialize memory manager
	DVec::memCheck.initialize( MByte( 1.0 ), inumerics::Byte( 0.0 ) );
    
	// init TOL
	inDouble TOL = 1e-12;
	inDouble norm = 999.0;
    
	// init counter
	unsigned int i = 0;
    
	// init...
	DVec x_0( 2 );
	DVec x_1( 2 );
	DVec Dx( 2 );
	DMat J;
    
	// initial guess
	x_0( 0 ) = 1;
	x_0( 1 ) = 2;
    
	// output
	ss << "Iteration" << "\t x1" << "\t\tx2\n\n";	
	
	while ( norm >= TOL )
	{
		// compute Jacobian
		J = Jacobian( x_0 );
		
		// solve
		Dx = J | f( x_0 );
        
		// step
		x_1 = x_0 - Dx;
        
		// update x_0 and norm
		x_0 = x_1;
		norm = Dx.norm2();
        
		// output
		ss << i << "\t\t" << x_1 << endl;
		
		// inc counter
		i++;
	}
    
	return ss.str();
}


