%for jr mpich
plot(jr_ser(:,1),jr_ser(:,4)./jr_mpi_2(:,4),'red',jr_ser(:,1),jr_ser(:,4)./jr_mpi_3(:,4),'blue',jr_ser(:,1),jr_ser(:,4)./jr_mpi_4(:,4),'green',jr_ser(:,1),jr_ser(:,4)./jr_mpi_5(:,4),'cyan');
xlabel('Number of equations');
ylabel('Speedup');