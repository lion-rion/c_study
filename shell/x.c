#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    while (1)
    {
        char command[100];
        char *p;
        char *argv[100]; //コマンド文字列を空白で区切ったもの
        int flag = 0;

        for (int n = 0; n < 100; n++)
        {
            argv[n] = NULL;
        }

        printf("$ ");
        fgets(command, 100, stdin);
        command[strlen(command) - 1] = '0';

        if (command == "exit" || command == "quit")
        {
            return 0;
        }

        int i = 0;

        p = strtok(command, " "); // 1つめ

        while (p != NULL) // 2つめ以降のスペース
        {
            argv[i] = p;
            i++;

            p = strtok(NULL, " ");
            if (strcmp(p, "&") == 0)
            {
                p = NULL;
                flag++;
            }
        }

        int res;

        int pid = fork();
        int status;
        if (pid == -1)
        {
            printf("error");
        }
        else if (pid == 0)
        {
            res = execv(argv[0], argv);
            exit(-1);
        }
        else if (pid > 0)
        {
            while (pid = waitpid(-1, &status, WNOHANG) > 0)
            {
                if (WIFEXITED(status) != 0)
                {
                    printf("バックグラウンド終了[%d]",pid);
                }
            }

            if (flag == 0)
            {
                pid = waitpid(-1, &status, 0);
                printf("フォアグラウンド終了[%d]",pid);
            }
        }
    }
}
