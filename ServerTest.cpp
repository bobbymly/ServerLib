#include "net/Server.h"
#include "net/EventLoop.h"
#include "base/Logging.h"
#include "net/Util.h"
#include <string.h>

using namespace std;
using namespace std::placeholders;


void readCallback(shared_ptr<Channel> ch)
{
    int fd = ch->getFd();
    char buf[10024];

    char ans[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=koi8-r\r\nETag: \"5b055519-266\"\r\nContent-Length: 182\r\nX-Cache: MISS from SK-SQUIDWEB-18\r\nAccept-Ranges: bytes\r\n\r\n<!DOCTYPE html><html><head><title>Welcome!</title><style>body {width: 35em;margin: 0 auto;font-family: Tahoma, Verdana, Arial, sans-serif;}</style></head><body><h1>ServerLib Test</h1></body></html>";
    int n;
    n=readn(fd,buf,sizeof buf);
    write(fd,ans,sizeof ans);
}



class TestServer
{
public:

    TestServer(EventLoop* loop,int threadNum,int port):
        myServer_(loop,threadNum,port)
    {
        myServer_.setReadCallback(readCallback);
    }
    void start()
    {
        myServer_.start();
    }
private:
    Server myServer_;
};



int main()
{
    EventLoop loop_;
    TestServer myServer_(&loop_,4,80);
    myServer_.start();
    loop_.loop();
    sleep(30);


    return 0;
}
