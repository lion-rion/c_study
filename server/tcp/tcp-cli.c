#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(int argc, char *argv[])
{
    char ip = "127.0.0.1";  //ip address
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
    char cmd[128];
    char *ptr;
    while (1)
    {
        printf("Message (or quit）) : : "); fgets(cmd, 128, stdin); //コマンドを入力させる
        cmd[strlen(cmd) - 1] = '\0';  //\nを削除
        write(sock, cmd, strlen(cmd));
        if(strcmp(cmd,"quit")==0) {
            //終了
            printf("Exit...\n");
            break;
        }
        
        //メッセージを受け取る
        memset(buf, 0, sizeof(buf));
        read(sock, buf, sizeof(buf));
        if(strcmp(buf, "quit") == 0)
        {
            printf("Disconnected\n");
            break;
        }
        
        printf("%s\n", buf);
    }
    
    /* socket の終了 */
    close(sock);
    return 0;
}