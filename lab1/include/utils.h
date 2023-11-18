#ifndef INC_1_UTILS_H
#define INC_1_UTILS_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

enum PipeEnd {
    READ_END,
    WRITE_END
};

void CreatePipe(int pipeFd[2]);
char* ReadString(FILE* stdin);

#endif //INC_1_UTILS_H
