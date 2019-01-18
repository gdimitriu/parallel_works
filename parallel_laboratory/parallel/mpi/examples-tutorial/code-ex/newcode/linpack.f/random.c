/***************************************************************************
*                                                                          *
*      The BLACS calls are modified/replaced to use MPI message-passing    *
*      primitives.                                                         *
*                                                                          *
*      Purushotham Bangalore (puri@cs.msstate.edu)                         *
*                                                                          *
***************************************************************************/
 
initmatrix_(a, m, n, myrow, mycol)
double *a;
int *m, *n, *myrow, *mycol;
{
    double drand48();
    void   srand48();
    long seed=246;
    int i, j;

    srand48(seed ^ (1000 * *myrow + *mycol));
    for (i = 0; i < *m; i++)
        for (j = 0; j < *n; j++)
            a[i + j * *m] = drand48() * 1000.0 + 900.0;
}

