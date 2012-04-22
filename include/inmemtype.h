/// \file   inmemtype.h
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the declaration of an abstraction of a C-array.

#include "intypes.h"
#include "inutil.h"
#include "inbyte.h"

#ifndef INMEMTYPE_H
#define INMEMTYPE_H

/**
 * \brief iNumerics Standard Namespace
 */
namespace iNumerics
{

	/**
 * \brief MemType class.
 * \author Michael Hoffer, 2012
 * \section sec1 General Description:
 * <p>
 * MemType is an abstraction of a C-array. It is
 * designed to provide a simple interface for accessing elements of the array and
 * for storing information about the number of references on the memory.
 * </p>
 * <p>
 * If you want to use MemCollect as memory management of your class, it is necessary
 * to use MemType for handling the memory.
 * </p>
 * \section examples Examples:
 * \subsection memtypeexample1 Using MemType like conventional Arrays:
 * \code
 * {
 * 	MemType<int> mem1(5);
 *	for (int i = 0;i < 5;i++) mem(i) = i;
 *	for (int i = 0;i < 5;i++) cout << mem(i) << ", ";
 *	cout << endl;
 * }
 * \endcode
 * The memory will be deleted automatically after the destructor of \b mem1 is called. If, however, a reference to the memory exists
 * and the destructor is called the memory won't be deleted:
 * \subsection memtypeexample2 References:
 * \code
 *
 * {
 *	MemType<int> mem1(5);
 *	// initialize and work on mem1 ...
 *	mem1.addReference();
 * }
 * // memory has not been deleted as mem1 is still referenced!
 * \endcode
 * \section sec2 Range of use:
 * <p>
 * MemType is not designed as stand-alone class. It is meant to be used with MemCollect.
 * </p>
 */
	template <class T>
	class MemType
	{
		public:
			/**
			 *               Default constructor.
			 */
			MemType();

			/**
			 *               Constructor. Allocates memory.
			 * @param n		Size of the array (Range: 0 .. ULONG_MAX).
			 */
			MemType ( inULong n );

			/**
			 *               Constructor. Initialize internal memory pointer.
			 * @param mem		Pointer to memory (array) (Range: depends on address space).
			 * @param size 		Size of the array (Range: 0 .. ULONG_MAX).
			 *
			 * @warning 		Using this constructor implies that memory management has to be done manually. 
			 *			The destructor will not free the memory.
			 */
			MemType ( T* mem, inULong size );

			void zero();

			/**
			 *               Returns the number of references to memory.
			 * @return 		Number of references (Range: 0 .. ULONG_MAX).
			 */
			const inULong getRefCount() const;


			/**
			 *               Checks whether memory is referenced.
			 * @return 		True if referenced. False otherwise (Range: false,true).
			 */
			const bool isReferenced() const;

			/**
			 *               Adds reference to memory. Actually it just increases
			 *		the internal referenceCounter.
			 */
			void addReference();


			/**
			 *               Removes reference from memory. Actually it just decreases
			 *		the  internal referenceCounter.
			 */
			void removeReference();

			/**
			 *               Returns the size of the array.
			 * @return 		Dimension of the array (Range: 0 .. ULONG_MAX).
			 */
			const inULong size() const;


			/**
			 *               Returns the size of the allocated memory.
			 * @return 		The size of the allocated memory (Range: Byte(0) .. Byte(ULONG_MAX) ).
			 */
			const Byte memSize() const;

			/**
			 *               Gives read-access to element (i) of the array.
			 * @param i 		Index of the accessed element (Range: 0 .. ULONG_MAX).
			 * @return 		Element (i) of the array (Range: depends on T).
			 */
			const T& operator() ( inULong i ) const;

			/**
			 *		 Gives read/write - access to element (i) of the array.
			 * @param i	 	Index of the accessed element (Range: 0 .. ULONG_MAX).
			 * @return 		Element (i) of the array (Range: depends on T).
			 */
			T& operator() ( inULong i );

			/**
			 *       Returnes a pointer to the array, i.e. the allocated
			 *		 memory.
			 * @return 		Pointer to allocated memory (Range: depends on address space).
			 */
			T* getMem();

			/**
			 *       Checks whether memory is managed manually. Usually a MemType object
			 *		 has no manual memory management. But if the constructor MemType( T* mem, inULong size)
			 *		 is used, memory management is manual.
			 *
			 * @warning		There is no possibility to change the type of memory management. If initialized
			 *			by the constructor, the behavior is fixed.
			 *
			 * @return	 	True, if manual memory management is enabled. False otherwise (Range: false,true).
			 */
			const bool manualMemoryManagement() const;

			/**
			 *               Destructor. If _manualMemoryManagement is true,
			 *               the internal memory won't be deleted.
			 */
			~MemType();

		private:
			/**
			 *		 Pointer to allocated memory.
			 */
			T* _memory;

			/**
			 *		 Number of references.
			 */
			inULong _referenceCounter;

			/**
			 *		 Dimension of the array.
			 */
			inULong _size;

			/**
			 *		 Size of allocated memory.
			 */
			Byte _memSize;

			/**
			 *		 Defines whether memory management is done automatically.
			 */
			bool _manualMemoryManagement;
	};

}

#ifndef INMEMTYPE_HPP
#include "inmemtype.hpp"
#endif /*INMEMTYPE_HPP*/

#endif /*INMEMTYPE_H*/

