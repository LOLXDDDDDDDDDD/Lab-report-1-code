%t for time, a for acceleration, s for dispacement
a = 2;
t = (0:0.01:10);
s = a * t.^2; %derieved from a=v/t, v=s/t
plot(t, s);
title('distance vs time for 10 second');
ylabel('distance (m)');
xlabel('time (s)');
