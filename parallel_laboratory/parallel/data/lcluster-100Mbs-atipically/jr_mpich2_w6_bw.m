%for jr mpich2 black maxim 6 workers
plot(dual_jr_gcc(:,1),dual_jr_gcc(:,4)./jr_mpich2_2(:,4),'black',dual_jr_gcc(:,1),dual_jr_gcc(:,4)./jr_mpich2_3(:,4),'black',dual_jr_gcc(:,1),dual_jr_gcc(:,4)./jr_mpich2_4(:,4),'black',dual_jr_gcc(:,1),dual_jr_gcc(:,4)./jr_mpich2_5(:,4),'black',dual_jr_gcc(:,1),dual_jr_gcc(:,4)./jr_mpich2_6(:,4),'black');
xlabel('Number of equations');
ylabel('Speedup');