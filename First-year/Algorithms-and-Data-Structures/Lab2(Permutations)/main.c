#include<stdio.h>
#include<stdlib.h>
//#pragma warning(disable : 4996)

int scan_str(FILE* f,char *str, int *size) {
    int size_m = *size;
    int flag[10] = { 0 };
    str[size_m++] = getc(f);
    while (str[size_m - 1] != '\n') {

        int point = str[size_m - 1] - '0';
        if (!(str[size_m - 1] >= '0' && str[size_m - 1] <= '9') || flag[point]) {
            printf("bad input");
            fclose(f);
            return 1;
        }

        flag[point] = 1;
        str[size_m] = getc(f);
        size_m++;
    }
    str[size_m - 1] = '\0';
    size_m--;
    *size = size_m;
    return 0;
}

void swap(char* a, char* b) {
    int r = *a;
    *a = *b;
    *b = r;
}

void print_mass(int size, char* str) {
    for (int i = 0; i < size; i++) {
        printf("%c", str[i]);
    }
    printf("\n");
}

int permutations(char *str, int size) {
    for (int i = size - 2; i >= 0; i--) {
        
        if (str[i] >= str[i + 1]) continue;
        int tmp = 0;
        for (int j = i + 1; j < size; j++) {
            if (str[j] > str[i]) tmp = j;
        }
        swap(&str[i], &str[tmp]);
        int end = size - 1;
        for (int j = i + 1; j <= (size - i) / 2; j++) {
            swap(&str[j], &str[end--]);
        }
        return 0;
    }
    return 1;
}

int main(void) {

    char str[11];
    FILE* f = fopen("in.txt", "r");
    if (f == NULL) return 0;
     int size = 0;
    
    if (scan_str(f, str, &size)) return 0;

    int count;
    if (!fscanf(f, "%d", &count)) {
        fclose(f);
        return 0;
    }

    for (int i = 0; i < count; i++) {
        if (permutations(str, size)) {
            fclose(f);
            return 0;
        }
        print_mass(size, str);
    }



    fclose(f);
    return EXIT_SUCCESS;
}
