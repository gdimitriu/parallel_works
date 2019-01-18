% openmp implementation of inverse with GJ with icc and omni
plot(dual_gj_inv_gcc(:,1),dual_gj_inv_gcc(:,4)./gj_inv_omni_2(:,4),'red',dual_gj_inv_icc(:,1),dual_gj_inv_icc(:,4)./gj_inv_icc_2(:,4),'blue');
xlabel('Number of equations');
ylabel('Speedup');