	program main
	include 'mpif.h'
C       In MPI-2, you may be able to use an MPI module that will give
C       enhanced compile-time error checking
	integer rank, value, ierr

	call MPI_Init( ierr )

	call MPI_Comm_rank( MPI_COMM_WORLD, rank, ierr )
 10	continue
C
C       For batch use, we just set a value
        if (rank .eq. 0) value = 10
C	if (rank .eq. 0) read *, value
	call MPI_Bcast( value, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, ierr )
	
	print *, 'Process ', rank, ' got ', value
C	if (value .ge. 0) goto 10

	call MPI_Finalize( ierr )
	end
