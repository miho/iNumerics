//// \file   inmatrix.cpp
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the definition of the matrix class.

#define INMATRIX_HPP

#include <algorithm>

#include "inmatrix.h"
#include "inbaseobject.h"

//#define DEBUG 2

namespace iNumerics {

    template <class T>
    Matrix<T>::Matrix() : Vector<T>() {
        // Debug-Output
        IN_DISPLAY("(" << this->_objID << ") Matrix<T>::Matrix()", 1);

        // Initializing member variables
        _nRows = 0;
        _nCols = 0;
        _memPosRow = 0;
        _memPosCol = 0;
        _memDimRows = 0;
        _memDimCols = 0;
        _decompType = NO_DECOMP;
    }

    template <class T>
    Matrix<T>::Matrix(const Matrix& M) {
        // Setting ObjCounter and ObjIDCounter
        this->_objCounter++;
        this->_objIDCounter++;
        this->_objID = this->_objIDCounter;

        // Debug-Output
        IN_DISPLAY("(" << this->_objID << ") Matrix<T>::Matrix ( const Matrix& M )", 1);

        // If input-object allows memory sharing, do flatCopy(),
        // deepCopy() otherwise
        if (!M._allowMemSharing /*&& !this->_allowMemSharing*/) {

            this->_allowMemSharing = false;
            deepCopy(M);
        } else {
            this->_allowMemSharing = false;
            flatCopy(M);
        }
    }

    template <class T>
    Matrix<T>::Matrix(inULong nRows, inULong nCols, bool allowMemSharing) : Vector<T> (nRows*nCols, allowMemSharing) {
        // Debug-Output
        IN_DISPLAY("(" << this->_objID << ") Matrix<T>::Matrix ( inULong nRows, inULong nCols, bool allowMemSharing )", 1);

        // Initializing member variables
        _nRows = nRows;
        _nCols = nCols;
        _memPosRow = 0;
        _memPosCol = 0;
        _memDimRows = _nRows;
        _memDimCols = _nCols;
        _decompType = NO_DECOMP;
    }

    template <class T>
    Matrix<T>::Matrix(T *v,
    inULong memDimRows,
    inULong memDimCols,
    inULong sizeRows,
    inULong sizeCols,
    inULong memPosRow,
    inULong memPosCol,
    bool shareMem,
    bool allowMemSharing) {
        // Setting ObjCounter and ObjIDCounter
        this->_objCounter++;
        this->_objIDCounter++;
        this->_objID = this->_objIDCounter;

        // Debug-Output
        IN_DISPLAY("(" << this->_objID << ") Matrix<T>::Matrix ( T *v, inULong memDimRows,inULong memDimCols,inULong memPosRow = 0,inULong memPosCol = 0,bool shareMem = false,bool allowMemSharing = false )", 1);

        // Initializing member variables
        this->_size = sizeRows*sizeCols;
        _nRows = sizeRows;
        _nCols = sizeCols;
        this->_allowMemSharing = allowMemSharing;
        _decompType = NO_DECOMP;

        IN_ASSERT((memPosRow + sizeRows - 1) < memDimRows, 0);
        IN_ASSERT((memPosCol + sizeCols - 1) < memDimCols, 0);

        // If shareMem is enabled, do flatCopy() as Matrix will operate on memory
        // passed on via parameter v. Do deepCopy otherwise.
        if (shareMem) {
            // Debug-Output
            IN_DISPLAY("(" << this->_objID << ") >> FLAT COPY!", 2);

            this->_mem = new MemType<T > (v, memDimRows * memDimCols);
            // 			this->_mem = new MemType<T> ( &v[memPosRow+memPosCol*_memDimRows], memDimRows*memDimCols );
            this->_mem->addReference();

            // Initializing member variables
            _memPosRow = memPosRow;
            _memPosCol = memPosCol; // is equal to offset from Vector
            _memDimRows = memDimRows;
            _memDimCols = memDimCols;
            // 			this->_memOffset = memPosCol;
        } else {
            // Debug-Output
            IN_DISPLAY("(" << this->_objID << ") >> DEEP COPY!", 2);

            this->_mem = new MemType<T > (nRows() * nCols());
            this->_mem->addReference();

            // Initializing member variables
            _memPosRow = 0;
            _memPosCol = 0;
            _memDimRows = nRows();
            _memDimCols = nCols();
            // 			this->_memOffset = 0;

            // DeepCopy
            for (inULong i = 0; i < sizeRows; i++) {
                for (inULong j = 0; j < sizeCols; j++) {
                    (*this) (i, j) = v [ (memPosRow + i) + (memPosCol + j) * memDimRows ];
                }
            }
        }
    }

    template <class T>
    Matrix<T>::~Matrix() {
        //nothing to do.
    }

    template <class T>
    const T Matrix<T>::operator() (inULong i, inULong j) const {
        IN_ASSERT(j < _nCols, 0);
        IN_ASSERT(i < _nRows, 0);

        return ( * (this->_mem)) ((i + _memPosRow) + (j + _memPosCol) * _memDimRows);
    }

    template <class T>
    T& Matrix<T>::operator() (inULong i, inULong j) {
        IN_ASSERT(j < _nCols, 0);
        IN_ASSERT(i < _nRows, 0);

        _decompType = NO_DECOMP;

        return ( * (this->_mem)) ((i + _memPosRow) + (j + _memPosCol) * _memDimRows);
    }

    template <class T>
    Matrix<T>& Matrix<T>::operator=(const Matrix<T>& M) {
        // Debug-Output
        IN_DISPLAY("(" << this->_objID << ") Matrix<T>::operator= ( const Matrix<T>& M )", 1);

        // An object must not assigning itself
        if (&M == this) {
            // Debug-Output
            IN_DISPLAY("(" << this->_objID << ") >> \"nothing to do!\"", 2);

        } else {
            if (!M._allowMemSharing /*&& !this->_allowMemSharing*/) {
                // Deleting memory, i.e.
                // passing memory on to MemCollect (freeMemList)
                this->memCheck.freeMem(this->_mem);

                deepCopy(M);
            } else {
                // Deleting memory, i.e.
                // passing memory on to MemCollect (freeMemList)
                this->memCheck.freeMem(this->_mem);

                flatCopy(M);
            }
        }
        return *this;
    }

    template <class T>
    const inULong Matrix<T>::nRows() const {
        return _nRows;
    }

    template <class T>
    const inULong Matrix<T>::nCols() const {
        return _nCols;
    }

    template <class T>
    const inULong Matrix<T>::memDimRows() const {
        return _memDimRows;
    }

    template <class T>
    T* Matrix<T>::getFVector() const {
        return &this->_mem->getMem() [_memPosCol * _memDimRows + _memPosRow];
    }

    template <class T>
    void Matrix<T>::assign(T *v,
    inULong sizeRows,
    inULong sizeCols,
    bool shareMem,
    bool allowMemSharing) {
        IN_DISPLAY("(" << this->_objID << ") Vector<T>::assign ( T *v, inULong n, bool shareMem )", 1);
        IN_ASSERT(nRows() == sizeRows, 0);
        IN_ASSERT(nCols() == sizeCols, 0);

        if (shareMem) {
            // Debug-Output
            IN_DISPLAY("(" << this->_objID << ") >> FLAT COPY!", 2);

            this->memCheck.freeMem(this->_mem);

            // _mem is not controlled by memCheck
            // if destructor is called it will be deleted by memCheck
            this->_mem = new MemType<T > (v, sizeRows * sizeCols);
            this->_mem->addReference();

            _memDimRows = sizeRows;
            _memDimCols = sizeCols;
            _memPosCol = 0;
            _memPosRow = 0;
        } else {
            // Debug-Output
            IN_DISPLAY("(" << this->_objID << ") >> DEEP COPY!", 2);

            //DeepCopy
            for (inULong i = 0; i < nCols(); i++) {
                for (inULong j = 0; j < nRows(); j++) {
                    (*this) (i, j) = v [ (0 + i) + (0 + j) * sizeRows ];
                }
            }
        }
    }

    template <class T>
    void Matrix<T>::assign(T *v,
    inULong memDimRows,
    inULong memDimCols,
    inULong sizeRows,
    inULong sizeCols,
    inULong memPosRow,
    inULong memPosCol,
    bool shareMem,
    bool allowMemSharing) {
        IN_DISPLAY("(" << this->_objID << ") Vector<T>::assign ( T *v, inULong n, bool shareMem )", 1);
        IN_ASSERT(nRows() == sizeRows, 0);
        IN_ASSERT(nCols() == sizeCols, 0);

        if (shareMem) {
            // Debug-Output
            IN_DISPLAY("(" << this->_objID << ") >> FLAT COPY!", 2);

            this->memCheck.freeMem(this->_mem);

            // _mem is not controlled by memCheck
            // if destructor is called it will be deleted by memCheck
            this->_mem = new MemType<T > (v, memDimRows * memDimCols);
            this->_mem->addReference();

            _memDimRows = memDimRows;
            _memDimCols = memDimCols;

            _memPosRow = memPosRow;
            _memPosCol = memPosCol;
        } else {
            // Debug-Output
            IN_DISPLAY("(" << this->_objID << ") >> DEEP COPY!", 2);

            //DeepCopy
            for (inULong i = 0; i < nCols(); i++) {
                for (inULong j = 0; j < nRows(); j++) {
                    (*this) (i, j) = v [ (memPosRow + i) + (memPosCol + j) * memDimRows ];
                }
            }
        }
    }

    template <class T>
    Vector<T> Matrix<T>::getDiag(bool shareMem) {
        inULong dim = std::min(this->nRows(), this->nCols());

        // If shareMem is enabled, do flatCopy() as Vector will operate on memory
        // of the Matrix. Do deepCopy() otherwise.
        if (shareMem) {
            return typename Vector<T>::Vector(&this->_mem->getMem() [_memPosCol * _memDimRows + _memPosRow],
                    DIAG_VEC,
                    dim,
                    _memDimRows,
                    _memDimRows + 1L,
                    shareMem,
                    true);
        } else {
            Vector<T> v(dim, true);

            for (inULong i = 0; i < dim; i++) {
                v(i) = (*this) (i, i);
            }
            return v;
        }
    }

    template <class T>
    Matrix<T> Matrix<T>::getSubMatrix(inULong memPosRow,
    inULong memPosCol,
    inULong sizeRows,
    inULong sizeCols,
    bool shareMem,
    bool allowMemSharing) {
        return typename Matrix<T>::Matrix(this->_mem->getMem(),
                _memDimRows,
                _memDimCols,
                sizeRows,
                sizeCols,
                _memPosRow + memPosRow,
                _memPosCol + memPosCol,
                shareMem,
                allowMemSharing);
    }

    template <class T>
    Vector<T> Matrix<T>::getRow(inULong i,
    bool shareMem
    ) {
        IN_ASSERT(i < nRows(), 0);
        return typename Vector<T>::Vector(&this->_mem->getMem() [_memPosRow + i + _memPosCol * _memDimRows],
                ROW_VEC,
                this->nCols(),
                _memDimRows,
                _memDimRows,
                shareMem,
                true
                );
    }

    template <class T>
    Vector<T> Matrix<T>::getCol(inULong i,
    bool shareMem
    ) {
        IN_ASSERT(i < nCols(), 0);
        return typename Vector<T>::Vector(&this->_mem->getMem() [_memPosRow + i * _memDimRows + _memPosCol * _memDimRows],
                COL_VEC,
                this->nRows(),
                _memDimRows,
                1,
                shareMem,
                true
                );

    }

    template <class T>
    void Matrix<T>::print() const {
        std::cout << *this << std::endl;
    }

    /******************************************************************************
     *	                                                                          *
     *       Class: iNumerics::Matrix ( EXTRA-METHODS TEMPLATE )                   *
     *                                                                             *
     ******************************************************************************/

    template <class T>
    void Matrix<T>::deepswap(Matrix<T>& B) {
        IN_DISPLAY("(" << B._objID << ") Matrix<T>::deepswap ( Matrix<T>& B )", 1);
        IN_ASSERT(this->nRows() == B.nRows(), 0);
        IN_ASSERT(this->nCols() == B.nCols(), 0);

        T temp = 0;

        for (inULong i = 0; i < B.nRows(); ++i)
            for (inULong j = 0; j < B.nCols(); ++j) {
                temp = (*this) (i, j);
                (*this) (i, j) = B(i, j);
                B(i, j) = temp;
            }
    }

    // 	template <class T>
    // 	const T Matrix<T>::frobNorm ()
    // 	{
    // 		IN_DISPLAY ( "(" << this->_objID << ") Matrix<T>::frobNorm ()",1 );
    //
    // 		T frobNorm = 0;
    //
    // 		for ( inULong i = 0;i < this->nRows();++i )
    //			for ( inULong j = 0;j < this->nCols();++j )
    // 		{
    // 		{
    // 			frobNorm += ( *this ) (i, j ) * ( *this ) (i, j );
    // 		}
    //
    // 		frobNorm = sqrt ( frobNorm );
    //
    // 		return frobNorm;
    // 	}

    template <class T>
    void Matrix<T>::scal(const Vector<T>& B) {
        IN_DISPLAY("(" << B._objID << ") Matrix<T>::scal ( const Vector<T>& B )", 1);
        IN_ASSERT(this->nCols() == B.size(), 0); // number of cols must agree with size of vector
        IN_ASSERT(this->_decompType == NO_DECOMP, 0);

        for (inULong j = 0; j < this->nRows(); ++j) //row
        {
            for (inULong k = 0; k < this->nCols(); ++k) //col
            {
                (*this) (j) (k) = (*this) (j) (k) * B(j);
            }
        }
    }

    template <class T>
    void Matrix<T>::rank1op(const T& alpha, const Vector<T>& x, const Vector<T>& y) {
        IN_DISPLAY("(" << this->_objID << ") Matrix<T>::rank1op ( const T& alpha, const Vector<T>& x, const Vector<T>& y )", 1);

        IN_ASSERT(this->nRows() == x.size(), 0);
        IN_ASSERT(this->nCols() == y.size(), 0);

        IN_ASSERT(this->stride() == x.stride(), 0);
        IN_ASSERT(this->stride() == y.stride(), 0);

        IN_ASSERT(this->_decompType == NO_DECOMP, 0);

        inULong row = this->nRows();
        inULong col = this->nCols();

        Matrix<T> C(row, col, true);

        for (inULong j = 0; j < row; ++j) //row
        {
            for (inULong k = 0; k < col; ++k) //col
            {
                C(j, k) = alpha * x(j) * y(k);
            }
        }

        (*this) += C;
    }

    template <class T>
    void Matrix<T>::rank1opSym(const T& alpha, const Vector<T>& x) {
        //performs the symmetric rank 1 operation   A := alpha*x*x' + A,
        //where alpha is a real scalar, x is an n element vector and A is an n by n symmetric matrix, supplied in packed form.
        IN_DISPLAY("(" << this->_objID << ") Matrix<T>::rank1opSym ( const T& alpha, const Vector<T>& x)", 1);
        IN_ASSERT(this->nRows() == x.size(), 0);
        IN_ASSERT(this->nRows() == this->nCols(), 0);
        //		IN_ASSERT ( this->stride() == x.stride(), 0 );
        IN_ASSERT(this->_decompType == NO_DECOMP, 0);

        inULong row = this->nRows();
        inULong col = this->nCols();

        Matrix<T> C(row, col, true);

        for (inULong j = 0; j < row; ++j) //row
        {
            for (inULong k = 0; k <= j; ++k) //col	// not really sure if correct
            {
                C(k, j) = C(j, k) = alpha * x(j) * x(k);
            }
        }

        (*this) += C;
    }

    template <class T>
    const bool Matrix<T>::isDecomposed() const {
        if (_decompType != NO_DECOMP) return true;

        return false;
    }

    template <class T>
    void Matrix<T>::LUdecomposition() {
        IN_DISPLAY("(" << this->_objID << ") Matrix<T>::LUdecomposition()", 1);
        IN_ASSERT(this->nRows() == this->nCols(), 0);

        if (this->_decompType == NO_DECOMP) {

            // /*("should be")*/ programmed for col-wise saved matrices
            for (inULong k = 0; k < this->nCols() - 1; k++) {
                //calculates the k-th Col
                for (inULong i = k + 1; i < this->nCols(); i++) {
                    (*this) (i, k) /= (*this) (k, k);
                }

                //transform the restmatrix
                for (inULong j = k + 1; j < this->nCols(); j++) //Col
                {
                    for (inULong i = k + 1; i < this->nCols(); i++) //row
                    {
                        (*this) (i, j) = (*this) (i, j) - (*this) (k, j) * (*this) (i, k);
                    }
                }
            }

            _decompType = LU_DECOMP;

        } else {
            IN_DISPLAY(">> is already LU decomposed!", 2);
        }
    }

    template <class T>
    void Matrix<T>::LUforward(Vector<T>& x, const Vector<T>& b) {
        //forward L*y=b
        for (inULong i = 0; i < this->nCols(); i++) {
            x(i) = b(i);
            for (inULong j = 0; j < i; j++) {
                x(i) -= (*this) (i, j) * x(j);
            }
        }
    }

    template <class T>
    void Matrix<T>::LUbackward(Vector<T>& x, const Vector<T>& b) {
        //backward R*x=y
        x(this->nCols() - 1) = b(this->nCols() - 1) / (*this) (this->nCols() - 1, this->nCols() - 1);

        T sum = 0;
        for (inULong i = this->nCols() - 1; i > 0; i--) {
            sum = b(i - 1);
            for (inULong j = i + 1; j <= this->nCols(); j++) {
                sum -= (*this) (i - 1, j - 1) * x(j - 1);
            }
            x(i - 1) = sum / (*this) (i - 1, i - 1);
        }
    }

    template <class T>
    const Vector<T> Matrix<T>::LUsolving(const Vector<T>& b) {
        IN_DISPLAY("(" << this->_objID << ") and (" << b.ID() << ") Matrix<T>::LUsolving()", 1);
        IN_ASSERT(this->_decompType == LU_DECOMP, 0);
        IN_ASSERT(this->nCols() == b.size(), 0);

        Vector<T> x(this->nCols(), true);


        if (this->_decompType == LU_DECOMP) {
            LUforward(x, b);

            LUbackward(x, x);
        } else {
            IN_DISPLAY(">> is not LU decomposed!", 2);
        }

        return x;
    }

    /******************************************************************************
     *	                                                                          *
     *       Class: iNumerics::Matrix (PRIVATE-MEMBERS)                    	      *
     *                                                                             *
     ******************************************************************************/

    template <class T>
    void Matrix<T>::deepCopy(const Matrix<T>& M) {
        // Debug-Output
        IN_DISPLAY("(" << this->_objID << ") >> \"DEEP COPY\"", 2);

        this->_size = M.size();
        _nRows = M.nRows();
        _nCols = M.nCols();
        _memPosRow = 0;
        _memPosCol = 0;
        _memDimRows = _nCols;
        _memDimCols = _nRows;
        _decompType = NO_DECOMP;
        // 		this->_memOffset = 0;

        this->memCheck.allocMem(this->_mem, M.nRows() * M.nCols());

        // DeepCopy
        for (inULong i = 0; i < M.nRows() * M.nCols(); i++) {
            (* (this->_mem)) (i) = M(i);
        }
    }

    template <class T>
    void Matrix<T>::flatCopy(const Matrix<T>& M) {
        // Debug-Output
        IN_DISPLAY("(" << this->_objID << ") >> \"FLAT COPY\"", 2);

        this->_size = M.size();
        _nRows = M.nRows();
        _nCols = M.nCols();
        _memPosRow = M._memPosRow;
        _memPosCol = M._memPosCol;
        _memDimRows = M._memDimRows;
        _memDimCols = M._memDimCols;
        _decompType = M._decompType;
        // 		this->_memOffset = M._memOffset;

        this->_mem = M._mem;
        this->_mem->addReference();

    }

    template <class T>
    const T Matrix<T>::operator() (inULong i) const {
        // Highly unefficient :(
        return ( *this) (i % nRows(), i / nRows());
    }

    template <class T>
    T& Matrix<T>::operator() (inULong i) {
        // Highly unefficient :(
        return ( *this) (i % nRows(), i / nRows());
    }

    /******************************************************************************
     *	                                                                          *
     *       Class: iNumerics::Matrix ( TEMPLATE FRIENDS )                         *
     *                                                                             *
     ******************************************************************************/

    template <class T>
    std::ostream& operator <<(std::ostream &os, const Matrix<T> &obj) {
        for (inULong i = 0; i < obj.nRows(); ++i) {
            os << "[";
            for (inULong j = 0; j < obj.nCols(); ++j) {
                if (j < obj.nCols() - 1) {
                    os << obj(i, j) << ",\t";
                } else {
                    os << obj(i, j);
                }

            }
            os << "]" << std::endl;
        }

        return os;
    }

    template <class T>
    Matrix<T> operator+(const Matrix<T>& A, const Matrix<T>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator+ ( const Matrix<T>& A, const Matrix<T>& B )", 1);

        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);

        IN_ASSERT(A.nCols() == B.nCols(), 0);
        IN_ASSERT(A.nRows() == B.nRows(), 0);

        inULong n = B.nRows();
        inULong m = B.nCols();

        Matrix<T> C(n, m, true);

        for (inULong i = 0; i < C.nRows(); i++) {
            for (inULong j = 0; j < C.nCols(); j++) {
                C(i, j) = A(i, j) + B(i, j);
            }
        }
        return C;
    }

    template <class T>
    Matrix<T> operator+(const Matrix<T>& A, const T& s) {
        IN_DISPLAY("(" << A._objID << ") iNumerics::operator+ ( const Matrix<T>& A, const T& s )", 1);
        IN_ASSERT(!(A.isDecomposed()), 0);

        inULong n = A.nRows();
        inULong m = A.nCols();

        Matrix<T> C(n, m, true);

        for (inULong i = 0; i < C.nRows(); ++i) {
            for (inULong j = 0; j < C.nCols(); j++) {
                C(i, j) = A(i, j) + s;
            }
        }
        return C;
    }

    template <class T>
    Matrix<T> operator-(const Matrix<T>& A, const Matrix<T>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator- ( const Matrix<T>& A, const Matrix<T>& B )", 1);
        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);

        IN_ASSERT(A.nCols() == B.nCols(), 0);
        IN_ASSERT(A.nRows() == B.nRows(), 0);

        inULong n = B.nRows();
        inULong m = B.nCols();

        Matrix<T> C(n, m, true);

        for (inULong i = 0; i < C.nRows(); ++i) {
            for (inULong j = 0; j < C.nCols(); j++) {
                C(i, j) = A(i, j) - B(i, j);
            }
        }
        return C;
    }

    template <class T>
    Matrix<T> operator-(const Matrix<T>& A, const T& s) {
        IN_DISPLAY("(" << A._objID << ") iNumerics::operator- ( const Matrix<T>& A, const T& s )", 1);
        IN_ASSERT(!(A.isDecomposed()), 0);

        inULong n = A.nRows();
        inULong m = A.nCols();

        Matrix<T> C(n, m, true);

        for (inULong i = 0; i < C.nRows(); ++i) {
            for (inULong j = 0; j < C.nCols(); j++) {
                C(i, j) = A(i, j) - s;
            }
        }
        return C;
    }

    template <class T>
    Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator* ( Matrix<T>& A, const Matrix<T>& B )", 1);

        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);
        IN_ASSERT(A.nCols() == B.nRows(), 0);

        inULong n = A.nRows();
        inULong m = B.nCols();

        Matrix<T> C(n, m, true);

        for (inULong i = 0; i < C.nRows(); ++i) {
            for (inULong j = 0; j < C.nCols(); j++) {
                for (inULong k = 0; k < A.nCols(); k++) {
                    C(i, j) += A(i, k) * B(k, j);
                }
            }
        }

        return C;
    }

    template <class T>
    Matrix<T> operator*(Matrix<T>& A, const T& s) {
        IN_DISPLAY("(" << A._objID << ") iNumerics::operator* ( Matrix<T>& A, const T& s )", 1);
        IN_ASSERT(!(A.isDecomposed()), 0);

        inULong n = A.nRows();
        inULong m = A.nCols();

        Matrix<T> C(n, m, true);

        for (inULong i = 0; i < A.nRows(); i++) {
            for (inULong j = 0; j < A.nCols(); j++) {
                C(i, j) = A(i, j) * s;
            }
        }

        return C;
    }

    template <class T>
    Matrix<T>& operator*=(Matrix<T>& A, const Matrix<T>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator*= ( Matrix<T>& A, const Matrix<T>& B )", 1);

        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);
        IN_ASSERT(A.nCols() == B.nRows(), 0);

        inULong n = A.nRows();
        inULong m = B.nCols();

        Matrix<T> C(n, m, true);

        for (inULong i = 0; i < C.nRows(); i++) {
            for (inULong j = 0; j < C.nCols(); j++) {
                for (inULong k = 0; k < A.nCols(); k++) {
                    C(i, j) += A(i, k) * B(k, j);
                }
            }
        }
        A = C;

        return A;
    }

    template <class T>
    Matrix<T>& operator*=(Matrix<T>& A, const T& s) {
        IN_DISPLAY("(" << A._objID << ") iNumerics::operator*= ( Matrix<T>& A, const T& s )", 1);
        IN_ASSERT(!(A.isDecomposed()), 0);

        for (inULong i = 0; i < A.nRows(); i++) {
            for (inULong j = 0; j < A.nCols(); j++) {
                A(i, j) = A(i, j) * s;
            }
        }

        return A;
    }

    template <class T>
    Matrix<T>& operator+=(Matrix<T>& A, const Matrix<T>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator+= ( Matrix<T>& A, const Matrix<T>& B )", 1);
        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);

        IN_ASSERT(A.nCols() == B.nCols(), 0);
        IN_ASSERT(A.nRows() == B.nRows(), 0);

        for (inULong i = 0; i < A.nRows(); i++) {
            for (inULong j = 0; j < A.nCols(); j++) {
                A(i, j) = A(i, j) + B(i, j);
            }
        }

        return A;
    }

    template <class T>
    Matrix<T>& operator+=(Matrix<T>& A, const T& s) {
        IN_DISPLAY("(" << A._objID << ") iNumerics::operator+= ( Matrix<T>& A, const T& s )", 1);
        IN_ASSERT(!(A.isDecomposed()), 0);

        for (inULong i = 0; i < A.nRows(); i++) {
            for (inULong j = 0; j < A.nCols(); j++) {
                A(i, j) = A(i, j) + s;
            }
        }

        return A;
    }

    template <class T>
    Matrix<T>& operator-=(Matrix<T>& A, const Matrix<T>& B) {
        IN_DISPLAY("(" << A._objID << ") and (" << B._objID << ") iNumerics::operator-= ( Matrix<T>& A, const Matrix<T>& B )", 1);
        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);

        IN_ASSERT(A.nCols() == B.nCols(), 0);
        IN_ASSERT(A.nRows() == B.nRows(), 0);

        for (inULong i = 0; i < A.nRows(); i++) {
            for (inULong j = 0; j < A.nCols(); j++) {
                A(i, j) = A(i, j) - B(i, j);
            }
        }

        return A;
    }

    template <class T>
    Matrix<T>& operator-=(Matrix<T>& A, const T& s) {
        IN_DISPLAY("(" << A. _objID << ") iNumerics::operator-= ( Matrix<T>& A, const T& s )", 1);
        IN_ASSERT(!(A.isDecomposed()), 0);

        for (inULong i = 0; i < A.nRows(); i++) {
            for (inULong j = 0; j < A.nCols(); j++) {
                A(i, j) = A(i, j) - s;
            }
        }

        return A;
    }

    template <class T>
    Vector<T> operator*(const Matrix<T>& A, const Vector<T>& B) {
        IN_DISPLAY("(" << A.ID() << ") and (" << B.ID() << ") iNumerics::operator* ( const Matrix<T>& A, const Vector<T>& B )", 1);

        IN_ASSERT((!A.isDecomposed()) && (!B.isDecomposed()), 0);
        IN_ASSERT(A.nCols() == B.size(), 0);

        inULong n = A.nRows();
        Vector<T> C(n, true);

        for (inULong j = 0; j < C.size(); j++) {
            for (inULong k = 0; k < A.nCols(); k++) {
                C(j) += A(j, k) * B(k);
            }
        }

        return C;
    }

    template <class T>
    Vector<T> operator|(Matrix<T>& A, const Vector<T>& B) {
        A.LUdecomposition();
        return A.LUsolving(B);
    }


    // This section is for completely specialized template operators only!

#ifndef INCLUDED_IN_INMATRIX_H

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
        inInt * _ipiv = (inInt*)&ipiv;
        
        dgesv( &n, &nrhs, _A, &lda, _ipiv, _b_and_x, &ldb, &info );
        /// Check for the exact singularity
//        if( info > 0 ) {
//                printf( "The diagonal element of the triangular factor of A,\n" );
//                printf( "U(%i,%i) is zero, so that A is singular;\n", info, info );
//                printf( "the solution could not be computed.\n" );
//                exit( 1 );
//        }

        
        return x;
    }


#endif /*INCLUDED_IN_INMATRIX_H*/
}

