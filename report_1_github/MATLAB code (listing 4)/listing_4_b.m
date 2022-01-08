%v for constant velocity after t=0.5s, t for time, a for acceleration, s for dispacement
a = 2;
t = (0:0.01:10);

%v at t=0.5s
v = a * 0.5; %dereived from a=v/t

s = a * t.^2; %derieved from a=v/t, v=s/t

%at 0.5/0.01 (0.5s) and on, s will be always be v*t which t is a varaible
for i = 50:1000
    s(i) = v * t(i);
end

%search for value larger or equal to 5 and set them to 5
for i = 1:1000

    if s(i) >= 5
        s(i) = 5;
    end

end

plot(t, s);
axis([0 10 0 6]); %setting limit of graph
title('distance vs time (acclearation at 2ms^-2 for 0.5s, stops at 5m)');
ylabel('distance (m)');
xlabel('time (s)');
