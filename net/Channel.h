#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <sys/epoll.h>
#include <functional>
#include <sys/epoll.h>
#include "../base/Logging.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
class EventLoop;

typedef std::function<void()> CallBack;

class Channel
{
public:
	Channel(EventLoop *loop);
	Channel(EventLoop *loop,int fd);
	~Channel();
	int getFd();
	void setFd(int fd);

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


	std::string eventsToString(int ev)
	{
		std::ostringstream oss;
		oss << ": ";
		if (ev & EPOLLIN)
			oss << "IN ";
		if (ev & EPOLLPRI)
			oss << "PRI ";
		if (ev & EPOLLOUT)
			oss << "OUT ";
		if (ev & EPOLLHUP)
			oss << "HUP ";
		if (ev & EPOLLRDHUP)
			oss << "RDHUP ";
		if (ev & EPOLLERR)
			oss << "ERR ";


		return oss.str().c_str();
	}

	void handleEvents()
	{
		LOG <<"HANDLE EVENT";
		events_=0;
		//std::cout<<eventsToString(revents_)<<std::endl;
		LOG <<eventsToString(revents_);
		if((revents_&EPOLLRDHUP)&&(revents_&EPOLLIN))
		{
			LOG <<"CLOSE EVENT";
			events_=0;
			return;
		}
		if(revents_&EPOLLERR)
		{
			LOG <<"ERROR EVENT";
			if(errorHandler_)errorHandler_();
			events_=0;
			return ;
		}
		if(revents_&(EPOLLIN|EPOLLPRI|EPOLLRDHUP))
		{
			LOG <<"READ EVENT";
			handleRead();
		}
		if(revents_&EPOLLOUT)
		{
			LOG <<"WRITE EVENT";
			handleWrite();
		}
		events_=0;
		handleConn();
	}

	void handleRead();
	void handleWrite();
	void handleError(int fd,int err_num,std::string short_msg);
	void handleConn();

	void setRevents(__uint32_t revents)
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
	
	EventLoop *loop_;
	int fd_;
	__uint32_t events_;
	__uint32_t revents_;
	__uint32_t lastEvents_;


	int parse_URI();
	int parse_Headers();
	int analysisRequest();

	CallBack readHandler_;
	CallBack writeHandler_;
	CallBack errorHandler_;
	CallBack connHandler_;
};



