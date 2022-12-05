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
    struct sockaddr_in server;
    int sock;
    char buf[65536];
    int n, ret;
    /* ソケットの作成 */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* 接続先指定用構造体の準備 */
    server.sin_family = AF_INET;
    server.sin_port = htons(4000);
    /* 127.0.0.1 は localhost */
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
    connect(sock, (struct sockaddr *)&server, sizeof(server));
    char cmd[128];
    char *ptr;
    while (1)
    {
        printf("送信するメッセージ(終了する場合はquit) : "); fgets(cmd, 128, stdin); //コマンドを入力させる
        cmd[strlen(cmd) - 1] = '\0';  //\nを削除
        write(sock, cmd, strlen(cmd));
        if(strcmp(cmd,"quit")==0) {
            //終了
            printf("終了します\n");
            break;
        }
        
        //メッセージを受け取る
        memset(buf, 0, sizeof(buf));
        read(sock, buf, sizeof(buf));
        if(strcmp(buf, "quit") == 0)
        {
            printf("サーバーから切断されました\n");
            break;
        }
        
        printf("%s\n", buf);
        //内部コマンドの判定
        // if (check_cmd(cmd) == 1) {
        //     return 0;
        // }
    }
    
    /* socket の終了 */
    close(sock);
    return 0;
}