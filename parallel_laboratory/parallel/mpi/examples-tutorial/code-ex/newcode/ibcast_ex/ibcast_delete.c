/* destructor for ibcast cached information */
void Ibcast_delete(MPI_Comm *comm, int *keyval, void *attr_val,
                   void *extra)
{
  Ibcast_syshandle *syshandle = (Ibcast_syshandle *)attr_val;
  /* do we have a valid keyval */
  if ( ( (*keyval)    == MPI_KEYVAL_INVALID )        ||
       ( (*keyval)    != ibcast_keyval )             ||
       ( (syshandle   == (Ibcast_syshandle *)0) ) )
    return;
  /* free the "hidden" communicator and memory for syshandle */
  MPI_Comm_free ( &(syshandle->comm) );
  free (syshandle);
}

