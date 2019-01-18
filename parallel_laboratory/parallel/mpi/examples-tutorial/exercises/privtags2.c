/*@
  MPE_ReturnTags - Returns tags allocated with MPE_GetTags.

  Input Parameters:
. comm - Communicator to return tags to
. first_tag - First of the tags to return
. ntags - Number of tags to return.
@*/
int MPE_ReturnTags( comm, first_tag, ntags )
MPI_Comm comm;
int      first_tag, ntags;
{
int *tagvalp, flag, mpe_errno;

if (mpe_errno = MPI_Attr_get( comm, MPE_Tag_keyval, &tagvalp, &flag ))
    return mpe_errno;

if (!flag) {
    /* Error, attribute does not exist in this communicator */
    return MPI_ERR_OTHER;
    }
if (*tagvalp == first_tag) 
    *tagvalp = first_tag + ntags;

return MPI_SUCCESS;
}

