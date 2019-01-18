%for gj gt2
plot(dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_gt2_2(:,4),'red',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_gt2_3(:,4),'blue',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_gt2_4(:,4),'green',dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_gt2_5(:,4),'cyan');
xlabel('Number of equations');
ylabel('Speedup');