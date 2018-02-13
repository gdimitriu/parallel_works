% GJ mpich2 vs mpich2 with pthread
plot(ge_gcc(:,1),ge_gcc(:,4)./ge_mpi_score_2(:,4),'red',ge_gcc(:,1),ge_gcc(:,4)./ge_mpi_score_3(:,4),'blue',ge_gcc(:,1),ge_gcc(:,4)./ge_mpi_score_4(:,4),'green',ge_gcc(:,1),ge_gcc(:,4)./ge_mpi_score_5(:,4),'cyan',ge_gcc(:,1),ge_gcc(:,4)./ge_mpi_score_6(:,4),'magenta',ge_gcc(:,1),ge_gcc(:,4)./ge_mpi_score_7(:,4),'yellow');
xlabel('Number of equations');
ylabel('Speedup');