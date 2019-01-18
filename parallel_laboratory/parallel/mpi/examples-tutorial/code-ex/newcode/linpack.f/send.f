CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      subroutine dgesd2d( m, n, a, lda, idest, comm, tag)
      include "mpif.h"
*
*     .. scalar arguments
      integer m, n, lda, idest, comm, tag
*     ..
*     .. array arguments
      double precision a( lda, * )
*
*     Purpose
*     =======
*
*     DGESD2D sends a message from this process to the process
*     with rank IDEST 
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
*  IDEST   (input) INTEGER
*          Rank of destination node
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
         call MPI_SEND( a, m * n, MPI_DOUBLE_PRECISION, idest, tag,
     $        comm, ierror )
      else
         call MPI_COMM_RANK( comm, myrank, ierror )

         call MPI_TYPE_EXTENT( MPI_DOUBLE_PRECISION, sizeofdouble, 
     $        ierror )

         call MPI_TYPE_VECTOR( n, m, lda, MPI_DOUBLE_PRECISION,
     $        submatrix, ierror )

         call MPI_TYPE_COMMIT( submatrix, ierror )

         call MPI_SEND( a, 1, submatrix, idest, tag, comm, ierror )
      endif

      return 
      end
