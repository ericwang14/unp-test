#include <unp.h>

#define NDG     200000
#define DGLEN   2000

void dg_cli1(FILE *fp, int sockfd, SA *pservaddr, socklen_t servlen) {
    int i;
    char sendline[MAXLINE];

    for (i = 0; i < NDG; i++) {
        Sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen);
    }
}
