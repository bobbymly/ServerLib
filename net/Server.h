#pragma once
#include "EventLoop.h"
#include "Channel.h"
#include "EventLoopThreadPool.h"
#include <memory>

typedef std::function<void(shared_ptr<Channel>)> CallBackWithCh;

class Server
{
public:
	Server(EventLoop *loop,int threadNum,int port);
	~Server(){	}

	EventLoop *getLoop()const{	return loop_;}
	void start();
	void handNewConn();
	void handThisConn(){	loop_->updatePoller(acceptChannel_);}

	void setReadCallback(CallBackWithCh & cb )
	{
		readCallback_ = cb;
	}
	//void setCloseCallback();
	void setWriteCallback(CallBackWithCh & cb )
	{
		writeCallback_ = cb;
	}
	
	void setErrorCallback(CallBackWithCh & cb )
	{
		errorCallback_ = cb;
	}
private:
	CallBackWithCh readCallback_;
	CallBackWithCh writeCallback_;
	CallBackWithCh errorCallback_;
	//Callback closeCallback_; 


	EventLoop *loop_;
	int threadNum_;
	std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
	bool started_;
	std::shared_ptr<Channel> acceptChannel_;
	int port_;
	int listenFd_;
	static const int MAXFDS=100000;


};