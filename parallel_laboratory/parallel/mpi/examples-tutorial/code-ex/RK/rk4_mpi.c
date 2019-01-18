/*
	rk4_mpi.c			created: 05-May-94
	authors:			A. Skjellum, K. Viswanathan, P. Bangalore

	updated:			
					
	Copyright 1983, 1984 (c) Mississippi State University.

	Purpose:
	integrate M first order differential equations in parallel using MPI
        and LA_Dvector class library.

	y'[i] = f[i](t;y[j=1...M])

	M equations.

	sequential algorithm:	see "Numerical Analysis," (2nd edition)
	                        Burden, Faires, Reynolds, p. 239-240
			        also see p. 205

		1. interval t = [a,b]
		2. choose N > 0 as as partition of interval (N steps)
		3. define step size h = (b-a)/N.
		4. Initial conditions: (denote w[i,j] as approxes to y's)

			w[i,0] = alpha[i]
			means: ith w at time zero is set to initial value
			       alpha[i].

		5. Computing the w[i,j+1] from w[i,j] is done as follows:

			loop over i = 1 to M

			    compute k1[i] = h*f[i](t,w[1,j],...,w[M,j])

			end of loop

			loop over i = 1 to M

			    compute k2[i] = h*f[i](t+h/2,w[1,j]+.5*k1[1],...,
					    w[M,j] + .5*k1[M])

			end of loop

			loop over i = 1 to M

			    compute k3[i] =h*f[i](t+h/2,w[1,j]+.5*k2[1],...,
					   w[M,j] + .5*k2[M])

			end of loop

			loop over i = 1 to M

			    compute k4[i] =h*f[i](t+h,w[1,j]+k3[1],...,
					   w[M,j] + k3[M])

			end of loop

			loop over i = 1 to M

			    w[i,j+1] = w[i,j] +
				       {k1[i] + 2*k2[i] + 2*k3[i] + k4[i]}/6
                                                                              
                        end of loop                       

*/
#include "rk4_mpi.h"

RK4_problem_init(RK4_problem *problem, function, a, b, M, npoints, alpha)
int (*function)();	/* pointer to function which returns deriv info (RHS) */
int M;			/* number of equations */
double a;		/* start of interval */
double b;		/* end   of interval */
int npoints;		/* number of points */
LA_Dvector *alpha;      /* array of initial values */
{
	int i; /* looping variable */
	LA_Dvector_binop *copy_alpha_to_y_obj;

	problem -> time = problem -> a = a;
	problem -> b    = b;
	problem -> M = M;
	problem -> npoints = npoints;
	problem -> h = (b-a)/(double)npoints;	/* step size */

	problem -> function = function;
   	
	/*  First assign initial values  */

	y = LA_Dvector_new(M, distribution_info);
	for(i = 0; i < 4; i++)
	    problem -> kuttas[i] = LA_Dvector_new(M, distribution_info);

	/* y := alpha */
	copy_alpha_to_y_obj = LA_Dvector_copy_new(alpha, y, 0.0, 0.0);
	(*copy_alpha_to_y_obj.op)(copy_obj, NULL);
	(*copy_alpha_to_y_obj.delete)(&copy_obj);
}

RK4_problem_integrate_1(RK4_problem *problem)
{
	/* k1: */
	(*function)(problem -> time, problem -> y, problem -> temp_vec);
	(*problem -> k1_post_fn.op)(problem -> k1_post_fn, NULL);  /* k1 := h * temp_vec */

        /* k2: */
	(*problem -> k2_pre_fn.op)(problem -> k2_pre_fn, NULL); /* temp_vec2 := y + .5*k1 */
	(*function)(problem -> time + (problem -> h/2.0), problem -> temp_vec2,
		    problem -> temp_vec);
	(*problem -> k2_post_fn.op)(problem -> k2_post_fn, NULL);  /* k2 := h * temp_vec */

        /* k3: */
	(*problem -> k3_pre_fn.op)(problem -> k3_pre_fn, NULL); /* temp_vec3 := y + .5*k2 */
	(*function)(problem -> time + (problem -> h/2.0), problem -> temp_vec2,
		    problem -> temp_vec);
	(*problem -> k3_post_fn.op)(problem -> k3_post_fn, NULL);  /* k3 := h * temp_vec */

        /* k4: */
	(*problem -> k4_pre_fn.op)(problem -> k4_pre_fn, NULL); /* temp_vec3 := y + k3 */
	(*function)(problem -> time + problem -> h, problem -> temp_vec2,
		    problem -> temp_vec);
	(*problem -> k4_post_fn.op)(problem -> k4_post_fn, NULL);  /* k4 := h * temp_vec */

	/* here we do temp_vec := temp_vec + k1/6; */
        (*problem -> add_k1.op)(problem -> add_k1, NULL);
	/* here we do temp_vec := temp_vec + k2/3; */
        (*problem -> add_k2.op)(problem -> add_k2, NULL);
	/* here we do temp_vec := temp_vec + k3/3; */
        (*problem -> add_k3.op)(problem -> add_k3, NULL);
	/* here we do y := y + k4/6; */
	(*problem -> add_k4.op)(problem -> add_k4, NULL); 

	/* advance time: */
	problem -> time += problem -> h;
}


