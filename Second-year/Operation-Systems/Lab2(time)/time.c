#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SUCCESS 0
#define NOT_ENOUGH_MEMORY 1
#define PUTENV_ERROR -1
#define TIME_IS_UNAVAILABLE 2
#define TIME_ERROR -1
#define CANNOT_REPRESENT 3

extern char *tzname[];

int main(){
    time_t now;
    struct tm *struct_pointer;
    char ch[] = "TZ=America/Los_Angeles"; //Наиболее употребительный формат значения TZ, применяемый в современных
                                          //Unix-системах — это имя файла в каталоге /usr/share/lib/zoneinfo/
    int put_res = putenv(ch);
    if (put_res == PUTENV_ERROR){
        perror("changing TZ");
        return NOT_ENOUGH_MEMORY;
    }
    
    time_t tm = time(&now);
    if (tm == TIME_ERROR){
        perror("working time");
        return TIME_IS_UNAVAILABLE;
    }
    char *str_date = ctime(&now);
    if (str_date == NULL){
        perror("working ctime");
        return CANNOT_REPRESENT;
    }
    printf("%s", str_date);
    
    struct_pointer = localtime(&now);
    if (struct_pointer == NULL){
        perror("working localtime");
        return CANNOT_REPRESENT;
    }

    printf("%d/%d/%d %d:%02d %s\n",
        struct_pointer->tm_mon + 1, struct_pointer->tm_mday,
        struct_pointer->tm_year + 1900, struct_pointer->tm_hour,
        struct_pointer->tm_min, tzname[struct_pointer->tm_isdst]);

    return SUCCESS;
}
