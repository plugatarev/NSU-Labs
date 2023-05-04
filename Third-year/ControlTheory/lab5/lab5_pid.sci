K = 1.4;
Ti = 5.2;
n = 5;
Tt = 0;
T0 = 1.16;
Td = Ti/4;
Tc = Td/8;
s = poly(0,'s');
W_obj = (1 - s*Tt+(s*Tt)^2/2 )/ (1 + T0*s)^n;
W_dir = (1+1/(Ti*s) +(Td*s)/(1+Tc*s))*K*W_obj;
W = W_dir/(1+W_dir);
Sys = syslin('c',W);
[A B C D] = abcd(Sys);
disp('A: ', A);
I = eye(A);
H = lyap(A,-I,'c');
disp('H: ', H);
l = spec(H);
disp('Собственные числа H: ', l);
if l > 0 then
    k = 2*norm(A,2)*norm(H,2);
else
    k= %inf;
end
disp('k: ', k);
