      program main
      implicit none
      include 'mpif.h'
      integer maxn
      parameter (maxn=10000)
      integer nloop, ntests
      parameter (nloop=20,ntests=10)
      double precision buf(maxn), t0, t1, tmin
      integer i, j, k
      integer rank, size, dest, source, ierr, tag
      integer status(MPI_STATUS_SIZE)
      integer isizes(5)
      data isizes/1,10,100,1000,10000/
C
C Variables:
C     maxn - Maximum message size
C     nloop - Number of time to perform communication (number of
C             pingpongs)
C     ntests - Number of times to run test 
C     isizes - Message lengths
C     tmin   - minimum time for a particular test
C     
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, size, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, rank, ierr )
      
      do 10 i=1,5
         tmin = 1000.0
         do 20 j=1,ntests
            tag = j
            dest = rank + 1
            if (dest .eq. size) dest = MPI_PROC_NULL
            source = rank - 1
            if (source .lt. 0) source = MPI_PROC_NULL
            call MPI_BARRIER( MPI_COMM_WORLD, ierr )
            t0 = MPI_WTIME()
            do 30 k=1,nloop
               call MPI_SEND( buf, isizes(i), MPI_DOUBLE_PRECISION,
     $              dest, tag, MPI_COMM_WORLD, ierr )
               call MPI_RECV( buf, isizes(i), MPI_DOUBLE_PRECISION,
     $              source, tag, MPI_COMM_WORLD, status, ierr )
 30         continue
            t1 = MPI_WTIME() - t0
            if (t1 .lt. tmin) tmin = t1
 20      continue
C     Convert tmin to the per-loop time
         tmin = tmin / nloop
         print *, rank, ': Size (double)', isizes(i), ' Time ', tmin,
     $        ' Rate MB/s ',1.0e-6*isizes(i) * 8.0 / tmin
 10   continue

      call MPI_FINALIZE( ierr )
      end
