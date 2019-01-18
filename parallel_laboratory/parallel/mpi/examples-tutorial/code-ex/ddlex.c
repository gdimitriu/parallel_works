#include <math.h>
#include "mpi.h"
#include "DDL.h"
#include "DDL_Grid.h"
#include "DDL_Reg.h"
#include "DDL_Solver.h"
main(argc, argv)
int argc;
char *argv[];
{
	int num_procs, my_id;
	DDL_Handle *handle;
	double *a, *x, *rhs;
	int *adims, *bdims, *xdims, *rdims;
	DDL_Solver_template *solve;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
/* Initialize DDL */
	if ((handle = DDL_Init(num_procs, my_id, *++argv)) == NULL) {
            printf("Library initialization failed Exiting\n", error);
	    exit(1);
	}
/* Create template for JGS solver with 7 point stencil */
	solve = DDL_Create_Solver_template(6, x, rhs, xdims, handle);
/* Make arrays for coefficients, unknowns and rhs */
	adims = DDL_Create_array(handle, &a);
	xdims = DDL_Create_array(handle, &x);
	rdims = DDL_Create_array(handle, &rhs);
/* Apply boundary and initial conditions */
	Bc_init(a, x, rhs);
/* Set up stencil for the coefficients */
	DDL_Add_coeffs(solve, a, a dims, 0, 1);
	DDL_Add_coeffs(solve, a, adims, 0, -1);
	DDL_Add_coeffs(solve, a, adims, 1, 1);
	DDL_Add_coeffs(solve, a, adims, 1, -1);
	DDL_Add_coeffs(solve, a, adims, 2, 1);
	DDL_Add_coeffs(solve, a, adims, 2, -1);
/* Call the solver */
	DDL_Solve_JGS(solve, 15);
	MPI_Finalize();
}
