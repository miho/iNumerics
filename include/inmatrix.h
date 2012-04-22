/// \file   inmatrix.h
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the declaration of the matrix class.

#include <iostream>
// #include <iomanip>
// #include <sstream>
// #include <fstream>
#include <cmath>

#ifndef INMATRIX_H
#define INMATRIX_H

#include "intypes.h"
#include "inmemtype.h"
#include "inmemcollect.h"
#include "inblaswrapper.h"
#include "invector.h"
#include "inbaseobject.h"

/**
 * \brief iNumerics Standard Namespace
 */
namespace iNumerics {

    /**
     * Defines decomposition type of matrix.
     */
    enum decompType {
        NO_DECOMP /**Matrix is not decomposed.*/,
        LU_DECOMP /**Matrix is LU-decomposed.*/
    };

    //Forward Declaration
    template <class T>
    class Matrix;

    typedef Matrix<inDouble> DMat;

    /**
     *	Stream-Operator.
     *	@param	os	Output-stream.
     *	@param	obj	Matrix that is to be printed.
     *	@return		Output-stream.
     */
    template <class T>
    std::ostream& operator <<(std::ostream &os, const Matrix<T> &obj);

    template <class T>
    Matrix<T> operator+(const Matrix<T>& A, const Matrix<T>& B);

    template <class T>
    Matrix<T> operator+(const Matrix<T>& A, const T& s);

    template <class T>
    Matrix<T> operator-(const Matrix<T>& A, const Matrix<T>& B);

    template <class T>
    Matrix<T> operator-(const Matrix<T>& A, const T& s);

    template <class T>
    Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B);

    template <class T>
    Matrix<T> operator*(const Matrix<T>& A, const T& s);

    template <class T>
    Matrix<T>& operator*=(Matrix<T>& A, const Matrix<T>& B);

    template <class T>
    Matrix<T>& operator*=(Matrix<T>& A, const T& s);

    template <class T>
    Matrix<T>& operator+=(Matrix<T>& A, const Matrix<T>& B);

    template <class T>
    Matrix<T>& operator+=(Matrix<T>& A, const T& s);

    template <class T>
    Matrix<T>& operator-=(Matrix<T>& A, const Matrix<T>& B);

    template <class T>
    Matrix<T>& operator-=(Matrix<T>& A, const T& s);

    template <class T>
    Vector<T> operator|(Matrix<T>& A, const Vector<T>& B);

    /**
     * \author Michael Hoffer, 2012
     * \brief Generic matrix class.
     * \section general General Description:
     * 
     * Matrix is a generic matrix class for numerical computations.
     * The \<inDouble\> template specialization uses Fortran BLAS functions to achieve high performance.
     * Otherwise it uses generic C++ implementations. Matrix uses MemCollect for memory management.
     * 
     * \section examples Examples:
     * 
     * \subsection basicexample Basic usage:
     * 
     * Each iNumeric program must start with the initialization of the memory manager for each template instance.
     * See MemCollect for detailed description of the parameters.
     * 
     * \code
     * Vector<inLong>::memCheck.initialize ( MByte ( 1.0 ), Byte ( 0 ) );
     * \endcode
     *
     * Construct a square matrix of integers with 64 elements:
     *
     * \code
     * Vector<inLng> m(8,8);
     * \endcode
     * 
     * Now follows a small example program:
     * 
     * \code
     * #include <invector.h>
     * using namespace iNumerics;
     * 
     * inInt main()
     * {
     *	Matrix<inDouble>::memCheck.initialize ( MByte ( 1.0 ), Byte ( 0 ) );
     *
     * 	Matrix <inDouble>m(5,3);
     * 
     * 	for (inULong i = 0; i < 5;i++)
     *	{
     *		for (inULong j = 0; j < 3;j++)
     *		{
     *			m(i,j) = i+j;
     *		}
     *	}
     * 
     * 	m.print();
     * 
     * 	return 0;
     * }
     * \endcode
     * 
     * The output:
     *
     * \code
     * [0,     1,      2]
     * [1,     2,      3]
     * [2,     3,      4]
     * [3,     4,      5]
     * [4,     5,      6]
     * \endcode
     * 
     * \warning Each template instance has it's own memory management (static member). Therefore you should
     * consider that MemCollect will create a memory list for each template instance. You
     * have to manually initialize Matrix\<T\>::::memCheck for each template instance T you use:
     * \code
     * Matrix<inInt>::memCheck.initialize ( KByte ( 1.0 ), Byte ( 0 ) );
     * \endcode
     * 
     * But matrices and vectors with same template parameter use the same instance of MemCollect. So it doesn't matter if 
     * memCheck is initialized as member of Matrix\<T\> or Vector\<T\>.
     *
     * \see MemCollect
     * 
     * @todo how does the freeMemList look like if a = b*c*d ? BAD!
     * 
     */
    template <class T>
    class Matrix : public Vector<T> {
    public:
        /**
         * Default-Constructor.
         * \author Michael Hoffer, 2012
         */
        Matrix();

        /**
         * Copy-Constructor.
         * @param M	Matrix to be copied.
         * \author Michael Hoffer, 2012
         */
        Matrix(const Matrix<T>& M);

        /**
         * Constructor.
         * @param nRows		Number of rows (Range: 0 .. ULONG_MAX).
         * @param nCols 	Number of columns (Range: 0 .. ULONG_MAX).
         * @param allowMemSharing
         *			Indicates weather the Matrix will allow other objects
         *			to share it's internal memory (Range: false,true).
         * \author Michael Hoffer, 2012
         */
        Matrix(inULong nRows, inULong nCols, bool allowMemSharing = false);

        /**
         *        	Constructor.
         *        	Uses an array as initialization.
         * @param v 		Pointer to Matrix, data is stored Fortran-like (Range: depends on address space).
         * @param memDimRows	Number of rows of the memory (Range: 0 .. ULONG_MAX).
         * @param memDimCols 	Number of columns of the memory (Range: 0 .. ULONG_MAX).
         * @param sizeRows 	Number of rows of the sub-Matrix (Range: 0 .. ULONG_MAX).
         * @param sizeCols	Number of columns of the sub-Matrix (Range: 0 .. ULONG_MAX).
         * @param memPosRow	Row position of the matrix. If Matrix is no subobject, memPosRow is usually zero.
         *			If Matrix is a subobject, memPosRow is the row position
         *			of the Matrix inside the inherited memory.
         * @param memPosCol	Col position of the matrix. If Matrix is no subobject, memPosCol is usually zero.
         *			If Matrix is a subobject, memPosCol is the col position
         *			of the Matrix inside the inherited memory.
         *			 
         * @param shareMem 	Defines weather the Matrix will operate on the transferred memory v or not.
         *			(Range: false,true).
         * @param allowMemSharing
         *			Defines weather the Matrix allows other objects to share 
         *			it's memory (Range: false,true).
         * \author Michael Hoffer, 2012
         */
        Matrix(T *v,
                inULong memDimRows,
                inULong memDimCols,
                inULong sizeRows,
                inULong sizeCols,
                inULong memPosRow = 0,
                inULong memPosCol = 0,
                bool shareMem = false,
                bool allowMemSharing = false);

        /**
         * Initializes the memory of the Matrix with zero bytes.
         * \author Michael Hoffer, 2012
         */
        void zero() {
            this->_mem.zero();
        };

        /**
         * Gives read access to entry (i,j) of the Matrix. Don't use it for efficient operations. 
         * \note Use BLAS functions instead.
         * @param i 	Row index (Range: 0 .. ULONG_MAX).
         * @param j 	Col index (Range: 0 .. ULONG_MAX).
         * @return 	Element (i,j) of the array (Range: depends on T).
         * \author Michael Hoffer, 2012
         */
        const T operator() (inULong i, inULong j) const;

        /**
         * Gives read/write access to entry (i,j) of the Matrix.
         * \note Use BLAS functions instead.
         * @param i 	Row index (Range: 0 .. ULONG_MAX).
         * @param j 	Col index (Range: 0 .. ULONG_MAX).
         * @return Element (i,j) of the array (Range: depends on T).
         * @warning This operator always sets _decompType to NO_DECOMP.
         * \author Michael Hoffer, 2012
         */
        T& operator() (inULong i, inULong j);

        /**
         * Assignment-Operator.
         * @param M 	Reference to Matrix to be copied.
         * @return 	Reference to Matrix.
         * \author Michael Hoffer, 2012
         */
        Matrix<T>& operator=(const Matrix<T>& M);


        /**
         * Returns the number of rows of the Matrix.
         * @return 	Number of rows (Range: 0 .. ULONG_MAX).
         * \author Michael Hoffer, 2012
         */
        const inULong nRows() const;

        /**
         * Returns the number of columns
         * @return 	Number of columns (Range: 0 .. ULONG_MAX).
         * \author Michael Hoffer, 2012
         */
        const inULong nCols() const;

        /**
         * Returns leading dimension of memory on which the Matrix
         * operates on.
         */
        const inULong memDimRows() const;

        /**
         * Returns pointer to internal memory of the matrix with offset. Its main usage is to give 
         * Fortran functions direct memory access.
         * \warning The data can be changed in any way by using this method. Use it with caution.
         * @return Pointer to internal memory that is stored as Fortran-Array 
         *		  (Range: depends on address space).
         * \author Michael Hoffer, 2012
         */
        T* getFVector() const;


        /**
         * Initializes the Matrix as (sizeRows x sizeCols) - Matrix. Uses an array as initialization.
         * If shareMem = true then no deepCopy() will be done. Therefore the array and
         * the initialized Vector use the same memory. Modifying one will also change
         * the other.
         * @param v 		    Pointer to Matrix, data is stored Fortran-like (Range: depends on address space).
         * @param sizeRows	Number of rows of the memory (Range: 0 .. ULONG_MAX).
         * @param sizeCols 	Number of columns of the memory (Range: 0 .. ULONG_MAX).
         * @param shareMem 	Defines weather the Matrix will operate on the transferred memory v or not.
         *			(Range: false,true).
         * @param allowMemSharing
         *		 Defines weather the Matrix allows other objects to share 
         *		 it's memory (Range: false,true).
         * \author Michael Hoffer, 2012
         */

        void assign(T *v,
                inULong sizeRows,
                inULong sizeCols,
                bool shareMem = false,
                bool allowMemSharing = false);

        /**
         *		Initializes the Matrix as (sizeRows x sizeCols) - Matrix. Uses an array as initialization.
         *		If shareMem = true then no deepCopy() will be done. Therefore the array and
         *		the initialized Vector use the same memory. Modifying one will also change
         *		the other.
         * @param v 		Pointer to Matrix, data is stored Fortran-like (Range: depends on address space).
         * @param memDimRows	Number of rows of the memory (Range: 0 .. ULONG_MAX).
         * @param memDimCols 	Number of columns of the memory (Range: 0 .. ULONG_MAX).
         * @param sizeRows 	Number of rows of the Matrix (Range: 0 .. ULONG_MAX).
         * @param sizeCols	Number of columns of the Matrix (Range: 0 .. ULONG_MAX).
         * @param memPosRow 	Row position of the Matrix (Range: 0 .. ULONG_MAX).
         * @param memPosCol	Col position of the Matrix (Range: 0 .. ULONG_MAX).
         * @param shareMem 	Defines weather the Matrix will operate on the transferred memory v or not.
         *			(Range: false,true).
         * @param allowMemSharing
         *			Defines weather the Matrix allows other objects to share 
         *			it's memory (Range: false,true).
         * \author Michael Hoffer, 2012
         */

        void assign(T *v,
                inULong memDimRows,
                inULong memDimCols,
                inULong sizeRows,
                inULong sizeCols,
                inULong memPosRow = 0,
                inULong memPosCol = 0,
                bool shareMem = false,
                bool allowMemSharing = false);

        /**
         * 
         *              Returns the diagonal of the Matrix. In case of non-quadratic matrices
         *		it returns a Vector, containing \f$ M(i,i) ~ \forall ~ 0 <\ i <\ dim \f$ where 
         *		dim = min( nRows() , nCols() ).
         * @param shareMem 	Defines weather returned Vector will operate on the memory of the Matrix.
         *			(Range: false,true).
         * @return 		Vector, containing entries of the diagonal of the Matrix.
         * \see \ref Sub-Objects
         * \todo Nebendiagonale
         * \author Michael Hoffer, 2012
         */
        Vector<T> getDiag(bool shareMem = false);

        /**
         *		Returns a sub-Matrix of dimension ( sizeRows x sizeCols ) and starting index (memPosRow,memPosCol).
         * @param memPosRow	Starting row-index (Range: 0 .. ULONG_MAX).
         * @param memPosCol	Starting col-index (Range: 0 .. ULONG_MAX).
         * @param sizeRows	Number of rows of the sub-Matrix (Range: 0 .. ULONG_MAX).
         * @param sizeCols	Number of cols of the sub-Matrix (Range: 0 .. ULONG_MAX).
         * @param shareMem	Defines weather returned sub-Matrix will operate on the memory 
         * 			of the base-Matrix (Range: false,true).
         * @param allowMemSharing
         *			Defines weather the Matrix allows other objects to share 
         *			it's memory (Range: false,true).
         * @return		Sub-Matrix, containing entries of the base-Matrix.
         * \see \ref Sub-Objects
         * \author Michael Hoffer, 2012
         */
        Matrix<T> getSubMatrix(inULong memPosRow,
                inULong memPosCol,
                inULong sizeRows,
                inULong sizeCols,
                bool shareMem = false,
                bool allowMemSharing = false);

        /**
         * Returns row (i) as Vector\<T\>.
         * @param i 		Row-Index (Range: 0 .. ULONG_MAX).
         * @param shareMem 	Defines weather returned Vector will operate on the memory
         * 			of the base-Matrix (Range: false,true).
         * @return 		Vector, containing entries of the base-Matrix.
         * \see \ref Sub-Objects
         */
        Vector<T> getRow(inULong i,
                bool shareMem = false
                );

        /**
         * Returns column (i) as Vector\<T\>.
         * @param i 		Row-Index (Range: 0 .. ULONG_MAX).
         * @param shareMem 	Defines weather returned Vector will operate on the memory
         * 			of the base-Matrix (Range: false,true).
         * @return 		Vector, containing entries of the base-Matrix.
         * \see \ref Sub-Objects
         */
        Vector<T> getCol(inULong i,
                bool shareMem = false
                );

        /**
         *
         * Print Matrix on standard output.
         */
        void print() const;
        
/**
         * Copies this matrix (each matrix has its own memory).
         * @param allowMemSharing Defines whether the returned Matrix allows 
         *		          other objects to share its memory (Range: false,true).
         * @return a copy of this matrix
         */
        Matrix<T> copy(bool allowMemSharing) const;

        /**
         *
         * Destructor.
         *        	
         */
        ~Matrix();

        //start additional methods
        //@{
        /// \name Additional Methods for Matrices
        /// <p>
        /// Rules for specializations:<br>
        ///
        /// @warning Because the template specializations use blas-routines, object dimensions must not be greater
        /// than inInt.
        /// @warning In this version (0.1), only integer and floating point data types are tested.
        /// </p>


        /**		Interchanges the elements of the calling Matrix and Matrix B.<br>
         *		Both matrices must exist and agree in dimension and element type.<br>
         * 
         * @param B 	Matrix
         */
        void deepswap(Matrix<T>& B);


        /**
         *		Computes the Frobenius norm  for the calling matrix.<br>
         *		<br>
         *		A specialization for \<inDouble\> and \<inLong\> exists.<br>
         *		
         * @warning	No generic implementation available.
         *
         * @return 	Frobenius norm  of the matrix
         */
        const T frobNorm();

        /**
         *		Multiply each entry in i-th row of the matrix with the element B(i).<br>
         *		<br>
         *		No specialization.
         *			
         * @param B 	Vector
         * @return 	The modified matrix
         */
        void scal(const Vector<T>& B);

        /**
         *		Rank 1 operation     A := alpha*x*y' + A,<br>
         *		where alpha is a scalar, x is an m element vector, 
         *		y is an n element vector and A is an m by n matrix.<br>
         *		<br>
         *		A specialization for \<inDouble\> exists.
         *		
         * @param alpha 	Scalar
         * @param x 		Vector
         * @param y	 	Vector
         */
        void rank1op(const T& alpha, const Vector<T>& x, const Vector<T>& y);

        /**
         *		Symmetric rank 1 operation   A := alpha*x*x' + A,<br> 
         *		where alpha is a real scalar, x is an n element vector and
         *		A is an n by n symmetric matrix, supplied in packed form.<br>
         *		<br>
         * @todo		A specialization for \<inDouble\> exists.
         * 
         * @param alpha 	Scalar
         * @param x 		Vector
         */
        void rank1opSym(const T& alpha, const Vector<T>& x);

        /**
         * 		LU-decomposition without pivoting, on the memory of the calling (n x n)-matrix.<br>
         * 		The values of the calling matrix are lost. <br>
         * 		Only for floating point data types sensible.<br> 
         * 
         * @todo	A specialization for \<inDouble\> exists.
         * 		
         */
        void LUdecomposition();

        /**
         * @return	True if decomposed, false otherwise (Range: false, true).
         */
        const bool isDecomposed() const;


        /**
         * 		Solves the equation A*x=b in two steps. L*y=b and U*x=y.<br>
         * 		A must be a LU-decomposed matrix.<br>
         * 		The vector b will be \b not modified during the calculation.<br>
         * 		Only for floating point data types sensible.<br> 
         * 
         * @todo	A specialization for \<inDouble\> exists.
         * 		
         * @param b 	Vector with the values of the right hand side.
         * @return 	The solution-vector x.
         */
        const Vector<T> LUsolving(const Vector<T>& b);

        //@}

        //end additional methods

        //@{
        /// \name Matrix Operators
        ///
        /// <p>
        /// Rules for all specializations:<br>
        ///
        /// @warning Because template specializations use blas-routines, object dimensions must not be greater
        /// than inInt.
        /// @warning Element types must match.
        /// @warning In this version (0.1), only integer and floating point data types are tested.
        /// </p>

        /**
         *		Adds two matrices.
         *		Both matrices must exist and agree in dimension and element types.<br>
         *		<br>
         *		A specialization for \<inDouble\> exists.<br>
         * 
         * @param A 	Matrix
         * @param B 	Matrix
         * @return 	A+B
         */
        friend Matrix<T> operator+ <T> (const Matrix<T>& A, const Matrix<T>& B);

        /**
         * 		Adds scalar s to each element of matrix A.<br>
         * 		The scalar and the matrix must have the same types.<br>
         * 		<br>
         * 		No specialization.<br>
         * 
         * @param A 	Matrix
         * @param s 	Scalar
         * @return 	A+s
         */
        friend Matrix<T> operator+ <T> (const Matrix<T>& A, const T& s);


        /**
         *		Subtracts a matrix from another matrix.
         *		Both matrices must exist and agree in dimension and element types.<br>
         *		<br>
         *		A specialization for \<inDouble\> exists.<br>
         * 
         * @param A 	Matrix
         * @param B 	Matrix
         * @return 	A-B
         */
        friend Matrix<T> operator- <T> (const Matrix<T>& A, const Matrix<T>& B);

        /**
         * 		Subtracts scalar s from each element of matrix A.<br>
         * 		The scalar and the matrix must have the same types.<br>
         * 		<br>
         * 		No specialization.<br>
         * 
         * @param A 	Matrix
         * @param s 	Scalar
         * @return 	A-s
         */
        friend Matrix<T> operator- <T> (const Matrix<T>& A, const T& s);


        /**
         *		Matrix matrix multiplication.<br>
         *		Both matrices must exist, have matched dimensions and element types.<br>
         *		<br>
         *		A specialization for \<inDouble\> exists.<br>
         *
         * @param A 	Matrix
         * @param B 	Matrix
         * @return 	A*B
         */
        friend Matrix<T> operator* <T> (const Matrix<T>& A, const Matrix<T>& B);

        /**
         * 		Multiplies scalar s to each element of matrix A.<br>
         * 		The scalar and the matrix must have the same types.<br>
         * 		<br>
         * 		A specialization for \<inDouble\> exists.<br>
         * 		
         * @param A 	Matrix
         * @param s 	Scalar
         * @return 	A*s
         */
        friend Matrix<T> operator* <T> (const Matrix<T>& A, const T& s);


        /**
         * 		Matrix matrix multiplication.<br>
         *		Both matrices must exist, have matching dimensions and element types.<br>
         *		<br>
         * 		No specialization.<br>
         * 		
         * @param A 	Matrix n x m
         * @param B 	Matrix m x m
         * @return 	A*=B
         */
        friend Matrix<T>& operator*= <T> (Matrix<T>& A, const Matrix<T>& B);

        /**
         * 		Multiplies scalar s to each element of matrix A.<br>
         * 		The scalar and the matrix must have the same types.<br>
         * 		<br>
         * 		A specialization for \<inDouble\> exists.<br>
         * 		
         * @param A 	Matrix
         * @param s 	Scalar
         * @return 	A*=s
         */
        friend Matrix<T>& operator*= <T> (Matrix<T>& A, const T& s);


        /**
         * 		Adds B to A.<br>
         * 		Both matrices must exist and agree in dimension and element types.<br>
         * 		<br>
         * 		A specialization for \<inDouble\> exists.<br>
         * 		
         * @param A 	Matrix
         * @param B 	Matrix
         * @return 	A+=B
         */
        friend Matrix<T>& operator+= <T> (Matrix<T>& A, const Matrix<T>& B);

        /**
         * 		Adds scalar s to each element of matrix A.<br>
         * 		The scalar and the matrix must have the same types.<br>
         * 		<br>
         * 		No specialization.<br>
         * 		
         * @param A 	Matrix
         * @param s 	Scalar
         * @return 	A+=s
         */
        friend Matrix<T>& operator+= <T> (Matrix<T>& A, const T& s);


        /**
         * 		Subtracts B from A.<br>
         * 		Both matrices must exist and agree in dimension and element types.<br>
         * 		<br>
         * 		A specialization for \<inDouble\> exists.<br>
         * 		
         * @param A 	Matrix
         * @param B 	Matrix
         * @return 	A-=
         */
        friend Matrix<T>& operator-= <T> (Matrix<T>& A, const Matrix<T>& B);

        /**
         * 		Subtracts scalar s from each element of matrix A.<br>
         * 		The scalar and the matrix must have the same types.<br>
         * 		<br>
         * 		No specialization.<br>
         * 		
         * @param A 	Matrix
         * @param s 	Scalar
         * @return 	A-=s
         */
        friend Matrix<T>& operator-= <T> (Matrix<T>& A, const T& s);



        /**
         *		Solves the equation A*x=b.
         *		A must be a square matrix. A and vector b must exist and agree in dimension
         *		(col-dim matrix = size of vector) and element types.<br>
         *		Only for floating point data types sensible.<br> 
         *		<br>
         * @todo	A specialization for \<inDouble\> exist.
         *
         * @param A 	Matrix
         * @param B 	Vector
         * @return 	The solution of the equation A*x=b
         */
        friend Vector<T> operator| <T> (Matrix<T>& A, const Vector<T>& B);


        //@}

    private:

        /**
         *	Performs flat-copy.
         */
        void flatCopy(const Matrix<T>& M);

        /**
         *	Performs deep-copy.
         */
        void deepCopy(const Matrix<T>& M);

        /**
         *
         *	Gives read access to entry (i) of the Matrix.
         * @param i 	Index of element (i) (Range: 0 .. ULONG_MAX).
         * @return	Element (i) of the array (Range: depends on T).
         * @warning Don't use this operator.
         */
        const T operator() (inULong i) const;

        /**
         *
         *	Gives read/write access to entry (i) of the Matrix.
         * @param i 	Index of element (i) (Range: 0 .. ULONG_MAX).
         * @return 	Element (i) of the array (Range: depends on T).
         * @warning Don't use this operator.
         */
        T& operator() (inULong i);

        /**
         *	Row-dimension.
         */
        inULong _nRows;

        /**
         *	Col-dimension.
         */
        inULong _nCols;

        /**
         *	Row position of the matrix. If Matrix is no subobject, _memPosRow is usually zero.
         *	If Matrix is a subobject (created with getSubMatrix()), _memPosRow is the row position
         *	of the Matrix inside the inherited memory.
         */
        inULong _memPosRow;

        /**
         *	Col position of the matrix. If Matrix is no subobject, _memPosCol is usually zero.
         *	If Matrix is a subobject (created with getSubMatrix()), _memPosCol is the col position
         *	of the Matrix inside the inherited memory.
         */
        inULong _memPosCol;

        /**
         *	Row dimension of memory. If Matrix is no subobject, _memDimRows is equal to _nRows
         */
        inULong _memDimRows;

        /**
         *	Row dimension of memory. If Matrix is no subobject, _memDimRows is equal to _nRows
         */
        inULong _memDimCols;

        /**
         * @todo	Eigenvalue should be charged. No implementation at this time.
         * 		So you shouldn't use the norm2-method for matrices.
         * 		
         */
        T norm2();

        /**
         *	The dot product makes no sense for matrices.
         *	
         */
        const T dotprod(const Vector<T>& B);

        /**
         *	Is part of LUsolving. Calculates y in the equation L*y=b.
         *	
         */
        void LUforward(Vector<T>& x, const Vector<T>& b);

        /**
         *	Is part of LUsolving. Calculates x in the equation R*x=y.
         *	
         */
        void LUbackward(Vector<T>& x, const Vector<T>& b);

        /**
         * Defines the decomposition type of the Matrix.
         * \see decompType
         * 
         */
        decompType _decompType;

        //maybe this is the reason why the operator*(mat,vec) isn´t working
        //friend Vector<T> operator* <T> ( const Vector<T>& A, const Vector<T>& B );

    };
}

#ifndef INMATRIX_HPP
//#define INCLUDED_IN_INMATRIX_H
#include "inmatrix.hpp"
//#undef INCLUDED_IN_INMATRIX_H
#endif /*INMATRIX_HPP*/

#endif /*INMATRIX_H*/
