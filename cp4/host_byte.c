#include <unp.h>

int main()
{
    uint32_t loca = 0xe00000ff;
    printf("value is %s\r", htonl(loca));
    return 0;
}
