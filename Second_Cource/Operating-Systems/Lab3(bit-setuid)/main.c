#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESS 0
#define NOT_FREE_STREAMS 1
#define ERROR_CODE_SETUID -1
#define FILE_CLOSE_ERROR 2
#define EID_IS_NOT_SET 3

int open_file(){
    FILE* file = fopen("file", "r");
    if (file == NULL){
        perror("opening file");
        return NOT_FREE_STREAMS;
    } 
    printf("Fine\n");
    int close_res = fclose(file);
    if (close_res == EOF) return FILE_CLOSE_ERROR;  
    return SUCCESS;
}

void print_procces_id(){
    printf("Real = %d\n", getuid()); //Печать реального идентификатора пользователя, вызвавшего процесс
    printf("Effective ID = %d\n", geteuid()); //Печать эффективного идентификатора пользователя, вызвавшего процесс
}

int main(){
    print_procces_id();
    open_file(); //Первая попытка прочитать файл   
 
    int setuid_res = setuid(getuid()); //Установка эффективого индентификатора равным действительному
    if (setuid_res == ERROR_CODE_SETUID){ 
        perror("setuid");
        return EID_IS_NOT_SET;
    }
    
    print_procces_id();
    open_file(); //Вторая попытка прочитать файл 
    return SUCCESS;
}
