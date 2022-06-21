#include<stdio.h>
//#pragma warning(disable : 4996)
#include<stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
void form_shift(int form_len, unsigned char* form, int* displacement) {
    int count = form_len - 1;
    for (int i = (form_len - 2); i >= 0; i--) {
        int res = form[i];
        if (displacement[res] == form_len) {
            displacement[res] -= count;
        }
        count--;
    }
}

int scanf_str(FILE* f, unsigned char* str, int size, int form_len) {
    int j = 0;
    for (int i = size; i < form_len; i++) {
        str[j++] = str[i];
    }

    size = form_len - j;

    if (!fread(str+j, 1, size, f)) {
        return 0;
    }
    if (feof(f)) return 0;
    return 1;
}

int boyer_moore(FILE* f, int form_len, unsigned char* form, int* displacement, int* count) {
    unsigned char str[17];

    if (scanf_str(f, str, form_len, form_len) == 0) return 0;
    while (1) {
        int i = 0;
        for (i = form_len - 1; i >= 0; i--) {

            printf("%d ", i + *count + 1);
            if (form[i] != str[i] || i == 0) {
                break;
            }

        }
        int shift = displacement[str[form_len - 1]]; //сдвиг строки
        *count = *count + shift;
        if (scanf_str(f, str, shift, form_len) == 0) return 0;

    }
}


int main(void) {

    unsigned char form[17];
    FILE* f = fopen("in.txt", "rt");
    if (f == NULL) return 0;
    int form_len = 0;

    for (int i = 0; i < 17; ++i) {
        if (!(form[i] = fgetc(f))) {
            fclose(f);
            return 0;
        }
        if (form[i] == '\n') {
            form[i] = '\0';
            break;
        }
        form_len += 1;

    }
    int displacement[256]; //для смещений
    for (int i = 0; i < 256; i++) {
        displacement[i] = form_len;
    }
    //находим сдвиг
    form_shift(form_len, form, displacement);

    int count = 0;
    boyer_moore(f, form_len, form, displacement, &count);

    fclose(f);
    return EXIT_SUCCESS;
}
