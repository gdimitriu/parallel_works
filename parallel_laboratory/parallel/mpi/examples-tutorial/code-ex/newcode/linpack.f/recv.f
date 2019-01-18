CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      subroutine dgerv2d( m, n, a, lda, isrc, comm, tag)
      include "mpif.h"
*
*     .. scalar arguments
      integer m, n, lda, isrc, comm, tag
*     ..
*     .. array arguments
      double precision a( lda, * )
*
*     Purpose
*     =======
*
*     DGERV2D receives a message from the process ISRC
*
*  Arguments
*  =========
*
*  M       (input) INTEGER
*          The number of rows of the matrix A to be sent.  
*          M >= 0.
*
*  N       (input) INTEGER
*          The number of columns of the matrix A to be sent.  
*          M >= 0.
*
*  LDA     (input) INTEGER
*          Leading dimension of A
* 
*  A       (input) DOUBLE PRECISION array of dimension ( LDA, N )
*          Data to be sent
*
*  ISRC    (input) INTEGER
*          Rank of source node
*
*  COMM    (input) INTEGER
*          Communicator 
*
*  TAG     (input) INTEGER
*          Message identifier
*
*  =============================================================
*
      integer myrank, ierror
      integer sizeofdouble, submatrix
      integer status(MPI_STATUS_SIZE)
*     ..
*     .. Executable Statements ..

      if ((m .eq. lda) .or. (n .eq. 1)) then
         call MPI_RECV( a, m * n, MPI_DOUBLE_PRECISION, isrc, tag,
     $        comm, status, ierror )
      else
         call MPI_COMM_RANK( comm, myrank, ierror )

         call MPI_TYPE_EXTENT( MPI_DOUBLE_PRECISION, sizeofdouble, 
     $        ierror )

         call MPI_TYPE_VECTOR( n, m, lda, MPI_DOUBLE_PRECISION,
     $        submatrix, ierror )

         call MPI_TYPE_COMMIT( submatrix, ierror )

         call MPI_RECV( a, 1, submatrix, isrc, tag, comm, status, 
     $        ierror )
      endif
      
      return
      end
