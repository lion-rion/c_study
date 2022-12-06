#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

/*
struct sockaddr_in {
    u_char  sin_len;    （このメンバは古いOSでは存在しない）
    u_char  sin_family;    （アドレスファミリ．今回はAF_INETで固定）
    u_short sin_port;    （ポート番号）
    struct  in_addr sin_addr;    （IPアドレス）
    char    sin_zero[8];    （無視してもよい．「詰め物」のようなもの）
};
*/

int main(int argc, char *argv[])
{
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    int n, ret;
    char buf[65536];
    /* ソケットの作成 */
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    /* ソケットの設定 */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4000);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    /* TCP クライアントからの接続要求を待てる状態にする */
    listen(sock0, 5);
    /* TCP クライアントからの接続要求を受け付ける */
    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr *)&client, &len);
    char cmd[128];
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        read(sock, buf, sizeof(buf));
        if (strcmp(buf, "quit") == 0)
        {
            printf("クライアントから切断されました\n");
            break;
        }
        printf("%s\n", buf);

        printf("送信するメッセージ : "); fgets(cmd, 128, stdin); //コマンドを入力させる
        cmd[strlen(cmd) - 1] = '\0';  //\nを削除
        write(sock, cmd, strlen(cmd));
        if(strcmp(cmd,"quit")==0) {
            //終了
            printf("終了します\n");
            break;
        }
        
    }
    
    /* TCP セッションの終了 */
    close(sock);
    /* listen する socket の終了 */
    close(sock0);
    return 0;
}