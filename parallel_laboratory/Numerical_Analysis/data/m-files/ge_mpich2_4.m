%for ge mpich2
plot(p3_ge_gcc(:,1),p3_ge_gcc(:,4)./ge_mpich2_2(:,4),'red',p3_ge_gcc(:,1),p3_ge_gcc(:,4)./ge_mpich2_3(:,4),'blue',p3_ge_gcc(:,1),p3_ge_gcc(:,4)./ge_mpich2_4(:,4),'green');
xlabel('Number of equations');
ylabel('Speedup');