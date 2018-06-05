#include "Channel.h"
//#include "Util.h"
#include "Epoll.h"
#include "EventLoop.h"
#include <unistd.h>
#include <queue>
#include <cstdlib>
#include <iostream>
using namespace std;

Channel::Channel(EventLoop *loop):
	loop_(loop),
	events_(0),
	lastEvents_(0)
{ }

Channel::Channel(EventLoop *loop,int fd):
	loop_(loop),
	fd_(fd),
	events_(0),
	lastEvents_(0)
{ }

Channel::~Channel()
{
	close(fd_);
	//cout<<"~Channel()"<<endl;
}

void Channel::closeChannel()
{
	std::shared_ptr<Channel> ch = shared_from_this();
	//cout<<"count = "<<ch.use_count()<<endl;
	loop_ -> removeFromPoller(ch);
	//close(fd_);
}

int Channel::getFd()
{
	return fd_;
}

void Channel::setFd(int fd)
{
	fd_=fd;
}

void Channel::handleRead()
{
	if(readHandler_)
	{
		readHandler_();
	}
}

void Channel::handleWrite()
{
	if(writeHandler_)
	{
		writeHandler_();
	}
}

void Channel::handleConn()
{
	if(connHandler_)
	{
		connHandler_();
	}
}

void Channel::handleClose()
{
	if(closeHandler_)
	{
		closeHandler_();
	}
}
