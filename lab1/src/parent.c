#include "utils.h"
#include "parent.h"

void ParentRoutine(const char* child_exec_name) {
    // Читаем имя файла
    char fileName[256];
    fscanf(stdin, "%s", fileName);
    
    // Открываем файл для чтения
    FILE* fp1 = freopen(fileName, "r", stdin);
    if (fp1 == NULL) {
        printf("Can't open file in!\n");
        exit(EXIT_FAILURE);
    }
    
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
        if (dup2(pipe1[WRITE_END], 1) == -1) {
            printf("Can't change stdout!\n");
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

        FILE* fp2 = freopen("output.txt", "w", stdin);
        if (fp2 == NULL) {
            printf("Can't create file output!\n");
            exit(EXIT_FAILURE);
        }
        
        int res;
        
        while (read(pipe1[READ_END], &res, sizeof(int)) == sizeof(int)) {
            fprintf(fp2, "%d\n", res);
        }
        fclose(fp2);

        close(pipe1[READ_END]);

        wait(NULL);
    }
}
