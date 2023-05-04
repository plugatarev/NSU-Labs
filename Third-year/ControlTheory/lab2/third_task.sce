x = -10:0.1:10;
y = nthroot((x-4.5).^2 .* (x + 2), 3);
plot(x, y);
