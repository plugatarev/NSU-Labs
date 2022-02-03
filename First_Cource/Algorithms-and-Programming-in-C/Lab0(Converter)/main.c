#include <stdlib.h>
#include <math.h>
#include<stdio.h>


int bad_input(char num[], int b1, int b2, int* check_Dot, int* before_Dot_Size) {

	if (b1 < 2 || b1 > 16 || b2 < 2 || b2 > 16) {
		return 0;
	}

	int i;
	for (i = 0; num[i] != '\0'; i++) {

		if (num[i] - '0' < 10 && (num[i] - '0' > b1 - 1)) {
			return 0;
		}

		if (num[i] - '0' > 10 && (num[i] - '7' > b1 - 1) && !(num[i] >= 'a' && num[i] <= 'f')) {
			return 0;
		}

		//иные символы
		if (!((num[i] >= '0' && num[i] <= '9') || (num[i] >= 'A' && num[i] <= 'F') || (num[i] >= 'a' && num[i] <= 'f') || num[i] == '.')) {
			return 0;
		}


		if (num[i] == '.') {
			if (*check_Dot) {
				return 0;
			}

			*check_Dot = 1;
			continue;
		}

		if (!*check_Dot) {
			*before_Dot_Size += 1;
		}

	}
	if (i > 13) {
		return 0;
	}
	i -= 1;
	if (num[i] == '.' || num[0] == '.') {
		return 0;
	}
	return 1;
}

void convert_to_base(char num[], int before_Dot_Size, int b1, long long int* num_before, long double* num_after) {
	int n = 0;

	for (int i = before_Dot_Size - 1; i >= 0; i--) {
		if ((num[i] - '0') < 10) {
			*num_before += (num[i] - '0') * pow(b1, n);
			n += 1;
		}
		else {
			if (num[i] < 'a')
				*num_before += (num[i] - '7') * pow(b1, n);
			else
				*num_before += (num[i] - 'W') * pow(b1, n);

			n += 1;
		}
	}

	double b_pow = 1.0 / b1;

	for (int i = (before_Dot_Size + 1); num[i] != '\0'; i++) {
		if ((num[i] - '0') < 10) {
			*num_after += (num[i] - '0') * b_pow;
		}
		else {
			if (num[i] < 'a')
				*num_after += (num[i] - '7') * b_pow;
			else
				*num_after += (num[i] - 'W') * b_pow;

		}
		b_pow /= b1;
	}
}

void convert_base_to_b2(char num2[], long long num_before, int b2, int* check_Dot, long double num_after) {
	int i = 0;
	int num1[50];

	while (num_before != 0) {
		int ost;
		ost = num_before % b2;

		num1[i++] = ost;
		num_before /= b2;
	}
	int Dot = i;

	for (i = 0; i < Dot; i++) {
		if (num1[Dot - i - 1] < 10) {
			num2[i] = num1[Dot - i - 1] + '0';
		}
		else {
			num2[i] = num1[Dot - i - 1] + '7';
		}
	}
	if (Dot == 0) {
		num2[0] = '0';
		i += 1;
	}


	if (!check_Dot) {
		num2[i] = '\0';
		printf("%s", num2);
	}

	num2[i] = '.';
	i += 1;

	//Переводим в b2-ичную часть после точки

	if (num_after == 0) {
		num2[i] = '0';
		i += 1;
	}
	int Size_dr = 0;
	while (num_after != 0 && Size_dr <= 11) {
		short int temp_Num;

		Size_dr += 1;
		num_after *= b2;
		temp_Num = (int)num_after;
		num_after -= (int)num_after;
		if (temp_Num < 10) {
			num2[i] = temp_Num + '0';
		}
		else {
			num2[i] = temp_Num + '7';
		}
		i += 1;
	}
	num2[i] = '\0';
	printf("%s", num2);

}

int main(void) {
	int b1, b2;
	char num[15];

	if (!(scanf("%d %d", &b1, &b2))) {
		return 0;
	}

	if (!(scanf("%13s", num))) {
		return 0;
	}

	int check_Dot = 0;
	int before_Dot_Size = 0; //размер до точки

	//проверка входных данных
	if (!bad_input(num, b1, b2, &check_Dot, &before_Dot_Size)) {
		printf("bad input");
		return 0;

	}

	if (b1 == b2) {
		printf("%s", num);
		return 0;
	}

	//перевод в 10

	long long int num_before = 0;
	long double num_after = 0;

	convert_to_base(num, before_Dot_Size, b1, &num_before, &num_after);

	//Если выходная == 10
	if (b2 == 10) {
		if (num_before != 0)
			printf("%lld.%Lf", num_before, num_after);
		else
			printf("%lld", num_before);

		return 0;
	}

	//перевод числа из 10сс в b2сс
	char num2[50];
	convert_base_to_b2(num2, num_before, b2, &check_Dot, num_after);

	return EXIT_SUCCESS;
}
