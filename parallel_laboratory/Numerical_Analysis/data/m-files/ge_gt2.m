%for ge gt2
plot(dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_gt2_2(:,4),'red',dual_ge_gcc(:,1),dual_ge_gcc(:,4)./ge_gt2_3(:,4),'blue');
xlabel('Number of equations');
ylabel('Speedup');