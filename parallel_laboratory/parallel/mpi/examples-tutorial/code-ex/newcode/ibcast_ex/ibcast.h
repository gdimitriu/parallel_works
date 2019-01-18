/* ibcast.h */
#ifndef _IBCAST_H
#define _IBCAST_H
#include "mpi.h"

/* either IBCAST_VERSION1 or IBCAST_VERSION2 should be defined */
#define IBCAST_VERSION1

/* handle for a ibcast operations on a communicator */
typedef struct ibcast_syshandle
{
    MPI_Comm comm;
    int ordering_tag;

} Ibcast_syshandle;

/* handle for a particular ibcast operation */
typedef struct ibcast_handle
{
    MPI_Request          *req_array;
    MPI_Status           *stat_array;
    int                   num_sends;
    int                   num_recvs;

} Ibcast_handle;
#endif

