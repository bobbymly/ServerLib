#include "http_module.h"
using std::string;


bool http_module::setMethod(const char* begin,const char* end)
{
    string str(begin,end);
    if(str == "GET")method_ = GET;
    else if(str == "POST")method_ =  POST;
    else if(str == "HEAD")method_ =  HEAD;
    else if(str == "PUT")method_ =  PUT;
    else if(str == "DELETE")method_ =  DELETE;
    return method_ != DEFUALT;
}



//请求行解析
bool http_module::processRequestLine(const char* begin,const char* end)
{
    if(processSucceed)return true;
    const char* start = begin;
    const char* space = std::find(start,end,' ');
    if(space != end && setMethod(start,end))
    {
        ++start;
        space =std::find(start,end,' ');
        const char* question std::find(start,space,'?');
        setPath(start,question);
        if(question != space)setQuery(question,space);
        ++start;
        processSucceed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
        if (processSucceed)
        {
            if (*(end-1) == '1')
            {
                request_.setVersion(HTTP_11);
            }
            else if (*(end-1) == '0')
            {
                request_.setVersion(HTTP_10);
            }
            else
            {
                return processSucceed = false;
            }
        }
    }

    return processSucceed = true;
}

bool http_module::parseHeaders(const char* begin,const char* end)
{
    const char* start = begin;
    const char* step;
    const char* equel_s;
    while(start < end)
    {
        step = std::equel(start,end,"\r\n");
        equel_s = std::find(start,end,'=');
        headers[string(start,equel_s)] = string(equel_s + 1,step);
        start = step + 2;
    }
    return true;
}



bool http_module::parseRequest()
{
    const char* start = buf_.data();
    const char* request_line_end = buf_.find("\r\n");
    processRequestLine(start,request_line_end);
    //start += 2;
    const char* headers_end = std::find(buf_.data(),buf_.end(),"\r\n\r\n");
    parseHeaders(request_line_end + 2,headers_end);
    setBody(headers_end + 2,buf_.data() + buf_.length());

    return true;
}