CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE PDGEMV( N, NB, A, LDA, X, Y, WORK, INFO, COMM )
      INCLUDE "mpif.h"
*
*     Parallel blocked matrix vector multiplication routine
*
*
*
*     .. Scalar Arguments ..
      INTEGER            LDA, N, NB, LDWORK, INFO
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), X( * ), Y( * ), WORK( * )
      INTEGER            COMM( 3 )
*     ..
*
*  Purpose
*  =======
*
*  PDGEMV computes 
*            y = A x 
*  where initially x is distributed like the first row of
*  the matrix, and y is left distributed like the first
*  column of the matrix.
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
*  A       (input) DOUBLE PRECISION array, dimension (LDA,LOCALN)
*          This node's portion of n by n matrix 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A that holds the local
*          portion of the matrix.  LDA >= max(1,LOCALM).
*
*  X       (input) DOUBLE PRECISION array of dimension (LOCALN)
*          Vector to be multiplied by A
*
*  Y       (output) DOUBLE PRECISION array, dimension ( LOCALM )
*          Result vector.
*
*  WORK    DOUBLE PRECISION array, dimension ( LOCALM )
*          Work array.
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -k, the k-th argument had an illegal value
*
*  =====================================================================
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
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0E+0, ONE = 1.0E+0 )
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
      INTEGER           ROWCOMM, COLCOMM, IERR
*     ..
*     .. External Functions ..
      DOUBLE PRECISION  PDDMNRM1
      LOGICAL           LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           PDLUBR, XERBLA, IMYPART, 
     $                   PLAMCH2, PDGECON
*     ..
*     .. Intrinsic Functions ..
*     ..
*     .. Executable Statements ..
*
*     Test the input parameters.
*
      INFO = 0
      ROWCOMM = COMM( 2 )
      COLCOMM = COMM( 3 )
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
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'PDGESVX ', -INFO )
         RETURN
      ENDIF
*
*     Distribute X within columns of nodes
*
c
c     call ZGEBC2D( 'TREE', 'COLUMN', LOCALN, 1, X, LOCALN, 
c    $     0, MYCOL, itype_col() )
*
      CALL DGEBC2D( LOCALN, 1, X, LOCALN, 0, COLCOMM )
*
*
*     Perform local matrix vector multiply
*
      call dgemv( 'No Transpose', LOCALM, LOCALN, ONE, A, 
     $     LDA, X, 1, ZERO, WORK, 1 )
*
*     Combine local results
*
c
c     call ZGSUM2D( 'Row', LOCALM, 1, WORK, LOCALM, y, LOCALM,
c    $     MYROW, 0 )
*
      CALL MPI_ALLREDUCE( WORK, y, LOCALM, MPI_DOUBLE_PRECISION,
     $     MPI_SUM, ROWCOMM, IERR )
*
*  End of PDGEMV
*
      END

      

