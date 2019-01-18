/*
 * length = total length of vector 
 * type   = row or column vector?
 * dis	  = template for distribution on specified grid
 */
LA_Dvector *LA_Dvector_new(int length, int type, LA_Distrib_2d *dis)
{
  LA_Dvector *vec;
  int         local_length;
  int         P, Q, p, q;
  /* Create a new vector */
  vec = NEW(LA_Dvector, 1);
  
  LA_Dvector_init(vec, length, type, dis);
  return(vec);
}
void LA_Dvector *LA_Dvector_init(LA_Dvector *vec,
int length, int type, LA_Distrib_2d *dis)
{
  /* figure the local length - i.e., how */
  /* many coefficients (and which ones)  */
  /* map to this process                 */
  if(type == LA_GRID_ROW)
  {
     (*dis -> row -> local_len)(p, P,
	       length, dis -> row -> extra, &local_length);
  }
  else {  /* LA_GRID_COL */
     (*dis -> col -> local_len)(q, Q,
	       length, dis -> col -> extra, &local_length);
  } 
  /* Fill in the vector structure */
  vec->length	    = length;
  vec->local_length = local_length;
  vec->data	    = NEW(double,local_length);
  vec->type         = type;
  vec->grid	    = dis->grid;
  vec->dis	    = dis; 
  return(vec);
}
