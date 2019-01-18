typedef struct la_local_dvector
{
  int m;             /* local vector length */
  double *data;      /* vector data */
} LA_Local_Dvector;

typedef struct la_dvector
{
  LA_Local_Dvector v;    /* Local vector */

  int M;                 /* full length of vector */
  int type;              /* row or column type */

  LA_Distrib_2d  *dis;   /* how to map data on grid */

} LA_Dvector;
