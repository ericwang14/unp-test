#include <unp.h>
#include <time.h>

int main()
{
    int conntdf, listendf;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buff[MAXLINE];
    time_t timet;

    listendf = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9999);

    //Bind(listendf,(SA *) &servaddr, sizeof(servaddr));
    Listen(listendf, LISTENQ);

   for ( ; ; ) {
       len = sizeof(cliaddr);
       conntdf = Accept(listendf, (SA *)&cliaddr, &len);
       printf("connted sockect client %s port %d\n", Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
       timet = time(NULL);
       snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&timet));
       write(conntdf, buff, strlen(buff));
       Close(conntdf);
   }
   return 0;
}
