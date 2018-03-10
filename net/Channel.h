#pragma once
#include "Timer.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <sys/epoll.h>
#include <functional>
#include <sys/epoll.h>

class EventLoop;
class HttpData;



class Channel
{
public:
	Channel(EventLoop *loop);
	Channel(EventLoop *loop,int fd);
	~Channel();
	int getFd();
	void setFd(int fd);

	void setHolder(std::shared_ptr<HttpData> holder)
	{
		holder_=holder;
	}
	std::shared_ptr<HttpData> getHolder()
	{
		std::shared_ptr<HttpData> ret(holder_.lock());
		return ret;
	}

	void setReadHandler(CallBack &&readHandler)
	{
		readHandler_=readHandler;
	}
	void setWriteHandler(CallBack &&writeHandler)
	{
		writeHandler_=writeHandler_;
	}
	void setErrorHandler(CallBack &&errorHandler)
	{
		errorHandler_=errorHandler;
	}
	void setConnHandler(CallBack &&connHandler)
	{
		connHandler_=connHandler;
	}

	void handleEvents()
	{
		events_=0;
		if((revents_&EPOLLHUP)&&!(revents_&EPOLLIN))
		{
			events_=0;
			return;
		}
		if(revents_&EPOLLERR)
		{
			if(errorHandler_)errorHandler_();
			events_=0;
			return ;
		}
		if(revents_&(EPOLLIN|EPOLLPRI|EPOLLRDHUP))
		{
			handleRead();
		}
		if(revents_&EPOLLOUT)
		{
			handleWrite();
		}
		handleConn();
	}

	void handleRead();
	void handleWrite();
	void handleError(int fd,int err_num,std::string short_msg);
	void handleConn();

	void setRevents(__unit32_t revents)
	{
		revents_=revents;
	}
	void setEvents(__uint32_t events)
	{
		events_=events;
	}

	__uint32_t& getEvents()
	{
		return events_;
	}
	bool EqualAndUpdateLastEvents()
	{
		bool ret=(lastEvents_==events_);
		lastEvents_=events_;
		return ret;
	}
	__uint32_t getLastEvents()
	{
		return lastEvents_;
	}


private:
	typedef std::function<void()> CallBack;
	EventLoop *loop_;
	int fd_;
	__unit32_t events_;
	__unit32_t revents_;
	__unit32_t lastEvents_;

	std::weak_ptr<HttpData> holder_;

	int parse_URI();
	int parse_Headers();
	int analysisRequest();

	CallBack readHandler_;
	CallBack writeHandler_;
	CallBack errorHandler_;
	CallBack connHandler_;
}



typedef std::shared_ptr<Channel> SP_Channel;