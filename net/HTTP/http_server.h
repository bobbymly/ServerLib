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
        std::function<void(weak_ptr <Channel>)> rcb = std::bind(&http_server::readCB,this,_1);
        myServer_.setReadCallback(rcb);
    }   

    void start()
    {   myServer_.start();}

    void GetMethod(http_request& req,int fd)
    {
        cout<<"fd = "<<fd<<endl;
        http_response response;
        string file_name;
        if(req.getPath() == "/")file_name = root_path_ + "/index.html";
        else file_name = root_path_ + req.getPath();
        std::ifstream file(file_name);
        if(!file.is_open())
        {
            response.setCode(404);
            write(fd,response.getResponse().c_str(),response.getResponse().length());
            file.close();
            return ;
        }
        response.setCode(200);
        response.setContentType(req.getContentType());
        std::stringstream buffer;
        buffer << file.rdbuf();  
        file.close();
        response.setPage(buffer.str());

        writen(fd,response.getResponse().c_str(),response.getResponse().length());
    }



    void readCB(weak_ptr <Channel> weakCh)
    {
        shared_ptr<Channel> ch = weakCh.lock();
        int fd = ch -> getFd();

        char buf[1024*1000];
        readn(fd,buf,sizeof buf);
        http_request request(buf);
        //cout<<buf<<endl;
        if(request.getMethod() == GET)GetMethod(request,fd);
        if(request.getMethod() == POST)
        {
            if(PostMethod)
            {
                PostMethod(request,fd);
            }else{
                http_response response;
                response.setCode(500);
                write(fd,response.getResponse().c_str(),response.getResponse().length());
                return ;
            }
            
        }
        map<string,string>& headers = request.getHeaders();
        //if(headers.find("Connection") != headers.end() && headers["Connection"] == "close")
        //close(fd);
        //ch.reset();
        //ch -> closeChannel();

    }

private:
    std::function<void (http_request,int)>PostMethod;
    Server myServer_;
    string root_path_;
};