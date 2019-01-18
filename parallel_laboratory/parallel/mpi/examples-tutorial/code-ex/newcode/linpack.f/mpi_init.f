CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      subroutine mpi_initialize( comm )
      integer comm( 3 )
      include "mpif.h"

      integer NDIMS
      parameter (NDIMS = 2)
      integer dims(NDIMS), local(NDIMS) 
      logical periods(NDIMS), reorder
      integer comm2d, row_comm, col_comm
      integer nprow, npcol, myrow, mycol, numnodes, ierr

      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, numnodes, ierr )
      dims(1) = 0
      dims(2) = 0
      call MPI_DIMS_CREATE( numnodes, NDIMS, dims, ierr )
      nprow = dims(1)
      npcol = dims(2)

      periods(1) = .TRUE.
      periods(2) = .TRUE.
      reorder = .TRUE.

      call MPI_CART_CREATE( MPI_COMM_WORLD, NDIMS, dims, periods, 
     $                    reorder, comm2d, ierr )

      call MPI_COMM_RANK( comm2d, myrank, ierr )
      call MPI_CART_COORDS( comm2d, myrank, DIMS, local, ierr )
      myrow = local(1)
      mycol = local(2)

      call MPI_COMM_SPLIT( comm2d, myrow, mycol, row_comm, ierr )
      call MPI_COMM_SPLIT( comm2d, mycol, myrow, col_comm, ierr )

      comm( 1 ) = comm2d
      comm( 2 ) = row_comm
      comm( 3 ) = col_comm

      return
      end

