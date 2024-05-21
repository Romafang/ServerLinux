#include"Connection.h"
#include<unistd.h>
#include<cassert>
#include<cstring>
#include<iostream>
#include<utility>
#include"Buffer.h"
#include"Channel.h"
#include"Socket.h"
#include"util.h"
Connection::Connection(EventLoop*loop,Socket *sock):
loop_(loop),sock_(sock){
    if (loop_!=nullptr)
    {
        channel_=new Channel(loop_,sock->GetFd());
        channel_->EnableRead();
        channel_->UseET();
    }
    read_buffer_=new Buffer();
    send_buffer_=new Buffer();
    state_=State::Connected;
    
}
Connection::~Connection()
{
    if (loop_!=nullptr)
    {
        delete channel_;
    }
    delete sock_;
    delete read_buffer_;
    delete send_buffer_;
    
}
void Connection::Read()
{
    ASSERT(state_==State::Connected,"连接状态已经断开连接");
    read_buffer_->Clear();
    if (sock_->IsNonBlocking())
    {
        ReadNonBlocking();
    }
    else{
        ReadBlocking();
    }
    
}
void Connection::Write()
{
    ASSERT(state_==State::Connected,"连接状态已经断开连接");
    if (sock_->IsNonBlocking())
    {
        WriteNonBlocking();
    }
    else{
        WriteBlocking();
    }
    send_buffer_->Clear();
}
void Connection::ReadNonBlocking()
{
    int sockfd=sock_->GetFd();
    char buf[1024];//一次读取大小
    while (true)
    {
        memset(buf,0,sizeof(buf));
        ssize_t bytes_read=read(sockfd,buf,sizeof(buf));
        if (bytes_read>0)
        {
            read_buffer_->Append(buf,bytes_read);
        }else if(bytes_read==-1&&errno==EINTR)
        {
            std::cout<<"继续读取..."<<std::endl;
            continue;
        }
        else if (bytes_read==-1&&((errno==EAGAIN)||(errno==EWOULDBLOCK)))
        {
            break;
        }
        else if (bytes_read==0)
        {
            std::cout<<"读取结束，客户端:"<<sockfd<<"断开连接"<<std::endl;
            state_=State::Closed;
            break;
        }
        else{
            std::cout<<"其他错误在客户端:"<<sockfd<<std::endl;
            state_=State::Closed;
            break;
        }
    }
    
}
void Connection::WriteNonBlocking()
{
    int sockfd=sock_->GetFd();
    char buf[send_buffer_->Size()];
    memcpy(buf,send_buffer_->ToStr(),send_buffer_->Size());
    int data_size=send_buffer_->Size();
    int data_left=data_size;
    while (true)
    {
        ssize_t bytes_write=write(sockfd,buf+data_size-data_left,data_left);
        if (bytes_write==-1&&errno==EINTR)
        {
            std::cout<<"继续写入"<<std::endl;
            continue;
        }
        if (bytes_write==-1&&errno==EAGAIN)
        {
            break;
        }
        if (bytes_write==-1)
        {
            std::cout<<"其他错误发生在客户端："<<sockfd<<"上..."<<std::endl;
            state_=State::Closed;
            break;
        }
        data_left-=bytes_write;
        
        
    }
    
}
void Connection::ReadBlocking()
{
    int sockfd=sock_->GetFd();
    unsigned int rcv_size=0;
    socklen_t len=sizeof(rcv_size);
    getsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,&rcv_size,&len);
    char buf[rcv_size];
    ssize_t bytes_read=read(sockfd,buf,sizeof(buf));
    if (bytes_read>0)
    {
        read_buffer_->Append(buf,bytes_read);
    }
    else if (bytes_read==0)
    {
        std::cout<<"读取结束，客户端："<<sockfd<<"已经断开"<<std::endl;
        state_=State::Closed;
    }
    else if (bytes_read==-1)
    {
        std::cout<<"其他错误发生在客户端："<<sockfd<<"上..."<<std::endl;
        state_=State::Closed;
    }
}
void Connection::WriteBlocking()
{
    int sockfd=sock_->GetFd();
    ssize_t bytes_write=write(sockfd,send_buffer_->ToStr(),send_buffer_->Size());
    if (bytes_write==-1)
    {
        std::cout<<"其他错误发生在客户端："<<sockfd<<"上..."<<std::endl;
        state_=State::Closed;
    }
    
}
void Connection::Close(){delete_connectioin_callback_(sock_);}
Connection::State Connection::GetState(){return state_;}
void Connection::SetSendBuffer(const char *str){send_buffer_->SetBuf(str);}
Buffer *Connection::GetReadBuffer(){return read_buffer_;}
Buffer* Connection::GetSendBuffer(){return send_buffer_;}
const char *Connection::ReadBuffer(){return read_buffer_->ToStr();}
const char *Connection::SendBuffer(){return send_buffer_->ToStr();}
void Connection::SetDeleteConnectionCallback(std::function<void(Socket*)>const &callback)
{
    delete_connectioin_callback_=callback;
}
void Connection::SetOnConnectionCallback(std::function<void(Connection*)>const &callback)
{
    on_connect_callback_=callback;
    channel_->SetReadCallback([this](){on_connect_callback_(this);});
}
void Connection::GetlineSendBuffer(){send_buffer_->Getline();}
Socket *Connection::GetSocket(){return sock_;}