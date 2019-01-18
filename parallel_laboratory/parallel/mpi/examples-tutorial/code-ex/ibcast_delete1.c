/* destructor for ibcast cached information */
int Ibcast_delete(MPI_Comm *comm, int *keyval, void *attr_val,
                   void *extra)
{
  Ibcast_syshandle *syshandle = (Ibcast_syshandle *)attr_val;
  /* do we have a valid keyval */
  if ( ( (*keyval)    == MPI_KEYVAL_INVALID )        ||
       ( (*keyval)    != ibcast_keyval )             ||
       ( (syshandle   == (Ibcast_syshandle *)0) ) ) {
      /* Give a non-zero return code to indicate an error */
    return 1;
  }
  /* free the "hidden" communicator and memory for syshandle */
  MPI_Comm_free ( &(syshandle->comm) );
  free (syshandle);
  return MPI_SUCCESS;
}

