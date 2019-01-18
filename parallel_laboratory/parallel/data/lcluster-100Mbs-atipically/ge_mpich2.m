%for ge mpich2
plot(dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_2(:,4),'red',dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_3(:,4),'blue',dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_4(:,4),'green',dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_5(:,4),'cyan');
xlabel('Number of equations');
ylabel('Speedup');