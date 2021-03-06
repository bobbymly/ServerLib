#include "Server.h"
#include "../base/Logging.h"
#include "Util.h"
#include <functional>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
using namespace std;

Server::Server(EventLoop *loop,int threadNum,int port):
	loop_(loop),
	threadNum_(threadNum),
	eventLoopThreadPool_(new EventLoopThreadPool(loop_,threadNum)),
	started_(false),
	acceptChannel_(new Channel(loop_)),
	port_(port),
	listenFd_(socket_bind_listen(port_))
{
	LOG << "New Server";
	acceptChannel_->setFd(listenFd_);
	handle_for_sigpipe();
	if(setSocketNonBlocking(listenFd_)<0)
	{
		perror("set socket non block failed");
		abort();
	}
}

void Server::start()
{
	eventLoopThreadPool_->start();
	acceptChannel_->setEvents(EPOLLIN|EPOLLET);
	acceptChannel_->setReadHandler(bind(&Server::handNewConn,this));
	acceptChannel_->setConnHandler(bind(&Server::handThisConn,this));
	loop_->addToPoller(acceptChannel_,0);
	started_=true;
}

void addChToLoop(shared_ptr<Channel> ch,EventLoop* loop)
{
	ch->setEvents(EPOLLIN | EPOLLET | EPOLLONESHOT| EPOLLHUP|EPOLLRDHUP);
	loop -> addToPoller(ch);
}

void Server::handNewConn()
{
	struct sockaddr_in client_addr;
	memset(&client_addr,0,sizeof(struct sockaddr_in));
	socklen_t client_addr_len=sizeof(client_addr);
	int accept_fd=0;
	while((accept_fd=accept(listenFd_,(struct sockaddr*)&client_addr,&client_addr_len))>0)
	{
		EventLoop *loop=eventLoopThreadPool_->getNextLoop();
		LOG<<"New connection from "<<inet_ntoa(client_addr.sin_addr)<<":"<<ntohs(client_addr.sin_port);
		
		if (accept_fd >= MAXFDS)
        {
            close(accept_fd);
            continue;
        }

		if(setSocketNonBlocking(accept_fd)<0)
		{
			LOG<<"Set non block failed!";
			return;
		}
		//  char temp_buf[255]="hello";
		//  write(accept_fd,temp_buf,sizeof temp_buf);
		setSocketNodelay(accept_fd);



		shared_ptr<Channel> ch(new Channel(loop,accept_fd));
	//	ch->setEvents(EPOLLIN|EPOLLHUP|EPOLLRDHUP);
		//ch->setReadHandler(std::bind(&Channel::readAll,ch));
		weak_ptr<Channel> weakCh = ch;
		ch->setReadHandler(std::bind(readCallback_,weakCh));
		ch->setWriteHandler(std::bind(writeCallback_,weakCh));
		ch->setErrorHandler(std::bind(errorCallback_,weakCh));
		//ch->setCloseHandler(std::bind(closeCallback_,ch));
		ch->setEvents(EPOLLIN | EPOLLET | EPOLLHUP|EPOLLRDHUP);

		//loop->queueInLoop(std::bind(addChToLoop,ch,loop));
		//loop->addToPoller(ch);
		loop->addToPoller(ch);
	}
	acceptChannel_->setEvents(EPOLLIN|EPOLLET);
}
