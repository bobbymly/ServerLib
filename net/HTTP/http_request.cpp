#include "http_request.h"
using std::string;


bool http_request::setMethod(const string& str)
{
    if(str == "GET")method_ = GET;
    else if(str == "POST")method_ =  POST;
    else if(str == "HEAD")method_ =  HEAD;
    else if(str == "PUT")method_ =  PUT;
    else if(str == "DELETE")method_ =  DELETE;
    return method_ != DEFAULT;
}



bool http_request::processRequestLine(const string& request)
{
    if(processSucceed)return true;
    int start = 0;
    int space = request.find(" ");
    string method = request.substr(0,space);
    string path,query;
    if(space < request.length() && setMethod(method))
    {
        start = space +1;
        space = request.find(" ",start);
        int question = request.find("?",start);
        if(question == string::npos)
        {
            path = request.substr(start,space - start);
            setPath(path);
        }else{
            path = request.substr(start,question - start);
            setPath(path);
            query = request.substr(question + 1,space -question -1);
            setQuery(query);
            parseArgs(query);
        }
        
        start = space + 1;
        if(request.find("HTTP/1.1") != string::npos)
        {
            setVersion(HTTP_11);
        }
        else if(request.find("HTTP/1.0") != string::npos)
        {
            setVersion(HTTP_10);
        }
        
    }

    return processSucceed = true;
}

bool http_request::parseArgs(const string& str)
{
    string key,value;
    int start = 0;
    while(start < str.length())
    {
        int mid = str.find("&",start);
        if(mid == string::npos)mid = str.length()-1;
        int sig = str.find("=",start);
        if(sig == string::npos)sig = str.length();
        key = str.substr(start,sig-start);
        
        value = str.substr(sig + 1,mid - sig -1);
        args_[key] = value;
        //cout<<"key:"<<key<<" value:"<<value<<endl;
        start = mid + 1;
    }
    return true;
}


bool http_request::parseHeaders(const string& str)
{
    int start = 0,step,equal_s;
    while(start < str.length())
    {
        step = str.find("\r\n",start);
        equal_s = str.find(":",start);
        if(equal_s == string::npos)break;
        headers_[str.substr(start,equal_s - start)] = str.substr(equal_s + 1,step - equal_s -1);
        if(step != string::npos)start = step + 2;
        else break;
    }
    return true;
}



bool http_request::parseRequest()
{
    int start = 0;
    string requestLine,headers,body;
    int  request_line_end = buf_.find("\r\n");
    requestLine = buf_.substr(start,request_line_end - start);
    processRequestLine(requestLine);
    int headers_end = buf_.find("\r\n\r\n");
    headers =  buf_.substr(request_line_end + 2,headers_end - request_line_end -2);
    parseHeaders(headers);
    body = buf_.substr(headers_end + 4,buf_.length() - headers_end);
    setBody(body);

    return true;
}