% openmp implementation of GJ with icc and omni
plot(dual_gj_gcc(:,1),dual_gj_gcc(:,4)./gj_omp_omni_2(:,4),'red',dual_gj_icc(:,1),dual_gj_icc(:,4)./gj_omp_icc_2(:,4),'blue');
xlabel('Number of equations');
ylabel('Speedup');