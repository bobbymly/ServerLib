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
        content_length_(0),
        total_length_(0),
        status_code_(404),
        version_(HTTP_11),
        content_type_("text/html")

    {

    }


    void setDate(const string& str)
    {   date_ = str;}

    void countLengthToString();

    void setCookie(const string& key,const string& value)
    {   cookie_[key] = value;}

    void setPage(const string& str)
    {   page_ = str;}

    string& getResponse();
    int getTotalLength()
    {   
        if(total_length_ == 0)getResponse();
        return total_length_;
    }

    void setCode(int code)
    {
        status_code_ = code;
    }

    void setContentType(const string& str)
    {
        if(str == "txt" || str == "html" || str == "htm")
            content_type_ = "text/html";
        if(str == "js")
            content_type_ = "application/javascript";
    }
private:
    bool parse_;
    Version version_;
    int status_code_;
    string content_type_;
    string date_;
    string content_length_string_;
    int content_length_;
    int total_length_;
    map <string,string> cookie_;
    string page_;
    string response_;

};