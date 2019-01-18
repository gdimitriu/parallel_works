%for gs mpich2
plot(dual_gs_gcc(:,1),dual_gs_gcc(:,4)./gs_mpich2_2(:,4),'red',dual_gs_gcc(:,1),dual_gs_gcc(:,4)./gs_mpich2_3(:,4),'blue',dual_gs_gcc(:,1),dual_gs_gcc(:,4)./gs_mpich2_4(:,4),'green',dual_gs_gcc(:,1),dual_gs_gcc(:,4)./gs_mpich2_5(:,4),'cyan');
xlabel('Number of equations');
ylabel('Speedup');