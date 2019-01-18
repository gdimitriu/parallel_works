	program main
	implicit none
	include 'mpif.h'
C       In MPI-2, you may be able to use an MPI module that will give
C       enhanced compile-time error checking
	integer rank, ierr
	integer i
	double precision t0, t1
	double precision value(1000)
	integer isizes(4)
	data isizes/1,10,100,1000/

	call MPI_INIT( ierr )

	call MPI_COMM_RANK( MPI_COMM_WORLD, rank, ierr )
C
	do 10 i=1, 4
	   call MPI_BARRIER( MPI_COMM_WORLD, ierr )
	   t0 = MPI_WTIME()
	   call MPI_BCAST( value, isizes(i), MPI_DOUBLE_PRECISION, 0,
	1	MPI_COMM_WORLD, ierr )
	   t1 = MPI_WTIME() - t0
	   if (rank .eq. 0) then
	      print *, 'Size ', isizes(i), ' took ', t1, ' seconds'
	   endif
 10	continue

	call MPI_FINALIZE( ierr )
	end
