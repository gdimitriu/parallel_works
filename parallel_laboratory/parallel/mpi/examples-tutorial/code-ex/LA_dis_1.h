typedef struct la_mapping
{
  int   map_type;       /* Used for quick comparison of mappings */

  void (*mu)(int I, int P, int N, void *extra, int *p, int *i);
                       /* Mapping of I->(p,i) */
  void (*mu_inv)(int p, int i, int P, int N, void *extra, int *I);
                       /* Inverse (p,i)->I:   */
  void (*local_len)(int p, int P, int N, void *extra, int *n);
                       /* # of coefficients mapped to each process: */

  void *extra;        /* for mapping-specific parameters */
} LA_Mapping;

/* some pre-defined mapping... */
extern LA_Mapping *LA_Mapping_Blk_Linear, *LA_Mapping_Blk_Scatter,
                  *LA_Mapping_Linear, *LA_Mapping_Scatter;
