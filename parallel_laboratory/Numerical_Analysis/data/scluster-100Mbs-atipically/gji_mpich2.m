%for gji mpich2
plot(dual_gji_gcc(:,1),dual_gji_gcc(:,4)./gji_mpich2_2(:,4),'red',dual_gji_gcc(:,1),dual_gji_gcc(:,4)./gji_mpich2_3(:,4),'blue',dual_gji_gcc(:,1),dual_gji_gcc(:,4)./gji_mpich2_4(:,4),'green',dual_gji_gcc(:,1),dual_gji_gcc(:,4)./gji_mpich2_5(:,4),'cyan');
xlabel('Number of equations');
ylabel('Speedup');