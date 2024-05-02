#pragma once
#include<functional>
class Channel;
class EventLoop;
class Socket;
class Buffer;
class Connection
{
private:
    EventLoop *loop;
    Socket*sock;
    Channel* channel;
    std::function<void(Socket*)>deleteConnectionCallback;
    std::string *inbuffer;
    Buffer *readbuffer;
public:
    Connection(EventLoop* _loop,Socket *_sock);
    ~Connection();
    void echo(int _sock);
    void setDeleteConnectionCallback(std::function<void(Socket*)>_cb);
};

