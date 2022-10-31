#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int execv(const char *path, char *const argv[]);

int main(){

    //内部コマンドの実装
    char inside_cmd[10][10] = {"exit","quit","job", "fg"}; 

    //全体をループ
    while(1)
    {   
        int pid;
        //printf("%d", pid);
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

        //&のチェック
        int bg_flag=0;
        for(int i = 0; p;i++)
        {
            
            if (strcmp(p, "&") == 0) //&があるか比較
            {
                bg_flag=1; //フラグ設定
                break; //&入れない
            }
            pargs[i] = p;
            p = strtok(NULL, " ");
        }

        //printf("%d\n", bg_flag); //デバッグ

        pid = fork();
        int status;

        //if(bg_flag == 0)
        if (pid == 0){
            execv(pargs[0], pargs);
            _exit(0); //終了システムコール 
        } else if (pid == -1) {
            //エラー
            printf("error");
        }
        else if(pid > 0) {
            //子プロセスのどれかが終了するまで待機
            while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
                if (WIFEXITED(status)) {
                    printf("BGプロセス終了[%d]\n", pid);
                }
            }

            if (bg_flag == 0) {
                //フォアグラウンド実行
                pid = wait(&status);
                printf("FGプロセス終了[%d]\n", pid);
            }
        }
    }
    return 0;
}