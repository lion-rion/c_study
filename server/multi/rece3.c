#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <net/if.h>
int main()
{
    int sock;
    struct addrinfo hints, *res;
    int err, n;
    int fd;
    struct group_req greq;
    char buf[2048];
    char filename[2048];
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    err = getaddrinfo(NULL, "12345", &hints, &res);
    if (err != 0)
    {
        printf("getaddrinfo : %s¥n", gai_strerror(err));
        return 1;
    }

    
    sock = socket(res->ai_family, res->ai_socktype, 0);
    bind(sock, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    err = getaddrinfo("239.103.111.2", NULL, &hints, &res);
    if (err != 0)
    {
        printf("getaddrinfo : %s¥n", gai_strerror(err));
        return 1;
    }
    memset(&greq, 0, sizeof(greq));
    greq.gr_interface = if_nametoindex("en0"); /* 任意のネットワークインターフェースを利用 */
    /* greq.gr_interface = if_nametoindex("en0"); mac 等で特定のインターフェースを指定
    する場合はこちら*/
    /* getaddrinfo()の結果を group_req 構造体へコピー */
    memcpy(&greq.gr_group, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    /* MCAST_JOIN_GROUP を利用してマルチキャストグループへ JOIN */
    if (setsockopt(sock, IPPROTO_IP, MCAST_JOIN_GROUP, (char *)&greq, sizeof(greq)) !=
        0)
    {
        perror("setsockopt");
        return 1;
    }
    int x;
    memset(filename, 0, sizeof(filename));
    n = recv(sock, filename, sizeof(filename), 0);
    fd = open(filename, O_WRONLY | O_CREAT, 0600);
    if (fd < 0) {
        perror("open");
        return 1;
    } 
    while(1) {
        printf("1");
        n = recv(sock, buf, sizeof(buf),0);
        if (n < 1) {
            perror("read");
            break;
        }
        printf("2");
        x = write(fd, buf, n);
        if (x < 1) {
            perror("write");
            break;
        }
        printf("3");
    } 
    close(sock);
    return 0;
}