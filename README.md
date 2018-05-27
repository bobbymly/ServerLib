# ServerLib
C++ Library for server
## 项目简介
用 C++ 编写的 Linux 平台上的轻量级多线程网络库
* 采用 Reactor 模型
* 多线程提高并发性，使用线程池避免线程频繁创建销毁的开销
* 使用 epoll 多路复用
* 实现高效的多线程异步日志降低日志I/O开销
## 项目结构
![](https://github.com/bobbymly/Serverlib/raw/master/pic/show.png)

## fd 管理
* Channel 类： 负责一个 fd 的事件，包含 fd 和 fd 事件的各种回调函数
* EventLoop 类： 每一个线程包含一个 EventLoop 对象，对 IO 线程（即 SubReactor）而言，每个EventLoop包含一个 Epoll ,管理多个 fd,即关联多个Channel,作为一个时间循环处理Epoll中的活跃事件，EventLoop 的核心一个 while 循环,循环处理事件。

## 多线程管理
* EventLoopThread : 包含一个 EventLoop , 封装了其在线程池中的创建销毁同步等线程相关操作。
* EventLoopThreadPool : 线程池，负责创建和维护线程池中的线程，向线程池中的线程分配任务。


 ## 基础设施
 * 互斥器 ：实现了用 RAII 手法封装的 MutexLock 和 MutexLockGuard
 * 条件变量 :封装了Condition 和 CountDownLatch 用以保证线程间同步
 * Epoll ：对 epoll 的封装
 
 ## 网络
 * Server ：用于编写网络服务器，接受客户连接，封装繁杂的函数
 * Util ： 包含对的 socket 的读写和设置等操作。 
 

## Log 实现
  详细设计：https://github.com/bobbymly/uy_log
  
  由于在多线程环境下频繁的文件读写会造成很大的性能损失，故采用双缓冲区设计。
  
  预先准备两个缓冲区 A B ,由各个产生 Log 信息的线程 向 A 缓冲区写入， 当 A 写满后 交换 A B 缓冲区(使用 std::move)移动语义避免内存拷贝造成的性能损失。而由 Log 线程向 Log 文件中写入 Log 信息。这样，零散而频繁的文件读写，变成了先收集齐一定的量的Log信息在一次性写入。这样也就只有一个线程需要向文件进行写操作，既降低了操作频率，同时也可以使用无锁操作写文件，提高了效率。
  
  同时，为了避免发生意外时 Log 信息的丢失，除了写满一块缓冲区时需要向文件写入一次外，每隔一段时间（无论 缓冲区 A 是否写满），都应交换缓冲区并写入一次Log.
  
  Log 的实现包括许多文件层层递进
  * FileUtil 包含了Log文件的 打开、关闭、写入。
  * LogFile  封装了FileUtil并设置了一个循环次数，每添加这么多次 Log 信息就向文件中写一次
  * AsyncLogging 负责启动一个LOG线程，应用双缓冲区技术
  * LogStream 重载各种<<运算符
  * Logging 对外的接口，内涵一个LogStream对象，在每次打Log时为其添加 文件名 和 行数 等格式化信息。
  
