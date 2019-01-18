      program main
      implicit none
      include 'mpif.h'
      integer lm, ln
      parameter (lm=10,ln=4)
      integer ierr, ifh, status(MPI_STATUS_SIZE)
      double precision Aglobal(2*lm,3*ln)
C
C This program can be used to print out the contents of the file created
C     by the write routines.  Note that only a single process reads the
C     file (run this with -np 1)
C      
      call MPI_INIT(ierr)
      call MPI_FILE_OPEN( MPI_COMM_SELF, 'test1', MPI_MODE_RDONLY,
     $     MPI_INFO_NULL, ifh, ierr )
      call MPI_FILE_READ( ifh, Aglobal, 2*lm*3*ln, MPI_DOUBLE_PRECISION
     $     ,status, ierr )
      call MPI_FILE_CLOSE( ifh, ierr )
      print *, Aglobal
      call MPI_FINALIZE( ierr )
      end

