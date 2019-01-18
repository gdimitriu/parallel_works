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
    retn = Ibcast_work(handle);
    /* prepare to update the cached information */
    ++(syshandle->ordering_tag); /* make bigger for next ibcast operation
                                    to avoid back-masking */
    (*handle_out) = handle;     /* return the handle */
    return(retn);
}
