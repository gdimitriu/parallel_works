CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE PDLUR( M, N, A, LDA, IPIV, ICURROW, 
     $     WORK, LDWORK, NB, INFO, COMM )
      INCLUDE "mpif.h"
*
*     .. Scalar Arguments ..
      INTEGER            INFO, LDA, M, N
*     ..
*     .. Array Arguments ..
      INTEGER            IPIV( * )
      DOUBLE PRECISION   A( LDA, * )
*
*     icur               index of row that holds current diagonal 
*                        block
*     nb                 block size of wrapping
*
      DOUBLE PRECISION   WORK(LDWORK, *)
      INTEGER            ICURROW, NB, LDWORK, COMM( 3 )
*     ..
*
*  Purpose
*  =======
*
*  PDLUR computes an LU factorization of a general m-by-n matrix A
*  using partial pivoting with row interchanges.
*
*  The factorization has the form
*     A = P * L * U
*  where P is a permutation matrix, L is lower triangular with unit
*  diagonal elements (lower trapezoidal if m > n), and U is upper
*  triangular (upper trapezoidal if m < n).
*
*  This is the right-looking Level 2 BLAS version of the algorithm.
*
*  Arguments
*  =========
*
*  M       (input) INTEGER
*          The number of rows of the matrix A.  M >= 0.
*
*  N       (input) INTEGER
*          The number of columns of the matrix A.  N >= 0.
*
*  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
*          On entry, the m by n matrix to be factored.
*          On exit, the factors L and U; the unit diagonal elements of L
*          are not stored.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A.  LDA >= max(1,M).
*
*  IPIV    (output) INTEGER array, dimension (min(M,N))
*          The pivot indices.  Row i of the matrix was interchanged with
*          row IPIV(i).
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
*     .. Parameters ..
      DOUBLE PRECISION   ONE, ZERO
      PARAMETER          ( ONE = 1.0E+0, ZERO = 0.0E+0 )
*     ..
*     .. Local Scalars ..
      INTEGER            J, JP
*     ..
*     .. local scalars ..
*
*     nprow          row dimension of node grid
*     npcol          column dimension of node grid
*
*     IMPORTANT:  this code assumes nprow < npcol
*
*     myrow          my row index
*     mycol          my column index
*
      INTEGER           NPROW, NPCOL, MYROW, MYCOL, MYRROW
      INTEGER           MYMROW, IPIVNODE
      INTEGER           IDUMMY
      DOUBLE PRECISION  DDUMMY( 2 ), DTEMP( 2 )
      INTEGER           GRIDCOMM, ROWCOMM, COLCOMM, IERROR
*     ..
*     .. External Functions ..
      INTEGER           IDAMAX
      EXTERNAL          IDAMAX, PURI_MAXLOC_FN
*     ..
*     .. external functions ..
c     INTEGER           ITYPE_TO, ITYPE_FROM, ITYPE_COL
c     DOUBLE PRECISION  DABS
*     ..
*     .. External Subroutines ..
      EXTERNAL           DCOPY, DGER, DSCAL, DSWAP, XERBLA, PLAMCH2
      EXTERNAL           DGESD2D, DGERV2D, DGEBC2D
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          MAX, MIN
*     ..
*     .. Executable Statements ..
*
*     Test the input parameters.
*
c     INFO = 0
c     IF( M.LT.0 ) THEN
c        INFO = -1
c     ELSE IF( N.LT.0 ) THEN
c        INFO = -2
*      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
*         INFO = -4
c     END IF
c     IF( INFO.NE.0 ) THEN
c        CALL XERBLA( 'PDLUR  ', -INFO )
c        RETURN
c     END IF
c
      GRIDCOMM = COMM( 1 )
      ROWCOMM = COMM( 2 )
      COLCOMM = COMM( 3 )
c     print *, "After comm dup "
*
*     get machine parameters
*
      CALL PLAMCH2( NPROW, NPCOL, MYROW, MYCOL, COMM )
*
*     myrrow =    row index of node relative to node with row index
*                 icurrow
*                 
      MYRROW = MOD( MYROW - ICURROW + NPROW, NPROW )
*
*     mymrow = number of rows assigned to this node
*
      CALL IMYPART( 1, M, NB, IDUMMY, MYMROW, MYRROW, NPROW )
*
      DO 10 J = 1, MIN( NB, N )
*
*           Find pivot and test for singularity.
*
         IF ( MYROW .EQ. ICURROW ) THEN
            JP = J - 1 + IDAMAX( MYMROW-J+1, A( J, J ), 1 )
            CALL DCOPY( N, A( JP, 1 ), LDA, WORK( J, 1 ), 
     $           LDWORK )
         ELSE
            IF (MYMROW .GE. 1) THEN
               JP = IDAMAX( MYMROW, A( 1, J ), 1 )
               CALL DCOPY( N, A( JP, 1 ), LDA, WORK( J, 1 ), 
     $              LDWORK )
            ELSE
               JP = 0
               WORK(J,J) = 0.0D0
            ENDIF
         ENDIF
c
c        determine pivot row
c
         DTEMP( 1 ) = DABS( WORK( J, J ) )
         DTEMP( 2 ) = MYROW
c
c        CALL DGAMAX2D( 'COLUMN', 1, 1, DTEMP, 1, DDUMMY, 1, 
c    $        IPIVNODE, 1, IDUMMY, 1, -1, MYCOL )
c
 
         CALL MPI_OP_CREATE( PURI_MAXLOC_FN, .TRUE., PURI_MAXLOC, 
     $        IERROR )
         CALL MPI_ALLREDUCE( DTEMP, DDUMMY, 2, 
     $        MPI_DOUBLE_PRECISION,
     $        PURI_MAXLOC, COLCOMM, IERROR )
 
         IPIVNODE = DDUMMY( 2 )
         IF ( DDUMMY( 1 ) .EQ. 0.0D00 ) IPIVNODE = ICURROW
c
c        send pivot row to all nodes in this column
c
c        CALL ZGEBC2D( 'CTREE', 'COLUMN', 1, N, WORK( J, 1 ), LDWORK,
c    $        IPIVNODE, MYCOL, ITYPE_COL() )
c
*
         CALL DGEBC2D( 1, N, WORK( J, 1 ), LDWORK,
     $        IPIVNODE, COLCOMM )
*
         IF (MYROW .EQ. ICURROW) THEN
            IF ( IPIVNODE .EQ. MYROW ) THEN
               IF ( J .NE. JP ) 
     $              CALL DSWAP( N, A( J, 1 ), LDA, A( JP, 1 ), 
     $              LDA )
               IPIV( J ) = JP
            ELSE
c              CALL ZGESD2D( 1, N, A( J, 1 ), LDA, IPIVNODE, 
c    $              MYCOL, ITYPE_TO( IPIVNODE, MYCOL ) )
*
               CALL DGESD2D( 1, N, A( J, 1 ), LDA, 
     $              IPIVNODE, COLCOMM, IPIVNODE )
*
               CALL DCOPY( N, WORK( J, 1 ), LDWORK, A( J, 1 ), LDA )
               IPIV( J ) = - IPIVNODE
            ENDIF
         ELSEIF ( MYROW .EQ. IPIVNODE ) THEN
c           CALL ZGERV2D( 1, N, A( JP, 1 ), LDA, 
c    $           ITYPE_FROM( ICURROW, MYCOL) )
*
            CALL DGERV2D( 1, N, A( JP, 1 ), LDA,
     $           ICURROW, COLCOMM, IPIVNODE )
*
            IPIV( J ) = JP
         ELSE
            IPIV( J ) = - IPIVNODE
         ENDIF

*        IF (MYROW .EQ. 0 .AND. MYCOL .EQ. 0) THEN
*            call prtmat( 1, nb, work( j, 1 ), ldwork )
*            call prtmat( 1, nb, a( j, 1 ), lda )
*        ENDIF
*
*           Compute elements J+1:M of J-th column.
*
         IF (WORK ( J, J ) .EQ. ZERO ) THEN
            PRINT *, "EXACT SINGULARITY DETECTED:TROUBLE !!"
         ELSE   
            IF (MYROW .EQ. ICURROW) THEN
               IF( J.LT.MYMROW )
     $              CALL DSCAL( MYMROW-J, ONE/WORK( J, J ), 
     $              A( J+1, J ), 1 )
            ELSE
               IF ( MYMROW .GT. 0 )
     $              CALL DSCAL( MYMROW, ONE/WORK( J, J ), A( 1, J ),
     $              1 )
            ENDIF
         ENDIF
*
*

         IF( J+1.LE.N .AND. WORK (J, J) .NE. ZERO) THEN
*
*           Update trailing submatrix.
*
            IF ( MYROW .EQ. ICURROW ) THEN
               IF (MYMROW .GT. J) 
     $              CALL DGER( MYMROW-J, N-J, -ONE, A( J+1, J ), 1, 
     $              WORK ( J, J+1 ), LDWORK, A( J+1, J+1 ), LDA )
            ELSEIF ( MYMROW .GT. 0 ) THEN
               CALL DGER( MYMROW, N-J, -ONE, A( 1, J ), 1, 
     $              WORK ( J, J+1 ), LDWORK, A( 1, J+1 ), LDA )
            ENDIF
         END IF

 10   CONTINUE

      RETURN
*
*     End of PDLUR
*
      END
 

