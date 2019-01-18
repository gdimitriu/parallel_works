% openmp implementation of GJ inv sun
plot(sun_ser_gj_inv_500(:,1),sun_ser_gj_inv_500(:,4)./sun_par_gj_inv_500_o5_2(:,4),'red');
xlabel('Number of equations');
ylabel('Speedup');