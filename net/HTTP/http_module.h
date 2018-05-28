#include "..../base/Logging.h"
#include "..../base/FixedBuffer.h"
#include <iostream>
#include <string>
using namespace std;



class http_module
{
public:
enum State
{

};
enum Method
{
    DEFUALT,GET,POST,HEAD,PUT,DELETE
};
enum Version
{
    HTTP_10,HTTP_11,HTTP_20
};

http_module():
    processSucceed(false),
    method_(DEFUALT)
{   }

bool setMethod(const char* begin,const char* end);
void setPath(const char* begin,const char* end)
{   path_.assign(begin,end);}

void setQuery(const char* begin,const char* end)
{   query_.assign(begin,end);}

void http_module::setBody(const char* begin,const char* end)
{
    body_.assign(begin,end);
}
void setVersion(Version v)
{   version_ = v;}
bool processRequestLine(const char* begin, const char* end);


bool parseRequest();

void showDetail()
{
    cout<<"Version:"<<version_<<endl;
    cout<<"Method:"<<method_<<endl;
    cout<<"Path:"<<path_<<endl;
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
}