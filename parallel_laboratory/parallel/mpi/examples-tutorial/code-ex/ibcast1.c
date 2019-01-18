#include "ibcast.h"
int ibcast_keyval = MPI_KEYVAL_INVALID; /* keyval for
                                           attribute cacheing */
int Ibcast(void *buf, int count, MPI_Datatype datatype, int root,
           MPI_Comm comm, Ibcast_handle **handle_out)
{
    Ibcast_syshandle *syshandle;
    Ibcast_handle    *handle;
    int                   flag, mask, relrank;
    int                   retn, size, rank;
    int                   req_no = 0;
    /* get info about the communicator */
    MPI_Comm_size ( comm, &size );
    MPI_Comm_rank ( comm, &rank );
    /* If size is 1, this is trivially finished */
    if (size == 1) {
      (*handle_out) = (Ibcast_handle *)0;
      return (MPI_SUCCESS);
    }
    /* first see if this library has ever been called. Generate new
       key value if not. */
    if(ibcast_keyval == MPI_KEYVAL_INVALID) {
      MPI_Keyval_create(NULL, NULL, &ibcast_keyval, NULL);
    }
    /* this communicator might already have used this collective
       operation, and so it would consequently have information
       of use to us cached on it. */
    MPI_Attr_get(comm, ibcast_keyval, (void **)&syshandle, &flag);
    if(flag == 0)  { /* there was no attribute previously cached */
        syshandle = 
          (Ibcast_syshandle *)malloc(sizeof(Ibcast_syshandle));
        /* fill in the attribute information */
        syshandle->ordering_tag = 0; /* start with tag zero */
        MPI_Comm_dup(comm, &(syshandle->comm)); /* duplicate comm */
        /* attach information to the communicator */
        MPI_Attr_put(comm, ibcast_keyval, (void *)syshandle);
    }
