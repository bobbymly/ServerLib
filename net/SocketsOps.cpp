#include "SocketsOps.h"
#include "../base/Logging.h"
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>

const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr)
{
    return static_cast<const struct sockaddr*>((void*)addr);
}

struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr)
{
    return static_cast<struct sockaddr*>((void*)addr);
}

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr)
{
    return static_cast<const struct sockaddr*>((void*)addr);
}

const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr)
{
    return static_cast<const struct sockaddr_in*>((void*)addr);
}

const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr)
{
    return static_cast<const struct sockaddr_in6*>((void*)addr);
}



//在buf中写入IP地址和端口号
void sockets::toIpPort(char* buf,size_t size,const struct sockaddr* addr)
{
    toIp(buf,size,addr);
    size_t end = strlen(buf);
    const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
    uint16_t port = ntohs(addr4->sin_port);
    assert(size>end);
    snprintf(buf+end,size-end,":%u",port);
}
//在buf中写入端口号
void sockets::toIp(char* buf,size_t size,const struct sockaddr* addr)
{
    if(addr->sa_family == AF_INET)
    {
        assert(size>=INET_ADDRSTRLEN);
        const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
        inet_ntop(AF_INET,&addr4->sin_addr,buf,static_cast<socklen_t>(size));
    }
    else if(addr->sa_family == AF_INET6)
    {
        assert(size>=INET6_ADDRSTRLEN);
        const struct sockaddr_in6* addr6 = sockaddr_in6_cast(addr);
        inet_ntop(AF_INET6,&addr6->sin6_addr,buf,static_cast<socklen_t>(size));
    }
}
//向 sockaddr_in 中填充 ip 和 端口 信息
void sockets::fromIpPort(const char* ip,uint16_t port,struct sockaddr_in* addr)
{
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if(inet_pton(AF_INET,ip,&addr->sin_addr)<=0)
    {
        LOG<<"fromIpPort error";
    }
}

//向 sockaddr_in6 中填充 ip 和 端口 信息
void sockets::fromIpPort(const char* ip,uint16_t port,struct sockaddr_in6* addr)
{
    addr->sin6_family = AF_INET6;
    addr->sin6_port = htons(port);
    if(inet_pton(AF_INET6,ip,&addr->sin6_addr)<=0)
    {
        LOG<<"fromIpPort error";
    }
}


void sockets::close(int sockfd)
{
    if(::close(sockfd)<0)
    {
        LOG<<"sockets::close";
    }
}

void sockets::bindOrDie(int sockfd,const struct sockaddr* addr)
{
    int ret = ::bind(sockfd,addr,static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    if(ret < 0)
    {
        LOG<<"sockets::listenOrDie";
    }
}

void sockets::listenOrDie(int sockfd)
{
    int ret = ::listen(sockfd,SOMAXCONN);
    if(ret < 0)
    {
        LOG<<"sockets::listenOrDie";
    }
}


int sockets::accept(int sockfd,struct sockaddr_in6* addr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
    LOG << "3";
    int connfd = ::accept(sockfd,sockaddr_cast(addr),&addrlen);
    LOG <<"2";
    if(connfd<0)
    {
        int savedErrno = errno;
        LOG<<"Socket::accept";
        switch(savedErrno)
        {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPERM:
            case EMFILE:
                errno = savedErrno;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                LOG<<"unexpected error of ::accept "<<savedErrno;
                break;
            default:
                LOG<<"unknown error of ::accept "<<savedErrno;
                break;
        }
    }
    return connfd;
}

void sockets::shutdownWrite(int sockfd)
{
    if(::shutdown(sockfd,SHUT_WR)<0)
    {
        LOG<<"sockets:: shutdownWrite error";
    }
}

