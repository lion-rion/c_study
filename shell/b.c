/*
目標 入力からこの形式を目指す
char *pargs[] = {cmd, "a.c",NULL};
*/



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int execv(const char *path, char *const argv[]);

int main(){

    //内部コマンドの実装
    char inside_cmd[10][10] = {"exit","quit"}; 

    //全体をループ
    while(1)
    {   
        char cmd[128];

        printf("$ "); fgets(cmd, 128, stdin); //コマンドを入力させる
        cmd[strlen(cmd) - 1] = '\0';  //\nを削除

        //内部コマンドの判定
        if (strcmp(cmd, inside_cmd[0]) == 0 || strcmp(cmd, inside_cmd[1]) == 0) {
            return 0;
        }
         
        char *pargs[128];
        //初期化
        for (int i = 0; i<128; i++)
        {
            pargs[i]= NULL;
        }
        char *p;
        
        p = strtok(cmd, " "); //スペースまでを代入
        for(int i = 0; p;i++)
        {
            pargs[i] = p;
            p = strtok(NULL, " ");
        }

        int *status;
        int pid = fork();
        if (pid == 0)
        {
            execv(pargs[0], pargs);
            _exit(0); //終了システムコール
            
        } else if (pid > 0) {
            //親プロセスを待つ
            wait(status);
        } else {
            printf("error");
        }
    }

    return 0;
}