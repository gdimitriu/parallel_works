%for posibility bw + JR p3
plot(distrib_mpich2_2(:,1),distrib_mpich2_2(:,4),'black',distrib_mpich2_2(:,1),distrib_mpich2_3(:,4),'black',distrib_mpich2_2(:,1),distrib_mpich2_4(:,4),'black',distrib_mpich2_2(:,1),distrib_mpich2_5(:,4),'black',distrib_mpich2_2(:,1),distrib_mpich2_6(:,4),'black',dual_jr_gcc(:,1),dual_jr_gcc(:,4),'blackx');
xlabel('Number of equations');
ylabel('Time(s)');