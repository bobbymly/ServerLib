#include "http_module.h"



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

bool http_module::setPath(const char* begin,const char* end)

//请求行解析
bool http_module::processRequestLine(const char* begin,const char* end)
{
    if(processSucceed)return true;
    const char* start = begin;
    const char* space = std::find(start,end," ");
    if(space != end && setMethod(start,end))
    {
        ++start;
        space =std::find(start,end," ");
        const char* question std::find(start,space,"?");
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

bool http_module::parseRequest()
{
    const char* start = buf_.begin();
    const char* end = buf_.find("/r/n",start - buf_.begin);
    processRequestLine(begin,end);


}