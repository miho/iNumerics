/// \file   inmemtype.cpp
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the definition of an abstraction of a C-array.

#include <iostream>
#include <string.h>
#define INMEMTYPE_HPP

#include "inmemtype.h"

namespace inumerics
{

	/******************************************************************************
	*	                                                                          *
	*       Class: MemType (PUBLIC-MEMBERS)                                       *
	*                                                                             *
	******************************************************************************/

	template <class T>
	MemType<T>::MemType()
	{
		// Initializing member variables
		_memory = NULL;
		_size = 0;
		_memSize = 0;
		_referenceCounter = 0;
		_manualMemoryManagement = false;
	}

	template <class T>
	MemType<T>::MemType ( inULong n )
	{
		// Initializing member variables
		_memory = new T[ n ];
		_size = n;
		_memSize = sizeof ( T ) *n;
		_referenceCounter = 0;
		_manualMemoryManagement = false;

		// Fill memory with zero-bytes.
		zero();
	}

	template <class T>
	MemType<T>::MemType ( T* mem, inULong size )
	{
		// Initializing member variables
		_memory = mem;
		_size = size;
		_memSize = sizeof ( T ) * size;
		_referenceCounter = 0;
		_manualMemoryManagement = true;
	}

	template <class T>
	void MemType<T>::zero()
	{
		// Fills memory with zero-bytes.
		// We don't know a better one :(
		memset ( _memory,'\0', _memSize.value() );
	}

	template <class T>
	const inULong MemType<T>::getRefCount() const
	{
		return _referenceCounter;
	}

	template <class T>
	const bool MemType<T>::isReferenced() const
	{
		bool tmpCond = _referenceCounter > 0;
		return tmpCond;
	}

	template <class T>
	void MemType<T>::addReference()
	{
		++_referenceCounter;
	}

	template <class T>
	void MemType<T>::removeReference()
	{
		IN_ASSERT ( _referenceCounter > 0, 0 );
		if ( _referenceCounter > 0 )
		{
			--_referenceCounter;
		}
	}

	template <class T>
	const inULong MemType<T>::size() const
	{
		return _size;
	}

	template <class T>
	const Byte MemType<T>::memSize() const
	{
		return _memSize;
	}

	template <class T>
	const T& MemType<T>::operator() ( inULong i ) const
	{
		IN_ASSERT ( i >= 0, 0 );
		IN_ASSERT ( i < size(), 0 );
		return _memory[ i ];
	}

	template <class T>
	T& MemType<T>::operator() ( inULong i )
	{
		IN_ASSERT ( i >= 0, 0 );
		IN_ASSERT ( i < size(), 0 );
		return _memory[ i ];
	}

	template <class T>
	T* MemType<T>::getMem()
	{
		return _memory;
	}

	template <class T>
	const bool MemType<T>::manualMemoryManagement() const
	{
		return _manualMemoryManagement;
	}

	template <class T>
	MemType<T>::~MemType()
	{
		if ( _memory != NULL )
		{
			if ( !_manualMemoryManagement ) delete[] _memory;
			_memory = NULL;
		}
		_size = 0;
	}

	/******************************************************************************
	*	                                                                          *
	*       Class: MemType (PRIVATE-MEMBERS)                                      *
	*                                                                             *
	******************************************************************************/
}
