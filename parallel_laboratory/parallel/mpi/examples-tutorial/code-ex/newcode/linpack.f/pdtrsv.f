CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE PDTRSV( UPLO, TRANS, DIAG, N, NB, A, LDA, X,
     $     WORK, INFO, COMM ) 
      INCLUDE "mpif.h"
*     .. Scalar Arguments ..
      INTEGER            LDA, N, NB
      CHARACTER*1        DIAG, TRANS, UPLO
*     .. Array Arguments ..
      INTEGER            COMM( 3 )
      DOUBLE PRECISION   A( LDA, * ), X( * ), WORK( * )
*     ..
*
*  Purpose
*  =======
*
*  PDTRSV  solves one of the systems of equations
*
*     A*x = b,   or   A'*x = b,   or   Conj(A')*x = b,
*
*  where b and x are n element vectors and A is an n by n unit, or
*  non-unit, upper or lower triangular matrix.
*
*  This is a parallel version that uses a 2D block wrapped version
*  that is a generalization of the Li-Coleman algorithm.
*
*  No test for singularity or near-singularity is included in this
*  routine. Such tests must be performed before calling this routine.
*
*  Parameters
*  ==========
*
*  UPLO   - CHARACTER*1.
*           On entry, UPLO specifies whether the matrix is an upper or
*           lower triangular matrix as follows:
*
*              UPLO = 'U' or 'u'   A is an upper triangular matrix.
*
*              UPLO = 'L' or 'l'   A is a lower triangular matrix.
*
*           Unchanged on exit.
*
*  TRANS  - CHARACTER*1.
*           On entry, TRANS specifies the equations to be solved as
*           follows:
*
*              TRANS = 'N' or 'n'   A*x = b.
*
*              TRANS = 'T' or 't'   A'*x = b.
*
*              TRANS = 'C' or 'c'   Conj(A')*x = b.
*
*           Unchanged on exit.
*
*  DIAG   - CHARACTER*1.
*           On entry, DIAG specifies whether or not A is unit
*           triangular as follows:
*
*              DIAG = 'U' or 'u'   A is assumed to be unit triangular.
*
*              DIAG = 'N' or 'n'   A is not assumed to be unit
*                                  triangular.
*
*           Unchanged on exit.
*
*  N      - INTEGER.
*           On entry, N specifies the order of the matrix A.
*           N must be at least zero.
*           Unchanged on exit.
*
*  NB     - INTEGER.
*           Block size used for the data distribution.
*
*  A      - DOUBLE PRECISION array of DIMENSION ( LDA, n ).
*           Before entry with  UPLO = 'U' or 'u', the leading n by n
*           upper triangular part of the array A must contain the upper
*           triangular matrix and the strictly lower triangular part of
*           A is not referenced.
*           Before entry with UPLO = 'L' or 'l', the leading n by n
*           lower triangular part of the array A must contain the lower
*           triangular matrix and the strictly upper triangular part of
*           A is not referenced.
*           Note that when  DIAG = 'U' or 'u', the diagonal elements of
*           A are not referenced either, but are assumed to be unity.
*           Unchanged on exit.
*
*  LDA    - INTEGER.
*           On entry, LDA specifies the first dimension of A as declared
*           in the calling (sub) program. LDA must be at least
*           max( 1, n ).
*           Unchanged on exit.
*
*  X      - DOUBLE PRECISION array of dimension at least
*           ( 1 + ( n - 1 ) ).
*           Before entry, the incremented array X must contain the n
*           element right-hand side vector b. On exit, X is overwritten
*           with the solution vector x.
*
*
*     VERSION 2.0 DATED Aug. 1992
*     R. VAN DE GEIJN
*
*        This code resulted from a joint collaboration between  
*              Robert van de Geijn 
*     and the group for 'Research in Parallel Computational
*     Electromagnetics' at the Jet Propulsion Laboratory.  Futher 
*     information can be found in the paper,
*
*         T. Cwik, R. van de Geijn, and J. Patterson, 
*         "Application of Massively Parallel Computation to 
*          Integral Equation Models of Electromagnetic Scattering 
*          (Invited Paper)," J. Opt. Soc. Am. A, Vol. 11, No. 4, 
*          April 1994.
*
*     Please email your comments to the authors at
*
*        rvdg@cs.utexas.edu (Robert van de Geijn)
*        cwik@yosemite.jpl.nasa.gov (Tom Cwik)
*        jep@yosemite.jpl.nasa.gov  (Jean Patterson)
*
*     Use of this code must properly reference that paper.
*
*     The code is free, and comes with no implicit or explicit 
*     guarantees.  
*
*
*     All rights reserved
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO
      PARAMETER        ( ZERO = 0.0E+0 )
*     .. Local Scalars ..
      INTEGER            INFO
*     .. External Functions ..
      LOGICAL            LSAME
      EXTERNAL           LSAME
*     .. External Subroutines ..
      EXTERNAL           XERBLA
*     .. Intrinsic Functions ..
      INTRINSIC          MAX
*     ..
*     .. Executable Statements ..
*
*     Test the input parameters.
*
      INFO = 0
      IF     ( .NOT.LSAME( UPLO , 'U' ).AND.
     $         .NOT.LSAME( UPLO , 'L' )      )THEN
         INFO = 1
      ELSE IF( .NOT.LSAME( TRANS, 'N' ).AND.
     $         .NOT.LSAME( TRANS, 'T' ).AND.
     $         .NOT.LSAME( TRANS, 'C' )      )THEN
         INFO = 2
      ELSE IF( .NOT.LSAME( DIAG , 'U' ).AND.
     $         .NOT.LSAME( DIAG , 'N' )      )THEN
         INFO = 3
      ELSE IF( N.LT.0 )THEN
         INFO = 4
*
*     check what goes here later
*
*      ELSE IF( LDA.LT.MAX( 1, N ) )THEN
*         INFO = 6
c      ELSE IF( INCX.EQ.0 )THEN
c         INFO = 8
c      ELSE IF( INCX .NE. 1) THEN
*
*        INCX .NE. 1 not yet implemented
*
c         INFO = 8
      END IF
      IF( INFO.NE.0 )THEN
         CALL XERBLA( 'PDTRSV ', INFO )
         RETURN
      END IF
*
*     Quick return if possible.
*
      IF( N.EQ.0 )
     $   RETURN


      IF ( LSAME( TRANS, 'N') ) THEN
         IF (LSAME( UPLO, 'U')) THEN
            CALL PDUTSV( DIAG, N, NB, A, LDA, X, WORK,
     $           COMM )
         ELSE
            CALL PDLTSV( DIAG, N, NB, A, LDA, X, WORK,
     $           COMM )
         ENDIF
c      ELSEIF (LSAME( TRANS, 'C') ) THEN
c         IF (LSAME( UPLO, 'U')) THEN
c            CALL PDUTCSV( DIAG, N, NB, A, LDA, X, WORK )
c         ELSE
c            CALL PDLTCSV( DIAG, N, NB, A, LDA, X, WORK )
c         ENDIF
      ELSEIF (LSAME( TRANS, 'T') ) THEN
         print *, "PDTRSV TRANS=T not yet implemented"
      ENDIF         
*
      RETURN
*
*     End of PDTRSV .
*
      END
