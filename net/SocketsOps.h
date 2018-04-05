#include <arpa/inet.h>


namespace sockets
{
    void toIpPort(char* buf,size_t size,const struct sockaddr* addr);
    void toIp(char* buf,size_t size,const struct sockaddr* addr);
    void fromIpPort(const char* ip,uint16_t port,struct sockaddr_in* addr);
    void fromIpPort(const char* ip,uint16_t port,struct sockaddr_in6* addr);

}
