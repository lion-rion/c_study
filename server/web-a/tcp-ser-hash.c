#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    char x[50];
    char buffer[1024];
    char password[40];

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
    addr.sin_port = htons(4000); // port number
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock0, 5);
    /* TCP クライアントからの接続要求を受け付ける */
    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr *)&client, &len);
    printf("パスワードを10文字以内で設定してください : ");
    fgets(password, 10, stdin); //コマンドを入力させる
        password[strlen(password) - 1] = '\0';  //\nを削除
    printf("パスワードは%sです\n", password);
    char syscmd[50] = "echo -n ";
    strcat(syscmd, password);
    strcat(syscmd, " | shasum");
    char *sys = syscmd;
    if ((fp = popen(sys, "r")) != NULL)
    {
        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
            for (int i = 0; i < 40; ++i)
            {
                x[i] = buf[i];
            }
        }
        pclose(fp);
        // printf("x = %s\n", x);
    }
    char cmd[128];
    char pass[40];
    memset(pass, 0, sizeof(pass)); // bufを0で初期化
    read(sock, pass, 40);          // パスワードを受信
    printf("buf = %s\n", pass);
    if (strcmp(pass, x) == 0)
    {
        printf("Password OK\n");
        //connectedを送信
        write(sock, "connected", 9);
        
    }
    else
    {
        // パスワードが一致しない
        printf("Password NG\n");
        //disconnectedを送信
        write(sock, "disconnected", 12);
        close(sock);
        close(sock0);
        return 0;
    }


    while (1)
        {
            memset(buf, 0, sizeof(buf));
            read(sock, buf, sizeof(buf));
            if (strcmp(buf, "quit") == 0)
            {
                printf("Disconnected\n");
                break;
            }
            printf("%s\n", buf);

            printf("MESSAGE (or quit) : ");
            fgets(cmd, 128, stdin);      // コマンドを入力させる
            cmd[strlen(cmd) - 1] = '\0'; //\nを削除
            write(sock, cmd, strlen(cmd));
            if (strcmp(cmd, "quit") == 0)
            {
                // 終了
                printf("Exit...\n");
                break;
            }
        }
    /* TCP セッションの終了 */
    close(sock);
    /* listen する socket の終了 */
    close(sock0);
    return 0;
}