%for posibility
%plot(distrib_mpich2_2(:,1),distrib_mpich2_2(:,4),'red',distrib_mpich2_2(:,1),distrib_mpich2_3(:,4),'blue',distrib_mpich2_2(:,1),distrib_mpich2_4(:,4),'green',distrib_mpich2_2(:,1),distrib_mpich2_5(:,4),'cyan',dual_ge_gcc(:,1),dual_ge_gcc(:,4),'+',dual_gj_gcc(:,1),dual_gj_gcc(:,4),'o',dual_jr_gcc(:,1),dual_jr_gcc(:,4),'x');
plot(distrib_mpich2_2(:,1),distrib_mpich2_2(:,4),'red',distrib_mpich2_2(:,1),distrib_mpich2_3(:,4),'blue',distrib_mpich2_2(:,1),distrib_mpich2_4(:,4),'green',distrib_mpich2_2(:,1),distrib_mpich2_5(:,4),'cyan',dual_jr_gcc(:,1),dual_jr_gcc(:,4),'x');
xlabel('Number of equations');
ylabel('Time(s)');