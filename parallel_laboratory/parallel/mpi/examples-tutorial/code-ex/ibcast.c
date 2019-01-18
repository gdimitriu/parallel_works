#include "ibcast.h"
int ibcast_keyval = MPI_KEYVAL_INVALID; /* keyval for attribute cacheing */
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
    /* first see if this library has ever been called. */
    if(ibcast_keyval == MPI_KEYVAL_INVALID) {
      /* our first mission is to create the process-local keyval
         for this library */
      /* add a copy-back, and delete callback capability to complete! */
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
    /* create a handle for this particular ibcast operation */
    handle = (Ibcast_handle *)malloc(sizeof(Ibcast_handle));
    handle->num_sends = 0;
    handle->num_recvs = 0;
    /* count how many send/recv handles we need */
    mask    = 0x1;
    relrank = (rank - root + size) % size;
    while ((mask & relrank) == 0 && mask < size) {
      if ((relrank | mask) < size) 
        handle->num_sends++;
      mask <<= 1;
    }
    if (mask < size) 
      handle->num_recvs++;
    /* allocate request and status arrays for sends and receives */
    handle->req_array  = (MPI_Request *)
      malloc(sizeof(MPI_Request) * (handle->num_sends + handle->num_recvs));
    handle->stat_array = (MPI_Status *)
      malloc(sizeof(MPI_Status) * (handle->num_sends + handle->num_recvs));
    /* create "permanent" send/recv requests */
    mask    = 0x1;
    relrank = (rank - root + size) % size;
    while ((mask & relrank) == 0 && mask < size) {
      if ((relrank|mask) < size) 
        MPI_Send_init(buf, count, datatype, ((relrank|mask)+root)%size, 
                      syshandle->ordering_tag, syshandle->comm, 
                      &(handle->req_array[req_no++]));
      mask <<= 1;
    }
    if (mask < size) 
      MPI_Recv_init(buf, count, datatype, 
                    ((relrank & (~ mask)) + root) % size,
                    syshandle->ordering_tag, syshandle->comm,
                    &(handle->req_array[req_no++]));
    /* synchronization begins here */
    retn = Ibcast_work(buf, count, datatype, root, handle);
    /* prepare to update the cached information */
    ++(syshandle->ordering_tag); /* make bigger for next ibcast operation
                                    to avoid back-masking */
    (*handle_out) = handle;     /* return the handle */
    return(retn);
}
