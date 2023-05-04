t = linspace(0, 5*%pi)';
x = [sin(t), cos(t)];
y = [sin(2*t), cos(2*t)];
z = list([t/5, sin(t)], [9,5]);
param3d1(x, y, z, 35, 45, "X@Y@Z");
