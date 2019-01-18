CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE PDLUBR( M, N, NB, A, LDA, WORK,
     $     IPIV, INFO, COMM)
      INCLUDE "mpif.h"
*
*     .. Scalar Arguments ..
      INTEGER            LDA, M, N, NB, INFO
*     ..
*     .. Array Arguments ..
c
      INTEGER            COMM( 3 )
c
      INTEGER            IPIV( * )
      DOUBLE PRECISION   A( LDA, * ), WORK( * )
*     ..
*
*  Purpose
*  =======
*
*  PDLUBR computes an LU factorization of a general m-by-n matrix A
*  using partial pivoting with row interchanges.  It assumes this
*  node's portion of the matrix is in array A.  A block wrapped 
*  storage scheme with blocksize nb is assumed.
*
*  The factorization has the form
*     A = P * L * U
*  where P is a permutation matrix, L is lower triangular with unit
*  diagonal elements (lower trapezoidal if m > n), and U is upper
*  triangular (upper trapezoidal if m < n).
*
*  This is based on the right-looking Level 3 BLAS LAPACK version
*  of the algorithm.
*
*  NOTE: not currently fully tested for N .ne. M
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
*  WORK    DOUBLE PRECISION array, dimension LDWORK
*          Work array.
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
**********************************************************************
c  COMM    (input) INTEGER array, dimension 3
c          Array of communicators. First element is the communicator to
c          the entire process group, second element is the communicator
c          to the row and third is for the column.
c
**********************************************************************
*  =====================================================================
*
*     VERSION 2.0 DATED Aug. 1992
*     R. VAN DE GEIJN
*
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
*     All rights reserved
*
*     .. Parameters ..
      DOUBLE PRECISION   ONE
      PARAMETER          ( ONE = 1.0E+0 )
*     ..
*     .. Local Scalars ..
*
*     ..
*     .. Misc. ..
      INTEGER            I, IINFO, J, JB
      INTEGER            GRIDCOMM, ROWCOMM, COLCOMM
      INTEGER            IERROR
*
*     nprow          row dimension of node grid
*     npcol          column dimension of node grid
*     myrow          my row index
*     mycol          my column index
*
      INTEGER           NPROW, NPCOL, MYROW, MYCOL
      INTEGER           LOCALM, LOCALN
      INTEGER           ICURROW, ICURCOL, IWORK2
      INTEGER           II, JJ, IDUMMY
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEMM, DLASWP, PDLUR, DTRSM, XERBLA
*
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          MAX, MIN
*     ..
*     .. Executable Statements ..
*
*     Test the input parameters.
*
      INFO = 0
c
c     Duplicate the communicators for safe communication
c
      CALL MPI_COMM_DUP( COMM(1), GRIDCOMM, IERROR )
      CALL MPI_COMM_DUP( COMM(2), ROWCOMM, IERROR )
      CALL MPI_COMM_DUP( COMM(3), COLCOMM, IERROR )
*
*     get machine parameters
*
**********************************************************************
      CALL PLAMCH2( NPROW, NPCOL, MYROW, MYCOL, COMM )
**********************************************************************
*
*     determine local number of rows and columns
*
      CALL IMYPART( 1, M, NB, IDUMMY, LOCALM, MYROW, NPROW )
      CALL IMYPART( 1, N, NB, IDUMMY, LOCALN, MYCOL, NPCOL )
*
*     check input parameters
*
c     IF( M.LT.0 ) THEN
c        INFO = -1
c     ELSE IF( N.LT.0 ) THEN
c        INFO = -2
c     ELSE IF( LDA.LT.MAX( 1, LOCALM ) ) THEN
c        INFO = -5
c     ELSE IF( NB.LE.0 ) THEN
c        INFO = -3
c     END IF
c     IF( INFO.NE.0 ) THEN
c        CALL XERBLA( 'PDLUBR ', -INFO )
c        RETURN
c     END IF

c     print *, "Entering PDLUBR"
      IWORK2 = NB*(LOCALM+NB)+1
*     
      ICURROW = 0
      ICURCOL = 0

      II = 1
      JJ = 1
      DO 20 J = 1, MIN( M, N ), NB
         JB = MIN( MIN( M, N )-J+1, NB )
*
*           Factor diagonal and subdiagonal blocks and test for exact
*           singularity.
*
         IF (MYCOL .EQ. ICURCOL) THEN
c           print *, "mycol = icurcol"
            CALL PDLUR( M-J+1, JB, A( II, JJ ), LDA, IPIV( J ), 
     $           ICURROW, WORK, NB, NB, 
     $           IINFO, COMM )
c           print *, "Returned from pdlur"
         ENDIF
*
*           Adjust INFO and the pivot indices.
*
         IF( INFO.EQ.0 .AND. IINFO.GT.0 )
     $        INFO = IINFO + J - 1
         DO 10 I = J, J + JB - 1
            IF (IPIV(I) .GT. 0) 
     $           IPIV( I ) = II - 1 + IPIV( I )
 10      CONTINUE
*
*     broadcast pivot info
*
c
c        CALL IGEBC2D( 'RING', 'ROW', JB, 1, IPIV(J), JB,            
c    $        MYROW, ICURCOL, ITYPE_ROW())
c
**********************************************************************
c        print *, "before bcast"
         CALL MPI_BCAST( IPIV(J), JB, MPI_INTEGER, ICURCOL, 
     $        ROWCOMM, IERROR )
c        print *, "after bcast"
**********************************************************************
*
*           Apply interchanges to columns outside this block.
*
         IF (MYCOL .NE. ICURCOL) THEN
            CALL PDLASWP( LOCALN, A, LDA, J, J+JB-1, 
     $           II, ICURROW, IPIV, COLCOMM )
         ELSE
            IF (JJ .GT. 1) THEN
               CALL PDLASWP( JJ-1, A, LDA, J, J+JB-1,
     $              II, ICURROW, IPIV, COLCOMM )
            ENDIF
            IF (JJ+JB .LE. LOCALN) THEN
               CALL PDLASWP( LOCALN-JJ-JB+1, A(1,JJ+JB), LDA, 
     $              J, J+JB-1, II, ICURROW, IPIV, 
     $              COLCOMM )
            ENDIF
         ENDIF
c        print *, "after swap"
*
         IF (J+JB .LE. N) THEN
*
*     broadcast factored panel
*
            IF (MYROW .EQ. ICURROW) II = II + JB
            IF (MYCOL .EQ. ICURCOL) 
*
*     copy this node's part of factored panel into work array
*
     $           CALL DLACPY( 'G', LOCALM-II+1, JB, A( II, JJ ),
     $           LDA, WORK( NB*NB+1 ), LOCALM-II+1 )
*     
*     NOTICE: for some rows, sending too much
*
c
c           CALL ZGEBCS ('CTREE', 'ROW', LOCALM-II+1+NB, JB, 
c    $           WORK, LOCALM-II+1+NB, MYROW, ICURCOL, 
c    $           ITYPE_ROW() )
**********************************************************************
            CALL DGEBC2D( (LOCALM-II+1+NB), JB, WORK, 
     $           (LOCALM-II+1+NB), ICURCOL, ROWCOMM )
**********************************************************************
            IF (MYROW .EQ. ICURROW) II = II-JB
         ENDIF
*            
         IF (MYCOL .EQ. ICURCOL) JJ = JJ+JB
*
         IF (JJ .LE. LOCALN) THEN
*     
*     update horizontal panel
*
            IF ( MYROW .EQ. ICURROW) THEN
               CALL DTRSM( 'LEFT', 'LOWER', 'NO TRANSPOSE', 
     $              'UNIT', JB, LOCALN-JJ+1, ONE, WORK, NB,
     $              A( II, JJ ), LDA )
               CALL DLACPY( 'G', JB, LOCALN-JJ+1, A( II, JJ ),
     $              LDA, WORK(IWORK2), JB )
c
c              CALL ZGEBCS ( 'CTREE', 'COLUMN', JB, LOCALN-JJ+1, 
c    $              WORK(IWORK2), JB, ICURROW, MYCOL,
c    $              ITYPE_COL() )
**********************************************************************
               CALL DGEBC2D ( JB, LOCALN-JJ+1, WORK(IWORK2), JB,
     $              ICURROW, COLCOMM )
**********************************************************************
               II = II+JB
            ELSE
c
c              CALL ZGEBCS ( 'CTREE', 'COLUMN', JB, LOCALN-JJ+1, 
c    $              WORK(IWORK2), JB, ICURROW, MYCOL,
c    $              ITYPE_COL() )
**********************************************************************
               CALL DGEBC2D( JB, LOCALN-JJ+1, WORK(IWORK2), JB, 
     $              ICURROW, COLCOMM )
**********************************************************************
            ENDIF
*
            IF( II .LE. LOCALM ) THEN
*
*                 Update trailing submatrix.
*
               CALL DGEMM( 'NO TRANSPOSE', 'NO TRANSPOSE', 
     $              LOCALM-II+1, LOCALN-JJ+1, JB, -ONE, 
     $              WORK(NB*NB+1), 
     $              LOCALM-II+1, WORK(IWORK2), JB, 
     $              ONE, A(II, JJ),
     $              LDA )
            END IF
         ELSEIF (MYROW .EQ. ICURROW) THEN
            II = II+JB
         END IF
         ICURROW = MOD( ICURROW+1, NPROW )
         ICURCOL = MOD( ICURCOL+1, NPCOL )
 20      CONTINUE

      RETURN
*
*     End of PDLUBR
*
      END
