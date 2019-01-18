% GJ mpich2 vs mpich2 with pthread
plot(gj_gcc(:,1),gj_gcc(:,4)./gj_mpi_score_2(:,4),'red',gj_gcc(:,1),gj_gcc(:,4)./gj_mpi_score_3(:,4),'blue',gj_gcc(:,1),gj_gcc(:,4)./gj_mpi_score_4(:,4),'green',gj_gcc(:,1),gj_gcc(:,4)./gj_mpi_score_5(:,4),'cyan',gj_gcc(:,1),gj_gcc(:,4)./gj_mpi_score_6(:,4),'magenta',gj_gcc(:,1),gj_gcc(:,4)./gj_mpi_score_7(:,4),'yellow');
xlabel('Number of equations');
ylabel('Speedup');