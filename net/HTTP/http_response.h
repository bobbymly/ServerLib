#pragma once
#include "../../base/Logging.h"
#include <iostream>
#include <string>
#include <map>
#include "http_request.h"
using namespace std;


class http_response
{
public:
    http_response():
        parse_(false),
        status_code_(404),
        version_(HTTP_11),
        content_type_("text/html")
    {    }


    void setDate(const string& str)
    {   date_ = str;}

    void countLengthToString();

    void setCookie(const string& key,const string& value)
    {   cookie_[key] = value;}

    void setPage(const string& str)
    {   page_ = str;}

    string& getResponse();

    void setCode(int code)
    {
        status_code_ = code;
    }

    void setContentType(const string& str)
    {
        if(str == "txt" || str == "html" || str == "htm" || str == "jsp")
            content_type_ = "text/html";
        else if(str == "js")
            content_type_ = "application/javascript";
        else if(str == "asp")
            content_type_ = "text/asp";
        else if(str == "bmp")
            content_type_ = "application/x-bmp";
        else if(str == "ico")
            content_type_ = "image/x-icon";
        else if(str == "img")
            content_type_ = "application/x-img";        
        else if(str == "java")
            content_type_ = "java/*";        
        else if(str == "jpe" || str =="jpeg" ||str == "jpg")
            content_type_ = "image/jpeg";
        else if(str == "png")
            content_type_ = "application/x-png";
        else if(str == "xml" || str == "svg")
            content_type_ = "text/xml";
        else if(str == "txt")
            content_type_ = "text/plain";
        else if(str == "pic")
            content_type_ = "application/x-pic";
    }
private:
    bool parse_;
    Version version_;
    int status_code_;
    string content_type_;
    string date_;
    string content_length_string_;
    map <string,string> cookie_;
    string page_;
    string response_;
};