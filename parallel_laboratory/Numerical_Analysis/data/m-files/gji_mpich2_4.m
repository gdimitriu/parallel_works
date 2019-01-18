%for gji mpich2
plot(p3_gji_gcc(:,1),p3_gji_gcc(:,4)./gji_mpich2_2(:,4),'red',p3_gji_gcc(:,1),p3_gji_gcc(:,4)./gji_mpich2_3(:,4),'blue',p3_gji_gcc(:,1),p3_gji_gcc(:,4)./gji_mpich2_4(:,4),'green');
xlabel('Number of equations');
ylabel('Speedup');