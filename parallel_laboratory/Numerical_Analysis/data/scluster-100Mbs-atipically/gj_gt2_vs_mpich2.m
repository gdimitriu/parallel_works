%for gj gt2 vs mpich2
plot(dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_gt2_2(:,4),'red',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_gt2_3(:,4),'blue',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_gt2_4(:,4),'green',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_gt2_5(:,4),'cyan',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_mpich2_2(:,4),'redx',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_mpich2_3(:,4),'bluex',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_mpich2_4(:,4),'greenx',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_mpich2_5(:,4),'cyanx');
xlabel('Number of equations');
ylabel('Speedup');