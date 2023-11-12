#include "utils.h"

int main(void) {
    
    char* input = NULL;
    int strLength, result, number, start; // start - индекс начала след числа в строке
    char flag; // есть ли первое число

    while((input = ReadString(stdin)) != NULL) {

        strLength = strlen(input);
        flag = 0;
        result = 0;
        start = 0;

        for(int i = 0; i < strLength; ++i) {
            if(isspace(input[i])) {
                if (atoi(input + start)==0) {
                    exit(EXIT_FAILURE);
                }
                number = atoi(input + start);
                if (!flag) {
                    result = number;
                    flag = 1;
                } else {
                    result /= number;
                }
                input[i] = '\0';
                start = i + 1;
            }
        }
        
        if(start != strLength) {
            result /= atoi(input + start);
        }

        if(flag == 1) {
            write(1, &result, sizeof(int));
        }

        free(input);
    }
    return 0;
}