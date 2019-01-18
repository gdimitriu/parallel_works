%for gj mpich2 max 3 workers
plot(dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_mpich2_2(:,4),'red',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_mpich2_3(:,4),'blue');
xlabel('Number of equations');
ylabel('Speedup');