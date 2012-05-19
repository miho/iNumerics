/// \file   invector.hpp
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the definition of the vector class.

#define INVECTOR_HPP
#include "invector.h"
#include"inbaseobject.h"

//#define DEBUG 2


namespace iNumerics
{
	/******************************************************************************
	*	                                                                      *
	*       Class: iNumerics::baseObject (STATIC-MEMBERS)                             *
	*                                                                             *
	******************************************************************************/

//#ifndef INCLUDED_IN_INVECTOR_H
	inULong BaseObject::_objCounter = 0;
	inULong BaseObject::_objIDCounter = 0;
//#endif

	/******************************************************************************
	*	                                                                      *
	*       Class: iNumerics::Vector (STATIC-MEMBERS)                                 *
	*                                                                             *
	******************************************************************************/

	template <class T>
	MemCollect<T> Vector<T>::memCheck;

	/******************************************************************************
	*	                                                                      *
	*       Class: iNumerics::Vector (PUBLIC-MEMBERS)                                 *
	*                                                                             *
	******************************************************************************/

	template <class T>
	Vector<T>::Vector()
	{
		// Setting ObjCounter and ObjIDCounter
		_objCounter++;
		_objIDCounter++;
		_objID = _objIDCounter;
		
		// Debug-Output
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::Vector() ",1 );
		
		// Initializing member variables
		_vecType = PLAIN_VEC;
		_mem = NULL;
		_size = 0;
		_stride = 1;
		_memDimRows = _size;
		_allowMemSharing = true;
	}

	template <class T>
	Vector<T>::Vector ( const Vector& M )
	{
		// Setting ObjCounter and ObjIDCounter
		_objCounter++;
		_objIDCounter++;
		_objID = _objIDCounter;
		
		
		// Debug-Output
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::Vector ( const Vector& M )",1 );
                
                
                this->_allowMemSharing = false;
		deepCopy ( M );
                
                return;
		
		// If input-object allows memory sharing, do flatCopy(),
		// deepCopy() otherwise
		if ( !M._allowMemSharing /*&& !this->_allowMemSharing*/ )
		{
			this->_allowMemSharing = false;
			deepCopy ( M );
		}
		else
		{
			this->_allowMemSharing = false;
			flatCopy ( M );
		}
	}
        

	template <class T>
	Vector<T>::Vector ( inULong n, bool allowMemSharing )
	{
		// Setting ObjCounter and ObjIDCounter
		_objCounter++;
		_objIDCounter++;
		_objID = _objIDCounter;
		
		// Debug-Output
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::Vector ( inULong n, bool allowMemSharing )",1 );

		// Initializing member variables
		_vecType = PLAIN_VEC;
		_size = n;
		_allowMemSharing = allowMemSharing;
		_stride = 1;
		_memDimRows = _size;

		// allocating memory
		memCheck.allocMem ( _mem, n );
	}


	template <class T>
	Vector<T>::Vector ( T* v, inULong n, inULong stride, bool shareMem , bool allowMemSharing )
	{
		// Setting ObjCounter and ObjIDCounter
		_objCounter++;
		_objIDCounter++;
		_objID = _objIDCounter;
		
		// Debug-Output
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::Vector ( T* v, inULong n, inULong stride , bool allowMemSharing )",1 );
		
		// Initializing member variables
		_vecType = PLAIN_VEC;
		_allowMemSharing = allowMemSharing;
		_size = n;

		if ( shareMem )
		{
			_stride = stride;
			_memDimRows = _size;

			// Debug-Output
			IN_DISPLAY ( "(" << _objID << ") >> FLAT COPY!",2 );

			// _mem is not controlled by memCheck
			// if destructor is called it will be deleted by memCheck
			_mem = new MemType<T> ( v, n*stride );
			_mem->addReference();
		}
		else
		{
			_stride = 1;
			_memDimRows = _size;

			// We need a temporary memType object, as we decided
			// not to perform any direct operations on the
			// memory
			MemType<T> tmpMem ( v, n*stride );

			// Allocating memory
			memCheck.allocMem ( _mem, n );

			// Debug-Output
			IN_DISPLAY ( "(" << _objID << ") >> DEEP COPY!",2 );

			//DeepCopy
			for ( inULong i = 0; i < _size;i++ )
			{
				( *_mem ) ( i ) = tmpMem ( i*stride );
			}
		}

	}

	template <class T>
	Vector<T>::Vector ( T* v, vecType vType ,inULong n,inULong memDimRows, inULong stride, bool shareMem , bool allowMemSharing )
	{
		// Setting ObjCounter and ObjIDCounter
		_objCounter++;
		_objIDCounter++;
		_objID = _objIDCounter;

		// Debug-Output
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::Vector ( T* v, vecType vType, inULong n, inULong memDimRows, inULong stride , bool allowMemSharing )",1 );

		// Initializing member variables
		_vecType = vType;
		_allowMemSharing = allowMemSharing;
		_size = n;

		if ( shareMem )
		{
			_stride = stride;
			_memDimRows = memDimRows;

			// Debug-Output
			IN_DISPLAY ( "(" << _objID << ") >> FLAT COPY!",2 );

			// _mem is not controlled by memCheck
			// if destructor is called it will be deleted by memCheck
			_mem = new MemType<T> ( v, n*stride );
			_mem->addReference();
		}
		else
		{
			_stride = 1;
			_memDimRows = _size;
			_vecType = PLAIN_VEC;

			// We need a temporary memType object, as we decided
			// not to perform any direct operations on the
			// memory
			MemType<T> tmpMem ( v, n*stride );

			// Allocating memory
			memCheck.allocMem ( _mem, n );

			// Debug-Output
			IN_DISPLAY ( "(" << _objID << ") >> DEEP COPY!",2 );

			//DeepCopy
			for ( inULong i = 0; i < _size;i++ )
			{
				( *_mem ) ( i ) = tmpMem ( i*stride );
			}
		}

	}

	template <class T>
	Vector<T>::~Vector()
	{
		// Debug-Output
		IN_DISPLAY ( "(" << _objID << ") Vector::Destructor()",1 );

		// Deleting memory, i.e.
		// passing memory on to MemCollect (freeMemList)
		memCheck.freeMem ( _mem );

		// Decrement object counter as object is being deleted.
		--_objCounter;

		// Debug-Output
		IN_DISPLAY ( "(" << _objID << ") >> Number of remaining Objects: " << _objCounter,2 );
	}

	template <class T>
	const T Vector<T>::operator() ( inULong i ) const
	{
		return ( *_mem ) ( i * stride() );
	}

	template <class T>
	T& Vector<T>::operator() ( inULong i )
	{
		return ( *_mem ) ( i * stride() );
	}

	template <class T>
	Vector<T>& Vector<T>::operator= ( const Vector<T>& M )
	{
		// Debug-Output
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::operator= ( const Vector<T>& M )",1 );

		// An object must not assigning itself
		if ( &M == this )
		{
			// Debug-Output
			IN_DISPLAY ( "(" << _objID << ") >> \"nothing to do!\"",2 );
		}
		else
		{
			// If input-object allows memory sharing, do flatCopy(),
			// deepCopy() otherwise
			if ( !M._allowMemSharing /*&& !this->_allowMemSharing*/ )
			{
				// Deleting memory, i.e.
				// passing memory on to MemCollect (freeMemList)
				memCheck.freeMem ( _mem );

				deepCopy ( M );
			}
			else
			{
				// Deleting memory, i.e.
				// passing memory on to MemCollect (freeMemList)
				memCheck.freeMem ( _mem );

				flatCopy ( M );
			}
		}
		return *this;
	}
        
        template <class T>
        Vector<T> Vector<T>::copy(bool allowMemSharing) const{
            Vector<T> result(size(),allowMemSharing);
            result.deepCopy(*this);
            return result;
        }

	template <class T>
	const inULong Vector<T>::size() const
	{
		return _size;
	}

	template <class T>
	const inULong Vector<T>::stride() const
	{
		return _stride;
	}

	template <class T>
	const inULong Vector<T>::memDimRows() const
	{
		return _memDimRows;
	}

// 	template <class T>
// 	MemType<T>* Vector<T>::getMem() const
// 	{
// 		return _mem;
// 	}

	template <class T>
	const inULong Vector<T>::ID() const
	{
		return _objID;
	}

	template <class T>
	T* Vector<T>::getFVector() const
	{
		return _mem->getMem();
	}

	template <class T>
	void Vector<T>::assign ( T *v, bool shareMem )
	{
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::assign ( T *v, inULong n, bool shareMem )",1 );
// 		IN_ASSERT ( size() == n, 0 );

		if ( shareMem )
		{
			// Debug-Output
			IN_DISPLAY ( "(" << _objID << ") >> FLAT COPY!",2 );

			memCheck.freeMem ( _mem );

			// _mem is not controlled by memCheck
			// if destructor is called it will be deleted by memCheck
			_mem = new MemType<T> ( v, size() );
			_mem->addReference();
		}
		else
		{
			// Debug-Output
			IN_DISPLAY ( "(" << _objID << ") >> DEEP COPY!",2 );

			//DeepCopy
			for ( inULong i = 0; i < size(); i++ )
			{
				( *this ) ( i ) = v[i];
			}
		}
	}


	template <class T>
	void Vector<T>::assign ( T *v,
	                         inULong memDimRows,
	                         inULong offset,
	                         inULong stride,
	                         bool shareMem )
	{
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::assign ( T *v, inULong memDimRows, inULong offset, inULong stride, bool shareMem )",1 );
// 		IN_ASSERT ( _size == n, 0 );

		if ( shareMem )
		{
			_memDimRows = memDimRows;
			_stride = stride;

			memCheck.freeMem ( _mem );

			// _mem is not controlled by memCheck
			// if destructor is called it will be deleted by memCheck
			_mem = new MemType<T> ( &v[offset], _size*stride );
			_mem->addReference();
		}
		else
		{
			//DeepCopy
			for ( inULong i = 0; i < _size;i++ )
			{
				( *this ) ( i ) = v [ offset + i*stride ];
			}
		}
	}

	template <class T>
	void Vector<T>::assign ( T *v,
	                         vecType vType,
	                         inULong memDimRows,
	                         inULong offset,
	                         bool shareMem )
	{
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::assign ( T *v, vecType vType, inULong memDimRows, inULong offset, bool shareMem )",1 );
// 		IN_ASSERT ( _size == n, 0 );

		// need to temporarily change _vecType and _memDimRows to match
		// array v. In case of deepCopy we have to set them back.
		vecType tmpVType = _vecType;
		inLong tmpMemDimRows = _memDimRows;

		// changing
		_vecType = vType;
		_memDimRows = memDimRows;
		inULong memOffset = getMemOffset ( offset );

		if ( shareMem )
		{
			_stride = getMemStride();

			memCheck.freeMem ( _mem );

			// _mem is not controlled by memCheck
			// if destructor is called it will be deleted by memCheck
			_mem = new MemType<T> ( &v[memOffset], _size*_stride );
			_mem->addReference();
		}
		else
		{
			// turning back values as Vector has seperate memory
			_vecType = tmpVType;
			_memDimRows = tmpMemDimRows;

			//DeepCopy
			for ( inULong i = 0; i < _size;i++ )
			{
				( *this ) ( i ) = v [ memOffset + i*_stride ];
			}
		}
	}




	template <class T>
	Vector<T> Vector<T>::getSubVector ( inULong startIndex, inULong size, bool shareMem )
	{
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::getSubVector ( inULong startIndex, inULong size, bool allowMemSharing )",1 );

		IN_ASSERT ( startIndex + size -1 < this->size(),0 );

		return Vector<T> ( &_mem->getMem() [ getMemOffset ( startIndex ) ], _vecType, size, _memDimRows, stride(), shareMem );
	}

	template <class T>
	void Vector<T>::print() const
	{
		std::cout << *this << std::endl;
	}


	/******************************************************************************
	*	                                                                      *
	*       Class: iNumerics::Vector ( EXTRA-METHODS TEMPLATE )                       *
	*                                                                             *
	******************************************************************************/

	template <class T>
	void Vector<T>::deepswap ( Vector<T>& B )
	{
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::deepswap ( Vector<T>& B )",1 );
// 		IN_ASSERT ( this->stride() == B.stride(), 1 );
		IN_ASSERT ( this->size() == B.size(), 1 );

		for ( inULong j = 0;j < B.size();j++ )
		{
			T temp;
			temp = ( *this ) ( j );
			( *this ) ( j ) =B ( j );
			B ( j ) =temp;
		}
	}

//	template <class T>
//	T Vector<T>::norm2 ( )
//	{
//		IN_DISPLAY ( "(" << _objID << ") Vector<T>::norm2 ( )",1 );
//
//		T dotprod = 0;
//
// 		for ( inULong j = 0;j < this->size();j++ )
// 		{
// 			dotprod += ( *this ) ( j ) * ( *this ) ( j );
// 		}
//
// 		dotprod = sqrt ( dotprod );
//
//		std::cerr <<"(" << _objID << ") >> ERROR: is available for \"inLong\" and \"inDouble\" only!" << std::endl;
//		exit(-1);
//
//		return dotprod;
//	}

	template <class T>
	const T Vector<T>::dotprod ( const Vector<T>& B )
	{
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::dotprod ( const Vector<T>& B )",1 );
// 		IN_ASSERT ( this->stride() == B.stride(), 1 );
		IN_ASSERT ( this->size() == B.size(), 1 );

		T dotprod = 0;

		for ( inULong j = 0;j < B.size();j++ )
		{
			dotprod += ( *this ) ( j ) *B ( j );
		}

		return dotprod;
	}

	template <class T>
	const T Vector<T>::sumAbsValues()
	{
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::sumAbsValues()",1 );

		T absValue = 0;

		for ( inULong j = 0;j < this->size();j++ )
		{
			absValue += absValue ( ( *this ) ( j ) );
		}

		return absValue;

	}

	template <class T>
	const inULong Vector<T>::maxElemIndex()
	{
		IN_DISPLAY ( "(" << _objID << ") Vector<T>::maxElemIndex()",1 );

		inULong index = 0;

		T max=0;

		for ( inULong i=0; i < ( this->size() ); i++ )
		{
			if ( max < absValue ( ( *this ) ( i ) ) )
			{
				max = absValue ( ( *this ) ( i ) );
				index = i;
			}
		}

		return index;
	}


	/******************************************************************************
	*	                                                                      *
	*       Class: iNumerics::Vector (PRIVATE-MEMBERS)                                *
	*                                                                             *
	******************************************************************************/

	template <class T>
	void Vector<T>::deepCopy ( const Vector<T>& M )
	{
		// Debug-Output
		IN_DISPLAY ( "("<< _objID << ") >> \"DEEP COPY\"",2 );

		_vecType = PLAIN_VEC;
		_size = M.size();
		_stride = 1;
		_memDimRows = _size;

		//Allocating memory
		memCheck.allocMem ( _mem, M.size() );

		//DeepCopy
		for ( inULong i = 0; i < M.size();i++ )
		{
			( *_mem ) ( i ) = M ( i );
		}
	}

	template <class T>
	void Vector<T>::flatCopy ( const Vector<T>& M )
	{
		// Debug-Output
		IN_DISPLAY ( "(" << _objID << ") >> \"FLAT COPY\"",2 );

		_vecType = M._vecType;
		_size = M.size();
		_stride = M._stride;
		_memDimRows = M._memDimRows;
		_mem = M._mem;
		_mem->addReference();

	}

	template <class T>
	const inULong Vector<T>::getMemOffset ( inULong i ) const
	{
		if ( _vecType == ROW_VEC )
		{
			return i*_memDimRows;
		}
		else if ( _vecType == COL_VEC )
		{
			return i;
		}
		else if ( _vecType == DIAG_VEC )
		{
			return i* ( _memDimRows+1L );
		}

		// if (_VecType == PLAIN_VEC)
		return i;
	}

	template <class T>
	const inULong Vector<T>::getMemStride ( ) const
	{
		if ( _vecType == ROW_VEC )
		{
			return _memDimRows;
		}
		else if ( _vecType == COL_VEC )
		{
			return 1;
		}
		else if ( _vecType == DIAG_VEC )
		{
			return _memDimRows+1L;
		}

		// if (_VecType == PLAIN_VEC)
		return 1;
	}

	/******************************************************************************
	*	                                                                      *
	*       Class: iNumerics::Vector (FRIENDS)                                        *
	*                                                                             *
	******************************************************************************/

// This section is for generic TEMPLATE operators only!

	template <class T>
	std::ostream& operator << ( std::ostream &os,const Vector<T> &obj )
	{
		os << "[";
		for ( inULong i = 0;i < obj.size();i++ )
		{
			if ( i < obj.size()-1 )
			{
				os << obj ( i ) << ",\t";
			}
			else
			{
				os << obj ( i );
			}
		}
		os << "]";

		return os;
	}


	template <class T>
	Vector<T> operator+ ( const Vector<T>& A, const Vector<T>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator+ <T> (const Vector<T>& A, const Vector<T>& B)" ,1 );
// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );

		inULong m = B.size();
// 		inULong k = 1 /*B.stride()*/ ;

		Vector<T> C ( m, true );

		for ( inULong j = 0;j < C.size(); j++ )
		{
			C ( j ) =  A ( j ) + B ( j );
		}

		return C;
	}


	template <class T>
	Vector<T> operator+ ( const Vector<T>& A, const T& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator+ <T> (const Vector<T>& A, const T& s)",1 );

		inULong m = A.size();
// 		inULong k = 1 /*A.stride()*/;

		Vector<T> C ( m,  true );

		for ( inULong j = 0;j < C.size(); j++ )
		{
			C ( j ) =  A ( j ) + s;
		}

		return C;
	}

	template <class T>
	Vector<T> operator- ( const Vector<T>& A, const Vector<T>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator- <T> (const Vector<T>& A, const Vector<T>& B)",1 );
// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );

		inULong m = B.size();
// 		inULong k = 1 /*B.stride()*/;

		Vector<T> C ( m, true );

		for ( inULong j = 0;j < C.size(); j++ )
		{
			C ( j ) =  A ( j ) - B ( j );
		}

		return C;
	}

	template <class T>
	Vector<T> operator- ( const Vector<T>& A, const T& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator- <T> (const Vector<T>& A, const T& s)",1 );

		inULong m = A.size();
		//inULong k = 1/*A.stride()*/;

		Vector<T> C ( m, true );

		for ( inULong j = 0;j < C.size(); j++ )
		{
			C ( j ) =  A ( j ) - s;
		}

		return C;
	}

	template <class T>
	T operator* ( const Vector<T>& A, const Vector<T>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator* <T> (const Vector<T>& A, const Vector<T>& B)",1 );
// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );

		inULong m = A.size();
		//inULong k = 1/*A.stride()*/;

		Vector<T> C ( m, true );

		for ( inULong j = 0;j < C.size(); j++ )
		{
			C ( j ) =  A ( j ) * B ( j );
		}

		return C;
	}

	template <class T>
	Vector<T> operator* ( const Vector<T>& A, const T& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator* <T> (const Vector<T>& A, const T& s)",1 );

		inULong m = A.size();
		//inULong k = 1/*A.stride()*/;

		Vector<T> C ( m, true );

		for ( inULong j = 0;j < C.size(); j++ )
		{
			C ( j ) =  A ( j ) * s;
		}

		return C;
	}

	template <class T>
	Vector<T>& operator*= ( Vector<T>& A, const Vector<T>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator*= <T> ( Vector<T>& A, const Vector<T>& B)",1 );
// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );

		for ( inULong j = 0;j < A.size(); j++ )
		{
			A ( j ) =  A ( j ) * B ( j );
		}
		return A;
	}

	template <class T>
	Vector<T>& operator*= ( Vector<T>& A, const T& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator*= <T> ( Vector<T>& A, const T& s)",1 );

		for ( inULong j = 0;j < A.size(); j++ )
		{
			A ( j ) = A ( j ) * s;
		}
		return A;
	}

	template <class T>
	Vector<T>& operator+= ( Vector<T>& A, const Vector<T>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator+= <T> ( Vector<T>& A, const Vector<T>& B)",1 );
// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );

		for ( inULong j = 0;j < A.size();j++ )
		{
			A ( j ) =  A ( j ) + B ( j );
		}
		return A;
	}

	template <class T>
	Vector<T>& operator+= ( Vector<T>& A, const T& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator+= <T> ( Vector<T>& A, const T& s)",1 );

		for ( inULong j = 0;j < A.size(); j++ )
		{
			A ( j ) = A ( j ) + s;
		}
		return A;
	}

	template <class T>
	Vector<T>& operator-= ( Vector<T>& A, const Vector<T>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator-= <T> ( Vector<T>& A, const Vector<T>& B)",1 );
// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );

		for ( inULong j = 0;j < A.size(); j++ )
		{
			A ( j ) =  A ( j ) - B ( j );
		}
		return A;
	}

	template <class T>
	Vector<T>& operator-= ( Vector<T>& A, const T& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator-= <T> ( Vector<T>& A, const T& s)",1 );

		for ( inULong j = 0;j < A.size();j++ )
		{
			A ( j ) = A ( j ) - s;
		}
		return A;
	}

	template <class T>
	T operator/ ( const Vector<T>& A, const Vector<T>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator/ <T> (const Vector<T>& A, const Vector<T>& B)",1 );
// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );

		inULong m = A.size();
// 		inULong k = 1/*A.stride()*/;

		Vector<T> C ( m, true );

		for ( inULong j = 0;j < C.size(); j++ )
		{
			C ( j ) =  A ( j ) / B ( j );
		}

		return C;
	}

	template <class T>
	Vector<T> operator/ ( const Vector<T>& A, const T& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator/ <T> (const Vector<T>& A, const T& s)",1 );

		inULong m = A.size();
// 		inULong k = 1/*A.stride()*/;

		Vector<T> C ( m,  true );

		for ( inULong j = 0;j < C.size(); j++ )
		{
			C ( j ) =  A ( j ) / s;
		}

		return C;
	}

	template <class T>
	Vector<T>& operator/= ( Vector<T>& A, const Vector<T>& B )
	{
		IN_DISPLAY ( "(" << A._objID << ") and (" << B._objID << ") iNumerics::operator/= <T> ( Vector<T>& A, const Vector<T>& B)",1 );
// 		IN_ASSERT ( A.stride() == B.stride(), 0 );
		IN_ASSERT ( A.size() == B.size(), 0 );

		for ( inULong j = 0;j < A.size(); j++ )
		{
			A ( j ) =  A ( j ) / B ( j );
		}
		return A;
	}

	template <class T>
	Vector<T>& operator/= ( Vector<T>& A, const T& s )
	{
		IN_DISPLAY ( "(" << A._objID << ") iNumerics::operator/= <T> ( Vector<T>& A, const T& s)",1 );

		for ( inULong j = 0;j < A.size();j++ )
		{
			A ( j ) = A ( j ) / s;
		}
		return A;
	}

// This section is for completely SPECIALIZED TEMPLATE methods only!
#ifndef INCLUDED_IN_INVECTOR_H

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

#endif /*INCLUDED_IN_IN_VECTOR_H*/
}
