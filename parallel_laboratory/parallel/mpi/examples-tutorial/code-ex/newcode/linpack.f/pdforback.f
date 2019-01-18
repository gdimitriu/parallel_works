CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE PDFORBACK ( N, NB, A, LDA, X, WORK, IPIV, INFO, 
     $     COMM ) 
      INCLUDE "mpif.h"
*
*     Parallel blocked forward and backward substitution routine 
*     that uses output from PDGESVX
*
*
*
*     .. Scalar Arguments ..
      INTEGER            LDA, N, NB, INFO
*     ..
*     .. Array Arguments ..
      INTEGER            IPIV( * ), COMM( 3 )
      DOUBLE PRECISION   A( LDA, * ), X( * ), WORK( * )
*     ..
*
*  Purpose
*  =======
*
*  PDFORBACK performs forward and backward substitution on a single 
*  right hand side using the output of DDGESVX.  It assumes this
*  node's portion of the factorization is in array A.  A block wrapped 
*  storage scheme with blocksize nb is assumed.
*
*  The factorization has the form
*     A = P * L * U
*  where P is a permutation matrix, L is lower triangular with unit
*  diagonal elements (lower trapezoidal if m > n), and U is upper
*  triangular (upper trapezoidal if m < n).
*
*  Arguments
*  =========
*
*  N       (input) INTEGER
*          The number of rows and columns of the matrix A.  N >= 1.
*
*  NB      (input) INTEGER
*          The block size for the blocked algorithm.  NB > 0.
*
*  A       (input/output) DOUBLE PRECISION array, dimension (LDA,LOCALN)
*          On entry, this node's portion of m by n matrix to be factored.
*          On exit, this node's factors L and U; the unit diagonal 
*          elements of L are not stored.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A that holds the local
*          portion of the matrix to be factored.  LDA >= max(1,LOCALM).
*
*  X       (input/output) double precision array of dimension 
*          max( localn, localm)
*          Input: right hand side
*          Output: solution vector
*
*  WORK    DOUBLE PRECISION array, dimension LDWORK
*          Work array.
*
*  LDWORK  (input) INTEGER
*          Size of work array WORK.  LDWORK > ????
*
*  IPIV    (output) INTEGER array, dimension (min(M,N))
*          The pivot indices.  Row i of the matrix was interchanged with
*          row IPIV(i) if IPIV(i) is a row that resides on this node.
*          otherwise -IPIV(i) equals the row index of the node that
*          holds the pivot row.
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -k, the k-th argument had an illegal value
*          > 0: if INFO = k, U(k,k) is exactly zero. The factorization
*               has been completed, but the factor U is exactly
*               singular, and division by zero will occur if it is used
*               to solve a system of equations or to compute the inverse
*               of A.
*
*  =====================================================================
*
*     VERSION 2.0 DATED Aug. 1992
*     R. VAN DE GEIJN
*
*     All rights reserved
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO
      PARAMETER          ( ZERO = 0.0E+0 )
*     
*     Scope parameters
*
      INTEGER            COLUMN, ROW
      PARAMETER          ( COLUMN = 1, ROW = 2 )
*
*     Topology parameters
*
      INTEGER            TREE, RING, CTREE
      PARAMETER          ( RING = 1, TREE = 2, CTREE = -2 )
*     ..
*     .. Local Scalars ..
*
*     ..
*     .. Misc. ..
      INTEGER            IINFO
      DOUBLE PRECISION   ANRM1
*
*     nprow          row dimension of node grid
*     npcol          column dimension of node grid
*     myrow          my row index
*     mycol          my column index
*
      INTEGER           NPROW, NPCOL, MYROW, MYCOL
      INTEGER           LOCALM, LOCALN, IDUMMY
*     ..
*     .. External Functions ..
      DOUBLE PRECISION  PDDMNRM1
      LOGICAL           LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           PDLUBR, XERBLA, IMYPART, PLAMCH2
*     ..
*     .. Intrinsic Functions ..
*     ..
*     .. Executable Statements ..
*
*     Test the input parameters.
*
      INFO = 0

*
*     get machine parameters
*
      CALL PLAMCH2( NPROW, NPCOL, MYROW, MYCOL, COMM )
*
*     determine local number of rows and columns
*
      CALL IMYPART( 1, N, NB, IDUMMY, LOCALM, MYROW, NPROW )
      CALL IMYPART( 1, N, NB, IDUMMY, LOCALN, MYCOL, NPCOL )
      
*
*     check input parameters
*
      IF( N.LT.0 ) THEN
         INFO = -1
      ELSE IF( LDA.LT.MAX( 1, LOCALM ) ) THEN
         print *, "LDA = ", lda, "localm = ", localm
         INFO = -4
      ELSE IF( NB.LE.0 ) THEN
         INFO = -2
c      ELSE IF (LDWORK .LT. (LOCALM+NB+LOCALN)*NB ) THEN
c         INFO = -6
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'PDFORBACK ', -INFO )
         RETURN
      END IF
*
*     pivot right-hand-side consistent with the pivoting of the
*     matrix during the factorization
* 
      
      IF (MYCOL .EQ. 0) THEN
         CALL PDRHSSWAP( N, NB, X, IPIV, COMM )
      ENDIF
*      
*     forward solve
*
      CALL PDTRSV('LOWER TRIANGULAR', 'NO TRANSPOSE', 
     $     'UNIT', N, NB, A, LDA, X, WORK, INFO, COMM )
*      
*     backward solve
*
      CALL PDTRSV('UPPER TRIANGULAR', 'NO TRANSPOSE', 
     $     'NON UNIT', N, NB, A, LDA, X, WORK, INFO,
     $     COMM )
*
*     send result to first row of nodes, leaving it distributed
*     like the first row of the matrix
*     
      CALL PDVREDISTR( 'COLUMN', 'DIAG', 0, N, NB, X, 
     $     'ROW', 'REGULAR', 0, WORK, COMM )

      RETURN
*
*  End of PDFORBACK
*
      END

      

