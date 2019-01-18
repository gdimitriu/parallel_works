%for ge 5 workers
plot(ser_ge(:,1),ser_ge(:,4)./wge_2(:,4),'red',ser_ge(:,1),ser_ge(:,4)./wge_3(:,4),'blue',ser_ge(:,1),ser_ge(:,4)./wge_4(:,4),'yellow',ser_ge(:,1),ser_ge(:,4)./wge_5(:,4),'magenta');
xlabel('Number of equations');
ylabel('Speedup');