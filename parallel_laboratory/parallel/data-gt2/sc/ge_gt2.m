%for ge gt2
plot(p3_ge_gcc(:,1),p3_ge_gcc(:,4)./ge_gt2_2(:,4),'red',p3_ge_gcc(:,1),p3_ge_gcc(:,4)./ge_gt2_3(:,4),'blue',p3_ge_gcc(:,1),p3_ge_gcc(:,4)./ge_gt2_4(:,4),'green');
xlabel('Number of equations');
ylabel('Speedup');