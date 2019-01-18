//l'algorithm c'est dans "SPICE Simularea si analiza circuitelor electronice"
int crout_normal(double **mat,double *x,double *y,long variable);
int crout_modificat(double **mat,double *x,double *y,long variable);
//l'algorithm c'est dans "SPICE Simularea si analiza circuitelor electronice"
int doolittle_normal(double **mat,double *x,double *y,long variable);
int doolittle_modificat(double **mat,double *x,double *y,long variable);
//gauss
int gauss(double **mat,double *x,double *y,long variable);
