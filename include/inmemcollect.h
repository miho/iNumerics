/// \file   inmemcollect.h
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the declaration of the memory management.

#include <iostream>
#include <deque>

#include "intypes.h"
#include "inmemtype.h"

#ifndef INMEMCOLLECT_H
#define INMEMCOLLECT_H


/**
 * \brief iNumerics Standard Namespace
 */
namespace iNumerics {

    /**
     * \author Michael Hoffer, 2012
     * \section sec1 General Description:
     * \brief Memory management class.
     * <p>
     * MemCollect provides a basic memory management. In this very brief introduction we
     * assume that MemCollect is used as static member of a class. We will not
     * distinguish between "user" and "object" if we are talking about the calling instance.
     * </p>
     * <p>
     * Generally speaking MemCollect frees the user from
     * using \b new and \b delete explicitly. It also can - if used appropriately - reduce the time
     * of memory allocation. Allocation of memory naturally takes some time as the program
     * has to wait for the operating system to provide the memory. The idea and the basic
     * concept of optimizing memory allocation, used by MemCollect, is to have a list with
     * pointers to freed memory, i.e. memory that has been freed by the user and not referenced by
     * any other object (but not really deleted). Whenever a new object is created that needs
     * memory MemCollect will check the list for a memory entry of the correct size. If it
     * found one, it removes the entry from the list and the object can use the corresponding
     * memory. If the list is empty or there is no matching entry, MemCollect allocates new
     * memory for the object. Whenever an object frees its memory and that memory is not
     * referenced by any other object, a pointer to it is added to the list.
     * 
     * \image html deque01.png
     * \image latex deque01.png
     * 
     * Fragmentation: Actually fragmentation
     * is not really possible. If the list contains too many entries, the oldest one is being
     * deleted. So the list is cleaned out from time to time.
     * </p>
     *
     * @warning	For being efficient MemCollect needs to have enough memory to operate on.
     *		One can change the maximum size of the list (in bytes) and set the
     *		so called memTolerance. It specifies how much the size of memory requested
     *		by the object and the size of memory in the list may differ. Of course the
     *		size of allocated memory has always to be at least equal to the size of the
     *		memory requested by the object.
     *
     *
     * \section examples Examples:
     * The following code shows a very simple
     * implementation of an array using a static member (MemCollect) and MemType as memory management.
     * \subsection example1 Small Array-Class:
     * \code
     * #include <iostream>
     * #include <inmemcollect.h>
     * #include <instopwatch.h>
     * using namespace std;
     * using namespace iNumerics;
     * 
     * class SmallArray
     * {
     *	public:
     *		SmallArray(int n)
     * 		{
     * 			cout << "******SmallArray::CONSTRUCTOR******" << endl;
     *			//_mem = new MemType<int>(n);
     *			memCheck.allocMem(_mem, n);
     * 		};
     *		int& operator() ( int i )
     * 		{
     *			return (*_mem)(i);
     * 		};
     *		~SmallArray()
     * 		{
     *			cout << "******SmallArray::DESTRUCTOR******" << endl;
     *			//delete _mem;
     *			memCheck.freeMem(_mem);
     * 		}
     *		static MemCollect<int> memCheck;
     * 	private:
     *		MemType<int> * _mem;
     * };
     *
     * MemCollect<int> SmallArray::memCheck;
     *
     * int main()
     * {
     * 	SmallArray::memCheck.initialize(KByte(100),KByte(0));
     *
     *	//we need a block to demonstrate how MemCollect works
     * 	{
     *		cout << "******A******" << endl;
     *		SmallArray a(8);
     *		for (int i = 0; i < 8;i++) a(i) = i;
     *	}
     *	cout << "******B******" << endl;
     *	SmallArray b(8);
     *	for (int i = 0; i < 8;i++) b(i) = i;
     *
     *	return 0;
     * }
     * \endcode
     * <p>
     * Set \b DEBUG=1 to see how MemCollect works. iNumerics emits the following debug-output:
     * </p>
     * \code
     * MemCollect<T>::MemCollect ( )
     * MemCollect<T>::initialize( Byte maxMem, Byte memTolerance )
     * ******A******
     * ******SmallArray::CONSTRUCTOR******
     * MemCollect<T>::allocMem(inULong n)
     * >> &MEM:0x8053568
     * ******SmallArray::DESTRUCTOR******
     * MemCollect<T>::freeMem( MemType<T>*& mem )
     * >> References to Memory:0
     * MemCollect<T>::attachToMemList ( MemType<T>* mem )
     * >> List-Size: 1 (3.05176e-05 MByte)
     * ******B******
     * ******SmallArray::CONSTRUCTOR******
     * MemCollect<T>::allocMem(inULong n)
     * >> WE HAVE A MEMLIST! Size=1 (3.05176e-05 MByte)
     * MemCollect<T>::detachFromMemList ( typename deque< MemType<T>* >::iterator i )
     * >> FOUND MEM in List
     * >> &MEM:0x8053568
     * ******SmallArray::DESTRUCTOR******
     * MemCollect<T>::freeMem( MemType<T>*& mem )
     * >> References to Memory:0
     * MemCollect<T>::attachToMemList ( MemType<T>* mem )
     * >> List-Size: 1 (3.05176e-05 MByte)
     * MemCollect<T>::Destructor()
     * MemCollect<T>::delMem( T* &mem )
     * MemCollect<T>::detachFromMemList ( typename deque< MemType<T>* >::iterator i )
     * >> List-Size: 0 (0 MByte)
     * \endcode
     * 
     * Here you can see what happens if MemCollect finds free memory in its internal freeMemList.
     * It reuses it without allocating new memory.
     * 
     * \see \ref Debug-Information
     *
     * \section advantages Advantages:
     * <p>
     * MemCollect tries to enhance the performance of memory allocation and to prevent the user
     * from leaving memory leaks. But we will concentrate on efficiency aspects.
     *
     * Therefore we will do a small experiment. We now have two arrays: \b SimpleArray and \b SimpleArray2 
     * (which has no memory manager).
     * In the main()-function we do nothing, except for instantiating objects in a for-loop.
     * Furthermore we return the address of the object to prevent the compiler from removing the object by optimization.
     * </p>
     * \code
     * #include <iostream>
     * #include <inmemcollect.h>
     * #include "instopwatch.h"
     * using namespace std;
     * using namespace iNumerics;
     *
     * class SmallArray
     * {
     *	public:
     *		SmallArray(int n)
     * 		{
     *			memCheck.allocMem(_mem, n);
     * 		};
     *		int& operator() ( int i )
     *		{
     *			return (*_mem)(i);
     *		};
     *		~SmallArray()
     *		{
     *			memCheck.freeMem(_mem);
     *		};
     *		static MemCollect<int> memCheck;
     *	private:
     *		MemType<int> * _mem;
     * };
     *
     * MemCollect<int> SmallArray::memCheck;
     *
     * class SmallArray2
     * {
     *	public:
     *		SmallArray2(int n)
     *		{
     *			_mem = new MemType<int>(n);
     *		};
     *		int& operator() ( int i )
     *		{
     *			return (*_mem)(i);
     *		};
     *		~SmallArray2()
     *		{
     *			delete _mem;
     *		}
     *	private:
     *		MemType<int> * _mem;
     * };
     *
     * int main()
     * {
     *	MemCollect<int> SmallArray::memCheck.initialize(MByte(40),MByte(0));
     *
     *	StopWatch s;
     *	s.reset();
     *	s.start();
     *	for (int i = 0; i < 10;i++)
     * 	{
     *		cout <<"(" << i << ") ----------------------------------------------" << endl;
     *		SmallArray a(10000000);
     *		cout << & a << endl;
     * 	}
     * 	s.stop();
     * 	cout << "First experiment, elapsed time: " << s.read() << " sec" << endl;
     *
     *	s.reset();
     *	s.start();
     *	for (int i = 0; i < 10;i++)
     * 	{
     *		cout <<"(" << i << ") ----------------------------------------------" << endl;
     *		SmallArray2 a(10000000);
     *		cout << & a << endl;
     * 	}
     *	s.stop();
     *	cout << "Second experiment, elapsed time: " << s.read() << " sec" << endl;
     *
     *	return 0;
     * }
     * \endcode
     * <p>
     * Use the \b -O (Optimization) compiler flag and compile without \b DEBUG to have a clean output:
     * </p>
     * \code
     * First experiment, elapsed time: 0.38 sec
     * Second experiment, elapsed time: 0.5 sec
     * \endcode
     * <p>
     * Memory allocation really takes time. So it can be useful to keep previously allocated
     * memory and reuse it.
     * 
     * But MemCollect needs memory to operate on. Otherwise it is absolutely inefficient. Changing
     * the maximum size of the freeMemList, e.g. setting it to 1 MB results in highly inefficient memory management. In our
     * case it gives us the following output:
     * </p>
     * \code
     * First experiment, elapsed time: 0.81 sec
     * Second experiment, elapsed time: 0.5 sec
     * \endcode
     */
    template <class T>
    class MemCollect {
    public:

        /**
         *
         *              Constructor.
         * 
         */
        MemCollect();

        /**
         *			Initializes MemCollect. Must be called before usage!
         * @param maxMem	The maximum size of freeMemList (Range: Byte(0) .. Byte(ULONG_MAX).
         * @param memTolerance	The value, specifying how much requested and 
         *			available memory may differ (Range: Byte(0) .. Byte(ULONG_MAX).
         */
        void initialize(Byte maxMem, Byte memTolerance);

        /**
         *
         *              Allocates memory.
         * @param mem		Pointer to the memory (Range: depends on address space).
         * @param n		Size of memory (Range: 0 .. ULONG_MAX).
         */
        void allocMem(MemType<T>*& mem, inULong n);

        /**
         *
         *              Frees memory.
         * @param mem		Pointer to memory that is to be freed (Range: depends on address space).
         */
        void freeMem(MemType<T>*& mem);


        /**
         * 
         * Frees all allocated memory. The complete freeMemList
         * will be emptied.
         */
        void freeAll();


        /**
         *
         *              Checks whether memory is already in freeMemList.
         * @param mem 		Pointer to the memory thats availability shall be checked (Range: depends on address space).
         * @return 		True if mem is in list. False otherwise (Range: false,true).
         */
        bool isInList(MemType<T>* mem);

        /**
         *               Destructor.
         */
        ~MemCollect();
    private:
        /**
         *              Attaches memory to freeMemList.
         * @param mem		Pointer to the memory to be attached (Range: depends on address space).
         */
        void attachToMemList(MemType<T>* mem);
        /**
         *
         * 		Detaches memory from freeMemList.
         * @param i		Iterator that specifies the element that is to be detached.
         */
        void detachFromMemList(typename std::deque< MemType<T>* >::iterator i);

        /**
         *
         *              Really deletes memory. Takes care of deleting memory entry
         *		from freeMemList.
         * @param mem		Pointer to the memory to be deleted (Range: depends on address space).
         * @warning		If the memory is not attached to the freeMemList, it will 
         *			also be deleted!
         */
        void delMem(MemType<T>*& mem);

        /**
         *
         *		This deque is a list containing pointers to allocated memory that is
         *		freed, i.e. not referenced anymore.
         */
        std::deque<MemType<T>* > freeMemList;

        /**
         *
         *		Maximum size of the freeMemList.
         */
        Byte _maxMem;

        /**
         *		Actual size of the freeMemList.
         */
        Byte _memListSize;

        /**
         *		Specifies how much size of requested memory and size of provided
         *		memory (list entry) may differ.
         */
        Byte _memTolerance;

        /**
         *		Defines if object is initialized or not.
         */
        bool _initialized;
    };
}

#ifndef INMEMCOLLECT_HPP
#include "inmemcollect.hpp"
#endif /*INMEMCOLLECT_HPP*/

#endif /*INMEMCOLLECT_H*/


