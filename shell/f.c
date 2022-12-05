#include <dirent.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define NUM 128  //プロセスの上限
#define COMMAND_MAX 128 //コマンド文字の上限
#define LEN_ENV 2048  //環境変数の上限
#define NUM_ENV_VAR 256 //環境変数の個数

char inside_cmd[10][10] = {"exit","quit","jobs", "fg"};     

typedef struct {
    pid_t pid; //プロセスid
    bool run; //実行中か
    bool bg; //バックグラウンド実行か
} child_t;

typedef struct {
    child_t child[NUM]; 
    int len_child;              
    int num_run_child;         
} process_t;

process_t process_manager = {{0, 0, 0}, 0, 0}; 
pid_t foreground_pid = 0;    

int devide_sentence(char *sentence, char *word_list[], char deviding_char[1]) {
    int size = sizeof(*word_list) / sizeof(char);
    for (int i = 0; i < size; i++) word_list[i] = NULL;

    // 引数に分割
    char *word;      
    int num_word = 0;
    word = strtok(sentence, deviding_char);
    while (word != 0) {
        word_list[num_word] = word;
        word = strtok(NULL, deviding_char);
        num_word += 1;
    }

    return num_word;
}

//jobsコマンド
void jobs(process_t process_manager) {
    printf("-------PID------- \n");

    child_t *child = process_manager.child;    
    int len_child = process_manager.len_child; 
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



//プロセス管理リスト中の終了したプロセスを終了済状態にする
void update_exited(int pid, process_t *process_manager) {
    printf("終了[%d]\n", pid);
    child_t *child = process_manager->child;    
    int len_child = process_manager->len_child;

    int i;
    for (i = 0; i < len_child; i++) {
        if (child[i].pid == pid) break;
    }
    child[i].run = false;

    process_manager->num_run_child -= 1;
}

//fgコマンド
void fg(char pid[], process_t *process_manager) {
    int fg_pid = (int)strtol(pid, NULL, 10);  //char型の数値をint型に変換

    int status;

    child_t *child = process_manager->child; 
    int len_child = process_manager->len_child; 

    int i;
    for (i = 0; i <= len_child; i++) {
        if (fg_pid == child[i].pid) {
            waitpid(fg_pid, &status, 0);
        }
    }
    //プロセス管理リスト中の終了したプロセスを終了済状態にする
    update_exited(fg_pid, process_manager);
}

bool check_background(char *command_lines[], int len_command_lines) {
    if (len_command_lines <= 0) return 0;

    if (strncmp(command_lines[len_command_lines - 1], "&", COMMAND_MAX) == 0) {
        command_lines[len_command_lines - 1] = NULL;
        return 1;
    }

    return 0;
}

//内部コマンド実行
bool inner_command(char *pargs[], process_t *process_manager) {

    if (pargs[0] == NULL) return true;

    //終了コマンド
    if (strncmp(pargs[0], inside_cmd[0], COMMAND_MAX) == 0){
        exit(0);
    }
    if (strncmp(pargs[0], inside_cmd[1], COMMAND_MAX) == 0){
        exit(0);
    }

    //jobsコマンド
    if (strncmp(pargs[0], inside_cmd[2], COMMAND_MAX) == 0) {
        jobs(*process_manager);
        return true;
    }

    //fgコマンド
    if (strncmp(pargs[0], inside_cmd[3], COMMAND_MAX) == 0) {
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

void handler_keybord_interrupt(int sig) {
    if (process_manager.num_run_child != 0) {
        kill(foreground_pid, sig);
    }
}


//子プロセスの実行終了用シグナルハンドラ
void handler_child_exited(int sig) {
    pid_t pid_exited; /* プロセスID */
    int status;

    //バックグラウンド実行では子プロセスの終了を待たない
    while ((pid_exited = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status) != 0) {
            //プロセス管理リスト中の終了したプロセスを終了済状態にする
            update_exited(pid_exited, &process_manager);
        }
    }
}


// 環境変数を取得
void get_env_list(char *env_list[], int *num_env) {
    // 環境変数一覧を取得
    char *all_env = getenv("PATH");

    *num_env = devide_sentence(all_env, env_list, ":");
}

// フルパスを取得
void get_full_path(char *pargs[], char *env_list[], int num_env) {
    // ディレクトリ検索
    //printf("debugxxx: %s\n", pargs[0]);
    for (int i = 0; i < num_env; i++) {
        DIR *dir_stream;

        if ((dir_stream = opendir(env_list[i])) == NULL) {
            // printf("%d ", i);
            continue;
        }

        // ファイル検索
        struct dirent *file;
        while ((file = readdir(dir_stream)) != NULL) {
            // コマンド名と名前が一致するファイルを検索
            if (strncmp(file->d_name, pargs[0], COMMAND_MAX) == 0) {
                // フルパスを生成
                char full_path[COMMAND_MAX];
                strncpy(full_path, env_list[i], COMMAND_MAX);
                strncat(full_path, "/", COMMAND_MAX - 1);
                strncat(full_path, file->d_name, COMMAND_MAX - 1);
                
                pargs[0] = full_path;
                //printf("\n\n%s\n\n", pargs[0]);
                closedir(dir_stream);
                return;
            }
        }
        closedir(dir_stream);
    }
    return;
}


//シグナルハンドラの設定
void set_handler() {
    //キーボード割り込み用シグナル
    signal(SIGINT, handler_keybord_interrupt);

    // //子プロセスの実行終了用シグナル
    signal(SIGCHLD, handler_child_exited);
}

int main(void) {
    set_handler();
    pid_t pid; /* プロセスID */
    int status;

        // 環境変数を取得
    char *env_list[NUM_ENV_VAR]; /* 環境変数リスト */
    int num_env;                 /* 環境変数の個数 */
    get_env_list(env_list, &num_env);

    while (1) {
        //プロンプトを出力
        char cmd[COMMAND_MAX];
        printf("\n$ "); fgets(cmd, COMMAND_MAX, stdin); //コマンドを入力させる
        cmd[strlen(cmd) - 1] = '\0';  //\nを削除
        //コマンドライン引数リスト作成
        char *pargs[COMMAND_MAX];
        int len_pargs;
        int background = 0;
        int len_command_lines; 
        len_command_lines = devide_sentence(cmd, pargs, " ");
        //くぎりとbgのチェック
        background = check_background(pargs, len_command_lines);
        //printf("バックグラウンド : %d\n", background); デバッグ用
         //内部コマンドの判定
        if (inner_command(pargs, &process_manager) == true) {
            continue;
        }
        //printf("debug1: %s\n", pargs[0]);
        get_full_path(pargs, env_list, num_env);

        char *hello[128];

        strcpy(hello[0], pargs[0]);
        //printf("debug2: %s\n", pargs[0]);
        //printf("hello world\n\n");

        //プロセス生成
        pid = fork();

        //子プロセス
        if (pid == 0) {
            setpgid(0, 0);
            execv(hello[0], pargs);
            exit(-1);
        }

        //フォーク失敗
        if (pid == -1) {
            printf("fork error");
        }

        //親プロセス
        if (pid > 0) {
            printf("開始 %s (pid %d) \n", hello[0], pid);

            write_new(pid, background, &process_manager);

            //フォアグラウンド実行
            if (background == 0) {
                foreground_pid = pid;
                pid = waitpid(pid, &status, 0);
                update_exited(pid, &process_manager);
            }
        }
    }

    return 0;
}