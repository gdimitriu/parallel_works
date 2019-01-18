%for ge mpich
plot(ge_ser(:,1),ge_ser(:,4)./ge_mpi_2(:,4),'red',ge_ser(:,1),ge_ser(:,4)./ge_mpi_3(:,4),'blue',ge_ser(:,1),ge_ser(:,4)./ge_mpi_4(:,4),'green',ge_ser(:,1),ge_ser(:,4)./ge_mpi_5(:,4),'cyan');
xlabel('Number of equations');
ylabel('Speedup');