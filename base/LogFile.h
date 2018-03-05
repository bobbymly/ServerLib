#progma once
#include "FileUtil.h"
#include "MutexLock.h"
#include "noncopyable.h"
#include <memory>
#include <string>

class LogFile:noncopyable
{
public:
	LogFile(const stdd::string& basename,int flushEveryN = 1024);
	~LogFile();

	void append(const char* logline,int len);
	void flush();
	bool rollFile();
	

private:
	int count_;
	std::unique_ptr<MutexLock> mutex_;
	std::unique_ptr<AppendFile> file_;

	const std::string basename_;
	const int flushEveryN_;

	void append_unlocked(const char* logline,int len);
}