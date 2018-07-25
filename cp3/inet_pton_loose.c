#include <unp.h>

int inet_pton_loose(int family, const char *strptr, void *addrptr);

int main(int argv, char **argc) 
{
   struct in_addr addr_s;
   
   if( (inet_pton_loose(AF_INET,  argc[1], &addr_s)) == 1)
	printf("address is %x\n", addr_s.s_addr);
   else 
	printf("can't convert %s to network byte order address\n", argc[1]);

   
}

int inet_pton_loose(int family, const char *strptr, void *addrptr)
{
   if (family == AF_INET) {
    	if( (inet_pton(family, strptr, addrptr)) <= 0)
		if (inet_aton(strptr, (struct in_addr *)addrptr) == 0)
			return -1;
		else 
			return 1;
	else
		return 1; 
    }
}
