#include "net/Server.h"
#include "net/EventLoop.h"
#include "base/Logging.h"
#include <string.h>
//#include <unistd.h>
//#include <iostream>
//#include <memory>
using namespace std;
using namespace std::placeholders;






class EchoServer
{
public:

    EchoServer(EventLoop* loop,int threadNum,int port):
        myServer_(loop,threadNum,port)
    {
        //void (temp)(shared_ptr<Channel> ch) = std::bind(&EchoServer::readCallback,this,_1);
        std::function<void(shared_ptr<Channel>)> readCB = std::bind(&EchoServer::readCallback,this,_1);
        myServer_.setReadCallback(readCB);
    }


    void start()
    {
        myServer_.start();
    }




private:
    Server myServer_;
    void readCallback(shared_ptr<Channel> ch)
    {
        int fd = ch->getFd();
        char buf[10024];
        //char ans[] = "HTTP/1.1 200 OK\r\n";
        //HTTP/1.1 200 OK
        char ans[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=koi8-r\r\nContent-Length: 175\r\nAccept-Ranges: bytes\r\n\r\n<html><head><title>Welcome!</title><style>body {width: 35em;margin: 0 auto;font-family: Tahoma, Verdana, Arial, sans-serif;}</style></head><body><h1>Welcome</h1></body></html>";
//<!DOCTYPE html>
       
       
        int n;
        n=read(fd,buf,sizeof buf);
        //std::cout<<"here"<<buf;
        while(n>0)
        {
            std::cout<<"Echo message : "<<buf;
            write(fd,ans,sizeof ans);
            n=read(fd,buf,sizeof buf);
        }
        if(n == 0)LOG<<"socket closed";
    }
   
};



int main()
{
    EventLoop loop_;
    LOG <<"begin";
    EchoServer myServer_(&loop_,4,8011);
    myServer_.start();
    loop_.loop();
    sleep(30);













    return 0;
}


// #include "net/Server.h"
// #include "net/EventLoop.h"
// #include "base/Logging.h"
// int main()
// {
//     EventLoop loop_;
//     LOG <<"begin";
//     Server myServer_(&loop_,4,8011);
//     myServer_.start();
//     loop_.loop();
//     sleep(30);













//     return 0;
// }