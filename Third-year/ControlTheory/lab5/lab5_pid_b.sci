K = 1.4;
Ti = 5.2;
n = 5;
Tt = 0;
T0 = 1.16;
Td = Ti/4;
Tc = Td/8;
s = poly(0,'s');
W_obj = 1/(1 + T0*s)^n;
W_dir = (1+1/(Ti*s) +(Td*s)/(1+Tc*s))*K*W_obj;
W = W_dir/(1+W_dir);
Sys = syslin('c',W);
[A B C D] = abcd(Sys);
h = 0.1;
Sysd = dscr(Sys, h);
Ad = Sysd.A;
I = eye(Ad);
H = lyap(Ad,-I,'d');
disp('H: ', H);
l = spec(H);
disp('Собственные числа H: ', l);
if l > 0 then
    k = norm(H,2);
else
    k= %inf;
end
disp('k: ', k);
