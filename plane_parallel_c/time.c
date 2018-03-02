#include <stdio.h>
#include <time.h>

int print_time(void){
    char* c_time_string;
    time_t current_time;

    current_time = time(NULL);
    c_time_string = ctime(&current_time);

    printf("Current time: %s\n", c_time_string);

    return 0;
}