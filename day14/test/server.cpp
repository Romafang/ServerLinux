#include"Server.h"
#include<iostream>
#include"Buffer.h"
#include"Connection.h"
#include"EventLoop.h"
#include"Socket.h"
int main()
{
    EventLoop *loop=new EventLoop();
    Server *server=new Server(loop);
    server->OnConnect([](Connection*conn){
        conn->Read();
        if (conn->GetState()==Connection::State::Closed)
        {
            conn->Close();
            return;
        }
        std::cout<<"来自客户端："<<conn->GetSocket()->GetFd()<<"的消息："<<conn->ReadBuffer()<<std::endl;
        conn->SetSendBuffer(conn->ReadBuffer());
        conn->Write();
    });
    loop->Loop();
    delete server;
    delete loop;
    return 0;
}