%for distribution
plot(distrib_mpich2_2(:,1),distrib_mpich2_2(:,4),'red',distrib_mpich2_2(:,1),distrib_mpich2_3(:,4),'blue',distrib_mpich2_2(:,1),distrib_mpich2_4(:,4),'green',distrib_mpich2_2(:,1),distrib_mpich2_5(:,4),'cyan');
xlabel('Number of equations');
ylabel('Time(s)');