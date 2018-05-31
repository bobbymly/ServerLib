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

    void getMethod(http_request& req,int fd)
    {
        http_response response;
        string file_name;
        if(req.getPath() == "/")file_name = root_path_ + "/index.html";
        else file_name = root_path_ + req.getPath();
        std::ifstream file(file_name);
        if(!file.is_open())
        {
            response.setCode(404);
            write(fd,response.getResponse().c_str(),response.getResponse().length());
            return ;
        }
        response.setCode(200);
        response.setContentType(req.getContentType());
        std::stringstream buffer;
        buffer << file.rdbuf();  
        response.setPage(buffer.str());

        writen(fd,response.getResponse().c_str(),response.getResponse().length());
    }


    void readCB(shared_ptr <Channel> ch)
    {
        int fd = ch -> getFd();
        //http_response response;
        char buf[1024*1000];
        readn(fd,buf,sizeof buf);
        http_request request(buf);
        getMethod(request,fd);


        //request.showDetail();
        // string file_name;
        // if(request.getPath() == "/")file_name = root_path_ + "/index.html";
        // else file_name = root_path_ + request.getPath();
        // //cout<<"file_name:"<<file_name<<endl;
        // std::ifstream file(file_name);
        // if(!file.is_open())
        // {
        //     response.setCode(404);
        //     write(fd,response.getResponse().c_str(),response.getResponse().length());
        //     return ;
        // }
        // response.setCode(200);
        // response.setContentType(request.getContentType());
        // std::stringstream buffer;  
        // buffer << file.rdbuf();  
        // response.setPage(buffer.str());

        // writen(fd,response.getResponse().c_str(),response.getResponse().length());

        close(fd);
    }

private:

    Server myServer_;
    string root_path_;
};