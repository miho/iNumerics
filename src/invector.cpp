#include "invector.h"

namespace iNumerics {

// This section is for completely SPECIALIZED TEMPLATE methods only!
//#ifndef INCLUDED_IN_INVECTOR_H

	/******************************************************************************
	*	                                                                      *
	*       Class: iNumerics::Vector (PUBLIC-MEMBERS, Template Specialization)        *
	*                                                                             *
	******************************************************************************/

	template <>
	void Vector<inDouble>::deepswap ( Vector<inDouble>& B )
	{
		IN_DISPLAY ( "(" << B._objID << ") Vector<inDouble>::deepswap ( Vector<inDouble>& B )",1 );
// 		IN_ASSERT ( this->stride() == B.stride(), 0 );
		IN_ASSERT ( this->size() == B.size(), 0 );
		IN_ASSERT ( this->size() <= INT_MAX, 0 )

		inInt n=B.size();
		inInt inc_A = this->stride();
		inInt inc_B = B.stride();//stepsize of the vector

		inDouble * _B = B.getFVector();
		inDouble * _A = this->getFVector();

		dswap_ ( &n, _B, &inc_B, _A,  &inc_A );
	}

	template <>
	inDouble Vector<inDouble>::norm2 ( )
	{
		IN_DISPLAY ( "(" << this->_objID << ") Vector<inDouble>::norm2 ( )",1 );

		inInt incx = 1/*this->stride()*/;
		inInt n = this->size();
		inDouble * _x = this->getFVector();

		return dnrm2_ ( &n, _x, &incx );
	}

	template <>
	inLong Vector<inLong>::norm2 ( )
	{
		IN_DISPLAY ( "(" << this->_objID << ") Vector<inLong>::norm2 ( )",1 );

		inULong dotprod = 0;

		for ( inULong j = 0;j < this->size();j++ )
		{
			dotprod += ( *this ) ( j ) * ( *this ) ( j );
		}

		dotprod = ( inULong ) sqrt ( dotprod );

		return dotprod;
	}

	template <>
	const inDouble Vector<inDouble>::dotprod ( const Vector<inDouble>& B )
	{
		IN_DISPLAY ( "(" << this->_objID << ") and (" << B._objID << ") Vector<inDouble>::dotprod ( const Vector<inDouble>& B )",1 );
// 		IN_ASSERT ( this->stride() == B.stride(), 0 );
		IN_ASSERT ( this->size() == B.size(), 0 );
		IN_ASSERT ( this->size() <= INT_MAX, 0 );

		inInt n = B.size();
		inInt incA = this->stride();
		inInt incB = B.stride();

		inDouble * _B = B.getFVector();
		inDouble * _A = this->getFVector();

		return ddot_ ( &n, _A,  &incA,  _B,  &incB );
	}

	template <>
	const inDouble Vector<inDouble>::sumAbsValues()
	{
		IN_DISPLAY ( "(" << this->_objID << ") Vector<inDouble>::sumAbValue()",1 );
		IN_ASSERT ( this->size() <= INT_MAX, 0 );

		inDouble * _A = this->getFVector();
		inInt incA = this->stride();
		inInt n = this->size();

		return dasum_ ( &n, _A, &incA );
	}

	template <>
	const inULong Vector<inDouble>::maxElemIndex()
	{
		IN_DISPLAY ( "(" << this->_objID << ") Vector<inDouble>::maxElemIndex()",1 );
		IN_ASSERT ( this->size() <= INT_MAX, 0 );

		inDouble * _A = this->getFVector();
		inInt incA = this->stride();
		inInt n = this->size();

		return idamax_ ( &n, _A, &incA )-1;
	}

	template <>
	void Vector<inDouble>::deepCopy ( const Vector<inDouble>& M )
	{
		// Debug-Output
		IN_DISPLAY ( "("<< _objID << ") >> \"DEEP COPY\"",2 );

		_vecType = PLAIN_VEC;
		_stride = 1;
		_size = M.size();
		_memDimRows = _size;

		inInt sizeM = M.size();
		inInt strideM = M._stride;

		inInt strideT=_stride;

		memCheck.allocMem ( _mem, M.size() );

		inDouble * _M = M.getFVector();
		inDouble * _A = this->getFVector();

		dcopy_ ( &sizeM, _M, &strideM, _A, &strideT );
	}

	/******************************************************************************
	*	                                                                      *
	*       Class: iNumerics::Vector (FRIENDS, Template Specialization)               *
	*                                                                             *
	******************************************************************************/
	template <>
	Vector<inDouble> operator+ ( const Vector<inDouble>& A, const Vector<inDouble>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator+ <inDouble> (const Vector<inDouble>& A, const Vector<inDouble>& B)",1 );
// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );
// 		IN_ASSERT ( A.size() <= INT_MAX, 0 );


		inInt m = B.size();
		//inInt k = 1/*B.stride()*/;

		Vector<inDouble> C ( m, true );

		inDouble * _B = B.getFVector();
		inDouble * _A = A.getFVector();

		inDouble * _C = C.getFVector();

		inDouble da = 1;//adds the da-times of B to C

		inInt inc_A = A.stride();
		inInt inc_B = B.stride();// stepsize of the Vector
		inInt inc_C = 1/*C.stride()*/;// =

		daxpy_ ( &m, &da ,_B ,&inc_B ,_C ,&inc_C );
		daxpy_ ( &m, &da ,_A ,&inc_A ,_C ,&inc_C );
		return C;
	}

	template <>
	Vector<inDouble> operator- ( const Vector<inDouble>& A, const Vector<inDouble>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator- <inDouble> (const Vector<inDouble>& A, const Vector<inDouble>& B)",1 );

// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );
// 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

		inInt m = B.size();
// 		inInt k = 1/*B.stride()*/;

		Vector<inDouble> C ( m, true );

		inDouble * _B = B.getFVector();
		inDouble * _A = A.getFVector();

		inDouble * _C = C.getFVector();

		inDouble db = -1;//adds the db-times of B to C
		inDouble da = 1;//adds the da-times of A to C

		inInt inc_A = A.stride();
		inInt inc_B = B.stride();// stepsize of the Vector
		inInt inc_C = 1/*C.stride()*/;// =

		daxpy_ ( &m, &db ,_B ,&inc_B ,_C ,&inc_C );
		daxpy_ ( &m, &da ,_A ,&inc_A ,_C ,&inc_C );
		return C;
	}

	template <>
	Vector<inDouble> operator* ( const Vector<inDouble>& A, const inDouble& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator* <inDouble> (const Vector<inDouble>& A, const inDouble& s)",1 );
// 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

		inInt m = A.size();
// 		inInt k = 1/*A.stride()*/;

		Vector<inDouble> C ( m, true );
		inInt incC = 1/*C.stride()*/; //stepsize of the Vector

		C = A;

		inDouble * _C = C.getFVector();

		dscal_ ( &m, &s, _C, &incC );

		return C;
	}

	template <>
	Vector<inDouble>& operator*= ( Vector<inDouble>& A, const inDouble& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator*= <inDouble> (const Vector<inDouble>& A, const inDouble& s)",1 );
// 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

		inInt m = A.size();
		inInt incA = A.stride(); //stepsize of the Vector

		inDouble * _A = A.getFVector();

		dscal_ ( &m, &s, _A, &incA );

		return A;
	}

	template <>
	Vector<inDouble>& operator+= ( Vector<inDouble>& A, const Vector<inDouble>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator+= <inDouble> (const Vector<inDouble>& A, const Vector<inDouble>& B)",1 );

// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );
// 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

		inInt m = B.size();

		inDouble * _B = B.getFVector();
		inDouble * _A = A.getFVector();

		inDouble da = 1;//adds the da-times of B to A

		inInt inc_A = A.stride();
		inInt inc_B = B.stride();// stepsize of the Vector

		daxpy_ ( &m, &da ,_B ,&inc_B ,_A ,&inc_A );

		return A;
	}

	template <>
	Vector<inDouble>& operator-= ( Vector<inDouble>& A, const Vector<inDouble>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator-= <inDouble> (const Vector<inDouble>& A, const Vector<inDouble>& B)",1 );

// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );
// 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

		inInt m = B.size();

		inDouble * _B = B.getFVector();
		inDouble * _A = A.getFVector();

		inDouble da = -1;//adds the da-times of B to A

		inInt inc_A = A.stride();
		inInt inc_B = B.stride();// stepsize of the Vector

		daxpy_ ( &m, &da ,_B ,&inc_B ,_A ,&inc_A );

		return A;
	}

	template <>
	Vector<inDouble> operator/ ( const Vector<inDouble>& A, const inDouble& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator/ <inDouble> (const Vector<inDouble>& A, const inDouble& s)",1 );
// 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

		inInt m = A.size();
		inDouble s1 = 1/s;

		Vector<inDouble> C ( m, true );
		inInt incC = 1/*C.stride()*/; //stepsize of the Vector

		C = A;

		inDouble * _C = C.getFVector();

		dscal_ ( &m, &s1, _C, &incC );

		return C;
	}

	template <>
	Vector<inDouble>& operator/= ( Vector<inDouble>& A, const inDouble& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator/= <inDouble> (const Vector<inDouble>& A, const inDouble& s)",1 );
// 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

		inInt m = A.size();
		inDouble s1 = 1/s;
		inInt incA = A.stride(); //stepsize of the Vector

		inDouble * _A = A.getFVector();

		dscal_ ( &m, &s1, _A, &incA );

		return A;
	}
}