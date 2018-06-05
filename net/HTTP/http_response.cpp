#include "../../base/Logging.h"
#include <iostream>
#include <string>
#include <map>
#include <string.h>
#include "http_response.h"
using namespace std;

string& http_response::getResponse()
{
    if(parse_)
    {
        return response_;
    }
    response_.clear();
    switch(version_)
    {
        case HTTP_10:
            response_ += "HTTP/1.0";
            break;
        case HTTP_11:
            response_ += "HTTP/1.1";
            break;
        case HTTP_20:
            response_ += "HTTP/2.0";
            break;
        default:
            response_ += "HTTP/1.0";
    }
    switch(status_code_)
    {
        case 200:
            response_ += " 200 OK\r\n";
            break;
        case 400:
            response_ += " 400 Bad Request\r\n";
            break;
        case 401:
            response_ += " 401 Unauthorized\r\n";
            break;
        case 403:
            response_ += " 403 Forbidden\r\n";
            break;
        case 404:
            response_ += " 404 Not Found\r\n";
            break;
        case 500:
            response_ += " 500 Internal Server Error\r\n";
            break;
        case 503:
            response_ += " 503 Server Unavailable\r\n";
            break;
        default:
            response_ += " 404 Not Found\r\n";
    }
    //if(status_code_ != 200)return response_;
    response_ += "Content-Type: ";
    response_ += content_type_ + "\r\n";
    response_ += "Connection: ";
    response_ += connection_ + "\r\n";
    for(auto it = cookie_.begin();it != cookie_.end();it++)
    {
        response_ += "Set-Cookie: ";
        response_ += it->first + "=" + it->second + "\r\n";
    }
    if(!date_.empty())
    {
        response_ += "Date: ";
        response_ += date_ + "\r\n";
    }
    countLengthToString();
    response_ += content_length_string_;
    response_ += "\r\n";
    response_ += page_;
    parse_ = true;
    return response_;
}

void http_response::countLengthToString()
{
    content_length_string_.clear();
    content_length_string_ = "Content-Length: "; 
    content_length_string_ += to_string(page_.length());
    content_length_string_ += "\r\n\r\n";
}

