%for gji gt2
plot(dual_gji_gcc(:,1),dual_gji_gcc(:,4)./gji_gt2_2(:,4),'red',dual_gji_gcc(:,1),dual_gji_gcc(:,4)./gji_gt2_3(:,4),'blue');
xlabel('Number of equations');
ylabel('Speedup');