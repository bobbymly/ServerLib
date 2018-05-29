#include "../../base/Logging.h"
//#include "../../base/FixedBuffer.h"
#include <iostream>
#include <string>
#include <map>
using namespace std;



class http_module
{
public:
enum State
{

};
enum Method
{
    DEFAULT,GET,POST,HEAD,PUT,DELETE
};
enum Version
{
    HTTP_10,HTTP_11,HTTP_20
};

string MethodToString()
{
    switch(method_)
    {
        case 1: return "GET";
        case 2: return "POST";
        case 3: return "HEAD";
        case 4: return "PUT";
        case 5: return "DELETE";
        default: return "DEFAULT";
    }
}

string VersionToString()
{
    switch(version_)
    {
        case 0: return "HTTP/1.0";
        case 1: return "HTTP/1.1";
        case 2: return "HTTP/2.0";
    }
}
http_module(string buf):
    buf_(buf),
    processSucceed(false),
    method_(DEFAULT)
{   this->parseRequest();}

bool setMethod(const string& str);
void setPath(const string& str)
{   path_ = str;}

void setQuery(const string& str)
{   query_ = str;}

void setBody(const string& str)
{   body_ = str;}
void setVersion(Version v)
{   version_ = v;}
bool processRequestLine(const string& str);
bool parseHeaders(const string& str);
bool parseArgs(const string& str);
bool parseRequest();

void showDetail()
{
    cout<<"Version:"<<VersionToString()<<endl;
    cout<<"Method:"<<MethodToString()<<endl;
    cout<<"Path:"<<path_<<endl;
    cout<<"Args:"<<endl;
    for(auto it = args_.begin();it != args_.end();++it)
    {
        cout<<it->first<<":"<<it->second<<end;
    }
    
    
    cout<<"Headers:"<<endl;

    for(auto it = headers_.begin();it != headers_.end();it ++)
    {
        cout<<it->first<<":"<<it->second<<endl;
    }
    cout<<"Body:"<<endl;
    cout<<body_<<endl;
}




private:
    //typedef FixedBuffer<1024*1024> Buffer;
    string buf_;
    State state_;
    Method method_;
    Version version_;
    bool processSucceed;
    string path_;
    string query_;
    string body_;
    map <string,string> headers_;
    map <string,string> args_;
};