#include <unp.h>

void str_cli(FILE *file, int sockfd);

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in cliaddr;
    if (argc != 2)
	err_quit("usage: tcpcli <IPAddress>");

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &cliaddr.sin_addr);
    Connect(sockfd, (SA *)&cliaddr, sizeof(cliaddr));

    str_cli(stdin, sockfd);

    exit(0);
}

void str_cli(FILE *fp, int sockfd) {
    int maxfdp1;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];
    
    FD_ZERO(&rset);
    for ( ; ; ) {
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);
        printf("the maxdfp1 is %d \n", maxfdp1);
        printf("the file descriptor is %d \n", fileno(fp));
        printf("the socket descriptor is %d \n", sockfd);
        printf("the fd_set size is %lu \n", sizeof(rset.fds_bits));
        printf("the fd_set first item is %d \n", rset.fds_bits[0]);
        printf("the fd_set first item is %x \n", rset.fds_bits[0]);
        if (FD_ISSET(sockfd, &rset)) {
            if (Readline(sockfd, recvline, MAXLINE) == 0)
                err_quit("str_cli: server terminated prematurely");
            Fputs(recvline, stdout);
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            if (Fgets(sendline, MAXLINE, fp) == NULL) {
                return;
            }
            Writen(sockfd, sendline, strlen(sendline));
        }
    }


    //while (Fgets(sendline, MAXLINE, fp) != NULL) {
    //	Writen(sockfd, sendline, strlen(sendline));

    //    if (Readline(sockfd, recvline, MAXLINE) == 0)
 //		err_quit("str_cli: server terminated prematurely");
//	Fputs(recvline, stdout);
 //   }
}
