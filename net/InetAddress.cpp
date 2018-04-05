#include <stdint.h>
#include <endian.h>
#include <InetAddress.h>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include <base/Logging.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "SocketOps.h"

static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;


InetAddress::InetAddress(uint16_t port,bool loopbackOnly,bool ipv6)
{
    static_assert(offsetof(InetAddress,addr6_)==0,"addr6_ offset 0");
    static_assert(offsetof(InetAddress,addr_)==0,"addr_ offset 0");
    if(ipv6)
    {
        bzero(&addr6_,sizeof addr6_);
        addr6_.sin6_family = AF_INET6;
        in6_addr ip = loopbackOnly? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr = ip;
        addr6_.sin6_port = htons(port);
    }
    else
    {
        bzero(&addr_,sizeof addr_);
        addr_.sin_family = AF_INET;
        in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;
        addr_.sin_addr.s_addr = ip;
        addr_.sin_port = htons(port);
    }
}

InetAddress::InetAddress(string ip,uint16_t port,bool ipv6)
{
    if(ipv6)
    {
        bzero(&addr6_,sizeof addr6_);
        sockets::fromIpPort(ip.c_str(),port,&addr6_);
    }
    else
    {
        bzero(&addr_,sizeof addr_);
        sockets::fromIpPort(ip.c_str(),port,&addr_);
    }
}

string InetAddress::toIpPort()const{
    char buf[64] = "";
    sockets::toIpPort(buf,sizeof buf,getSockAddr());
    return buf;
}

string InetAddress::toIp()const
{
    char buf[64] = "";
    sockets::toIp(buf,sizeof buf,getSockAddr());
    return buf;
}

uint16_t InetAddress::ipNetEndian()const
{
    assert(family() == AF_INET);
    return addr_.sin_addr.s_addr;
}

uint16_t InetAddress::toPort()const
{
    return ntohs(portNetEndian());
}

static __thread char t_resolveBuffer[64*1024];

bool InetAddress::resolve(string hostname,InetAddress* out)
{
    asset(out!=NULL);
    struct hostent hent;
    struct hostent* he = NULL;
    int herrno = 0;
    bzero(&hent,sizeof(hent));

    int ret = gethostbyname_r(hostname.c_str(),&hent,t_resolveBuffer,sizeof t_resolveBuffer,&he,&herrno);
    if(ret ==0&& he!= NULL)
    {
        assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
        out->addr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
        return true;
    }
    else
    {
        if(ret)
        {
            LOG<<"InetAddress::resolve";
        }
        return false;
    }
}