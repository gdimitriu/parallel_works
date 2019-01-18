CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      PROGRAM MAIN
      INCLUDE "mpif.h"
*
*     sample program to test double precision parallel dense linear
*     solve software
*
*
*     Purpose
*     =======
*
*     This driver generates a matrix, which is distributed 
*     among a logical grid of nodes roughly in 2d blocked
*     form:  e.g. if the grid of nodes is 3 x 4, then the
*     assignment is as follows:
*
*               +-----+-----+-----+-----+
*               |     |     |     |     |
*               |(0,0)|(0,1)|(0,2)|(0,3)|
*               |     |     |     |     |
*               +-----+-----+-----+-----+
*               |     |     |     |     |
*               |(1,0)|(1,1)|(1,2)|(1,3)|
*               |     |     |     |     |
*               +-----+-----+-----+-----+
*               |     |     |     |     |
*               |(2,0)|(2,1)|(2,2)|(2,3)|
*               |     |     |     |     |
*               +-----+-----+-----+-----+
*
*     The exact size of each block is further documented in 
*     the sample matrix generation routine matrix() in file
*     matrix.f.
*
*     The matrix is factored using the parallel LU factorization
*     routine pdlubr.f, which is based on the LAPACK variant
*     dlubr.f.  It assumes the matrix is block wrapped, as is
*     further described in the paper
*         Dongarra, J., van de Geijn, R., and Walker, D.,
*         "A Look at Scalable Dense Linear Algebra Libraries,"
*         LAPACK Working Note 43, University of Tennessee, May, 1992.
*         (Also in Proceedings of Scalable High Performance 
*         Concurrent Computing '92 (SHPCC92)}, April 27-29, 1992.)  
*     This report is available from netlib by sending the message
*                    send lawn43.ps from lapack/lawns
*     to netlib@ornl.gov.
*
*     Notice that the matrix is not generated assuming the wrapped
*     mapping used by the parallel LU factorization pdgesvx.  
*     However, this simply means that a permuted matrix is decomposed.
*     In addition to performing the factorization, pdgesvx returns
*     an estimate for the recipical of the condition number of the
*     matrix.
*  
*     Next, the driver performs the solve by calling the routine
*     pdtrsv for the lower and upper triangular matrix.  The 
*     right hand side is assumed to be initially distributed like
*     the first column of the matrix (no wrapping).  It is permuted
*     as required by the routine pdlaswp, before the lower and 
*     upper triangular solves proceed.  Finally, the results are
*     redistributed so that the solution vector is distributed like
*     the first row of the initial matrix.  It is this assumption that
*     guarantees that the initial distribution of the matrix does 
*     not affect the solution.
*
*     For further details how the user specified partitioning of
*     the matrix interacts with the way the solver views the matrix,
*     see the below mentioned paper.
*
*     NOTE: the current condition number estimator can cause 
*     overflow if the original problem is severely ill-conditioned.
*
*     ===============================================================
*
*     VERSION 2.0 DATED Aug. 1992
*     R. VAN DE GEIJN
*
*        This code resulted from a joint collaboration between  
*              Robert van de Geijn 
*     and the group for 'Research in Parallel Computational
*     Electromagnetics' at the Jet Propulsion Laboratory.  Futher 
*     information can be found in the paper,
*
*         T. Cwik, R. van de Geijn, and J. Patterson, 
*         "Application of Massively Parallel Computation to 
*          Integral Equation Models of Electromagnetic Scattering 
*          (Invited Paper)," J. Opt. Soc. Am. A, Vol. 11, No. 4, 
*          April 1994.
*
*     Please email your comments to the authors at
*
*        rvdg@cs.utexas.edu (Robert van de Geijn)
*        cwik@yosemite.jpl.nasa.gov (Tom Cwik)
*        jep@yosemite.jpl.nasa.gov  (Jean Patterson)
*
*     Use of this code must properly reference that paper.
*
*     The code is free, and comes with no implicit or explicit 
*     guarantees.  
*
*     All rights reserved
*     
*     .. Parameters ..
*
*     IASIZE         size of local double precision array that holds
*                    this node's portion of matrix
*     IWORKSIZE      size of work array
*     IXSIZE         size of this nodes portion of right-hand-side
*                    and solution
*     IPIVSIZE       size of vector holding pivot information
*
      INTEGER           IASIZE, IWORKSIZE, IXSIZE, IPIVSIZE
      PARAMETER         (IASIZE    = 210000, 
     $                   IWORKSIZE = 21000, 
     $                   IXSIZE    = 700,
     $                   IPIVSIZE  = 2600)
      DOUBLE PRECISION  ONE
      PARAMETER         ( ONE = 1.0E+0 )
*     ..
*     .. Local Scalars ..
*
*     nprow          row dimension of node grid
*     npcol          column dimension of node grid
*     myrow          my row index
*     mycol          my column index
*
      INTEGER           NPROW, NPCOL, MYROW, MYCOL
*
*     n              matrix dimension
*     nb             block size used for wrapping
*     mym            row dimension of local portion of matrix
*     myn            column dimension of local portion of matrix
*     lda            leading dimension of local portion of matrix
*     
      INTEGER            N, NB, MYM, MYN, LDA
      DOUBLE PRECISION   DMAX, DTEMP
*
*     timing variables
*
      DOUBLE PRECISION   TIME1, TIME2, TIME3 
*
*     misc.
* 
      INTEGER            INFO, IDUMMY, IROW, ICOL, IMAX
*     ..
*     .. Local Arrays ..
*
*     a              array in which local portion of matrix is stored
*     work           workarray
*     x              array in which local portion of rhs and solution
*                    is stored
*     y              array in which the vector computed by the
*                    condition number estimation is stored.
*     ipiv           array in which pivoting information is stored
*
      DOUBLE PRECISION   A( IASIZE ), WORK( IWORKSIZE), X( IXSIZE ),
     $                   Y( IXSIZE )
      DOUBLE PRECISION   B( IASIZE ), XX( IXSIZE )
      INTEGER            IPIV( IPIVSIZE ), COMM( 3 )
      INTEGER            GRIDCOMM, ROWCOMM, COLCOMM, IERR
      INTEGER            MESSAGE( 2 )
*     ..
*     .. External Functions ..
c      INTEGER            ITYPE_ROW, ITYPE_COL, IDAMAX 
c      DOUBLE PRECISION   DCLOCK, DCABS1, PDDGERES 
*     ..
*     .. External Subroutines ..
c      EXTERNAL           BLACS_INIT, PDGESVX, PDTRSV, PDRHSSWAP, 
c     $                   RETURN_VEC, PLAMCH2, IMYPART, IGEBC2D
c     $                   MATRIX, VECTOR, ZEROVEC
      EXTERNAL           MPI_INITIALIZE, PDGESVX, PDTRSV, PDRHSSWAP, 
     $                   PLAMCH2, IMYPART, MATRIX, VECTOR, ZEROVEC
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          MAX
*     ..
*     .. Executable Statements ..
*

*
*     Initialize MPI
*
      CALL MPI_INITIALIZE( COMM )
      GRIDCOMM = COMM( 1 )
      ROWCOMM = COMM( 2 )
      COLCOMM = COMM( 3 )
*
*     get architecture dependent information
*
      CALL PLAMCH2(NPROW, NPCOL, MYROW, MYCOL, COMM)
*
*     get problem dimension and block size used for 
*     wrapping 
*
      IF ( MYROW .EQ. 0 .AND. MYCOL .EQ. 0 ) THEN
c         PRINT *, "ENTER N, NB"
c         READ *, N, NB
         N = 2560
         NB = 8
         PRINT *, "N = ", N, " NB = ",NB
         PRINT *, "NPROW = ", NPROW, "    NPCOL = ", NPCOL
         MESSAGE( 1 ) = N
         MESSAGE( 2 ) = NB
      ENDIF

      CALL MPI_BCAST( MESSAGE, 2, MPI_INTEGER, 0, GRIDCOMM, IERR )
      N  = MESSAGE( 1 )
      NB = MESSAGE( 2 )

*
*     determine size of local block
*
      CALL IMYPART( 1, N, NB, IDUMMY, MYM, MYROW, NPROW )
      CALL IMYPART( 1, N, NB, IDUMMY, MYN, MYCOL, NPCOL )

      IF ( MYROW .EQ. 0 .AND. MYCOL .EQ. 0 ) THEN
         PRINT *, "MYN = ", MYN, "    MYM = ", MYM
      ENDIF
*
*     check if buffers are large enough
*      
      IF ( MYN * MYM .GT. IASIZE ) THEN
         PRINT *, "SIZE OF BUFFER HOLDING MATRIX TOO SMALL"
         STOP
      ELSEIF ( ( MYM + MYN + NB ) * NB .GT. IWORKSIZE ) THEN
         PRINT *, "SIZE OF WORKSPACE TOO SMALL"
         STOP
      ELSEIF ( MYM .GT. IXSIZE ) THEN
         PRINT *, "IXSIZE (RHS) TOO SMALL"
         STOP
      ELSEIF ( MYN .GT. IXSIZE ) THEN
         PRINT *, "IXSIZE (SOLUTION) TOO SMALL"
         STOP
      ELSEIF ( IPIVSIZE .LT. N ) THEN
         PRINT *, "SIZE OF PIVOT ARRAY TOO SMALL"
         STOP
      ENDIF
*
*     set leading dimension of matrix
*
      LDA = MAX( MYM, 1 )
*    
*     compute where my part of matrix starts
*
      CALL MYSUBMATRIX( N, NB, II, JJ, MYM, MYN,
     $     MYROW, MYCOL, NPROW, NPCOL )
*
*     generate matrix
*
      CALL INITMATRIX( A, MYM, MYN, MYROW, MYCOL )

c      do I = 1, MYM
c         print *, "(",MYROW,MYCOL,"):",(A(J), J = I, MYM*MYN, MYM)
c      enddo
*
*     copy local copy of matrix to B
*
      CALL DLACPY( 'General', MYM, MYN, A, LDA, B, LDA )
*
*     generate right-hand-side
*     on all other nodes, x must be initialized to zero
*
      IF ( MYCOL .EQ. 0 ) THEN
c        CALL VECTOR( 'COLUMN', N, NB, X, COMM )
         CALL INITMATRIX( X, MYM, 1, MYROW, MYCOL )
      ELSE
         do I = 1, MYM
            X( I ) = 0.0
         enddo
      ENDIF
*
*     Save RHS
*
      call dcopy( MYM, X, 1, XX, 1 )

      if ((myrow .eq. 0) .and. (mycol .eq. 0)) then
         print *, "first entry of rhs:", x(1)
      endif

      if ((myrow .eq. nprow-1) .and. (mycol .eq. 0)) then
         print *, "last entry of rhs:", x(mym)
      endif

c      print *, "(",MYROW,MYCOL,"):", (X(I),I=1,MYM)

c      CALL GSYNC()
      CALL MPI_BARRIER( GRIDCOMM, IERR )

      TIME1 = MPI_WTIME()
*
*     factor matrix and compute estimate of one-norm condition 
*     number.  
*
c      CALL PDGESVX( N, NB, A, LDA, Y, WORK, IWORKSIZE, IPIV, 
c     $     INFO, RCOND )
c
c     factor matrix 
c
      CALL PDLUBR( N, N, NB, A, LDA, WORK, IPIV, 
     $     INFO, COMM )

c      CALL GSYNC()
      CALL MPI_BARRIER( GRIDCOMM, IERR )
      
      TIME2 = MPI_WTIME()
      TIME1 = TIME2 - TIME1
*
*     Perform forward and backward substitution.
*
      CALL PDFORBACK( N, NB, A, LDA, X, WORK, IPIV, INFO,
     $     COMM )

c      CALL GSYNC()
      CALL MPI_BARRIER( GRIDCOMM, IERR )

      TIME2 = MPI_WTIME() - TIME2

c      print *, "(",MYROW,MYCOL,")", (X(I),I=1,MYM)
      
      if (myrow .eq. 0 .and. mycol .eq. 0) then
         print *, "first entry of solution :", x(1)
      endif

      if (myrow .eq. 0 .and. mycol .eq. NPCOL-1) then
         print *, "last entry of solution :", x(myn)
      endif

*
*     compute || Ax - xx ||  (residual check)
*
      dmax = PDDGERES( N, NB, B, LDA, X, XX, WORK, COMM )

      if (myrow .eq. 0 .and. mycol .eq. 0)  
     $     print *, "MAX RESIDUAL:", dmax
      
*
*     report timing results
*
      if (myrow .eq. 0 .and. mycol .eq. 0) then
c         print *, "Condition number = ", 1.0d00/RCOND
         print *, "TIME    SVX       = ", TIME1
         print *, "MFLOPS  SVX       = ", 8.0/3.0 * n * n * n / 
     $        TIME1 * 1.0E-6
         print *, "TIME    TR. SOLVE = ", TIME2
         print *, "MFLOPS  TR. SOLVE = ", 16.0 * n * n / 
     $        TIME2 * 1.0E-6
         print *, "TIME    TOTAL     = ", TIME1 + TIME2
         print *, "MFLOPS  TOTAL     = ", 8.0/3.0 * n * n * n / 
     $        (TIME1 + TIME2) * 1.0E-6
         print *, "MFLOPS/NODE TOTAL = ", 8.0/3.0 * n * n * n / 
     $        (TIME1 + TIME2) * 1.0E-6 / (NPROW*NPCOL)
      endif

      CALL MPI_FINALIZE( IERR )

      STOP
      END


