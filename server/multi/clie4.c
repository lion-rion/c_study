#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
int main(int argc, char *argv[])
{
    struct sockaddr_in server;
    struct addrinfo hints, *res;
    int sock;
    int fd;
    char buf[65536];
    int n, ret;
    char *hostname = argv[1];
    int err;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    /* ソケットの作成 */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* 接続先指定用構造体の準備 */
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    /* 127.0.0.1 は localhost */
    if ((err = getaddrinfo(hostname, NULL, &hints, &res)) != 0)
    {
        printf("error %d\n", err);
        return 1;
    }
    //ipアドレスを出力
    //printf("%s\n", inet_ntoa(((struct sockaddr_in *)res->ai_addr)->sin_addr));

    inet_pton(AF_INET, inet_ntoa(((struct sockaddr_in *)res->ai_addr)->sin_addr), &server.sin_addr.s_addr);

    /* サーバに接続 */
    connect(sock, (struct sockaddr *)&server, sizeof(server));
    char cmd[128];
        printf("Message : "); fgets(cmd, 128, stdin); //コマンドを入力させる
        n = write(sock, cmd, strlen(cmd));
        if (n < 1)
        {
            perror("write");
            return 1;
        }
        read(sock, buf, sizeof(buf) - 1);
        printf("received : %s", buf);
        //メッセージを受け取る
    
    /* socket の終了 */
    close(sock);
    return 0;
}