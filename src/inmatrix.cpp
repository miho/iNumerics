// This section is for completely specialized template operators only!

//#ifndef INCLUDED_IN_INMATRIX_H

#include "inmatrix.h"

namespace iNumerics {

    /******************************************************************************
     *	                                                                          *
     *       Class: iNumerics::Vector (PUBLIC-MEMBERS, Template Specialization)    *
     *                                                                             *
     ******************************************************************************/

    template <>
    void Matrix<inDouble>::deepswap(Matrix<inDouble>& B) {
        IN_DISPLAY("(" << this->_objID << ") and (" << B._objID << ") Matrix<inDouble>::deepswap ( Matrix<inDouble>& B )", 1);
        // 		IN_ASSERT ( this->size() <= INT_MAX, 0 );
        IN_ASSERT(this->nRows() == B.nRows(), 0);
        IN_ASSERT(this->nCols() == B.nCols(), 0);

        inInt n = B.size();
        inInt inc_A = this->stride();
        inInt inc_B = B.stride(); //stepsize of the vektor

        inDouble * _B = B.getFVector();
        inDouble * _A = this->getFVector();

        dswap_(&n, _B, &inc_B, _A, &inc_A);
    }

    template <>
    const inDouble Matrix<inDouble>::frobNorm() {
        IN_DISPLAY("(" << this->_objID << ") Matrix<inDouble>::frobNorm ()", 1);
        // 		IN_ASSERT ( this->size() <= INT_MAX, 0 );
        IN_ASSERT(!((*this).isDecomposed()), 0);

        inInt incx = 1/*this->stride()*/;
        inInt n = this->size();
        inDouble * _x = this->getFVector();

        return dnrm2_(&n, _x, &incx);
    }

    template <>
    const inLong Matrix<inLong>::frobNorm() {
        IN_DISPLAY("(" << this->_objID << ") Matrix<inLong>::frobNorm ()", 1);
        IN_ASSERT(!((*this).isDecomposed()), 0);

        inLong frobNorm = 0;

        for (inULong i = 0; i < this->nRows(); i++) {
            for (inULong j = 0; j < this->nCols(); j++) {
                frobNorm += (*this) (i, j) * (*this) (i, j);
            }
        }

        frobNorm = (inLong) sqrt(frobNorm);

        return frobNorm;
    }

    template <>
    void Matrix<inDouble>::rank1op(const inDouble& alpha, const Vector<inDouble>& x, const Vector<inDouble>& y) {
        IN_DISPLAY("(" << this->_objID << ") Matrix<inDouble>::rank1op ( const inDouble& alpha, const Vector<inDouble>& x, const Vector<inDouble>& y )", 1);
        IN_ASSERT(!((*this).isDecomposed()), 0);

        IN_ASSERT(this->nRows() == x.size(), 0);
        IN_ASSERT(this->nCols() == y.size(), 0);

        // 		IN_ASSERT ( this->stride() == x.stride(), 0 );
        // 		IN_ASSERT ( this->stride() == y.stride(), 0 );
        // 		IN_ASSERT ( this->nRows() <= INT_MAX, 0 );
        // 		IN_ASSERT ( this->nCols() <= INT_MAX, 0 );

        inInt n = this->nRows();
        inInt m = this->nCols();
        /*unsigned long*/ inInt stride = x.stride();

        inInt lda = this->memDimRows();

        inDouble * _A = this->getFVector();
        inDouble * _x = x.getFVector();
        inDouble * _y = y.getFVector();

        dger_(&n, &m, &alpha, _x, &stride, _y, &stride, _A, &lda);
    }

    template <>
    void Matrix<inDouble>::deepCopy(const Matrix<inDouble>& M) {
        // Debug-Output
        IN_DISPLAY("(" << _objID << ") >> \"DEEP COPY\"", 2);

        this->_size = M.size();
        _nRows = M.nRows();
        _nCols = M.nCols();
        _memPosRow = 0;
        _memPosCol = 0;
        _memDimRows = _nCols;
        _memDimCols = _nRows;
        _decompType = NO_DECOMP;
        // 		this->_memOffset = 0;

        inInt strideTmp = 1;
        inInt strideM = M.stride();
        inInt sizeTmp = _size;

        this->memCheck.allocMem(this->_mem, M.nRows() * M.nCols());

        inDouble * _M = M.getFVector();
        inDouble * _A = this->getFVector();

        dcopy_(&sizeTmp, _M, &strideM, _A, &strideTmp);
    }

    // 	template <>
    // 	void Matrix<inDouble>::rank1opSym ( const inDouble& alpha, const Vector<inDouble>& x)
    // 	{
    // 		IN_DISPLAY ( "(" << this->_objID << ") Matrix<inDouble>::rank1opSym ( const inDouble& alpha, const Vector<inDouble>& x)",1 );
    // 		IN_ASSERT ( this->nRows() == x.size(), 0 );
    // 		IN_ASSERT ( this->nRows() == this->nCols(), 0 );
    // 		//IN_ASSERT ( this->stride() == x.stride(), 0 );
    //		//IN_ASSERT ( A.size() <= INT_MAX, 0 );
    //	IN_ASSERT (! ( (*this).isDecomposed() ) , 0 );
    //
    // 		inInt row = this->nRows();
    //
    // 		inDouble *ap = new inDouble[row*(row+1)/2];
    // 		//Matrix<T> C ( row, col, true );
    //
    // 		inInt i=0;
    //
    // 		for ( inInt j = 0;j < row;++j ) //row
    // 		{
    // 			for ( inInt k = 0;k <= j;++k ) //col	// not really sure if correct
    // 			{
    // 				i++;
    // 				ap[i] = (*this)( j,k );
    // 			}
    // 		}
    //
    // 		char uplo='U';
    // 		/*unsigned long*/ inInt stride = x.stride();
    // 		inDouble * _x = x.getFVector();
    //
    //		/// NOT FINISHED STIL UNDER CONSTRUKTION
    //
    // //performs the symmetric rank 1 operation   A := alpha*x*x' + A,
    // //where alpha is a real scalar, x is an n element vector and A is an n by n symmetric matrix, supplied in packed form.
    // 		dspr_(&uplo,   &row,  &alpha,  _x,  &stride,  *ap);
    //
    // 		delete[] ap;
    //
    // 	}

    /******************************************************************************
     *	                                                                          *
     *       Class: iNumerics::Vector (PPRIVATE-MEMBERS, Template Specialization)  *
     *                                                                             *
     ******************************************************************************/

    /******************************************************************************
     *	                                                                          *
     *       Class: iNumerics::Vector (FRIENDS, Template Specialization)           *
     *                                                                             *
     ******************************************************************************/

    template <>
    Matrix<inDouble> operator+(const Matrix<inDouble>& A, const Matrix<inDouble>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator+ ( const Matrix<inDouble>& A, const Matrix<inDouble>& B )", 1);
        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);

        IN_ASSERT(A.nCols() == B.nCols(), 0);
        IN_ASSERT(A.nRows() == B.nRows(), 0);
        // 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

        inInt n = B.nRows();
        inInt m = B.nCols();

        Matrix<inDouble> C(n, m, true);

        inDouble * _B = B.getFVector();
        inDouble * _A = A.getFVector();

        inDouble * _C = C.getFVector();

        inInt k = B.size();
        inDouble da = 1; //adds  da-times B to C

        inInt inc_A = A.stride();
        inInt inc_B = B.stride(); //stepsize of the vector
        inInt inc_C = 1; //C.stride();

        daxpy_(&k, &da, _B, &inc_B, _C, &inc_C);
        daxpy_(&k, &da, _A, &inc_A, _C, &inc_C);

        return C;
    }

    template <>
    Matrix<inDouble> operator-(const Matrix<inDouble>& A, const Matrix<inDouble>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator- ( const Matrix<inDouble>& A, const Matrix<inDouble>& B )", 1);
        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);

        IN_ASSERT(A.nCols() == B.nCols(), 0);
        IN_ASSERT(A.nRows() == B.nRows(), 0);
        // 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

        inInt n = B.nRows();
        inInt m = B.nCols();

        Matrix<inDouble> C(n, m, true);

        inDouble * _B = B.getFVector();
        inDouble * _A = A.getFVector();

        inDouble * _C = C.getFVector();

        inInt k = B.size();
        inDouble db = -1; //adds  db-times B to C
        inDouble da = 1; //adds  da-times B to C

        inInt inc_A = A.stride();
        inInt inc_B = B.stride(); //stepsize of the vector
        inInt inc_C = 1; //C.stride();

        daxpy_(&k, &db, _B, &inc_B, _C, &inc_C);
        daxpy_(&k, &da, _A, &inc_A, _C, &inc_C);

        return C;
    }

    template <>
    Matrix<inDouble> operator*(const Matrix<inDouble>& A, const Matrix<inDouble>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator* ( const Matrix<inDouble>& A, const Matrix<inDouble>& B )", 1);
        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);
        IN_ASSERT(A.nCols() == B.nRows(), 0);
        // 		IN_ASSERT ( A.nCols() <= INT_MAX, 0 );
        // 		IN_ASSERT ( A.nRows() <= INT_MAX, 0 );

        inInt n = A.nRows();
        inInt m = B.nCols();
        inInt k = B.nRows();

        inInt lda = A.memDimRows();
        inInt ldb = B.memDimRows();

        Matrix<inDouble> C(n, m, true);

        inInt ldc = C.memDimRows();

        inDouble * _A = A.getFVector();
        inDouble * _B = B.getFVector();
        inDouble * _C = C.getFVector();

        char transa = 'N';
        char transb = 'N';
        inDouble alpha = 1.0;
        inDouble beta = 0.0;

        dgemm_(&transa, &transb, &n, &m, &k, &alpha, _A, &lda, _B, &ldb, &beta, _C, &ldc);

        return C;
    }

    template <>
    Matrix<inDouble> operator*(const Matrix<inDouble>& A, const inDouble& s) {
        IN_DISPLAY("(" << A._objID << ") iNumerics::operator* ( const Matrix<inDouble>& A, const inDouble& s )", 1);
        IN_ASSERT(!(A.isDecomposed()), 0);
        // 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

        inInt n = A.nRows();
        inInt m = A.nCols();

        Matrix<inDouble> C(n, m, true);

        inInt k = A.size();
        inInt incC = C.stride(); //stepsize of the Vector

        C = A;

        inDouble * _C = C.getFVector();

        dscal_(&k, &s, _C, &incC);

        return C;
    }

    template <>
    Matrix<inDouble>& operator*=(Matrix<inDouble>& A, const Matrix<inDouble>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator*= ( Matrix<inDouble>& A, const Matrix<inDouble>& B )", 1);
        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);

        A = A*B;

        return A;
    }

    template <>
    Matrix<inDouble>& operator*=(Matrix<inDouble>& A, const inDouble& s) {
        IN_DISPLAY("(" << A._objID << ") iNumerics::operator*= ( Matrix<inDouble>& A, const inDouble& s )", 1);
        IN_ASSERT(A.size() <= INT_MAX, 0);
        IN_ASSERT(!(A.isDecomposed()), 0);

        inInt k = A.size();
        inInt incA = A.stride(); //stepsize of the Vector

        inDouble * _A = A.getFVector();

        dscal_(&k, &s, _A, &incA);

        return A;
    }

    template <>
    Matrix<inDouble>& operator+=(Matrix<inDouble>& A, const Matrix<inDouble>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator+= ( Matrix<inDouble>& A, const Matrix<inDouble>& B )", 1);
        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);

        IN_ASSERT(A.nCols() == B.nCols(), 0);
        IN_ASSERT(A.nRows() == B.nRows(), 0);
        // 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

        inDouble * _B = B.getFVector();
        inDouble * _A = A.getFVector();

        inInt k = B.size();
        inDouble da = 1; //adds  da-times B to A

        inInt inc_A = A.stride();
        inInt inc_B = B.stride(); // stepsize of the Vector

        daxpy_(&k, &da, _B, &inc_B, _A, &inc_A);

        return A;
    }

    template <>
    Matrix<inDouble>& operator-=(Matrix<inDouble>& A, const Matrix<inDouble>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator-= ( Matrix<inDouble>& A, const Matrix<inDouble>& B )", 1);
        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);

        IN_ASSERT(A.nCols() == B.nCols(), 0);
        IN_ASSERT(A.nRows() == B.nRows(), 0);
        // 		IN_ASSERT ( A.size() <= INT_MAX, 0 );

        inDouble * _B = B.getFVector();
        inDouble * _A = A.getFVector();

        inInt k = B.size();
        inDouble da = -1; //adds  da-times B to A

        inInt inc_A = A.stride();
        inInt inc_B = B.stride(); // stepsize of the Vector

        daxpy_(&k, &da, _B, &inc_B, _A, &inc_A);

        return A;
    }

    /**
     *
     *		Matrix vector multiplication. matrix and vector must exist and agree in dimension 
     *		(col-dim matrix = size of vector) and element typ.
     * 		A specialization for \<inDouble\> exist.
     *
     * @param A 	Matrix
     * @param B 	Vector
     * @return 	The result of the multiplication
     *
     */
    template <>
    Vector<inDouble> operator*(const Matrix<inDouble>& A, const Vector<inDouble>& B) {
        IN_DISPLAY("(" << A.ID() << ") and (" << B.ID() << ") iNumerics::operator* ( const Matrix<inDouble>& A, const Vector<inDouble>& B )", 1);
        IN_ASSERT(!(A.isDecomposed()), 0);

        IN_ASSERT(A.nCols() == B.size(), 0);
        // 		IN_ASSERT ( A.stride() == B.stride(), 0 );
        // 		IN_ASSERT ( A.nCols() <= INT_MAX, 0 );
        // 		IN_ASSERT ( A.nRows() <= INT_MAX, 0 );

        char trans = 'N';
        inInt n = A.nRows();
        inInt m = A.nCols();
        inInt lda = A.memDimRows();

        inInt strideC = 1; // stride of new matrix C
        inInt strideB = B.stride(); // because fortan could make trouble if no inInt

        inDouble alpha = 1.0;
        inDouble beta = 0.0;

        Vector<inDouble> C(n, true);

        inDouble * _A = A.getFVector();
        inDouble * _B = B.getFVector();
        inDouble * _C = C.getFVector();

        dgemv_(&trans, &n, &m, &alpha, _A, &lda, _B, &strideB, &beta, _C, &strideC);

        return C;
    }

    

    template <>
    Vector<inDouble> operator|(Matrix<inDouble>& A, const Vector<inDouble>& b) {
        
        IN_DISPLAY("(" << A.ID() << ") and (" << b.ID() << ") iNumerics::operator| ( Matrix<inDouble>& A, const Vector<inDouble>& B )", 1);

        inInt n = A.nRows();
        inInt lda = A.memDimRows();
        inInt ldb = b.memDimRows();
        inInt nrhs = 1; // we only have one vector b and no multiple rhs

        Vector<inDouble> x = b.copy();

        inDouble * _A = A.getFVector();
        inDouble * _b_and_x = x.getFVector();

        inInt info = 0; // result info, if > 0 then an error occured, see below

        // we currently don't use pivoting 
        // this vector holds pivot indices
        //        Vector<inInt> ipiv = Vector<inInt>(n);
        //        inInt * _ipiv = ipiv.getFVector();

        inInt ipiv[n];
        inInt * _ipiv = (inInt*) & ipiv;

        dgesv_(&n, &nrhs, _A, &lda, _ipiv, _b_and_x, &ldb, &info);
        /// Check for the exact singularity
        //        if( info > 0 ) {
        //                printf( "The diagonal element of the triangular factor of A,\n" );
        //                printf( "U(%i,%i) is zero, so that A is singular;\n", info, info );
        //                printf( "the solution could not be computed.\n" );
        //                exit( 1 );
        //        }


        return x;
    }
}
