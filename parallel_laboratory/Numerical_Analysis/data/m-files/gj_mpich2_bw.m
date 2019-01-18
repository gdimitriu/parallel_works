%for gj mpich2
plot(dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_mpich2_2(:,4),'black',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_mpich2_3(:,4),'black',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_mpich2_4(:,4),'black',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_mpich2_5(:,4),'black');
xlabel('Number of equations');
ylabel('Speedup');