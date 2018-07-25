#include <unp.h>

int main(int argc, char **argv)
{
    int sockdf, n;
    struct sockaddr_in s_addr, s_addr_l;
    char buff[MAXLINE + 1];
    char *local_ip;
    socklen_t len;

    if (argc < 2)
	err_quit("no IP address");
    sockdf = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(13);

    if ( inet_pton(AF_INET, argv[1], &s_addr.sin_addr) <= 0)
	err_quit("inet_pton error for %s", argv[1]);

    Connect(sockdf, (SA *)&s_addr, sizeof(s_addr));
    len = sizeof(s_addr_l);
    if (getsockname(sockdf, (SA *)&s_addr_l, &len) == 0) {
    	local_ip = sock_ntop((SA *)&s_addr_l, sizeof(s_addr_l));
        printf("%s\n", local_ip);
    }
    while ( (n = read(sockdf, buff, MAXLINE)) > 0) {
        buff[n] = 0;
        if(fputs(buff, stdout) == EOF)
		err_sys("fputs error");
    }
    if (n < 0)
        err_sys("read error");
    return 0;
}
