#include"Channel.h"
#include"EventLoop.h"
#include"Connection.h"
#include"InetAddress.h"
#include"Buffer.h"
#include"Socket.h"
#include<unistd.h>
#include<string>
#include<string.h>
#include"util.h"
#include<iostream>
#define READ_BUFFER 1024
Connection::Connection(EventLoop* _loop,Socket *_sock):loop(_loop),sock(_sock),channel(nullptr),inbuffer(new std::string()),readbuffer(nullptr)
{
    channel=new Channel(loop,sock->getFd());
    std::function<void()>cb=std::bind(&Connection::echo,this,sock->getFd());
    channel->setCallback(cb);
    channel->enableReading();
    readbuffer =new Buffer();
}

Connection::~Connection()
{
    delete sock;
    delete channel;
}
void Connection::setDeleteConnectionCallback(std::function<void(Socket*)>_cb)
{
    deleteConnectionCallback=_cb;
}
void Connection::echo(int _sock)
{
    char buf[READ_BUFFER];
    while (true)
    {
        bzero(&buf,sizeof(buf));
        ssize_t bytes_read=read(_sock,buf,sizeof(buf));
        if(bytes_read>0)
        {

            readbuffer->append(buf,bytes_read);
        }
        else if(bytes_read==-1&&errno==EINTR) //客户端正常中断、继续读取
        {
            std::cout<<"持续读取中"<<std::endl;
            continue;
        }
        else if(bytes_read==-1&&((errno==EAGAIN)||(errno==EWOULDBLOCK)))//非阻塞IO，这个条件表示数据全部读取完毕
        {
            std::cout<<"已经完成读取了,errno:"<<errno<<std::endl;
            std::cout<<"消息来自客户端"<<_sock<<": "<<readbuffer->c_str()<<std::endl;
            errif(write(_sock,readbuffer->c_str(),readbuffer->size())==-1,"写入传输数据失败");
            readbuffer->clear();
            break;
        }
        else if(bytes_read==0)//断开连接
        {
            std::cout<<"EOF,客户端 fd "<<_sock<<" 已经断开连接"<<std::endl;
            deleteConnectionCallback(sock);
            break;
        }
    }
    
}