      program main
      implicit none
      include 'mpif.h'
      integer rank, size
      integer i
      integer status(MPI_STATUS_SIZE)
      integer ierr
      integer ntasks, nresults
      integer workmsg, endmsg
      parameter (workmsg=0, endmsg=1)
      double precision task(100), taskin(100), taskout(100)
      double precision t0, t1
      data taskout/100*0.0d0/
C
      call MPI_INIT( ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, rank, ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, size, ierr )
C
      call MPI_BARRIER( MPI_COMM_WORLD, ierr )
      if (rank .eq. 0) then
         t0 = MPI_WTIME()
         ntasks = 1000
C     Start by sending a task to each worker
         do 10 i=1, size-1
            call MPI_SEND( taskout, 100, MPI_DOUBLE_PRECISION, i, 0,
     $           MPI_COMM_WORLD, ierr )
 10      continue
         ntasks = ntasks - (size - 1)
C     Now, wait for values to arrive
         nresults = 0
 20      if (nresults .lt. 1000) then
            call MPI_RECV( taskin, 100, MPI_DOUBLE_PRECISION,
     $           MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, status,
     $           ierr ) 
            nresults = nresults + 1
            if (ntasks .gt. 0) then
               call MPI_SEND( taskout, 100, MPI_DOUBLE_PRECISION,
     $              status(MPI_SOURCE), workmsg, MPI_COMM_WORLD, ierr )
               ntasks = ntasks - 1
            endif
            goto 20
         endif
         t1 = MPI_WTIME() - t0
C     Tell every worker to quit
         do 30 i=1, size-1
            call MPI_SEND( MPI_BOTTOM, 0, MPI_DOUBLE_PRECISION, i, 
     $           endmsg, MPI_COMM_WORLD, ierr ) 
 30      continue
         print *, '1000 tasks in ', t1, ' secs'
      else
C     I am a worker
 40      continue
         call MPI_RECV( task, 100, MPI_DOUBLE_PRECISION, 0, MPI_ANY_TAG
     $        , MPI_COMM_WORLD, status, ierr )
         if (status(MPI_TAG) .eq. workmsg) then
            do 50 i=1,100
               task(i) = task(i) + 3.0
 50         continue
            call MPI_SEND( task, 100, MPI_DOUBLE_PRECISION, 0, 0,
     $           MPI_COMM_WORLD, ierr ) 
            goto 40
C        else tag .eq. endmsg => quit
         endif
      endif
C
      call MPI_FINALIZE( ierr )
      end
