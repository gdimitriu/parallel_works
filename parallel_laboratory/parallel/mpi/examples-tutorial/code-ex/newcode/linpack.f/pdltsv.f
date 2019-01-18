CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE PDLTSV( DIAG, N, NB, A, LDA, B, WORK, COMM ) 
      INCLUDE "mpif.h"
      INTEGER           N, NB, LDA
      CHARACTER*1       DIAG
      INTEGER           COMM( 3 )
      DOUBLE PRECISION  A( LDA, *), B( * ), WORK( * ) 
*
* Parallel double precision Lower Triangular Unit Solve
*
* Purpose
* =======
*
* pdltusv computes the solution of U x = b.
*     
* Arguments
* =========
*    
* DIAG   (input) character*1
*        indicates whether on not unit diagonal 
*        ('Unit' or 'Nonunit')
*
* n      (input) integer
*        dimension of matrix u.
*
* nb     (input) integer
*        block size
*
* a      (input) double precision array, dimension (lda,n)
*        upper triangular portion of a contains the uppertriangular 
*        matrix.
*
* lda    (input) integer
*        the leading dimension of the array a.  lda >= max(1,n).
*
* b      (input/output) double precision array, dimension n
*        On entry, this node's contribution to the
*        right hand side of the linear system.
*        On exit, this node's contribution to the solution vector x.
*
* work   double precision array, dimension MYNROW
*        array used to collect partial results
*
* info   (output) integer
*        = 0: successful exit
*        < 0: if info = -k, the k-th argument had an illegal value
*        > 0: if info = k, u(k,k) is exactly zero, no computation is
*             preformed.
*
* =======================================================================
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
*     .. parameters ..
      DOUBLE PRECISION  ONE
      PARAMETER         ( ONE = 1.0E+0 )
*     ..
*     .. local scalars ..
*
*     nprow          row dimension of node grid
*     npcol          column dimension of node grid
*
*     myrow          my row index
*     mycol          my column index
*
*     icurrow        index of node row that holds current diagonal block
*     icurcol        index of node column that holds current diagonal block
*
      INTEGER        NPROW, NPCOL, MYROW, MYCOL, ICURROW, ICURCOL
*
*     i              loop index
*     istart, ifin   start and end of local part of subvector (row index)
*     jstart         start of local part of current matrix block (column index)
*     isize          size of local part of subvector
*     jb             size of current diagonal block
*     mynrow         number of rows assigned to this node
*     idummy         dummy parameter
*
      INTEGER           I, ISTART, IFIN, JSTART, ISIZE, JB, MYNROW,
     $                  IDUMMY, II, J
*
*     neighbor indices
*
      INTEGER           INORTH, ISOUTH, JWEST, JEAST
      INTEGER           ROWCOMM, COLCOMM
*     ..
*     .. intrinsic functions ..
*
      INTRINSIC         MOD, MIN
*     ..
*     ..
*     .. external procedures ..
      EXTERNAL          DAXPY, DCOPY, PLAMCH2
     $                  DGEMV, DTRSV, IMYPART
*     ..
*     .. start of executable code ..

*     get machine parameters
*
      ROWCOMM = COMM( 2 )
      COLCOMM = COMM( 3 )
      CALL PLAMCH2( NPROW, NPCOL, MYROW, MYCOL, COMM )
*
*     get neighbor indices
*
      INORTH = MOD( MYROW-1+NPROW, NPROW )
      ISOUTH = MOD( MYROW+1, NPROW )
      JWEST = MOD( MYCOL-1+NPCOL, NPCOL )
      JEAST = MOD( MYCOL+1, NPCOL )
*
*     initialize  indices of node holds current diagonal block
*
      ICURROW = 0
      ICURCOL = 0
* 
*     mynrow = number of rows assigned to this node
*
      CALL IMYPART( 1, N, NB, I, MYNROW, MYROW, NPROW )
*
*     start of main loop
*
      DO 100 I = 1, N, NB
         IF (ICURCOL .EQ. MYCOL) THEN
            IF (I .GT. 1) THEN
*
*              compute where this node's part of the subvector starts
*              and ends
*
               CALL IMYPART( I, MIN( I+(NPCOL-1)*NB-1, N ) , NB, 
     $              ISTART, IFIN, MYROW, NPROW )
               ISIZE = IFIN-ISTART+1
*
*              receive update information and add to local information
*
               CALL DGERV2D( ISIZE, 1, WORK, ISIZE,
     $              JWEST, ROWCOMM, JWEST )
*
*              add to local result
*
               CALL DAXPY ( ISIZE, ONE, WORK, 1, B( ISTART ), 1 )
            ENDIF
            
            JB = MIN( NB, N-I+1 )

            IF (ICURROW .EQ. MYROW) THEN
*
*              solve triangular system
*
               CALL IMYPART( I, MIN( I+NB-1, N) , NB, ISTART, IFIN, 
     $              MYROW, NPROW )
               ISIZE = IFIN-ISTART+1

               CALL IMYPART( I, I, NB, JSTART, IDUMMY, MYCOL, NPCOL )

               CALL DTRSV ( 'LOWERTRIANGULAR', 'NOTRANSPOSE', DIAG,
     $              JB, A( ISTART, JSTART ), LDA, B( ISTART ), 1 )
*
*              pass result around ring
*
               IF (NPROW .GT. 1 .AND. I+NB .LE. N) THEN
                  CALL DGESD2D( JB, 1, B( ISTART ), JB,
     $                 ISOUTH, COLCOMM, MYROW )
               ENDIF
               CALL DCOPY ( JB, B( ISTART ), 1, WORK, 1 )
            ELSE
               IF (I+NB .LE. N) THEN
                  CALL DGERV2D( JB, 1, WORK, JB,
     $                 INORTH, COLCOMM, INORTH )

                  IF (ISOUTH .NE. ICURROW) THEN
                     CALL DGESD2D( JB, 1, WORK, JB,
     $                    ISOUTH, COLCOMM, MYROW )
                  ENDIF
               ENDIF
            ENDIF
            IF (I+NB .LE. N) THEN
*
*              update subvector
*
               CALL IMYPART( I+NB , MIN( I+NB*NPCOL-1, N) , NB, 
     $              ISTART, IFIN, MYROW, NPROW )
               ISIZE = IFIN-ISTART+1

               CALL IMYPART( I, I, NB, JSTART, IDUMMY, MYCOL, NPCOL )

               CALL DGEMV ( 'NOTRANSPOSE', ISIZE, JB, -ONE,
     $              A( ISTART, JSTART ), LDA, WORK, 1, ONE, 
     $              B( ISTART ), 1 )

*
*              pass subvector to right
*
               CALL DGESD2D( ISIZE, 1, B(ISTART), ISIZE,
     $              JEAST, ROWCOMM, MYCOL )
*
*              update rest of local result            
*
               CALL DGEMV ( 'NOTRANSPOSE', MYNROW-IFIN, JB, -ONE,
     $              A( IFIN+1, JSTART ), LDA, WORK, 1, ONE, 
     $              B( IFIN+1 ), 1 )
            ENDIF
         ENDIF
         ICURROW = MOD( ICURROW+1, NPROW )
         ICURCOL = MOD( ICURCOL+1, NPCOL )
 100  CONTINUE

*
*     zero all parts of x except those responding to the
*     diagonal elements of a
*
      ICURROW = 0
      ICURCOL = 0
      II = 1
      DO 200 I=1, N, NB
         IF (ICURROW .EQ. MYROW) THEN
            IF (ICURCOL .NE. MYCOL) THEN
               DO 150 J=1, MIN(NB, N-I+1)
                  B(II+J-1) = (0.0D00,0.0D00)
 150           CONTINUE
            ENDIF
            II = II+NB
         ENDIF
         ICURROW = MOD( ICURROW+1, NPROW)
         ICURCOL = MOD( ICURCOL+1, NPCOL)
 200  CONTINUE

      RETURN
      END

