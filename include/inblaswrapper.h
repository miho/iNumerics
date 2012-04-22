/// \file   inblaswrapper.h
/// \author Michael Hoffer, Christian Poliwoda
/// \date   2012
/// \brief Contains the declaration of the BLAS-functions.

//@{
/// \name BLAS-functions
/// \section general General Description:
/// <p>
/// BLAS-functions were used for the specializations of the additional methods
/// of the vector and the matrix class.
/// </p>
/// <p>
/// All this functions are obtained from BLAS. 	http://www.netlib.org/blas/
/// </p>

/**
 *<p> 
 *  DGEMM  performs one of the matrix-matrix operations
 *<br><br>
 *     C := alpha*op( A )*op( B ) + beta*C,
 *<br><br>
 *  where  op( X ) is one of
 *<br><br>
 *     op( X ) = X   or   op( X ) = X',
 *<br><br>
 *  alpha and beta are scalars, and A, B and C are matrices, with op( A )
 *  an m by k matrix,  op( B )  a  k by n matrix and  C an m by n matrix.
 *</p>
 * 
 * @param transa	On entry, TRANSA specifies the form of op( A ) to be used in
 *           		the matrix multiplication as follows:
 *           		<br><br>
 *              	TRANSA = 'N' or 'n',  op( A ) = A.
 *              	<br><br>
 *              	TRANSA = 'T' or 't',  op( A ) = A'.
 *              	<br><br>
 *              	TRANSA = 'C' or 'c',  op( A ) = A'.
 *              	<br><br>
 *           		Unchanged on exit.<br><br>
 *           		
 * @param transb	On entry, TRANSB specifies the form of op( B ) to be used in
 *           		the matrix multiplication as follows:
 *           		<br><br>
 *              	TRANSB = 'N' or 'n',  op( B ) = B.
 *              	<br><br>
 *              	TRANSB = 'T' or 't',  op( B ) = B'.
 *              	<br><br>
 *              	TRANSB = 'C' or 'c',  op( B ) = B'.
 *              	<br><br>
 *           		Unchanged on exit.<br><br>
 *
 * @param m		On entry,  M  specifies  the number  of rows  of the  matrix
 *           		op( A )  and of the  matrix  C.  M  must  be at least  zero.
 *           		Unchanged on exit.<br><br>
 *           		
 * @param n		On entry,  N  specifies the number  of columns of the matrix
 *           		op( B ) and the number of columns of the matrix C. N must be
 *		        at least zero.<br>
 *           		Unchanged on exit.<br><br>
 *           
 * @param k		On entry,  K  specifies  the number of columns of the matrix
 *           		op( A ) and the number of rows of the matrix op( B ). K must
 *           		be at least  zero.<br>
 *           		Unchanged on exit.<br><br>
 *           		
 * @param alpha		On entry, ALPHA specifies the scalar alpha.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param a		DOUBLE PRECISION array of DIMENSION ( LDA, ka ), where ka is
 *           		k  when  TRANSA = 'N' or 'n',  and is  m  otherwise.
 *           		Before entry with  TRANSA = 'N' or 'n',  the leading  m by k
 *           		part of the array  A  must contain the matrix  A,  otherwise
 *           		the leading  k by m  part of the array  A  must contain  the
 *           		matrix A.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param lda		On entry, LDA specifies the first dimension of A as declared
 *           		in the calling (sub) program. When  TRANSA = 'N' or 'n' then
 *           		LDA must be at least  max( 1, m ), otherwise  LDA must be at
 *           		least  max( 1, k ).<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param b		DOUBLE PRECISION array of DIMENSION ( LDB, kb ), where kb is
 *           		n  when  TRANSB = 'N' or 'n',  and is  k  otherwise.
 *           		Before entry with  TRANSB = 'N' or 'n',  the leading  k by n
 *           		part of the array  B  must contain the matrix  B,  otherwise
 *           		the leading  n by k  part of the array  B  must contain  the
 *           		matrix B.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param ldb		On entry, LDB specifies the first dimension of B as declared
 *           		in the calling (sub) program. When  TRANSB = 'N' or 'n' then
 *           		LDB must be at least  max( 1, k ), otherwise  LDB must be at
 *           		least  max( 1, n ).<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param beta		On entry,  BETA  specifies the scalar  beta.  When  BETA  is
 *           		supplied as zero then C need not be set on input.<br>
 *           		Unchanged on exit.<br><br>
 *           		
 * @param c		DOUBLE PRECISION array of DIMENSION ( LDC, n ).
 *           		Before entry, the leading  m by n  part of the array  C must
 *           		contain the matrix  C,  except when  beta  is zero, in which
 *           		case C need not be set on entry.<br>
 *           		On exit, the array  C  is overwritten by the  m by n  matrix
 *           		( alpha*op( A )*op( B ) + beta*C ).<br><br>
 *
 * @param ldc		On entry, LDC specifies the first dimension of C as declared
 *           		in  the  calling  (sub)  program.   LDC  must  be  at  least
 *           		max( 1, m ).<br>
 *           		Unchanged on exit.<br><br>
 * 
 * Added by Michael Hoffer and Christian Poliwoda
 */
extern "C" void dgemm_(char* transa,
        char* transb,
        inInt* m,
        inInt* n,
        inInt* k,
        inDouble* alpha,
        const inDouble* a,
        inInt* lda,
        const inDouble* b,
        inInt* ldb,
        inDouble* beta,
        inDouble* c,
        inInt* ldc);

/**
 *     Constant times a vector plus a vector. (y = a*x + y)<br>
 *     Uses unrolled loops for increments equal to one.
 * 
 * @param n		Size of the vectors.
 * @param da		The skalar which is multiplied with x.
 * @param dx		The vector x.
 * @param incx		The stepsize(spride) of the vector x.
 * @param dy		The vector y.
 * @param incy		The stepsize(spride) of the vector y.
 * 
 */
extern "C" void daxpy_(const inInt* n,
        const inDouble* da,
        const inDouble* dx,
        const inInt* incx,
        inDouble* dy,
        const inInt* incy);

/**
 *     Interchanges two vectors. (x <-> y)<br>
 *     Uses unrolled loops for increments equal one.
 * 
 * @param n		Size of the vectors.
 * @param dx		The vector x.
 * @param incx		The stepsize(spride) of the vector x.
 * @param dy		The vector y.
 * @param incy		The stepsize(spride) of the vector y.
 * 
 */
extern "C" void dswap_(const inInt* n,
        inDouble* dx,
        const inInt* incx,
        inDouble* dy,
        const inInt* incy);

/**
 *     Scales a vector by a constant.	(x=a*x)<br>
 *     Uses unrolled loops for increment equal to one.
 * 
 * @param n		Size of the vector.
 * @param da		The skalar which is multiplied with x.
 * @param dx		The vector x.
 * @param incx		The stepsize(spride) of the vector x.
 * 
 */
extern "C" void dscal_(const inInt* n,
        const inDouble* da,
        inDouble* dx,
        const inInt* incx);

/**
 *  DNRM2 returns the euclidean norm of a vector via the function
 *  name, so that   DNRM2 := sqrt( x'*x )
 * 
 * @param n		Size of the vector.
 * @param x		The vector x.
 * @param incx		The stepsize(spride) of the vector x.
 * @return		The euclidean norm of the vector x.
 * 
 */
extern "C" inDouble dnrm2_(const inInt* n,
        const inDouble* x,
        const inInt* incx);

/**
 *     Forms the dot product of two vectors.	(x' * y)<br>
 *     Uses unrolled loops for increments equal to one.
 * 
 * @param n		Size of the vectors.
 * @param dx		The vector x.
 * @param incx		The stepsize(spride) of the vector x.
 * @param dy		The vector y.
 * @param incy		The stepsize(spride) of the vector y.
 * @return		The result of the dot product of x and y.
 * 
 */
extern "C" inDouble ddot_(const inInt* n,
        const inDouble* dx,
        const inInt* incx,
        const inDouble* dy,
        const inInt* incy);

/**
 *  DGEMV  performs one of the matrix-vector operations
 *<br><br>
 *     y := alpha*A*x + beta*y,   or   y := alpha*A'*x + beta*y,
 *<br><br>
 *  where alpha and beta are scalars, x and y are vectors and A is an
 *  m by n matrix.
 * 
 * @param trans		On entry, TRANS specifies the operation to be performed as
 *           		follows:
 *<br><br>
 *              	TRANS = 'N' or 'n'   y := alpha*A*x + beta*y.
 *<br><br>
 *              	TRANS = 'T' or 't'   y := alpha*A'*x + beta*y.
 *<br><br>
 *              	TRANS = 'C' or 'c'   y := alpha*A'*x + beta*y.
 *<br><br>
 *           		Unchanged on exit.<br><br>
 *           		
 * @param m		On entry, M specifies the number of rows of the matrix A.
 *           		M must be at least zero.<br>
 *           		Unchanged on exit.<br><br>
 *           		
 * @param n		On entry, N specifies the number of columns of the matrix A.
 *           		N must be at least zero.<br>
 *           		Unchanged on exit.<br><br>
 *           		
 * @param alpha		On entry, ALPHA specifies the scalar alpha.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param a		DOUBLE PRECISION array of DIMENSION ( LDA, n ).
 *           		Before entry, the leading m by n part of the array A must
 *           		contain the matrix of coefficients.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param lda		On entry, LDA specifies the first dimension of A as declared
 *           		in the calling (sub) program. LDA must be at least
 *           		max( 1, m ).<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param x		DOUBLE PRECISION array of DIMENSION at least
 *           		( 1 + ( n - 1 )*abs( INCX ) ) when TRANS = 'N' or 'n'<br>
 *          		 and at least
 *           		( 1 + ( m - 1 )*abs( INCX ) ) otherwise.<br>
 *           		Before entry, the incremented array X must contain the
 *           		vector x.<br>
 *           		Unchanged on exit.<br><br>
 *           		
 * @param incx		On entry, INCX specifies the increment for the elements of
 *           		X. INCX must not be zero.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param beta		On entry, BETA specifies the scalar beta. When BETA is
 *           		supplied as zero then Y need not be set on input.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param y		DOUBLE PRECISION array of DIMENSION at least
 *           		( 1 + ( m - 1 )*abs( INCY ) ) when TRANS = 'N' or 'n'<br>
 *           		and at least
 *           		( 1 + ( n - 1 )*abs( INCY ) ) otherwise.<br>
 *           		Before entry with BETA non-zero, the incremented array Y
 *           		must contain the vector y. On exit, Y is overwritten by the
 *           		updated vector y.<br><br>
 *
 * @param incy		On entry, INCY specifies the increment for the elements of
 *           		Y. INCY must not be zero.<br>
 *           		Unchanged on exit.<br><br>
 *           		
 */
extern "C" void dgemv_(const char* trans,
        const inInt* m,
        const inInt* n,
        const inDouble* alpha,
        const inDouble* a,
        const inInt* lda,
        const inDouble* x,
        const inInt* incx,
        const inDouble* beta,
        const inDouble* y,
        const inInt* incy);

/**
 *     Copies a vector, x, to a vector, y.<br>
 *     Uses unrolled loops for increments equal to one.
 *  
 * @param n		Size of the vectors.
 * @param x		The vector x.
 * @param incx		The stepsize(spride) of the vector x.
 * @param y		The vector y.
 * @param incy		The stepsize(spride) of the vector y.
 * @return		The vector y with the values of x.
 * 
 */
extern "C" inDouble dcopy_(const inInt* n,
        const inDouble* x,
        const inInt* incx,
        const inDouble* y,
        const inInt* incy);

/**
 *     Takes the sum of the absolute values.
 * 
 * @param n		Size of the vector.
 * @param x		The vector x.
 * @param incx		The stepsize(spride) of the vector x.
 * @return		The sum of the absolute values of the vector x.
 * 
 */
extern "C" inDouble dasum_(const inInt* n,
        const inDouble* x,
        const inInt* incx);

/**
 *     Finds the index of element having max. absolute value.
 * 
 * @param n		Size of the vector.
 * @param x		The vector x.
 * @param incx		The stepsize(spride) of the vector x.
 * @return		The index of element having max. absolute value of the vector x.
 * 
 *   Added by Christian Poliwoda
 */
extern "C" inInt idamax_(const inInt* n,
        const inDouble* x,
        const inInt* incx);

/**
 *  DGER   performs the rank 1 operation
 *<br><br>
 *     A := alpha*x*y' + A,
 *<br><br>
 *  where alpha is a scalar, x is an m element vector, y is an n element
 *  vector and A is an m by n matrix.
 * 
 * @param m             On entry, M specifies the number of rows of the matrix A.<br>
 *         	  	M must be at least zero.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param n		On entry, N specifies the number of columns of the matrix A.<br>
 *           		N must be at least zero.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param alpha		On entry, ALPHA specifies the scalar alpha.<br>
 *           		Unchanged on exit.<br>
 *
 * @param x		DOUBLE PRECISION array of dimension at least
 *           		( 1 + ( m - 1 )*abs( INCX ) ).<br>
 *           		Before entry, the incremented array X must contain the m
 *           		element vector x.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param incx		On entry, INCX specifies the increment for the elements of
 *           		X. INCX must not be zero.<br>
 *           		Unchanged on exit.<br><br>
 *           		
 * @param y		DOUBLE PRECISION array of dimension at least
 *           		( 1 + ( n - 1 )*abs( INCY ) ).<br>
 *           		Before entry, the incremented array Y must contain the n
 *           		element vector y.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param incy		On entry, INCY specifies the increment for the elements of
 *           		Y. INCY must not be zero.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param a		DOUBLE PRECISION array of DIMENSION ( LDA, n ).
 *           		Before entry, the leading m by n part of the array A must
 *           		contain the matrix of coefficients. On exit, A is
 *           		overwritten by the updated matrix.<br><br>
 *
 * @param lda		On entry, LDA specifies the first dimension of A as declared
 *           		in the calling (sub) program. LDA must be at least
 *           		max( 1, m ).<br>
 *           		Unchanged on exit.<br><br>
 *    
 */
extern "C" void dger_(const inInt* m,
        const inInt* n,
        const inDouble* alpha,
        const inDouble* x,
        const inInt* incx,
        const inDouble* y,
        const inInt* incy,
        const inDouble* a,
        const inInt* lda);

/**
 *  DSPR    performs the symmetric rank 1 operation
 *<br><br>
 *     A := alpha*x*x' + A,
 *<br><br>
 *  where alpha is a real scalar, x is an n element vector and A is an
 *  n by n symmetric matrix, supplied in packed form.
 * 
 * @param uplo		On entry, UPLO specifies whether the upper or lower
 *           		triangular part of the matrix A is supplied in the packed
 *           		array AP as follows:
 *<br><br>
 *              	UPLO = 'U' or 'u'   <br>
 *              	The upper triangular part of A is supplied in AP.
 *<br><br>
 *              	UPLO = 'L' or 'l' <br>
 *              	The lower triangular part of A is supplied in AP.
 *<br><br>
 *           		Unchanged on exit.<br><br>
 *
 * @param n		On entry, N specifies the order of the matrix A.<br>
 *	           	N must be at least zero.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param alpha		On entry, ALPHA specifies the scalar alpha.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param x		DOUBLE PRECISION array of dimension at least
 *           		( 1 + ( n - 1 )*abs( INCX ) ).<br>
 *           		Before entry, the incremented array X must contain the n
 *           		element vector x.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param incx		On entry, INCX specifies the increment for the elements of
 *           		X. INCX must not be zero.<br>
 *           		Unchanged on exit.<br><br>
 *
 * @param ap		DOUBLE PRECISION array of DIMENSION at least
 *           		( ( n*( n + 1 ) )/2 ).<br>
 *          		Before entry with  UPLO = 'U' or 'u', the array AP must
 *           		contain the upper triangular part of the symmetric matrix
 *           		packed sequentially, column by column, so that AP( 1 )
 *           		contains a( 1, 1 ), AP( 2 ) and AP( 3 ) contain a( 1, 2 )
 *           		and a( 2, 2 ) respectively, and so on. On exit, the array
 *           		AP is overwritten by the upper triangular part of the
 *           		updated matrix.<br>
 *           		Before entry with UPLO = 'L' or 'l', the array AP must
 *           		contain the lower triangular part of the symmetric matrix
 *           		packed sequentially, column by column, so that AP( 1 )
 *           		contains a( 1, 1 ), AP( 2 ) and AP( 3 ) contain a( 2, 1 )
 *           		and a( 3, 1 ) respectively, and so on. On exit, the array
 *           		AP is overwritten by the lower triangular part of the
 *           		updated matrix.<br><br>
 *           		
 */extern "C" void dspr_(const char* uplo,
        const inInt* n,
        const inDouble* alpha,
        const inDouble* x,
        const inInt* incx,
        const inDouble ap);

/**
 *  DTRSV  solves one of the systems of equations
 *<br><br>
 *     A*x = b,   or   A'*x = b,
 *<br><br>
 *  where b and x are n element vectors and A is an n by n unit, or
 *  non-unit, upper or lower triangular matrix.
 *<br><br>
 *  No test for singularity or near-singularity is included in this
 *  routine. Such tests must be performed before calling this routine.
 *
 *
 * @param  uplo   	On entry, UPLO specifies whether the matrix is an upper or
 *           	lower triangular matrix as follows:
 *<br><br>
 *              	UPLO = 'U' or 'u'   A is an upper triangular matrix.
 *<br><br>
 *              	UPLO = 'L' or 'l'   A is a lower triangular matrix.
 *<br><br>
 *           	Unchanged on exit.<br>
 *
 * @param  trans  	On entry, TRANS specifies the equations to be solved as follows:
 *<br><br>
 *              	TRANS = 'N' or 'n'   A*x = b.
 *<br><br>
 *              	TRANS = 'T' or 't'   A'*x = b.
 *<br><br>
 *              	TRANS = 'C' or 'c'   A'*x = b.
 *<br><br>
 *           	Unchanged on exit.
 *
 * @param  diag   	On entry, DIAG specifies whether or not A is unit triangular as follows:
 *<br><br>
 *             	DIAG = 'U' or 'u'   A is assumed to be unit triangular.
 *<br><br>
 *             	DIAG = 'N' or 'n'   A is not assumed to be unit triangular.
 *<br><br>
 *           	Unchanged on exit.<br>
 *
 * @param  n      	On entry, N specifies the order of the matrix A.<br>
 *           	N must be at least zero.<br>
 *           	Unchanged on exit.<br>
 *
 * @param  a        DOUBLE PRECISION array of DIMENSION ( LDA, n ).
 *           	Before entry with  UPLO = 'U' or 'u', the leading n by n
 *           	upper triangular part of the array A must contain the upper
 *           	triangular matrix and the strictly lower triangular part of
 *           	A is not referenced.<br>
 *           	Before entry with UPLO = 'L' or 'l', the leading n by n
 *           	lower triangular part of the array A must contain the lower
 *           	triangular matrix and the strictly upper triangular part of
 *           	A is not referenced.<br>
 *           	Note that when  DIAG = 'U' or 'u', the diagonal elements of
 *           	A are not referenced either, but are assumed to be unity.
 *           	Unchanged on exit.<br>
 *
 * @param  lda	On entry, LDA specifies the first dimension of A as declared
 *           	in the calling (sub) program. LDA must be at least
 *          	 	max( 1, n ).<br>
 *           	Unchanged on exit.<br>
 *
 * @param  x       	DOUBLE PRECISION array of dimension at least ( 1 + ( n - 1 )*abs( INCX ) ).
 *           	Before entry, the incremented array X must contain the n
 *           	element right-hand side vector b. On exit, X is overwritten
 *           	with the solution vector x.<br>
 *
 * @param  incx   	On entry, INCX specifies the increment for the elements of X. INCX must not be zero.
 *           	Unchanged on exit.<br>
 */

extern "C" void dtrsv_(const char* uplo,
        const char* trans,
        const char* diag,
        const inInt* n,
        const inDouble* a,
        const inInt* lda,
        const inDouble* x,
        const inInt* incx);
//dtrsv_() isn't used until now
//@}


/**  DGESV computes the solution to a real system of linear equations
 *     A * X = B,
 *  where A is an N-by-N matrix and X and B are N-by-NRHS matrices.
 *
 *  The LU decomposition with partial pivoting and row interchanges is
 *  used to factor A as
 *     A = P * L * U,
 *  where P is a permutation matrix, L is unit lower triangular, and U is
 *  upper triangular.  The factored form of A is then used to solve the
 *  system of equations A * X = B.
 *
 *  Arguments
 *  =========
 *
 *  N       (input) INTEGER
 *          The number of linear equations, i.e., the order of the
 *          matrix A.  N >= 0.
 *
 *  NRHS    (input) INTEGER
 *          The number of right hand sides, i.e., the number of columns
 *          of the matrix B.  NRHS >= 0.
 *
 *  A       (input/output) REAL array, dimension (LDA,N)
 *          On entry, the N-by-N coefficient matrix A.
 *          On exit, the factors L and U from the factorization
 *          A = P*L*U; the unit diagonal elements of L are not stored.
 *
 *  LDA     (input) INTEGER
 *          The leading dimension of the array A.  LDA >= max(1,N).
 *
 *  IPIV    (output) INTEGER array, dimension (N)
 *          The pivot indices that define the permutation matrix P;
 *          row i of the matrix was interchanged with row IPIV(i).
 *
 *  B       (input/output) REAL array, dimension (LDB,NRHS)
 *          On entry, the N-by-NRHS matrix of right hand side matrix B.
 *          On exit, if INFO = 0, the N-by-NRHS solution matrix X.
 *
 *  LDB     (input) INTEGER
 *          The leading dimension of the array B.  LDB >= max(1,N).
 *
 *  INFO    (output) INTEGER
 *          = 0:  successful exit
 *          < 0:  if INFO = -i, the i-th argument had an illegal value
 *          > 0:  if INFO = i, U(i,i) is exactly zero.  The factorization
 *                has been completed, but the factor U is exactly
 *                singular, so the solution could not be computed.
 */
extern "C" void dgesv(const inInt* n,
        const inInt* nrhs,
        const inDouble* a,
        const inInt* lda,
        inInt* ipiv,
        inDouble* b,
        inInt* ldb,
        inInt* info);
