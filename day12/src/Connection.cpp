#include"Connection.h"
#include"Buffer.h"
#include"Channel.h"
#include"Socket.h"
#include"util.h"
#include<unistd.h>
#include<iostream>
#include<string.h>
using std::cout;
using std::endl;
using std::function;

Connection::Connection(EventLoop* _loop,Socket*_sock):
loop(_loop),sock(_sock),channel(nullptr),readBuffer(nullptr)
{
    channel=new Channel(loop,sock->getFd());
    channel->enableRead();
    channel->useET();
    function<void()>cd=std::bind(&Connection::echo,this,sock->getFd());
    channel->setReadCallback(cd);
    readBuffer=new Buffer();
}
Connection::~Connection()
{
    delete readBuffer;
    delete sock;
    delete channel;
}
void Connection::setDeleteConnectionCallback(function<void(int)>_cb)
{
    deleteConnectionCallback=_cb;
}
void Connection::echo(int sockfd)
{
    char buf[1024];//用来读取数据的一次最大字节，可以反复读取加入buffer类，大小无所谓
    while (true)
    {
        bzero(&buf,sizeof(buf));//循环读取需要将buf重置
        ssize_t bytes_read=read(sockfd,buf,sizeof(buf));
        if (bytes_read>0)
        {
            readBuffer->append(buf,bytes_read);
        }
        else if (bytes_read==-1&&errno==EINTR)
        {
            cout<<"继续读取中...."<<endl;
            continue;
        }
        else if (bytes_read==-1&&((errno==EAGAIN)||(errno==EWOULDBLOCK)))
        {
            cout<<"来自客户端"<<sockfd<<"的消息: "<<readBuffer->c_str()<<endl;
            send(sockfd);
            readBuffer->clear();
            break;
        }
        else if (bytes_read==0)
        {
            cout<<"错误，客户端"<<sockfd<<"断开连接..."<<endl;
            deleteConnectionCallback(sockfd);
            break;
        }
        else{
            cout<<"连接已经被重新设置"<<endl;
            deleteConnectionCallback(sockfd);
            break;
        }
    }
    
}
void Connection::send(int sockfd)
{
    char buf[readBuffer->size()];
    strcpy(buf,readBuffer->c_str());
    int data_size=readBuffer->size();
    int data_left=data_size;
    while (data_left>0)
    {
        ssize_t byte_write=write(sockfd,buf+data_size-data_left,data_left);
        if (byte_write==-1&&errno==EAGAIN)
        {
            break;
        }
        data_left-=byte_write;   
    }
}