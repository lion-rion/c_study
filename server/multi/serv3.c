#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int sock;
    char buf[65536];
    char filename[2048];
    int fd;
    struct sockaddr_in addr;
    in_addr_t ipaddr;
    int n;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = inet_addr("239.103.111.2");
    /* (複数の NIC がある場合に備えて)出力インターフェースの IP アドレスを設定 */
    /* 192.168.10.5 の部分を自分の PC の IP アドレス値に合わせて変更すること */
    ipaddr = inet_addr("172.30.88.130");
    if (setsockopt(sock,
            IPPROTO_IP,
            IP_MULTICAST_IF,
            (char *)&ipaddr, sizeof(ipaddr)) != 0)
    {
        perror("setsockopt");
        return 1;
    }
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    // argv[1]の中身をfilenameにコピ
    strcpy(filename, argv[1]);
    int x;
    x = sendto(sock, filename, strlen(filename), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (x < 1)
    {
        perror("sendto");
    }
    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        x = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&addr, sizeof(addr));
        if (x < 1)
        {
            perror("sendto");
            break;
        }
    }
    close(sock);
    return 0;
}