#include "ibcast.h"
int Ibcast_wait(Ibcast_handle **handle_out)
{
    Ibcast_handle *handle = (*handle_out);
    int retn, i;

    /* A NULL handle means there's nothing to do */
    if (handle == (Ibcast_handle *)0)
      return (MPI_SUCCESS);

#ifdef IBCAST_VERSION1

    /* If I wasn't the root, finish my recvs and start & wait on sends */
    if (handle->num_recvs != 0) {
      MPI_Waitall(handle->num_recvs,
                  &handle->req_array[handle->num_sends],
                  &handle->stat_array[handle->num_sends]);
      MPI_Startall ( handle->num_sends, handle->req_array );
    }

    /* Wait for my receives and then start all my sends */
    retn = MPI_Waitall(handle->num_sends, handle->req_array,
                       handle->stat_array);

#endif

#ifdef IBCAST_VERSION2

    /* Finish remaining pending recieves */
    MPI_Waitall(handle->num_recvs,
                &handle->req_array[handle->num_sends],
                &handle->stat_array[handle->num_sends]);

    /* Wait for my sends */
    MPI_Waitall(handle->num_sends, handle->req_array,
                handle->stat_array);

#endif 

    /* free permanent requests */
    for (i=0; i < (handle->num_sends + handle->num_recvs); i++)
        MPI_Request_free (&(handle->req_array[i]));

    /* Free request and status arrays and ibcast handle */
    free (handle->req_array);
    free (handle->stat_array);
    free (handle);

    /* Return a NULL handle */
    (*handle_out) = (Ibcast_handle *)0;
    return(retn);
}
