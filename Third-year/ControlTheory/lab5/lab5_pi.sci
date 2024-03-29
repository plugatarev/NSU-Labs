K = 1.2;
Ti = 7.3;
n = 5;
Tt = 0;
T0 = 1.16;
s = poly(0,'s');
W_obj = 1/(1 + T0*s)^n;
W_dir = (1+1/(Ti*s))*K*W_obj;
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
    k = norm(H,2);
else
    k= %inf;
end
disp('k: ', k);
