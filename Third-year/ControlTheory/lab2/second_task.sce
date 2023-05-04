A = [
     7 2 0;
    -7 -2 1;
     1 1 1;
];

B = [
     0 2 3;
     1 0 -2;
     3 1 1;
];

printf("A: ");
disp(A);
printf("\n");
printf("B: ");
disp(B);
printf("\n");

D = (A^2 - B^2)*(A + B^2);

printf("D: ");
disp(D);
printf("\n");

D_inv = D^(-1);

printf("D^(-1): ");
disp(D_inv);

E = D * D_inv;
printf("D * D^(-1): ");
disp(E);
printf("\n");
