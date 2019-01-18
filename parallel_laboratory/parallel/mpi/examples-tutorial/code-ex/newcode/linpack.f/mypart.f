CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE IMYPART( I, J, NB, MYI, MYJ, PROCNO, NPROC )
* 
*     .. Integer Arguments ..
      INTEGER            I, J, NB, MYI, MYJ, PROCNO, NPROC 
*
*  Purpose
*  =======
*
*  returns the local indices for the portion of the global vector
*  of elements i through j when this vector is wrapped among a 
*  ring of NPROC nodes using block size NB
*
*  Arguments
*  =========
*
*  I       (input) INTEGER
*          First element in global vector
*
*  J       (input) INTEGER
*          Last element in global vector
*
*  NB      (input) INTEGER
*          Block size used for wrapping of vector
*
*  MYI     (output) INTEGER
*          First element of local portion
*
*  MYJ     (output) INTEGER
*          Last element of local portion
*
*  PROCNO  (input) INTEGER
*          Index in ring of node for which MYI and MYJ are to be 
*          computed.
*
*  NPROC   (input) INTEGER
*          Number of nodes in ring
*
* =============================================================
*
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
*     ..
*     .. Local Scalars ..
      INTEGER        NBLOCKS
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC      MOD
*     ..
*     .. Executable Statements ..
      NBLOCKS = (I-1) / NB

      IF ( MOD( NBLOCKS, NPROC ) .LT. PROCNO ) THEN
         MYI = (NBLOCKS / NPROC) * NB + 1
      ELSE IF ( MOD( NBLOCKS, NPROC) .EQ. PROCNO ) THEN
         MYI = (NBLOCKS / NPROC) * NB + I - NBLOCKS * NB
      ELSE
         MYI = (NBLOCKS / NPROC) * NB + NB + 1
      ENDIF

      NBLOCKS = (J-1) / NB
      IF ( MOD( NBLOCKS, NPROC ) .LT. PROCNO ) THEN
         MYJ = (NBLOCKS/NPROC) * NB
      ELSE IF ( MOD( NBLOCKS, NPROC ) .EQ. PROCNO ) THEN
         MYJ = (NBLOCKS/NPROC) * NB + J - NBLOCKS * NB
      ELSE
         MYJ = (NBLOCKS/NPROC) * NB + NB
      ENDIF

      RETURN
*
*  End of IMYPART
*
      END



  
