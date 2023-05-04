A = [
     2 1 5 1;
     0.333 -1 0 -2;
     0 2 1 2;
     1 4 7 6;
    ];

b = [8; 3; -5; 0];

x = A^(-1)*b;

b_check = A * x;

printf("x: ");
disp(x);
printf("\n");

printf("b: ");
disp(b);
printf("\n");

printf("A*x: ");
disp(b_check);
