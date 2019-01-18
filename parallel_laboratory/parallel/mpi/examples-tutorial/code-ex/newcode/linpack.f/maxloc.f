CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                          C
C      The BLACS calls are modified/replaced to use MPI message-passing    C
C      primitives.                                                         C
C                                                                          C
C      Purushotham Bangalore (puri@cs.msstate.edu)                         C
C                                                                          C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      function PURI_MAXLOC_FN ( invec, inoutvec, len, type )
      double precision invec(len), inoutvec(len)
      integer len, type
      
      do 10 i=1,len,2
         if ( inoutvec(i) .eq. invec(i) ) then
            inoutvec(i+1) = min(inoutvec(i+1),invec(i+1))
         elseif ( inoutvec(i) .lt. invec(i) ) then
            inoutvec(i)   = invec(i)
            inoutvec(i+1) = invec(i+1)
         endif
 10   continue 
      
      PURI_MAXLOC_FN = 1
      return
      end      
