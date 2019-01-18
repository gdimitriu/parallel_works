#include "LA_dvector.h"   /* base classes for vector operations */

/* define local stuff for RK4 */

typedef struct _RK4_problem 
{
    int (*function)();   /* point to parallel fn evaluation of RHS */
    int M;               /* global # of equations */

    double a, b;         /* integration interval */
    int N;               /* # of points */

    double t;            /* current time value */
    LA_Dvector *y;       /* store numerical approximations here */
    LA_Dvector *kuttas[4]; /* k1, k2, k3, k4 vectors */

    int (*op)();         /* integration operation (1 step) */

} RK4_problem;

