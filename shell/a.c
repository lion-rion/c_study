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
        
        //引用 : コマンドの引数をポインタの配列として取る 
        //また、配列の最初は、ファイル名へのポインタとし、配列の最後は NULL にする必要がある。
        char *pargs[2] = {cmd, NULL};
        int *status;
        int pid = fork();
        if (pid == 0)
        {
            execv(pargs[0], pargs);
            _exit(0); //終了システムコール
            
        } else if (pid > 0) {
            //子プロセスを待つ
            wait(status);
        } else {
            printf("error");
        }
    }
    return 0;
}