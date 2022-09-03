#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR 1
#define PAUSE_ERROR -1
#define WRITE_ERROR -1
#define EXIT 1
#define TRUE 1
#define ALERT "\a"

int count_of_signals = 0;
int program_exit_flag = 0;

void catchsignal(int sig){
    if(sig == SIGQUIT){
        program_exit_flag = TRUE;
        return;
    }
    int write_result = write(STDOUT_FILENO, ALERT, sizeof(char));
    if (write_result == WRITE_ERROR) perror("write");
    count_of_signals++;
}

int main(){
   void* sigset_result = sigset(SIGINT, catchsignal);
   if (sigset_result == SIG_ERR){
        perror("working sigset");
        return ERROR;
    }
    sigset_result = sigset(SIGQUIT, catchsignal);
    if (sigset_result == SIG_ERR){
        perror("working sigset");
        return ERROR;
    }

    while(program_exit_flag != EXIT){
        pause();
    }
    
    printf("\nSignal was %d times\n", count_of_signals);  
    return SUCCESS;
}
