CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      subroutine plamch2( nprow, npcol, myrow, mycol, comm )
      include "mpif.h"
      integer nprow, npcol, myrow, mycol, comm( 3 )

      integer ierror

      call MPI_COMM_RANK(comm(2), mycol, ierror)
      call MPI_COMM_RANK(comm(3), myrow, ierror)

      call MPI_COMM_SIZE(comm(2), npcol, ierror)
      call MPI_COMM_SIZE(comm(3), nprow, ierror)      

      return
      end
