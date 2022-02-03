#include<stdio.h>
#include<stdlib.h>
//#pragma warning(disable : 4996)
#define SIZE 1000
int m = 0;

typedef struct stack {
    int num[SIZE / 2 + 1];
    int end;

}stack;

void push(stack* st, int ch) {
    st->end++;
    st->num[st->end] = ch;
}

void push_negative(stack* st, int ch) {
    st->end++;
    st->num[st->end] = -ch;
}

int pop(stack* st) {
    st->end--;
    return st->num[st->end + 1];
}

int is_digit(char c) {
    if (c >= '0' && c <= '9') return 1;
    else return 0;
}

int is_operator(char c) {
    if (c == '/' || c == '*' || c == '+' || c == '-' || c == '(' || c == ')') return 1;
    else return 0;
}

int syntax_error(char c) {
    if (is_digit(c) || is_operator(c) || c == '\n') return 0;
    else return 1;
}

int priority(char c) {
    if (c == '(' || c == ')') return 0;
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    return 0;
}

int check_operation(char ch, char* oper, int end) {
    if (end >= 0 && priority(ch) <= priority(oper[end])) return 1;
    else return 0;
}

int execution(stack* dg, char ch) {
    int res = 0;
    int a = pop(dg);
    int b = pop(dg);

    if (ch == '+') {
        res = b + a;
    }
    if (ch == '-') {
        res = b - a;
    }
    if (ch == '*') {
        res = b * a;
    }
    if (ch == '/') {
        if (a == 0) {
            printf("division by zero");
            return 1;
        }
        res = b / a;
    }
    push(dg, res);
    m = 1;
    return 0;

}

int chech_hugging(int count, char ch, int flag, char* c, int i) {

    if (i >= 0 && priority(ch) && priority(c[i]) && flag && !count) {
        printf("syntax error");
        return 1;
    }
    if (!count && ch == '\n' && flag) {
        printf("syntax error");
        return 1;
    }

    return 0;
}

int main(void) {
    FILE* f = fopen("in.txt", "r");
    if (f == NULL) {
        printf("ERROR OPENING FILE!");
        return 1;
    }

    stack digit;
    digit.end = -1;
    char oper[1001] = { 0 };
    int i = -1;
    int bracket = 0;
    char ch;
    ch = fgetc(f);
    digit.num[0] = 0;
    int count_in_bac = 0;
    int k = 0;
    while (1) {
        int count = 0;
        if (ch == '\n' && i < 0) break;
        if (ch == '\n') ch = oper[i];

        if (is_operator(ch)) {

            if (ch == '(') {
                count_in_bac = 0;
                oper[++i] = ch;
                bracket++;
            }
            else if (ch == ')') {
                bracket--;
                if ((i == 0 || digit.end == -1) && digit.end != 0) {
                    printf("syntax error");
                    fclose(f);
                    return 0;
                }

                while (oper[i] != '(') if (execution(&digit, oper[i--])) {
                    fclose(f);
                    return 0;
                }
                i--;
            }
        
            else {
                if (check_operation(ch, oper, i) || ch == '\n')
                {
                    while (i >= 0 && oper[i] != '(') {
                        if (execution(&digit, oper[i--])) {
                            fclose(f);
                            return 0;
                        }
                    }
                    oper[++i] = ch;
                }
                else oper[++i] = ch;
            }

        }
        if ((syntax_error(ch) && ch != '\n') || (i == -1 && priority(ch))) {
            printf("syntax error");
            fclose(f);
            return 0;
        }

        int flag = 0;
        int flag_bac = 0;
        int flag_bac2 = 0;
        if (!(ch == ')' || ch == '(')) flag = 1;
        if (ch == '(') flag_bac = 1;
        if (ch == ')') flag_bac2 = 1;
        if (is_digit(ch)) {
            int res = 0;
            while (!is_operator(ch) && !syntax_error(ch)) {
                if (ch == '\n') break;

                res = res * 10 + (ch - '0');
                ch = fgetc(f);
                count++;
            }
            count_in_bac++;
            if ( (count_in_bac == 1 && i >= 1 && oper[i] == '-' && oper[i - 1] == '(' && ch == ')') || (k == 1 && oper[i] == '-')) {
                push_negative(&digit, res);

                i--;
            }
            else push(&digit, res);

        }
        if (count == 0) ch = fgetc(f);

        if (flag_bac && priority(ch)) {
            printf("syntax error");
            fclose(f);
            return 0;
        }
        if (flag_bac && (ch == ')')) {
            printf("syntax error");
            fclose(f);
            return 0;
        }
        if (flag_bac2 && ch == '(') {
            printf("syntax error");
            fclose(f);
            return 0;
        }
         
        if (chech_hugging(count, ch, flag, oper, i)) return 0;
        if (ch < 0) break;
        k++;
    }

    if (bracket == 0 && (priority(oper[0]) != 0 || digit.num[0] != 0 || m) && digit.end == 0 ) printf("%d", digit.num[digit.end]);
    else printf("syntax error");
    fclose(f);
    return EXIT_SUCCESS;
}
