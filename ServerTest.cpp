#include "net/Server.h"
#include "net/EventLoop.h"
#include "base/Logging.h"
int main()
{
    EventLoop loop_;
    LOG <<"begin";
    Server myServer_(&loop_,4,8011);
    myServer_.start();
    loop_.loop();
    sleep(30);













    return 0;
}