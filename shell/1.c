#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* 変数配列[] = {コマンド, NULL} */

int execv(const char *path, char *const argv[]);

int main(){

    int res;

    while(1){
        char *pargs[2] = {"/bin/ls",  NULL}; 
        res = execv(pargs[0],pargs);
    }
    
    return 0;
}