/* copier for ibcast cached information */
void Ibcast_copy(MPI_Comm  *oldcomm, int *keyval,
     void *extra, void *attr_in, void **attr_out, int *flag)
{
  Ibcast_syshandle *syshandle = (Ibcast_syshandle *)attr_in;
  Ibcast_syshandle *new_syshandle;
  /* do we have a valid keyval */
  if ( ( (*keyval)    == MPI_KEYVAL_INVALID )        ||
       ( (*keyval)    != ibcast_keyval )             ||
       ( (syshandle   == (Ibcast_syshandle *)0) ) )
    return;
  /* create a new syshandle for the new communicator */
  new_syshandle = 
    (Ibcast_syshandle *)malloc(sizeof(Ibcast_syshandle));
  /* fill in the attribute information */
  new_syshandle->ordering_tag = 0; /* start with tag zero */
  /* dup the "hidden" communicator */
  MPI_Comm_dup(syshandle->comm, &(new_syshandle->comm));
  /* return the new syshandle and set flag to true */
  (*attr_out) = (void *)new_syshandle;
  (*flag)     = 1;
}

