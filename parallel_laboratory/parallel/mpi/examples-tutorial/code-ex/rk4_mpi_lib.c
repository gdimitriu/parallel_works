#include "rk4_mpi_lib.h"
int prk4_solve(prk4)  /* Runge-Kutta order-4 scheme via LA library */
prk4_problem *prk4;
{
    /* local row length of the Alpha vector */
    int local_len = prk4->alpha->n;
    LA_Dvector *alpha = prk4->alpha;  /* initial value of w */
    double h, t;                      /* step size, time */
    int k;                            /* step index */

    LA_Dvector *y, *k1, *k2, *k3, *k4, *tmp;
    int (*function)();  /* user rhs function */
    void (*ystore)();   /* user storage callback */

    /* step size */
    h  = (prk4->b - prk4->a) / (double)(prk4->n);
    t  = prk4->a;                     /* starting time */
    /* other shorthands: */
    y  = prk4->y;
    k1 = prk4->k1;
    k2 = prk4->k2;
    k3 = prk4->k3;
    k4 = prk4->k4;
    tmp = prk4->tmp;
    ystore = prk4->ystore;
    function = prk4->function;
    
    /* initialize the value of w */
    if(ystore) (*ystore)(0, prk4->a, alpha);

    /* copy alpha (initial value of y) to y vector */
    LA_dcopy(y, alpha);

    for(k = 1; k < prk4->n; k++)
    {
	/* calculate k1 */
	/* k1 := f(t, y) */
	(*function)(t, y, k1);
	/* k1 := h*k1 */
	LA_dscale(k1, h);
	
	/* calculate k2 */
	/* tmp := y + .5 * k1 */
	LA_daxpy(tmp, y, .5, k1); 
	/* k2 := f(t+h/2.0, tmp) */
	(*function)(t+h/2.0, tmp, k2);
	/* k2 := h*k2 */
	LA_dscale(k2, h);

	/* calculate k3 */
	/* tmp := y + .5 * k2 */
	LA_daxpy(tmp, y, .5, k2);
	/* k3 := f(t+h/2.0, tmp) */
	(*function)(t+h/2.0, tmp, k3);
	/* k3 := h*k3 */
	LA_dscale(k3, h);

	/* calculate k4 */
	/* tmp := y + k3 */
	LA_daxpy(tmp, y, 1.0, k3);
	/* k4 := f(t+h, tmp) */
	(*function)(t+h, tmp, k4);
        /* defer scaling of k4 till later... */

	/* y := y + (1/6)*(k1 + 2*(k2 + k3) + k4) */
	LA_daxpy(y, y, 1/6.0, k1);
	LA_daxpy(y, y, 1/3.0, k2);
	LA_daxpy(y, y, 1/3.0, k3);
	LA_daxpy(y, y, (1/6.0)*h, k4);

	/* update time */
	t += h;

	/* store kth solution step */
	if(ystore) (*ystore)(k, t, y); 
    }	    
    return(1); /* success */
}




