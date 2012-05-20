/// \file   invector.h
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the declaration of the vector class.

#ifndef INVECTOR_H
#define INVECTOR_H

#include <iostream>
#include <string>
#include <cmath>

#include "intypes.h"
#include "inmemtype.h"
#include "inmemcollect.h"
#include "inbaseobject.h"
#include "inblaswrapper.h"
#include "inutil.h"

/**
 * \brief iNumerics Standard Namespace
 */
namespace iNumerics {

    /**
     * \brief Defines the type of the memory the vector operates on.
     * 
     * The structure of the memory of a Vector object that is a row-vector of a Matrix object differs from
     * the memory structure of diagonal or column Vector objects (of course only if they share their memory with
     * their base matrix).
     * If one wants to create sub-objects it is necessary to determine the memory offset, i.e. the memory address
     * of the first element of the sub-vector. In case of a Vector object
     * that doesn't share its memory with matrices (PLAIN_VEC), the address of the first element of the sub-vector is just the position
     * concerning its base-vector.
     */
    enum vecType {
        PLAIN_VEC /**
                                *	\code T* adressOfFirstElement = &memPointer[startIndex]; \endcode
                                */,
        ROW_VEC /**
		     		*	\image html row_vec01.png
	    	    	    	*	\code
			        *	T* adressOfFirstElement = &memPointer[startIndex*memDimRows];
			        *	\endcode
		      		*	\image latex row_vec01.png
	    	    	        */,

        COL_VEC /**
		     		*	\image html col_vec01.png
	    	    	    	*	\code
	    	    	    	*	T* adressOfFirstElement = &memPointer[startIndex]; //just like PLAIN_VEC
	    	    	    	*	\endcode
		      		*	\image latex col_vec01.png
	    	    	    	*/,

        DIAG_VEC /**
		     		* 	\image html diag_vec01.png
	    	    	    	*	\code
	    	    	    	*	T* adressOfFirstElement = &memPointer[startIndex*(memDimRows+1L)];
	    	    	    	*	\endcode
		      		*	\image latex diag_vec01.png
	    	    	    	*/
    };

    //Forward Declarations

    template <class T>
    class Vector;


    //typedef Vector<inDouble> DVec;

    /**
     *	Stream-Operator.
     *	@param	os	Output-stream.
     *	@param	obj	Vector that is to be printed.
     *	@return		Output-stream.
     * \author Michael Hoffer, 2012
     */
    template <class T>
    std::ostream& operator <<(std::ostream &os, const Vector<T> &obj);

    template <class T>
    Vector<T> operator+(const Vector<T>& A, const Vector<T>& B);

    template <class T>
    Vector<T> operator+(const Vector<T>& A, const T& s);

    template <class T>
    Vector<T> operator-(const Vector<T>& A, const Vector<T>& B);

    template <class T>
    Vector<T> operator-(const Vector<T>& A, const T& s);

    template <class T>
    Vector<T> operator*(const Vector<T>& A, const Vector<T>& B);

    template <class T>
    Vector<T> operator*(const Vector<T>& A, const T& s);

    template <class T>
    Vector<T>& operator*=(Vector<T>& A, const Vector<T>& B);

    template <class T>
    Vector<T>& operator*=(Vector<T>& A, const T& s);

    template <class T>
    Vector<T>& operator+=(Vector<T>& A, const Vector<T>& B);

    template <class T>
    Vector<T>& operator+=(Vector<T>& A, const T& s);

    template <class T>
    Vector<T>& operator-=(Vector<T>& A, const Vector<T>& B);

    template <class T>
    Vector<T>& operator-=(Vector<T>& A, const T& s);

    template <class T>
    Vector<T> operator/(const Vector<T>& A, const Vector<T>& B);

    template <class T>
    Vector<T> operator/(const Vector<T>& A, const T& s);

    template <class T>
    Vector<T>& operator/=(Vector<T>& A, const Vector<T>& B);

    template <class T>
    Vector<T>& operator/=(Vector<T>& A, const T& s);

//    /**
//     *	\author Michael Hoffer, 2012
//     *	\brief BaseObject-Class
//     *	
//     *	This class has no real functionality. It only exists to ensure that all template
//     *	instances of Vector and Matrix use the same static members ::_objCounter and ::_objIDCounter.
//     */
//    class BaseObject {
//    protected:
//        static inULong _objCounter;
//        static inULong _objIDCounter;
//    };

    /**
     * \author Michael Hoffer, 2012
     * \brief Generic vector class.
     * \section general General Description:
     * 
     * Vector is a generic vector class for numerical computations.
     * The \<inDouble\> template specializations uses Fortran BLAS functions to achieve high performance.
     * Otherwise it uses generic C++ implementations. Vector uses MemCollect for memory management.
     * 
     * \section examples Examples:
     * 
     * \subsection basicexample Basic usage:
     * 
     * Each iNumerics program must start with the initialization of the memory manager for each template instance.
     * See MemCollect for detailed description of the parameters.
     * 
     * \code
     * Vector<inLong>::memCheck.initialize ( MByte ( 1.0 ), Byte ( 0 ) );
     * \endcode
     *
     * Construct a vector of integers with five elements:
     *
     * \code
     * Vector<inLng> v(5);
     * \endcode
     *
     * Now follows a small example program:
     * 
     * \code
     * #include <invector.h>
     * using namespace iNumerics;
     * 
     * int main()
     * {
     *	Vector<inLong>::memCheck.initialize ( MByte ( 1.0 ), Byte ( 0 ) );
     *
     * 	Vector <inLong> v(5);
     * 
     * 	for (int i = 0; i < 5;i++) v(i) = i;
     * 
     * 	v.print();
     * 
     * 	return 0;
     * }
     * \endcode
     * 
     * The output:
     *
     * \code
     * [0,     1,      2,      3,      4]
     * \endcode
     * 
     * Compiled wit \b DEBUG=2 iNumerics emits the following debug output:
     * \code
     * MemCollect<T>::MemCollect ( )
     * MemCollect<T>::MemCollect ( )
     * MemCollect<T>::initialize( Byte maxMem, Byte memTolerance )
     * (1) Vector<T>::Vector ( inULong n, bool allowMemSharing )
     * MemCollect<T>::allocMem(inULong n)
     * >> &MEM:0x8053798
     * [0,     1,      2,      3,      4]
     * (1) Vector::Destructor()
     * MemCollect<T>::freeMem( MemType<T>*& mem )
     * >> References to Memory:0
     * MemCollect<T>::attachToMemList ( MemType<T>* mem )
     * >> List-Size: 1 (1.90735e-05 MByte)
     * (1) >> Number of remaining Objects: 0
     * MemCollect<T>::Destructor()
     * MemCollect<T>::delMem( T* &mem )
     * MemCollect<T>::dettachFromMemList ( typename deque< MemType<T>* >::iterator i )
     * >> List-Size: 0 (0 MByte)
     * MemCollect<T>::Destructor()
     * \endcode
     * 
     * \see \ref Debug-Information
     *
     * \warning iNumerics constructs a memory manager for each template instance (static member).
     * MemCollect will create a memory list for each template instance. One has to manually
     * initialize Vector\<T\>::::memCheck for each template instance T that is used:
     * \code
     * Vector<inInt>::memCheck.initialize ( KByte ( 1.0 ), Byte ( 0 ) );
     * \endcode
     * 
     * But matrices and vectors with same template parameter use the same instance of MemCollect.
     * So it doesn't matter if memCheck is initialized as member of Matrix\<T\> or Vector\<T\>.
     * 
     * \see MemCollect
     * 
     * @todo how does the freeMemList look like if a = b*c*d ? BAD! 
     */

    template <class T>
    class Vector : public BaseObject {
    public:
        /**
         *      Default-Constructor.
         */
        Vector();

        /**
         * 
         *        	Copy-Constructor.
         * @param M	A Vector
         * @return 
         */
        Vector(const Vector<T>& M);

        /**
         *			Constructor.
         * 
         * @param n			Size of the Vector (Range: 0 .. ULONG_MAX).
         * @param allowMemSharing 	Defines whether the Vector allows other objects to share its
         * 				memory (Range: false,true).
         */
        Vector(inULong n, bool allowMemSharing = false);

        /**
         *        	Initializes vector from T array.
         *		If shareMem = true then no deepCopy() will be done! Therefore the array and
         *		the initialized Vector use the same memory. Modifying one will also change
         *		the other.
         *		
         * @param v		Pointer to T array, data is stored Fortran-like (Range: depends on address space).
         * @param n 		Size of the Vector (Range: 0 .. ULONG_MAX).
         * @param stride	Defines the step size of operator()(inULong i)
         *			(Range: 0 .. ULONG_MAX).
         * @param shareMem	Defines whether the Vector will use the same memory as the array (Range: 
         *			false,true).
         * @param allowMemSharing	Defines whether the Vector allows other objects to share its memory 
         *				(Range: false,true).
         * @warning		No memory management is done for array v. The user has to free the memory after 
         * 			all flat-copied vectors have been destroyed.
         * 
         */
        Vector(T *v,
                inULong n,
                inULong stride,
                bool shareMem = false,
                bool allowMemSharing = false);

        /**
         *        	Initializes vector from T array.
         *		If shareMem = true then no deepCopy() will be done! Therefore the array and
         *		the initialized Vector use the same memory. Modifying one will also change
         *		the other.
         *		
         * @param v		Pointer to T array, data is stored Fortran-like (Range: depends on address space).
         * @param vType		Memory type of array v (see ::vecType).
         * @param n 		Size of the Vector (Range: 0 .. ULONG_MAX).
         * @param stride	Defines the step size of operator()(inULong i)
         *			(Range: 0 .. ULONG_MAX).
         * @param memDimRows	Leading dimension of v (Range: n .. ULONG_MAX).
         *			(Range: 0 .. ULONG_MAX).
         * @param shareMem	Defines whether the Vector will use the same memory as the array (Range: 
         *			false,true).
         * @param allowMemSharing	Defines whether the Vector allows other objects to share its memory 
         *				(Range: false,true).
         * @warning		No memory management is done for array v. The user has to free the memory after 
         * 			all flat-copied vectors have been destroyed.
         * 
         */
        Vector(T *v,
                vecType vType,
                inULong n,
                inULong memDimRows,
                inULong stride,
                bool shareMem = false,
                bool allowMemSharing = false);

        /**
         *               Sets all Vector elements to zero.
         */
        void zero() {
            _mem->zero();
        };

        /**
         *	Gives read access to entry (i) of the Vector.
         *
         * @param i 	Index of the accessed element (Range: 0 .. ULONG_MAX).
         * @return Element (i) of the array (Range: depends on T).
         */
        const T operator() (inULong i) const;

        /**
         *	Gives read/write access to entry (i) of the Vector.
         *
         * @param i 	Index of the accessed element (Range: 0 .. ULONG_MAX).
         * @return Element (i) of the array (Range: depends on T).
         */
        T& operator() (inULong i);

        /**
         * 	Assignment operator. Performs 
         *	flat copy whenever possible.
         *
         * @param M
         * @return Reference to Vector.
         */
        Vector<T>& operator=(const Vector<T>& M);


        /**
         * Copies this vector (each vector has its own memory).
         * @param allowMemSharing Defines whether the returned Vector allows 
         *		          other objects to share its memory (Range: false,true).
         * @return a copy of this vector
         */
        Vector<T> copy(bool allowMemSharing = false) const;

        /**
         *        	Returns the size of the Vector.
         * @return 	Dimension of the Vector (Range: 0 .. ULONG_MAX).
         */
        const inULong size() const;

        /**
         *		Let
         *		
         *		\code 
         *		inLong mem[] = {1,2,3,4,5,6,7,8,9,10};
         *		\endcode
         *		
         *		be an array of type inDouble.
         *		
         *		Creation of a Vector object with \b stride=1 :
         *		
         *		\code
         *		Vector <inLong> v (mem, 10, 1);
         *		v.print();
         *		\endcode
         *		
         *		results in:
         *		
         *		\code
         *		[1,     2,      3,      4,      5,      6,      7,      8,      9,      10]
         *		\endcode
         *		
         *		Creation of a Vector object with \b stride=3 :	
         *	
         *   		\code
         *		Vector <inLong> v (mem, 5, 3);
         *		v.print();
         *		\endcode
         *
         *		results in:
         *		
         *		\code
         *		[1,     4,      7,      10]
         *		\endcode
         *
         *		For more information about stride consult the BLAS documentaton.
         *		
         * @return Stride of the Vector (Range: 0 .. ULONG_MAX).
         */
        const inULong stride() const;

        /**
         *		Returns leading dimension of memory on which the Matrix
         *		operates on.
         *
         */
        const inULong memDimRows() const;

        /**
         *              Returns unique object id;
         * @return 	Object ID (Range: 0 .. ULONG_MAX).
         */
        const inULong ID() const;

        /**
         *
         *              Returns pointer to internal memory of first element. Its main usage is to give 
         *		Fortran functions direct memory access.
         * \warning		The data can be changed in any way by using this method! Use it with caution!
         * @return		Pointer to internal memory that is stored as Fortran-Array 
         *			(Range: depends on address space).
         */
        T* getFVector() const;

        /**
         *		Uses an array as initialization.
         *		If shareMem = true then no deepCopy() will be done! Therefore the array and
         *		the initialized Vector use the same memory. Modifying one will also change
         *		the other.
         * @param v		Pointer to T array, data is stored Fortran-like (Range: depends on address space).
         * @param shareMem	Defines whether the Vector will use the same memory as the array (Range: 
         *			false,true).
         * @warning		No memory management is done for array v. The user has to free the memory after all 
         *			flat-copied vectors have been destroyed.
         */
        void assign(T *v, bool shareMem = false);

        /**
         *        	Uses an array as initialization.
         *		If shareMem = true then no deepCopy() will be done! Therefore the array and
         *		the initialized Vector use the same memory. Modifying one will also change
         *		the other.
         *		
         *		The argument stride can be seen as step size used by the operator()(inULong i).
         *		See stride() for details.
         *		
         * @param v		Pointer to T array, data is stored Fortran-like (Range: depends on address space).
         * @param memDimRows	Leading dimension of memory (Range: 0 .. ULONG_MAX).
         * @param stride	Defines the step size of operator()(inULong i)
         *			(Range: 0 .. ULONG_MAX).
         * @param offset	Starting position of operator()(inULong i)
         *			(Range: 0 .. ULONG_MAX).
         * @param shareMem	Defines whether the Vector will use the same memory as the array (Range: 
         *			false,true).
         * @warning		No memory management is done for array v. The user has to free the memory after all
         *			flat-copied vectors have been destroyed.
         * 
         */
        void assign(T *v,
                inULong memDimRows,
                inULong offset = 0,
                inULong stride = 0,
                bool shareMem = false);

        /**
         *        	Uses an array as initialization.
         *		If shareMem = true then no deepCopy() will be done! Therefore the array and
         *		the initialized Vector use the same memory. Modifying one will also change
         *		the other.
         *		
         *		The argument stride can be seen as step size used by the operator()(inULong i).
         *		See stride() for details.
         *		
         * @param v		Pointer to T array, data is stored Fortran-like (Range: depends on address space).
         * @param vType		Memory type of array v (see ::vecType).
         * @param memDimRows	Leading dimension of memory (Range: 0 .. ULONG_MAX).
         * @param offset	SStarting position of operator()(inULong i) (Range: 0 .. ULONG_MAX).
         * @param shareMem	Defines whether the Vector will use the same memory as the array (Range: 
         *			false,true).
         * @warning		No memory management is done for array v. The user has to free the memory after all
         *			flat-copied vectors have been destroyed.
         * 
         */
        void assign(T *v,
                vecType vType,
                inULong memDimRows,
                inULong offset,
                bool shareMem = false);

        /**
         *		Returns Sub-Vector of dimension \e size and starting index \e startIndex.
         * @param startIndex	Starting position of Sub-Vector.
         * @param size		Size of Sub-Vector (Range: 0 .. ULONG_MAX).
         * @param shareMem	Defines whether returned sub-Vector will operate on the memory 
         * 			of the base object (Range: false,true).
         * @return		Vector, containing entries of base object.
         * \see \ref Sub-Objects
         */
        Vector<T> getSubVector(inULong startIndex,
                inULong size,
                bool shareMem = false);

        /**
         *        	Prints Vector on the standard output.
         */
        void print() const;

        /**
         *        	Destructor.
         */
        ~Vector();

        //@{
        ///
        /// \name Additional Methods for Vector
        /// <p>
        /// Rules for specializations:<br>
        ///
        /// @warning Because the template specializations use blas-routines, object dimensions must not be greater
        /// than inInt.
        /// @warning In this version (0.1), only integer and floating point data types are tested.
        /// </p>


        /**		Exchanges the elements of the calling vector and the vector B.<br>
         *		Both vectors must exist and agree in dimension and element type.<br>
         *		<br>
         *		A specialization for \<inDouble> exists.
         * 
         * @param B 	Vector
         */
        void deepswap(Vector<T>& B);

        /**
         *		Calculates the euclidean norm of the calling vector.<br>
         *		<br>
         * 		A specialization for \<inDouble\> and \<inLong\> exists.
         * 		
         * @warning	No generic implementation avaible.
         * 		
         * @return 	The euclidean norm of the calling vector
         */
        T norm2();

        /**
         *		Calculates the dot product of the calling vector and the vector B.<br>
         *		Both vectors must exist and agree in dimension and element type.<br>
         *		<br>
         * 		A specialization for \<inDouble\> exists.
         * 		
         * @param B 	Vector
         * @return 	The dot product
         */
        const T dotprod(const Vector<T>& B);

        /**
         *		Takes the sum of the absolute values.<br>
         *		<br>
         * 		A specialization for \<inDouble\> exists.
         * 		
         * @return 	The sum of the absolute values
         */
        const T sumAbsValues();

        /**
         *		Search for the element having max. absolute value and returns its index.<br>
         *		<br>
         *		A specialization for \<inDouble\> exists.<br>
         *
         * @warning	Be careful starting to count with 0 like in C++.
         * 		
         * @return 	The index of element having max. absolute value.
         */
        const inULong maxElemIndex();

        /**
         *	Static instance of memory management.
         */
        static MemCollect<T> memCheck;

        //@} 
        /* Additional Methods end */

        //@{			/// \name Vector Operators
        /// <p>
        /// Rules for all specializations:<br>
        ///
        /// @warning Because template specializations use blas-routines, object dimensions must not be greater
        /// than inInt.
        /// @warning Element types must match.
        /// @warning In this version (0.1), only integer and floating point data types are tested.
        /// </p>

        /**
         *		Adds two vectors.<br>
         *		A specialization for \<inDouble\> exists.<br>
         * 
         * @param A 	Vector
         * @param B 	Vector
         * @return 	A+B
         */
        friend Vector<T> operator+ <T> (const Vector<T>& A, const Vector<T>& B);

        /**
         * 		Adds scalar s to each element of Vector A.<br>
         * 		The scalar and the vector must have the same types.<br>
         * 		No specialization.<br>
         * 
         * @param A 	Vector
         * @param s 	Scalar
         * @return 	A+s
         */
        friend Vector<T> operator+ <T> (const Vector<T>& A, const T& s);


        /**
         *		Subtracts a vector from another vector.<br>
         *		A specialization for \<inDouble\> exists.<br>
         * 
         * @param A 	Vector
         * @param B 	Vector
         * @return 	A-B
         */
        friend Vector<T> operator- <T> (const Vector<T>& A, const Vector<T>& B);

        /**
         * 		Subtracts scalar s from each element of Vector A.<br>
         * 		The scalar and the vector must have the same types.<br>
         * 		No specialization.<br>
         * 
         * @param A 	Vector
         * @param s 	Scalar
         * @return 	A-s
         */
        friend Vector<T> operator- <T> (const Vector<T>& A, const T& s);


        /**
         *		Multiplies A(i) with B(i) for all entries.<br>
         *		No specialization.<br>
         *
         * @param A 	Vector
         * @param B 	Vector
         * @return 	A*B
         */
        friend Vector<T> operator* <T> (const Vector<T>& A, const Vector<T>& B);

        /**
         * 		Multiplies scalar s to each element of Vector A.<br>
         * 		The scalar and the vector must have the same types.<br>
         * 		A specialization for \<inDouble\> exists.<br>
         * 		
         * @param A 	Vector
         * @param s 	Scalar
         * @return 	A*s
         */
        friend Vector<T> operator* <T> (const Vector<T>& A, const T& s);


        /**
         * 		Multiplies A(i) with B(i) for all entries.<br>
         * 		No specialization.<br>
         * 		
         * @param A 	Vector
         * @param B 	Vector
         * @return 	A*=B
         */
        friend Vector<T>& operator*= <T> (Vector<T>& A, const Vector<T>& B);

        /**
         * 		Multiplies scalar s to each element of Vector A.<br>
         * 		The scalar and the vector must have the same types.<br>
         * 		A specialization for \<inDouble\> exists.<br>
         * 		
         * @param A 	Vector
         * @param s 	Scalar
         * @return 	A*=s
         */
        friend Vector<T>& operator*= <T> (Vector<T>& A, const T& s);


        /**
         * 		Adds B(i) to A(i) for all entries.<br>
         * 		A specialization for \<inDouble\> exists.<br>
         * 		
         * @param A 	Vector
         * @param B 	Vector
         * @return 	A+=B
         */
        friend Vector<T>& operator+= <T> (Vector<T>& A, const Vector<T>& B);

        /**
         * 		Adds scalar s to each element of Vector A.<br>
         * 		The scalar and the vector must have the same types.<br>
         * 		No specialization.<br>
         * 		
         * @param A 	Vector
         * @param s 	Scalar
         * @return 	A+=s
         */
        friend Vector<T>& operator+= <T> (Vector<T>& A, const T& s);


        /**
         * 		Subtracts B(i) from A(i) for all entries.
         * 		A specialization for \<inDouble\> exists.<br>
         * 		
         * @param A 	Vector
         * @param B 	Vector
         * @return 	A-=B
         */
        friend Vector<T>& operator-= <T> (Vector<T>& A, const Vector<T>& B);

        /**
         * 		Subtracts scalar s from each element of Vector A.<br>
         * 		The scalar and the vector must have the same types.<br>
         * 		No specialization.<br>
         * 		
         * @param A 	Vector
         * @param s 	Scalar
         * @return 	A-=s
         */
        friend Vector<T>& operator-= <T> (Vector<T>& A, const T& s);

        /**
         * 		Calculates A(i) over B(i) for all entries.<br>
         * 		No specialization.<br>
         * 
         * @param A 	Vector
         * @param B 	Vector
         * @return 	A/B.
         */
        friend Vector<T> operator/ <T> (const Vector<T>& A, const Vector<T>& B);

        /**
         *		Calculates A(i) over s for each element of Vector A.<br>
         * 		The scalar and the vector must have the same types.<br>
         * 		A specialization for \<inDouble\> exists.<br>
         * 
         * @param A 	Vector
         * @param s 	Scalar
         * @return 	A/s
         */
        friend Vector<T> operator/ <T> (const Vector<T>& A, const T& s);

        /**
         * 		Elementwise division.<br>
         * 		The scalar and the vector must have the same types.<br>
         * 		No specialization.<br>
         * 		
         * @param A 	Vector
         * @param B 	Vector
         * @return 	A/=B
         */
        friend Vector<T>& operator/= <T> (Vector<T>& A, const Vector<T>& B);

        /**
         * 		Calculates A(i) over s for each element of Vector A.<br>
         * 		The scalar and the vector must have the same types.<br>
         * 		A specialization for \<inDouble\> exists.<br>
         * 		
         * @param A 	Vector
         * @param s 	Scalar
         * @return 	A/=s
         */
        friend Vector<T>& operator/= <T> (Vector<T>& A, const T& s);


        //@}

    protected:
        /**
         *	Performs flat copy.
         */
        void flatCopy(const Vector<T>& M);

        /**
         *	Performs deep copy.
         */
        void deepCopy(const Vector<T>& M);

        /**
         *	Defines whether the Vector allows other objects to share its memory.
         */
        bool _allowMemSharing;

        /**
         *	Unique object id. Mostly used for debug output.
         */
        inULong _objID;

        /**
         *	Size of the Vector.
         */
        inULong _size;

        /**
         *	Pointer to memory on which Matrix operates on.
         */
        MemType<T>* _mem;

        /**
         *	Row-dimension of the memory.
         */
        inULong _memDimRows;

    private:

        /**
         *	Computes memory offset.
         * @param i Index of element (i) (Range: 0 .. ULONG_MAX).
         * @return memory offset (Range: 0 .. ULONG_MAX).
         * \see vecType
         */
        const inULong getMemOffset(inULong i) const;

        /**
         *	Returns stride depending on _vecType.
         * @return stride (Range: 0 .. ULONG_MAX).
         */
        const inULong getMemStride() const;

        /**
         *	Step size of operator()( inULong i ), i.e. \f$ i \mapsto  i*stride \f$.
         */
        inULong _stride;

        /**
         *	Defines whether Vector is a plain Vector object or a subobject from matrix-row,
         *	matrix-col or matrix-diag.
         *	\see vecType
         */
        vecType _vecType;
    };
}

//#ifndef INVECTOR_HPP
//#define INCLUDED_IN_INVECTOR_H
#include "invector.hpp"
//#undef INCLUDED_IN_INVECTOR_H
//#endif /*INVECTOR_HPP*/

#endif /*INVECTOR_H*/
