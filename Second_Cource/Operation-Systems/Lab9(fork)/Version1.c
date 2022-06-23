#include <stdio.h>
#include <unistd.h>
#define FORK_ERROR -1
#define ERROR 1
#define SUCCESS 0
#define CHILD 0

int main() {
    int pid = fork();
    if (pid == FORK_ERROR) {
        perror("fork");
        return ERROR;
    }
    if (pid == CHILD) {
        execlp("cat", "cat", "text.txt", NULL);
        perror("execl: cat");
        return ERROR;
    }
    printf("Hello, World!\n");
    return SUCCESS;
}