      program main
      implicit none
      include 'mpif.h'
C     In MPI-2, you may be able to use an MPI module that will give
C     enhanced compile-time error checking
      integer rank, ierr
      integer i, j, size
      integer status(MPI_STATUS_SIZE)
      double precision t0, t1
      double precision value(1000)
      integer isizes(4)
      data isizes/1,10,100,1000/
      
      call MPI_INIT( ierr )
      
      call MPI_COMM_RANK( MPI_COMM_WORLD, rank, ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, size, ierr )
C     
      do 10 i=1, 4
         call MPI_BARRIER( MPI_COMM_WORLD, ierr )
         if (rank .eq. 0) then 
            t0 = MPI_WTIME()
            do 20 j=1, size-1
               call MPI_SEND( value, isizes(i), MPI_DOUBLE_PRECISION, j
     $              ,0,MPI_COMM_WORLD, ierr )
 20         continue
            t1 = MPI_WTIME() - t0
            print *, 'Size ', isizes(i), ' took ', t1, ' seconds'
         else
            call MPI_RECV( value, isizes(i), MPI_DOUBLE_PRECISION, 0, 0
     $           , MPI_COMM_WORLD, status, ierr )
         endif
 10   continue
      
      call MPI_FINALIZE( ierr )
      end
      
