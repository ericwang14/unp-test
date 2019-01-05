#include <unp.h>

#undef MAXLINE
#define MAXLINE 105000

union val {
  int                           i_val;
  long                          l_val;
  struct linger         linger_val;
  struct timeval        timeval_val;
} val;

void 
dg_cli1(FILE *fp, int sockfd, SA *pservaddr, socklen_t servlen) {
    int size;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    int n;
    socklen_t ss;
    
    size = 150000;
    ss = sizeof(val);
    Setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
    Setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &val, &ss);

    printf("size of send buff %d\n", val.i_val);
    Sendto(sockfd, sendline, MAXLINE, 0, pservaddr, servlen);

    n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);

    printf("received %d bytes\n", n);
}
