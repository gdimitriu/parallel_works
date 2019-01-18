      program main
      implicit none
      include 'mpif.h'
      integer rank, size
      integer i, buf
      integer status(MPI_STATUS_SIZE)
      integer ierr
C
      call MPI_INIT( ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, rank, ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, size, ierr )
C
      call MPI_BARRIER( MPI_COMM_WORLD, ierr )
      if (rank .eq. 0) then
         do 10 i=1, 100*(size-1)
            call MPI_RECV( buf, 1, MPI_INTEGER, MPI_ANY_SOURCE,
     $           MPI_ANY_TAG, MPI_COMM_WORLD, status, ierr )
            print *, 'Msg from ', status(MPI_SOURCE), ' and tag ',
     $           status(MPI_TAG) 
 10      continue
      else
         do 20 i=1,100
            call MPI_SEND( buf, 1, MPI_INTEGER, 0, i, MPI_COMM_WORLD,
     $           ierr )
 20      continue
      endif
C
      call MPI_FINALIZE( ierr )
      end
