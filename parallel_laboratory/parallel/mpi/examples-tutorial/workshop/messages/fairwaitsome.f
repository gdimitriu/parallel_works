      program main
      implicit none
      include 'mpif.h'
      integer large
      parameter (large=128)
      integer rank, size, i, cnt
      integer ierr
      integer sbuf
      integer requests(large), statuses(MPI_STATUS_SIZE,large)
     $     ,indices(large)  
      integer buf(large), j, ndone
C
C Variables:
C     cnt - number of tasks left to do
C
      call MPI_INIT( ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, rank, ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, size, ierr )
C
      call MPI_BARRIER( MPI_COMM_WORLD, ierr )
      if (rank .eq. 0) then
C     I am the master
         cnt = (size - 1) * 100
C        Prepost receives for all workers
         do 10 i=1,size-1
            call MPI_IRECV( buf(i), 1, MPI_INTEGER, i, MPI_ANY_TAG,
     $           MPI_COMM_WORLD, requests(i), ierr )
 10      continue
C     
 20      if (cnt .gt. 0) then
            call MPI_WAITSOME( size-1, requests, ndone, indices,
     $           statuses, ierr ) 
            do 30 i=1, ndone
               j = indices(i)
C     j is the request that needs to be reposted; all other data is 
C     in the first ndone entries of indices and statues
               print *, 'Msg from ', statuses(MPI_SOURCE,i),
     $              ' with tag ', statuses(MPI_TAG,i)
C     repost the Irecv
               call MPI_IRECV( buf(j), 1, MPI_INTEGER, j, MPI_ANY_TAG,
     $              MPI_COMM_WORLD, requests(j), ierr )
 30         continue
            cnt = cnt - ndone
            goto 20
         endif
C
C     At the end, cancel any remaining receives and recover them
         do 40 i=1, size-1
            call MPI_CANCEL( requests(i), ierr )
            call MPI_REQUEST_FREE( requests(i), ierr )
 40      continue
      else
C     I am a worker
         do 50 i=1,100
            call MPI_SEND( sbuf, 1, MPI_INTEGER, 0, i, MPI_COMM_WORLD,
     $           ierr )
 50      continue
      endif
C
      call MPI_FINALIZE( ierr )
      end
 
