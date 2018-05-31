#include "net/HTTP/http_server.h"
#include "net/Server.h"
#include "net/EventLoop.h"
#include "base/Logging.h"
#include "net/Util.h"
#include <string.h>
#include "net/HTTP/http_request.h"

using namespace std;
using namespace std::placeholders;


void readCallback(shared_ptr<Channel> ch)
{
    int fd = ch->getFd();
    char buf[10024];

    int n;
    n=readn(fd,buf,sizeof buf);
    cout<<"receive:"<<buf<<endl<<endl;
    http_request http(buf);
    http.showDetail();
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
    //TestServer myServer_(&loop_,4,80);
    http_server myServer_("/home/centos/web/",&loop_,4,80);
    
    myServer_.start();
    loop_.loop();
    sleep(30);


    return 0;
}
