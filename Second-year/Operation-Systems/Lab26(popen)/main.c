#include <stdio.h>
#include <string.h>
#define MESSAGE_SIZE 256
#define ERROR 1
#define SUCCESS 0
#define PCLOSE_ERROR -1
#define TRUE 1

int check_fgets(FILE* stream){
    if (ferror(stream)){
        perror("fgets");
        return ERROR;
    }
    if (feof(stream)) return EOF;
}

int read_message_from_pipe(char* message, FILE* reader_input_stream){
    char* fgets_error = fgets(message, MESSAGE_SIZE, reader_input_stream);
    if (fgets_error == NULL){
        int fgets_result = check_fgets(reader_input_stream);
        return fgets_result;  
    }
    return SUCCESS;
}

int write_message_to_pipe(char* message, FILE* writer_output_stream){
    int fputs_result = fputs(message, writer_output_stream);
    if (fputs_result == EOF){
        perror("fgets");
        return ERROR;
    }
    return SUCCESS;
}
int close_pipes(FILE* writer, FILE* reader){
    int error = pclose(reader);
    if (error == PCLOSE_ERROR){
        perror("close reader pipe");
        return ERROR;
    }
    error = pclose(writer);
    if (error == PCLOSE_ERROR){
        perror("close writer pipe");    
       return ERROR;
    }
    return SUCCESS;
}

int main(){
    char message[MESSAGE_SIZE];
    FILE* reader_input_stream = popen("./writer", "r");
    if (reader_input_stream == NULL){
        perror("popen writer");
        return ERROR;
    }
    FILE* writer_output_stream = popen("./reader", "w");
    if (writer_output_stream == NULL){
        perror("popen reader");
        return ERROR;
    }

    while (TRUE){
        int read_result = read_message_from_pipe(message, reader_input_stream);
        if (read_result == ERROR) return ERROR;
        if (read_result == EOF) break;  
        int write_result = write_message_to_pipe(message, writer_output_stream);
        if (write_result == ERROR) return ERROR;
    }

    int close_error = close_pipes(writer_output_stream, reader_input_stream);
    if (close_error == ERROR) return ERROR;
    return SUCCESS;
}
