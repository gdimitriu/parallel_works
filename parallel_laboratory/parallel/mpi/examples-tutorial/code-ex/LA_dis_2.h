typedef struct la_distrib_2d
{
  LA_Grid_2d  *grid;/* Base grid on which the distribution is based */
  LA_Mapping *row;  /* row mapping */
  LA_Mapping *col;  /* col mapping */

} LA_Distrib_2d;
