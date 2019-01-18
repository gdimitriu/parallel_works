CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      subroutine dgebc2d(m, n, a, lda, isrc, comm)
      include "mpif.h"
*     
*     .. Scalar Arguments ..
      integer            m, n, lda, isrc, comm
*     ..
*     .. Array Arguments ..
      double precision   a( lda, * )
*
*  Purpose
*  =======
*
*  DGEBC2D broadcasts a message from node ISRC to all the nodes
*  indicated by the communicator
*
*  Arguments
*  =========
*
*  M       (input) INTEGER
*          The number of rows of the matrix A to be broadcast.  
*          M >= 0.
*
*  N       (input) INTEGER
*          The number of columns of the matrix A to be broadcast.  
*          M >= 0.
*
*  A       (input/output) DOUBLE PRECISION array of dimension ( LDA, N )
*          Data to be broadcast/received
*
*  LDA     (input) INTEGER
*          Leading dimension of A
*
*  ISRC    (input) INTEGER
*          Rank of broadcast root
*
*  COMM    (input) INTEGER
*          Communicator
*
*  =============================================================
      integer myrank, ierror
      integer sizeofdouble, submatrix
      integer status(MPI_STATUS_SIZE)
*     ..
*     .. Executable Statements ..

      if ((m .eq. lda) .or. (n .eq. 1)) then
         call MPI_BCAST( a, m * n, MPI_DOUBLE_PRECISION, isrc, 
     $        comm, ierror )
      else
         call MPI_COMM_RANK( comm, myrank, ierror )

         call MPI_TYPE_EXTENT( MPI_DOUBLE_PRECISION, sizeofdouble, 
     $        ierror )

         call MPI_TYPE_VECTOR( n, m, lda, MPI_DOUBLE_PRECISION,
     $        submatrix, ierror )

         call MPI_TYPE_COMMIT( submatrix, ierror )

         call MPI_BCAST( a, 1, submatrix, isrc, comm, ierror )
      endif

      return
      end

