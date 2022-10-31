#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM 128 /* プロセス数の上限 */
#define LEN_COMMAND 128 /* コマンドの文字数の上限 */

char inside_cmd[10][10] = {"exit","quit","jobs", "fg"}; 

typedef struct {
    pid_t pid; //プロセスid
    bool run; //実行中か
    bool bg; //バックグラウンド実行か
} child_t;

typedef struct {
    child_t child[NUM]; /* プロセス管理リスト */
    int len_child;              /* プロセス管理リストの要素数 */
    int num_run_child;           /* 実行中のプロセスの個数 */
} process_t;

//コマンドライン引数リスト作成
void create_pargs(char *pargs[],int *bg, int *len_pargs, char command[]) {
    //pargsの初期化
    int i;
    for (i = 0; i < LEN_COMMAND; i++) pargs[i] = NULL;

    //引数に分割
    char *p;
    p = strtok(command, " ");
    for (i = 0; p; i++) {
        if (strcmp(p, "&") == 0) //&があるか比較
        {
            *bg = 1;
            pargs[i] = NULL;
            p = strtok(NULL, " ");
            continue;
        }
        pargs[i] = p;
        p = strtok(NULL, " ");
    }

    *len_pargs = i;
}

//プロセス管理リスト中の終了したプロセスを終了済状態にする
void update_exited(int pid, process_t *process_manager) {
    printf("終了[%d]\n", pid);

    child_t *child = process_manager->child;    /* プロセス管理リスト */
    int len_child = process_manager->len_child; /* プロセス管理リストの要素数 */

    int i;
    for (i = 0; i < len_child; i++) {
        if (child[i].pid == pid) break;
    }
    child[i].run = false;

    process_manager->num_run_child -= 1;
}

//jobsコマンド
void jobs(process_t process_manager) {
    printf("-------PID------- \n");

    child_t *child = process_manager.child;    /* プロセス管理リスト */
    int len_child = process_manager.len_child; /* プロセス管理リストの要素数 */

    int i;
    for (i = 1; i <= len_child; i++) {
        if (child[i].run == true) {
            printf("%d ", i);
            printf("%d ", child[i].pid);
            printf("run:%d ", child[i].run);
            printf("bg:%d ", child[i].bg);
            printf("\n");
        }
    }
}

//fgコマンド
void fg(char pid[], process_t *process_manager) {
    int fg_pid = (int)strtol(pid, NULL, 10);  //char型の数値をint型に変換

    int status;

    child_t *child = process_manager->child;    /* プロセス管理リスト */
    int len_child = process_manager->len_child; /* プロセス管理リストの要素数 */

    int i;
    for (i = 0; i <= len_child; i++) {
        if (fg_pid == child[i].pid) {
            waitpid(fg_pid, &status, 0);
        }
    }
    //プロセス管理リスト中の終了したプロセスを終了済状態にする
    update_exited(fg_pid, process_manager);
}

//内部コマンド実行
bool inner_command(char *pargs[], process_t *process_manager) {

    if (pargs[0] == NULL) return true;

    //終了コマンド
    if (strncmp(pargs[0], inside_cmd[0], LEN_COMMAND) == 0){
        exit(0);
    }
    if (strncmp(pargs[0], inside_cmd[1], LEN_COMMAND) == 0){
        exit(0);
    }

    //jobsコマンド
    if (strncmp(pargs[0], inside_cmd[2], LEN_COMMAND) == 0) {
        jobs(*process_manager);
        return true;
    }

    //fgコマンド
    if (strncmp(pargs[0], inside_cmd[3], LEN_COMMAND) == 0) {
        fg(pargs[1], process_manager);
        return true;
    }

    return false;
}

void write_new(pid_t pid, bool bg, process_t *process_manager) {
    process_manager->len_child += 1;
    process_manager->num_run_child += 1;

    child_t *child = process_manager->child;    
    int len_child = process_manager->len_child; 

    child[len_child].pid = pid;
    child[len_child].run = true;
    child[len_child].bg = bg;
}

int main(void) {
    process_t process_manager = {{0, 0, 0}, 0, 0};

    pid_t pid; /* プロセスID */
    int status;

    while (1) {
        //プロンプトを出力
        char cmd[128];
        printf("\n$ "); fgets(cmd, 128, stdin); //コマンドを入力させる
        cmd[strlen(cmd) - 1] = '\0';  //\nを削除
        //コマンドライン引数リスト作成
        char *pargs[LEN_COMMAND];
        int len_pargs;          
        int background = 0;
        //くぎりとbgのチェック
        create_pargs(pargs, &background, &len_pargs, cmd);

        //printf("バックグラウンド : %d\n", background); デバッグ用
         //内部コマンドの判定
        if (inner_command(pargs, &process_manager) == true) {
            continue;
        }

        //プロセス生成
        pid = fork();

        //子プロセス
        if (pid == 0) {
            execv(pargs[0], pargs);
            exit(0);
        }

        //フォーク失敗
        if (pid == -1) {
            printf("fork error");
        }

        //親プロセス
        if (pid > 0) {
            printf("開始 %s (pid %d) \n", pargs[0], pid);

            write_new(pid, background, &process_manager);
            while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
                if (WIFEXITED(status) != 0) {
                    update_exited(pid, &process_manager);
                }
            }
            //フォアグラウンド実行
            if (background == 0) {
                pid = waitpid(-1, &status, 0);
                update_exited(pid, &process_manager);
            }
        }
    }

    return 0;
}