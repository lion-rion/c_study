#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(int argc, char *argv[])
{
    char *
    ip = "127.0.0.1";  //ip address
    struct sockaddr_in server;
    int sock;
    char buf[65536];
    int n, ret;
    /* ソケットの作成 */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* 接続先指定用構造体の準備 */
    server.sin_family = AF_INET;
    server.sin_port = htons(4000); //port number
    inet_pton(AF_INET, ip, &server.sin_addr.s_addr);
    connect(sock, (struct sockaddr *)&server, sizeof(server));
    
    FILE* fp;
    char x[40];
    char  buffer[1024];
    char password[40];
    printf("パスワードを10文字以内で入力してください : ");
    fgets(password, 10, stdin); //コマンドを入力させる
        password[strlen(password) - 1] = '\0';  //\nを削除
    //printf("パスワードは%sです\n",password);
    char syscmd[50] = "echo -n ";
    strcat(syscmd, password);
    strcat(syscmd, " | shasum");
    char *sys = syscmd;
    if ((fp = popen(sys, "r")) != NULL) {
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            for (int i = 0; i < 40; ++i) {
                x[i] = buf[i];
            }
        }
        pclose(fp);
        //printf("x = %s\n", x);
    }

    //パスワードを送信
    write(sock, x, 40);
    char cmd[128];
    char *ptr;
    //1を受け取るまでループ
    while (1)
    {

        memset(buf, 0, sizeof(buf));
        read(sock, buf, sizeof(buf));
        printf("%s\n", buf);
        if ( strcmp(buf, "disconnected")== 0 )
        {
            break;
        }
        if(strcmp(buf, "quit")  )
        {
            printf("Disconnected\n");
            break;
        }
        printf("%s\n", buf);

        printf("Message (or quit）) : "); fgets(cmd, 128, stdin); //コマンドを入力させる
        cmd[strlen(cmd) - 1] = '\0';  //\nを削除
        write(sock, cmd, strlen(cmd));
        if(strcmp(cmd,"quit")==0) {
            //終了
            printf("Exit...\n");
            break;
        }
    }
    
    /* socket の終了 */
    close(sock);
    return 0;
}