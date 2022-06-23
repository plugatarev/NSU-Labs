#include <time.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 1
#define P2OPEN_ERROR -1
#define PCLOSE_ERROR -1
#define TIME_ERROR -1

int main(){
    time_t tm = time(NULL);
    if (time == TIME_ERROR){
        perror("working time");
        return ERROR;
    }
    
    // TODO:
    srand(tm);
    FILE* file_pointers[2];
    // Opening of pipes to and from a command
    int p2open_result = p2open("sort -n", file_pointers);
    if(p2open_result == P2OPEN_ERROR){
        perror("working p2open");
        return ERROR;
    }

    // Filling of fptrs by random counts.
    for(int i = 0; i < 100; ++i){
        int random = rand() % 100;
        int print_result = fprintf(file_pointers[0], "%d\n", random);
        if (print_result < 0){
            perror("fprintf");
            return ERROR;
        }
    }
    int close_res = fclose(file_pointers[0]);
    if (close_res == EOF){
        perror("fclose");
        return ERROR;
    }
    int print_result = fprintf(file_pointers[0], "%d\n", random);
        if (print_result < 0){
            perror("fprintf");
            return ERROR;
        }
    printf("Sorted:\n");
    int element;
    for(int i = 0; i < 10; ++i){
        for(int j = 0; j < 10; ++j){
            int fscanf_result = fscanf(file_pointers[1], "%d", &element);
            if (fscanf_result == EOF){
                perror("working fscanf");
                return ERROR;
            }
            print_result = printf("%2d ", element);
            if (print_result < 0){
                perror("working printf");
                return ERROR;
            }
        }
        //Обработать printf
        print_result = printf("\n");;
        if (print_result < 0){
            perror("working printf");
            return ERROR;
        }
    }
    int pclose_result = pclose(file_pointers[1]);
    if (pclose_result == PCLOSE_ERROR){
        perror("pclosing");
        return ERROR;
    }
    return SUCCESS;
}
