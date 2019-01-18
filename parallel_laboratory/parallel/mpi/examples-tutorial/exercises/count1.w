{{fn fn_name MPI_Finalize}}
  {{forallfn dis_fn}}
    if ({{dis_fn}}_ncalls_{{fileno}}) {
      printf( "{{dis_fn}}: %d calls\n",
               {{dis_fn}}_ncalls_{{fileno}} );
    }
  {{endforallfn}}    
  if (MPI_Send_ncalls_{{fileno}}) {
       printf( "%d bytes sent in %d calls with MPI_Send\n", 
	       MPI_Send_nbytes_{{fileno}}, MPI_Send_ncalls_{{fileno}} );
       }
{{callfn}}
{{endfn}}
