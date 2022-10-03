#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);

int main(){

    //全体をループ
    while(1)
    {
    //コマンドを入力させる
    
    char cmd[128];

    printf("$ "); fgets(cmd, 128, stdin); //コマンドを入力させる
    cmd[strlen(cmd) - 1] = '\0';  //\nを削除

    //引用 : コマンドの引数をポインタの配列として取る 
    //また、配列の最初は、ファイル名へのポインタとし、配列の最後は NULL にする必要がある。
    char *argv[] = {cmd, NULL};
    execv(argv[0], argv);
    }




    return 0;
}