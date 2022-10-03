#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {

    while (1)
    {
        char command[128];
        printf("$ "); fgets(command, 128, stdin); //コマンドを入力させる
        command[strlen(command) - 1] = '\0';  //\nを削除

        system(command);
    }

    return 0;
}