#include <unp.h>

void
dg_cli1(FILE *fp, int sockfd, SA *pservaddr, socklen_t servlen) {
    int n;
    char sendline[MAXLINE], recvline[MAXLINE];

    Connect(sockfd, (SA *) pservaddr, servlen);
    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        Write(sockfd, sendline, MAXLINE);

        n = Read(sockfd, recvline, MAXLINE);

        recvline[n] = 0;
        Fputs(recvline, stdout);

    }
}
