      program main
      implicit none
      include 'mpif.h'
      integer maxn
      parameter (maxn=10000)
      integer nloop, ntests
      parameter (nloop=20,ntests=10)
      double precision buf(maxn), t0, t1, tmin
      integer i, j, k
      integer rank, size, dest, ierr, tag
      integer dist
      parameter (dist=3)
      integer status(MPI_STATUS_SIZE)
      integer request
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
C     dist   - distance to send message to dest = (rank +/- dist)
C     
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, size, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, rank, ierr )
      
      do 10 i=1,5
         tmin = 1000.0
         do 20 j=1,ntests
            tag = j
            call MPI_BARRIER( MPI_COMM_WORLD, ierr )
            if (mod(rank,2) .eq. 0) then
               dest = mod(rank + dist,size)
               t0 = MPI_WTIME()
               do 30 k=1,nloop
                  call MPI_ISEND( buf, isizes(i), MPI_DOUBLE_PRECISION,
     $                 dest, tag, MPI_COMM_WORLD, request, ierr )
                  call MPI_WAIT( request, status, ierr )
                  call MPI_IRECV( buf, isizes(i), MPI_DOUBLE_PRECISION,
     $                 dest, tag, MPI_COMM_WORLD, request, ierr )
                  call MPI_WAIT( request, status, ierr )
 30            continue
               t1 = MPI_WTIME() - t0
               if (t1 .lt. tmin .and. t1 .gt. 0.0) tmin = t1
            else
               dest = mod(rank - dist + size, size )
               do 40 k=1,nloop
                  call MPI_IRECV( buf, isizes(i), MPI_DOUBLE_PRECISION,
     $                 dest, tag, MPI_COMM_WORLD, request, ierr )
                  call MPI_WAIT( request, status, ierr )
                  call MPI_ISEND( buf, isizes(i), MPI_DOUBLE_PRECISION,
     $                 dest, tag, MPI_COMM_WORLD, request, ierr )
                  call MPI_WAIT( request, status, ierr )
 40            continue
            endif
 20      continue
         if (rank .eq. 0) then
C           Convert tmin to the per-loop time
            tmin = tmin / nloop
C           Convert tmin to one-way time
            tmin = tmin / 2.0
            print *, 'Size (double)', isizes(i), ' Time ', tmin,
     $           ' Rate MB/s ',1.0e-6*isizes(i) * 8.0 / tmin
         endif
 10   continue

      call MPI_FINALIZE( ierr )
      end
