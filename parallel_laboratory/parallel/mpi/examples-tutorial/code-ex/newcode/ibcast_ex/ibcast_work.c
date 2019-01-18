#include "ibcast.h"
int Ibcast_work(void *buf, int count, MPI_Datatype datatype, 
                int root, Ibcast_handle *handle)
{
    MPI_Comm comm;
    int      size, rank;
    int      mask, relrank;
    int      num_dest = 0;
    int      source;

#ifdef IBCAST_VERSION1

    /* if I don't have any recv's, start all my sends -- the root */
    if (handle->num_recvs == 0)
      MPI_Startall ( handle->num_sends, handle->req_array );

    /* start all my recv's */
    else 
      MPI_Startall ( handle->num_recvs, 
                    &handle->req_array[handle->num_sends] );

#endif

#ifdef IBCAST_VERSION2

    /* Start all my receives */
    MPI_Startall ( handle->num_recvs,
                  &handle->req_array[handle->num_sends] );

    /* Wait on the receives if I have sends */
    if (handle->num_sends != 0) {
      MPI_Waitall  ( handle->num_recvs,
                    &handle->req_array[handle->num_sends], 
                    &handle->stat_array[handle->num_sends]);
      handle->num_recvs = 0; /* mark receives as being complete */
    }

    /* Start all the sends */
    MPI_Startall ( handle->num_sends, handle->req_array );

#endif

    return (MPI_SUCCESS);
}


