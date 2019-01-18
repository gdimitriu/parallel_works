%for ge mpich2 max 3 workers
plot(dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_2(:,4),'red',dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_3(:,4),'blue');
xlabel('Number of equations');
ylabel('Speedup');