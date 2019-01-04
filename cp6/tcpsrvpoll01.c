#include <unp.h>
#include <limits.h>
#include <unistd.h>

int main(int argc, char **argv) {
    printf("the maximum open file descriptor is %d\n", _SC_OPEN_MAX);
    printf("testing\n");
    int i, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t clilen;
    //int MAX_FILE = OPEN_MAX;
    int MAX_FILE = _SC_OPEN_MAX;
    struct pollfd client[MAX_FILE];
    struct sockaddr_in cliaddr, servaddr;
    
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < MAX_FILE; i++)
        client[i].fd = -1;
    maxi = 0;

    for ( ; ; ) {
        nready = Poll(client, maxi + 1, INFTIM);

        if (client[0].revents & POLLRDNORM) {
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *) &cliaddr, & clilen);

            for (i = 1; i < MAX_FILE; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    break;
                }
            }

            if (i == MAX_FILE)
                err_quit("too many clients");

            client[i].events = POLLRDNORM;
            if (i > maxi)
                maxi = i;

            if (--nready <= 0)
                continue;
        }

        for (i = i; i <= maxi; i++) {
            if ( (sockfd = client[i].fd) < 0)
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ( (n = read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        Close(sockfd);
                        client[i].fd = -1;
                    } else 
                        err_sys("read error");
                } else if ( n == 0) {
                    Close(sockfd);
                    client[i].fd = -1;
                } else 
                    Write(sockfd, buf, n);

                if ( --nready <= 0)
                    break;
            }
        }
    }
}
