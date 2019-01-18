%for gji mpich2 max 3 workers
plot(dual_gji_gcc(:,1),dual_gji_gcc(:,4)./gji_mpich2_2(:,4),'red',dual_gji_gcc(:,1),dual_gji_gcc(:,4)./gji_mpich2_3(:,4),'blue');
xlabel('Number of equations');
ylabel('Speedup');