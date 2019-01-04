#include <unp.h>

void str_echo(int sockfd);

int main(int argv, char **argc)
{
    int listenfd, connfd;
    pid_t childpid;
    struct sockaddr_in cliaddr, srvaddr;
    socklen_t len;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port = htons(19);
    Bind(listenfd, (SA *) &srvaddr, sizeof(srvaddr));
  
    Listen(listenfd, LISTENQ);

    for ( ; ; ) {
        len = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *) &cliaddr, &len);
     
        if ( (childpid = Fork()) == 0) {
            Close(listenfd);
            str_echo(connfd);
            exit(0);
        } 	

        Close(connfd);
    }
  
}

void str_echo(int sockfd) {
    int n;
    char buff[MAXLINE];
    
    again:
    //while ( (n = read(sockfd, buff, MAXLINE)) > 0 )
    while(1) {
        n = 1;
        buff[0] = 1;
	Writen(sockfd, buff, n);
    }

    if (n < 0 && errno == EINTR)
	goto again;
    else if (n < 0)
	err_sys("str_echo: read error");
}
