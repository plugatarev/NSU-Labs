#include <stdio.h>
#include <stdlib.h>

//#pragma warning(disable : 4996)

//степени тройки(3^0...3^15)
const int exp3[16] = { 1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441, 1594323, 4782969, 14348907 };

int get_hash(unsigned char* str, int size, int begin) {
    int hash = 0;
    for (int i = 0; i < size; i++) {
        hash += (str[begin % size] % 3) * exp3[i];
        begin++;
    }
    return hash;
}

void check_hash(unsigned char* form, unsigned char* str, int size, int* count, int begin) {
    for (int i = 0; i < size; i++) {
        int res = *count + i - size + 1;
        printf("%d ", res);
        if (str[begin % size] != form[i]) break;
        begin++;
    }
}

void rabin_karp(FILE* f, unsigned char* form, int size, int form_hash) {
    unsigned char str[17];
    int count = size;
    for (int i = 0; i < size; i++) str[i] = fgetc(f);
    if (feof(f)) return;
    int str_hash = get_hash(str, size, 0);

    if (str_hash == form_hash) check_hash(form, str, size, &count, 0);

    while (1) {
        str[count % size] = fgetc(f);
        if (feof(f)) break;

        int begin = count % size + 1;
        str_hash = get_hash(str, size, begin);
        count += 1;

        if (str_hash == form_hash) check_hash(form, str, size, &count, begin);

    }

}

int main(void) {

    FILE* f = fopen("in.txt", "r");
    if (f == NULL) return 0;
    unsigned char form[17];
    int size = 0;
    for (int i = 0; i < 17; i++) {

        form[i] = fgetc(f);

        if (form[i] == '\n') {
            form[i] = '\0';
            break;
        }
        size++;
    }

    int form_hash = get_hash(form, size, 0);
    printf("%d ", form_hash);

    rabin_karp(f, form, size, form_hash);


    fclose(f);
    return EXIT_SUCCESS;
}
