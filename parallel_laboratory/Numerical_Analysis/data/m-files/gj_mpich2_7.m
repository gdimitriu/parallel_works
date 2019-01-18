% GJ mpich2
plot(gj_gcc(:,1),gj_gcc(:,4)./gj_mpich2_2(:,4),'red',gj_gcc(:,1),gj_gcc(:,4)./gj_mpich2_3(:,4),'blue',gj_gcc(:,1),gj_gcc(:,4)./gj_mpich2_4(:,4),'green',gj_gcc(:,1),gj_gcc(:,4)./gj_mpich2_5(:,4),'cyan',gj_gcc(:,1),gj_gcc(:,4)./gj_mpich2_6(:,4),'magenta',gj_gcc(:,1),gj_gcc(:,4)./gj_mpich2_7(:,4),'yellow');
xlabel('Number of equations');
ylabel('Speedup');