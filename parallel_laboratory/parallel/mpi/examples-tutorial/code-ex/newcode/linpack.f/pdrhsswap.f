CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE  PDRHSSWAP( N, NB, X, IPIV, COMM )
      INCLUDE "mpif.h"
*
*     Parallel vector permutation routine
*
*     .. Scalar Arguments
      INTEGER               N, NB, COMM( 3 )
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION      X( * )
      INTEGER               IPIV( * )
*     ..
*
*  Purpose
*  =======
*
*  PDRHSSWAP permutes vector X according to the pivot information
*  in IPIV to correspond to the row interchanges performed by
*  PDLUBR/PDLUR.
*
*  Arguments
*  =========
*
*  N        (input) INTEGER
*           Length of vector X
*
*  NB       (input) INTEGER
*           The block size used for the parallel data distribution
*
*  X        (input/output) DOUBLE PRECISION array of length (myn)
*           Local portion of the vector.  Permuted upon completion
*           of the routine
*
*  IPIV     (input) INTEGER array, dimension (N)
*           The pivot indices.  Row i of the matrix was interchanged with
*           row IPIV(i) if IPIV(i) is a row that resides on this node.
*           otherwise -IPIV(i) equals the row index of the node that
*           holds the pivot row.
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
*     .. Parameters ..
*     ..
*     .. Local Scalars ..
*
*     nprow          row dimension of node grid
*     npcol          column dimension of node grid
*     myrow          my row index
*     mycol          my column index
*
      INTEGER           NPROW, NPCOL, MYROW, MYCOL
*
*     Misc
*
      INTEGER          ICURROW, II, I, COLCOMM
*     ..
*     .. External Subroutines ..
      EXTERNAL         PDLASWP
*     ..
*     .. External Functions ..
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC MOD
*     ..
*     .. Executable Statements ..
*
*     get machine parameters
*
      COLCOMM = COMM( 3 )
      CALL PLAMCH2(NPROW, NPCOL, MYROW, MYCOL, COMM)
*
*     swap one block at a time
*
      ICURROW = 0
      II = 1
      DO 100 I=1, N, NB
         CALL PDLASWP( 1, X, N, I, MIN(I+NB-1, N), II, 
     $        ICURROW, IPIV, COLCOMM )
         IF (ICURROW .EQ. MYROW) II = II+NB
         ICURROW = MOD( ICURROW+1, NPROW )
 100  CONTINUE
      
      RETURN
*
*     End of PDRHSSWAP
*
      END

