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
    char sendline[MAXLINE], recvline[MAXLINE];
    
    while (Fgets(sendline, MAXLINE, fp) != NULL) {
    	Writen(sockfd, sendline, strlen(sendline));

        if (Readline(sockfd, recvline, MAXLINE) == 0)
 		err_quit("str_cli: server terminated prematurely");
	Fputs(recvline, stdout);
    }
}
