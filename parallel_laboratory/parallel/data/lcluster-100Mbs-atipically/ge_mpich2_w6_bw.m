%for ge mpich2 bw 6 workers
plot(dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_2(:,4),'black',dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_3(:,4),'black',dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_4(:,4),'black',dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_5(:,4),'black',dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_mpich2_6(:,4),'black');
xlabel('Number of equations');
ylabel('Speedup');