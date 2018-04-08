#include "InetAddress.h"
#include <memory>
#include <netinet/tcp.h>
#include "EventLoop.h"

struct tcp_info;
class Channel;
class EventLoop;
class Socket;

class TcpConnection:noncopyable,public std::enable_shared_from_this<TcpConnection>
{
public:







private:
    EventLoop* loop_;
    const string name_;
    bool reading_;
    std::unique_ptr<Socket>socket_;
    std::unique_ptr<Channel>channel_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    







};