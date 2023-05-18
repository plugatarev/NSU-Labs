s = poly(0, 's');

function [Res]=Pade(delay, order)
    S=poly(0, 's');
    Res = (-delay*S+2*order)^order / (delay*S+2*order)^order;
endfunction

K = 1
Ti = 5.8
Td = Ti/4
Tc = Td/8
//Td = 0
//Tc = 0
T0 = 1.16
n = 5
TT = 1.5

W_obj = Pade(TT,n)/(1 + T0*s)^n;
W_dir = (1+1/(Ti*s)+(Td*s)/(1+Tc*s))*K*W_obj;
W = W_dir/(1+W_dir);
Sys=syslin('c', W);

//h = TT
//h = TT / 2;
//h = TT / 10;
h = TT / 100;

Sysd=dscr(Sys, h)
Ad = Sysd.A
Bd = Sysd.B
Cd = Sysd.C

tmax=200;
v=zeros(Bd)
t = [0:h:tmax]';
x=zeros(t)

printf("\nh : length(t)\n");
disp(h);
disp(length(t));

for k=1:length(t)
    v=Ad*v+Bd
    x(k)=Cd*v
end
plot(t, x.', "red")

[data, text] = fscanfMat("C:\Users\pluga\Downloads\circuit2.TNO");
t_m = data(:, 1);
y = data(:, 2);

plot(t_m, y, "blue");

err = sum((x-y).^2)/length(t);
disp(err);
