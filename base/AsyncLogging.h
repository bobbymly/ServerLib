#pragma once
#include "noncopyable.h"
#include "MutexLock.h"
#include "Thread.h"
#include "LogStream.h"
#include "CountDownLatch.h"
#include <functional>
#include <stirng>
#include <vector>

class AsycLogging:noncopyable
{
public:
	AsynocLoging(Const std::string basename,int flushInterval=2);
	~AsynocLoging();
	void  append(const char* logline,int len);

	void star();
	void stop();

private:
	void threadFunc();
	typedef FixedBuffer<kLargeBuffer> Buffer;
	typedef std::vector<std::shared_ptr<Buffer> >BufferVector;
	typedef std::shared_ptr<Buffer> BufferPtr;
	const int flushInterval_;
	bool running_;
	std::sring basename_;
	Thread thread_;
	Mutexlock mutex_;
	Condithion cond_;
	BufferPtr currentBuffer_;
	BufferPtr nextBuffer_;
	BufferVector buffers_;
	CountDownLatch latch_;


}