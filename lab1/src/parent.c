#include "utils.h"
#include "parent.h"

void ParentRoutine(const char* child_exec_name) {
    // Создание pipe1
    int pipe1[2];
    CreatePipe(pipe1);

    // Создание вилки
    int pid = fork();

    if (pid == -1) {
        printf("Can't fork child!\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { 
        // Дочерний процесс
        close(pipe1[READ_END]);
        
        // Перенаправляем стандартный вывод в pipe1[WRITE_END]
        if (dup2(pipe1[WRITE_END], STDOUT_FILENO) == -1) {
            printf("Can't change stdout!\n");
            exit(EXIT_FAILURE);
        }
        
        // Читаем имя файла из stream
        char fileName[256];
        fscanf(stdin, "%s", fileName);
        
        // Открываем файл для чтения
        FILE* fp1 = freopen(fileName, "r", stdin);
        if (fp1 == NULL) {
            printf("Can't open file!\n");
            exit(EXIT_FAILURE);
        }

        char* argv[3];
        argv[0] = "child";
        argv[1] = fileName;
        argv[2] = NULL;
        if (execv(child_exec_name, argv) == -1) {
            printf("Failed to exec\n");
        }
        close(pipe1[WRITE_END]);
    
    } else {
        // Родительский процесс
        close(pipe1[WRITE_END]);

        int res;
        while (read(pipe1[READ_END], &res, sizeof(int))) {
            printf("%d\n", res);
        }

        close(pipe1[READ_END]);
        wait(NULL);
    }
}
