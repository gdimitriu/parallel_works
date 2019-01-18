#include "mpi.h"
#include <stdlib.h>

static int MPE_Seq_keyval = MPI_KEYVAL_INVALID;

/*@
   MPE_Seq_begin - Begins a sequential section of code.  

   Input Parameters:
.  comm - Communicator to sequentialize.  
.  ng   - Number in group.  This many processes are allowed to execute
   at the same time.  Usually one.  

@*/
void MPE_Seq_begin( comm, ng )
MPI_Comm comm;
int      ng;
{
int        lidx, np;
int        flag;
MPI_Comm   local_comm;
MPI_Status status;

/* Get the private communicator for the sequential operations */
if (MPE_Seq_keyval == MPI_KEYVAL_INVALID) {
    MPI_Keyval_create( MPI_NULL_COPY_FN, 
                       MPI_NULL_DELETE_FN, 
                       &MPE_Seq_keyval, NULL );
    }
