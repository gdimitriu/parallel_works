typedef struct la_local_dmatrix
{
        int     storage_type; /* Storage strategy (row/column-major) */
        int     m, n;         /* Local dimensions */
        double  **data;       /* The local matrix, as set of pointers */
} LA_Local_Dmatrix;

typedef struct la_dmatrix
{
        LA_Local_Dmatrix a;   /* Local matrix */
        int     M, N;         /* Global dimensions of LA_Dmatrix. */

        LA_Distrib_2d *dis;   /* how to map data onto grid */
} LA_Dmatrix;
