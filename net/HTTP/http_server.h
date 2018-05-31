#pragma once
#include "../../base/Logging.h"
#include "../Server.h"
#include <iostream>
#include <string>
#include <map>
#include "http_request.h"
#include "http_response.h"
#include <fstream>  
#include <sstream> 
using namespace std::placeholders;
using namespace std;




class http_server
{
public:
    http_server(const string path,EventLoop* loop,int threadNum,int port = 80):
        root_path_(path),
        myServer_(loop,threadNum,port)
    {
        std::function<void(shared_ptr <Channel>)> rcb = std::bind(&http_server::readCB,this,_1);
        myServer_.setReadCallback(rcb);
    }   

    void start()
    {   myServer_.start();}

    void readCB(shared_ptr <Channel> ch)
    {
        cout<<"read"<<endl;
        int fd = ch -> getFd();
        http_response response;
        char buf[1024*1000];
        int n=readn(fd,buf,sizeof buf);
        http_request request(buf);
        request.showDetail();
        string file_name = root_path_ + request.getPath();
        std::ifstream file(file_name);
        if(!file.is_open())
        {
            write(fd,response.getResponse().c_str(),response.getTotalLength());
            return ;
        }
        response.setCode(200);
        response.setContentType(request.getContentType());
        std::stringstream buffer;  
        buffer << file.rdbuf();  
        response.setPage(buffer.str());

        const char* write_buf = response.getResponse().c_str();
        long long total_size = response.getResponse().length();
        long long sendBytes = 0,temp = 1;
        sendBytes = writen(fd,write_buf,total_size);
        // while(sendBytes < total_size)
        // {
        //     temp= send(fd,write_buf,2000);
        //     cout<<temp<<endl;
        //     write_buf += temp;
        //     sendBytes += temp;
        // }
        //cout<<response.getResponse().c_str()<<endl;
        cout<<response.getResponse().length()<<endl;
        cout<<"send:"<<sendBytes<<" bytes"<<endl;
        close(fd);
        //close(file);
        cout<<"read over"<<endl;
    }

private:
    //http_request request_;
    //http_response response_;
    Server myServer_;
    string root_path_;
};