#include "mpi.h"

{{foreachfn fn_name MPI_Send MPI_Bsend MPI_Isend}}
static long {{fn_name}}_nbytes_{{fileno}};{{endforeachfn}}

{{forallfn fn_name MPI_Init MPI_Finalize MPI_Wtime}}int {{fn_name}}_ncalls_{{fileno}};
{{endforallfn}}

{{fnall this_fn_name MPI_Finalize}}
  printf( "{{this_fn_name}} is being called.\n" );

  {{callfn}}

  {{this_fn_name}}_ncalls_{{fileno}}++;
{{endfnall}}

{{fn fn_name MPI_Send MPI_Bsend MPI_Isend}}
  {{vardecl int typesize}}

  {{callfn}}

  MPI_Type_size( {{datatype}}, (MPI_Aint *)&{{typesize}} );
  {{fn_name}}_nbytes_{{fileno}}+={{typesize}}*{{count}}
  {{fn_name}}_ncalls_{{fileno}}++;

{{endfn}}
