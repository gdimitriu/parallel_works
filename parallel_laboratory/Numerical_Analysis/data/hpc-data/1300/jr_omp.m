%for jr omp
plot(jr_ser(:,1),jr_ser(:,4)./jr_omp_2(:,4),'red',jr_ser(:,1),jr_ser(:,4)./jr_omp_3(:,4),'blue',jr_ser(:,1),jr_ser(:,4)./jr_omp_4(:,4),'green',jr_ser(:,1),jr_ser(:,4)./jr_omp_5(:,4),'cyan');
xlabel('Number of equations');
ylabel('Speedup');