%inverse with GJ in mpi on SCORE
plot(gj_inv_gcc(:,1),gj_inv_gcc(:,4)./gj_inv_mpi_score_2(:,4),'red',gj_inv_gcc(:,1),gj_inv_gcc(:,4)./gj_inv_mpi_score_3(:,4),'blue',gj_inv_gcc(:,1),gj_inv_gcc(:,4)./gj_inv_mpi_score_4(:,4),'green',gj_inv_gcc(:,1),gj_inv_gcc(:,4)./gj_inv_mpi_score_5(:,4),'cyan',gj_inv_gcc(:,1),gj_inv_gcc(:,4)./gj_inv_mpi_score_6(:,4),'magenta',gj_inv_gcc(:,1),gj_inv_gcc(:,4)./gj_inv_mpi_score_7(:,4),'yellow');
xlabel('Number of equations');
ylabel('Speedup');