#include <mpi.h>
#include <math.h>
#include "LA_dvector.h"

/* RK order four problem data structure */
typedef struct _prk4_problem
{
    int (*function)();	/* function to compute f(t, y) */
    double a;			/* starting time */
    double b;			/* ending time */
    int n;			/* no. of steps */
    LA_Dvector *alpha;		/* initial value of y */
    LA_Dvector *y;             /* latest value of y vector */
    void (*ystore)();		/* function to store wmatrix */

    /* temporary values in RK calculation (compatible with y) */
    LA_Dvector *k1, *k2, *k3, *k4, *tmp;

} prk4_problem;
