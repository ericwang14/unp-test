#include <unp.h>

void str_echo(int sockfd);

int main(int argv, char **argc)
{
    int listenfd, connfd, udpfd, nready, maxfdp1;
    char mesg[MAXLINE];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    const int on = 1;
    struct sockaddr_in cliaddr, srvaddr;
    socklen_t len;
    void sig_chld(int);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port = htons(SERV_PORT);

    Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    Bind(listenfd, (SA *) &srvaddr, sizeof(srvaddr));
  
    Listen(listenfd, LISTENQ);

    //UDP SERVER
    udpfd = Socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port = htons(SERV_PORT);

    Bind(udpfd, (SA *) &srvaddr, sizeof(srvaddr));

    Signal(SIGCHLD, sig_chld);

    FD_ZERO(&rset);
    maxfdp1 = max(listenfd, udpfd) + 1;

    for ( ; ; ) {
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);

        if ( (nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR)
                continue;
            else
                err_sys("select error");
        }

        if (FD_ISSET(listenfd, &rset)) {
            len = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *) &cliaddr, &len);
     
            if ( (childpid = Fork()) == 0) {
                Close(listenfd);
                str_echo(connfd);
                exit(0);
            } 	

            Close(connfd);
        }

        if (FD_ISSET(udpfd, &rset)) {
            len = sizeof(cliaddr);
            n = Recvfrom(udpfd, mesg, MAXLINE, 0, (SA *) &cliaddr, &len);

            Sendto(udpfd, mesg, n, 0, (SA *)&cliaddr, len);
        }
    }
}

void str_echo(int sockfd) {
    int n;
    char buff[MAXLINE];
    
    again:
    while ( (n = read(sockfd, buff, MAXLINE)) > 0 )
	Writen(sockfd, buff, n);

    if (n < 0 && errno == EINTR)
	goto again;
    else if (n < 0)
	err_sys("str_echo: read error");
}


void sig_chld(int signo) {
    pid_t pid;
    int stat;
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}
