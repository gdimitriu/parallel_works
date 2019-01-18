CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE MYSUBMATRIX( N, NB, II, JJ, MYM, MYN,
     $     MYROW, MYCOL, NPROW, NPCOL )
*
*  Purpose
*  =======
*
*  Return the indices that define the local portion of the global
*  matrix
*
*  Arguments
*  =========
*
*  N      (input) integer
*         dimension of matrix to be solved/factored (assumed to be
*         square) 
*
*  NB     (input) integer
*         block size used for wrapping the matrix
*
*  II     (output) integer
*         global row index of first entry of local submatrix
*
*  JJ     (output) integer
*         global column index of first entry of local submatrix
*
*  MYM    (output) integer
*         row dimension of local submatrix
*
*  MYN    (output) integer
*         column dimension of local submatrix
*
*  MYROW  (input) integer
*         this node's logical row index
*
*  MYCOL  (input) integer
*         this node's logical column index
*
*  NPROW  (input) integer
*         logical number of rows in the node mesh
*
*  NPCOL  (input) integer
*         logical number of columns in the node mesh
*
*  Details
*  =======
*  The matrix must be distributed among the logical grid of
*  nodes roughly in 2d blocked form:  
*  e.g. if the grid of nodes is 3 x 4, then the assignment is as 
*  follows:
*
*               +----------+----------+----------+----------+
*               |          |          |          |          |
*               |          |          |          |          |
*               |   (0,0)  |   (0,1)  |   (0,2)  |   (0,3)  |
*               |          |          |          |          |
*               |          |          |          |          |
*               +----------+----------+----------+----------+
*               |          |          |          |          |
*               |          |          |          |          |
*               |   (1,0)  |   (1,1)  |   (1,2)  |   (1,3)  |
*               |          |          |          |          |
*               |          |          |          |          |
*               +----------+----------+----------+----------+
*               |          |          |          |          |
*               |          |          |          |          |
*               |   (2,0)  |   (2,1)  |   (2,2)  |   (2,3)  |
*               |          |          |          |          |
*               |          |          |          |          |
*               +----------+----------+----------+----------+

*
*  The size of each block is determined by the fact that the
*  solve routine will be assuming a two dimensional wrap mapping
*  with block size nb.  This makes it complicated to determine
*  what portion of the matrix is assigned to which node.
*  We have provided this subroutine 
*
*            MYSUBMATRIX( N, NB, II, JJ, MYM, MYN,
*                        MYROW, MYCOL, NPROW, NPCOL )
*      
*  which given N, NB, MYROW, MYCOL, NPROW, NPCOL returns
*
*            II, JJ, MYM, MYN
*
*  where the local submatrix equals the submatrix of size 
*  MYM x MYN that starts at A( II, JJ ) in the global matrix.
*
*     VERSION 2.0 DATED Aug. 1992
*     R. VAN DE GEIJN
*
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
*  .. 
*  .. Local Scalars ..
*

      INTEGER N, NB, II, JJ, MYM, MYN, MYROW, MYCOL, NPROW, NPCOL
      INTEGER I, J
*    
*     compute where my part of matrix starts
*     inefficient, but it works
*
      II = 1
      DO I=0, MYROW-1
         CALL IMYPART( 1, N, NB, IDUMMY, MYM, I, NPROW )
         II = II+MYM
      ENDDO

      JJ = 1
      DO J=0, MYCOL-1
         CALL IMYPART( 1, N, NB, IDUMMY, MYN, J, NPCOL )
         JJ = JJ+MYN
      ENDDO
      
*
*     this node's portion of the matrix is dimension ( mym, myn )
*
      CALL IMYPART( 1, N, NB, IDUMMY, MYM, MYROW, NPROW )
      CALL IMYPART( 1, N, NB, IDUMMY, MYN, MYCOL, NPCOL )

      RETURN

      END
