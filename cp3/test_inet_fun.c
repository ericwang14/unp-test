#include <unp.h>

int main(int argc, char ** argv)
{
    if (argc < 2)
	err_quit("wrong parametes");

    in_addr_t addr;
    addr = inet_addr(argv[1]);

    printf("%x\n", addr);
    struct in_addr addr_stru;
    addr_stru.s_addr = addr;
    printf("convert back: %s\n", inet_ntoa(addr_stru));
    return 0;
}
