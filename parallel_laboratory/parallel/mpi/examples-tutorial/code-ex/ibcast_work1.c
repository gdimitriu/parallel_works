#include "ibcast.h"
int Ibcast_work(Ibcast_handle *handle)
{
    /* if I don't have any recv's, start all my sends -- the root */
    if (handle->num_recvs == 0)
      MPI_Startall ( handle->num_sends, handle->req_array );
    /* start all my recv's */
    else 
      MPI_Startall ( handle->num_recvs, 
                    &handle->req_array[handle->num_sends] );
    return (MPI_SUCCESS);
}
