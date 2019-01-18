CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE PDVREDISTR( DIR1, DIAG1, K1, N, NB, X,
     $     DIR2, DIAG2, K2, WORK, COMM )
      INCLUDE "mpif.h"
*
*     .. Scalar Arguments ..
      CHARACTER*1            DIR1, DIR2, DIAG1, DIAG2
      INTEGER                K1, N, NB, K2
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION       X( * ), WORK( * )
      INTEGER                COMM( 3 )
*
*  Purpose
*  =======
*
*  PDVREDISTR redistributes a vector among the logical grid
*  of nodes
*  Arguments
*  =========
*
*  DIR1    (input) CHARACTER*1
*          Direction in which vector is originally distributed
*
*  DIAG1   (input) CHARACTER*1
*          Indicates whether the vector is originally distributed
*          among the "diagonal" nodes 
*
*  K1      (input) INTEGER
*          if DIAG1 = 'R' then K1 indicates which row or column
*          originally holds the vector
*
*  N       (input) INTEGER
*          Length of vector
*
*  NB      (input) INTEGER
*          The block size used for wrapping
*
*  X       (input/output) DOUBLE PRECISION array of dimension 
*          ( myn or mym )
*          Holds local portion of vector
*
*  DIR2    (input) CHARACTER*1
*          Direction in which vector is finally distributed
*
*  DIAG2   (input) CHARACTER*1
*          Indicates whether the vector is finally  distributed
*          among the "diagonal" nodes 
*
*  K2      (input) INTEGER
*          if DIAG2 = 'R' then K2 indicates which row or column
*          finally holds the vector
*
*  WORK    DOUBLE PRECISION array of dimension ( myn or mym )
*          Work array
*
*  ============================================================
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
*     ..
*     .. Local Scalars ..
*
*     nprow          row dimension of node grid
*     npcol          column dimension of node grid
*     myrow          my row index
*     mycol          my column index
*
      INTEGER           NPROW, NPCOL, MYROW, MYCOL
      INTEGER           ICURROW, ICURCOL
      INTEGER           II, I, J, ISIZE, JJ, K, MYN, IDUMMY
      INTEGER           ROWCOMM, COLCOMM, IERR
      INTEGER           STATUS(MPI_STATUS_SIZE)
*     ..
*     .. External Functions ..
      INTEGER           ITYPE_FROM, ITYPE_TO
      LOGICAL           LSAME
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC         MIN, MOD
*     ..
*     .. Executable Statements ..

      ROWCOMM = COMM( 2 )
      COLCOMM = COMM( 3 )

      CALL PLAMCH2(NPROW, NPCOL, MYROW, MYCOL, COMM)
      
      IF ( LSAME( DIR1, DIR2 ) .AND. 
     $     LSAME( DIAG1, DIAG2 ) ) THEN
*
*     idir1  = idir2 
*     idiag1 = idiag2
*
*     do nothing
*
         RETURN
      ELSE IF ( LSAME( DIR1, DIR2 ) ) THEN
         PRINT *, "DIR1=DIR2=C AND DIAG1 != DIAG2 NOT 
     $        YET IMPLEMENTED"
         STOP
      ELSE IF ( LSAME( DIR1, 'C' ) .AND.
     $        LSAME( DIR2, 'R' ) .AND.
     $        LSAME( DIAG1, 'D' ) .AND.
     $        .NOT.LSAME( DIAG2, 'D' ) ) THEN
         CALL IMYPART( 1, N, NB, IDUMMY, MYN, MYCOL, NPCOL )
*     
*     send result to row of nodes that holds first 
*     row of matrix, distributing the result like
*     the first row.
*     
         
*     
*     pack local portion.  Inefficient, but it works.
*     
         ICURROW = 0
         ICURCOL = 0
         II = 1
         JJ = 1
         ISIZE = 0
         DO 100 I=1, N, NB
            IF (ICURROW .EQ. MYROW .AND. ICURCOL .EQ. MYCOL) THEN
               CALL DCOPY( MIN( NB, N-I+1 ), X(JJ), 1, WORK(II), 1 )
               II = II+NB
               ISIZE = ISIZE+MIN( NB, N-I+1 )
            ENDIF
            IF (ICURROW .EQ. MYROW) JJ = JJ+NB
            ICURROW = MOD( ICURROW+1, NPROW)
            ICURCOL = MOD( ICURCOL+1, NPCOL)
 100     CONTINUE
         
         IF (MYROW .EQ. K2) THEN
            JJ = MYROW
            DO 120 I=0,NPROW-1
               IF (JJ .NE. MYROW) THEN
c     
c                 CALL IGERV2D(1, 1, ISIZE, 1, 
c    $                 ITYPE_FROM( JJ, MYCOL ) )
c                 CALL DGERV2D( ISIZE, 1, WORK, ISIZE, 
c    $                 ITYPE_FROM( JJ, MYCOL ) )
*
                  CALL MPI_RECV( ISIZE, 1, MPI_INTEGER, JJ, K2,
     $                 COLCOMM, STATUS, IERR )
                  CALL MPI_RECV( WORK, ISIZE, MPI_DOUBLE_PRECISION,
     $                 JJ, K2, COLCOMM, STATUS, IERR )
*
               ENDIF
               II = NB*MYCOL
               ICURROW = MOD( MYCOL, NPROW )
               K = 1
               DO 110 J=1, MYN, NB
                  IF (ICURROW .EQ. JJ) THEN
                     CALL DCOPY( MIN( NB, MYN-J+1), WORK(K), 1, 
     $                    X(J), 1 )
                     K = K+NB
                  ENDIF
                  ICURROW = MOD( ICURROW + NPCOL, NPROW )
 110           CONTINUE
               JJ = MOD( JJ+1, NPROW )
 120        CONTINUE
         ELSE
c
c           CALL IGESD2D( 1, 1, ISIZE, 1, K2, MYCOL, 
c    $           ITYPE_TO( K2, MYCOL ) )
c           CALL DGESD2D( ISIZE, 1, WORK, ISIZE, K2, MYCOL, 
c    $           ITYPE_TO( K2, MYCOL ) )
*
            CALL MPI_SEND( ISIZE, 1, MPI_INTEGER, K2, K2, COLCOMM, 
     $           IERR )
            CALL MPI_SEND( WORK, ISIZE, MPI_DOUBLE_PRECISION,
     $           K2, K2, COLCOMM, IERR )
*
            DO 130 I=1, MYN
               X(I) = (0.0D0, 0.0D0)
 130        CONTINUE
         ENDIF
      ELSE IF ( LSAME( DIR1, 'C' ) .AND.
     $        LSAME( DIR2, 'R' ) )  THEN
         PRINT *, "DIR1=C, DIR2=R, DIAG2=D OR DIAG1=C NOT 
     $        YET IMPLEMENTED"
         STOP
      ELSE IF ( LSAME( DIR1, 'R' ) .AND. 
     $        LSAME( DIR2, 'C' ) .AND.
     $        LSAME( DIAG1, 'D' ) .AND.
     $        .NOT.LSAME( DIAG2, 'D' ) ) THEN
         CALL IMYPART( 1, N, NB, IDUMMY, MYN, MYROW, NPROW )
*     
*     send result to row of nodes that holds first 
*     column of matrix, distributing the result like
*     the first column.
*     
         
*     
*     pack local portion.  Inefficient, but it works.
*     
         ICURROW = 0
         ICURCOL = 0
         II = 1
         JJ = 1
         ISIZE = 0
         DO 140 I=1, N, NB
            IF (ICURROW .EQ. MYROW .AND. ICURCOL .EQ. MYCOL) THEN
               CALL DCOPY( MIN( NB, N-I+1 ), X(JJ), 1, WORK(II), 1 )
               II = II+NB
               ISIZE = ISIZE+MIN( NB, N-I+1 )
            ENDIF
            IF (ICURCOL .EQ. MYCOL) JJ = JJ+NB
            ICURROW = MOD( ICURROW+1, NPROW)
            ICURCOL = MOD( ICURCOL+1, NPCOL)
 140     CONTINUE
         
         IF (MYCOL .EQ. K2) THEN
            JJ = MYCOL
            DO 160 I=0,NPCOL-1
               IF (JJ .NE. MYCOL) THEN
c
c                 CALL IGERV2D(1, 1, ISIZE, 1, 
c    $                 ITYPE_FROM( MYROW, JJ ) )
c                 CALL DGERV2D( ISIZE, 1, WORK, ISIZE, 
c    $                 ITYPE_FROM( MYROW, JJ ) )
*
                  CALL MPI_RECV( ISIZE, 1, MPI_INTEGER, JJ, K2,
     $                 ROWCOMM, STATUS, IERR )
                  CALL MPI_RECV( WORK, ISIZE, MPI_DOUBLE_PRECISION,
     $                 JJ, K2, ROWCOMM, STATUS, IERR )
*
               ENDIF
               II = NB*MYROW
               ICURCOL = MOD( MYROW, NPCOL )
               K = 1
               DO 150 J=1, MYN, NB
                  IF (ICURCOL .EQ. JJ) THEN
                     CALL DCOPY( MIN( NB, MYN-J+1), WORK(K), 1, 
     $                    X(J), 1 )
                     K = K+NB
                  ENDIF
                  ICURCOL = MOD( ICURCOL + NPROW, NPCOL )
 150           CONTINUE
               JJ = MOD( JJ+1, NPCOL )
 160        CONTINUE
         ELSE
c
c           CALL IGESD2D( 1, 1, ISIZE, 1, MYROW, K2,
c    $           ITYPE_TO( MYROW, K2 ) )
c           CALL DGESD2D( ISIZE, 1, WORK, ISIZE, MYROW, K2,
c    $           ITYPE_TO( MYROW, K2 ) )
*
            CALL MPI_SEND( ISIZE, 1, MPI_INTEGER, K2, K2, ROWCOMM, 
     $           IERR )
            CALL MPI_SEND( WORK, ISIZE, MPI_DOUBLE_PRECISION,
     $           K2, K2, ROWCOMM, IERR )
*
            DO 170 I=1, MYN
               X(I) = (0.0D0, 0.0D0)
 170        CONTINUE
         ENDIF
      ELSE
         PRINT *, "PDVREDIST: DEFAULT NOT IMPLEMENTED"
      ENDIF
      
      RETURN
*     
*  End of PDVREDISTR
*
      END
