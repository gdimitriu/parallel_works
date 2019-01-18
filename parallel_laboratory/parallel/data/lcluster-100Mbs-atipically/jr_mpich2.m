%for jr mpich2
plot(dual_jr_gcc(:,1),dual_jr_gcc(:,4)./jr_mpich2_2(:,4),'red',dual_jr_gcc(:,1),dual_jr_gcc(:,4)./jr_mpich2_3(:,4),'blue',dual_jr_gcc(:,1),dual_jr_gcc(:,4)./jr_mpich2_4(:,4),'green',dual_jr_gcc(:,1),dual_jr_gcc(:,4)./jr_mpich2_5(:,4),'cyan');
xlabel('Number of equations');
ylabel('Speedup');