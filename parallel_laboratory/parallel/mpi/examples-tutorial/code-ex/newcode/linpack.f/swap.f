CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      subroutine pdlaswp( N, A, LDA, JFIRST, JLAST, II, 
     $     ICURROW, IPIV, COMM )
      INCLUDE "mpif.h"

      INTEGER N, LDA, JFIRST, JLAST, II, ICURROW, COMM
      DOUBLE PRECISION A( LDA, * )
      INTEGER IPIV( * )

      INTEGER I, JB, MYROW, IERROR, TAG1, TAG2
      PARAMETER ( TAG1 = 100, TAG2 = 200 )

      CALL MPI_COMM_RANK( COMM, MYROW, IERROR )

c      print *, "Inside pdlaswp, rank = ", myrow

      JB = JLAST - JFIRST + 1
      DO 10 I=1,JB
         IF (MYROW .EQ. ICURROW) THEN
c            print *, "myrow = icurrow"
            IF (IPIV(JFIRST+I-1) .GE. 1) THEN
               CALL DSWAP( N, A( II+I-1, 1 ), LDA, 
     $              A( IPIV(JFIRST+I-1), 1 ), LDA)
            ELSE
c               print *, "ipiv < 1"
               CALL DGESD2D( 1, N, A( II+I-1, 1 ), LDA,
     $              -IPIV(JFIRST+I-1), COMM, TAG1)
               CALL DGERV2D( 1, N, A( II+I-1, 1 ), LDA, 
     $              -IPIV(JFIRST+I-1), COMM, TAG2) 
            ENDIF
         ELSE
c            print *, "myrow != icurrow"
            IF (IPIV(JFIRST+I-1) .GE. 1) THEN
c               print *, "ipiv >= 1"
               CALL DGESD2D( 1, N, A(IPIV(JFIRST+I-1),1), 
     $              LDA, ICURROW, COMM, TAG2 )
               CALL DGERV2D( 1, N, A(IPIV(JFIRST+I-1),1), 
     $              LDA, ICURROW, COMM, TAG1 )
            ENDIF
         ENDIF
 10   CONTINUE
      
      RETURN
      END

