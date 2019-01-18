      program main
      include 'mpif.h'
      integer rank, size, i, buf, ierr
      integer status(MPI_STATUS_SIZE)
c
      call MPI_Init( ierr )
      call MPI_Comm_rank( MPI_COMM_WORLD, rank, ierr )
      call MPI_Comm_size( MPI_COMM_WORLD, size, ierr )
      if (rank .eq. 0) then
         do 10 i=0, 100*(size-1)
            call MPI_Recv( buf, 1, MPI_INTEGER, MPI_ANY_SOURCE, 
     *                     MPI_ANY_TAG, MPI_COMM_WORLD, status, ierr )
            print *, 'Msg from ', status(MPI_SOURCE), ' with tag',
     *               status(MPI_TAG)
 10      continue
      else
         do 20 i=0, 100-1
            call MPI_Send( buf, 1, MPI_INTEGER, 0, i, MPI_COMM_WORLD, 
     *                     ierr )
 20      continue
      endif
      call MPI_Finalize( ierr )
      end
