#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main()
{
    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in senderinfo;
    socklen_t addrlen;
    char buf[2048];
    int n;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    addrlen = sizeof(senderinfo);
    while (1)
    {
        n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
                 (struct sockaddr *)&senderinfo, &addrlen);
        write(fileno(stdout), buf, n);
        sendto(sock, buf, n, 0, (struct sockaddr *)&senderinfo, addrlen);
        if (n < 1)
        {
            perror("sendto");
            return 1;
        }
    }
    close(sock);
    return 0;
}