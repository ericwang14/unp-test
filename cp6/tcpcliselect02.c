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
    int maxfdp1, stdineof;
    fd_set rset;
    char buf[MAXLINE];
    int n = 0;
    
    stdineof = 0;
    FD_ZERO(&rset);
    for ( ; ; ) {
        if (stdineof == 0)
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
            if ( ( n = Readline(sockfd, buf, MAXLINE)) == 0 ) {
                if (stdineof == 1)
                    return;
                else 
                    err_quit("str_cli: server terminated prematurely");
            }
            Write(fileno(stdout), buf, n);
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            if ( ( n = Read(fileno(fp), buf, MAXLINE)) == 0) {
                stdineof = 1;
                Shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            Writen(sockfd, buf, n);
        }
    }


    //while (Fgets(sendline, MAXLINE, fp) != NULL) {
    //	Writen(sockfd, sendline, strlen(sendline));

    //    if (Readline(sockfd, recvline, MAXLINE) == 0)
 //		err_quit("str_cli: server terminated prematurely");
//	Fputs(recvline, stdout);
 //   }
}
