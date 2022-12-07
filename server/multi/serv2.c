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
    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in senderinfo;
    int n;
    char buf[2048];
    socklen_t addrlen;
    char *hostname = argv[1];
    struct addrinfo hints, *res;
    int err;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    if ((err = getaddrinfo(hostname, NULL, &hints, &res)) != 0)
    {
        printf("error %d\n", err);
        return 1;
    }
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, inet_ntoa(((struct sockaddr_in *)res->ai_addr)->sin_addr), &addr.sin_addr.s_addr);
    char cmd[128];
    while (1)
    {
        printf("Message : "); fgets(cmd, 128, stdin); //コマンドを入力させる
        n = sendto(sock, cmd, strlen(cmd), 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 1)
        {
            perror("sendto");
            return 1;
        }
        addrlen = sizeof(senderinfo);
        n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
                 (struct sockaddr *)&senderinfo, &addrlen);
        
        write(fileno(stdout), buf, n);
        //メッセージを受け取る
    }
    
    close(sock);
    return 0;
}