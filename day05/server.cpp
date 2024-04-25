#include <stdio.h>
#include<iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include "util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"
#define MAX_EVENTS 1024
#define READ_BUFFER 1024
void setnonblocking(int fd){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
void handleReadEvent(int);
int main()
{
    Socket *serv_sock=new Socket();
    InetAddress *serv_addr=new InetAddress("127.0.0.1",8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    Epoll *ep=new Epoll();
    serv_sock->setnonblocking();
    Channel *servChannel = new Channel(ep, serv_sock->getFd());
    servChannel->enableReading();
    while (true)
    {
        std::vector<Channel*>activeChannels=ep->poll();
        int nfds=activeChannels.size();
        for (int i = 0; i < nfds; i++)
        {
            int chfd=activeChannels[i]->getFd();
            if(chfd==serv_sock->getFd())
            {
                InetAddress*clnt_addr=new InetAddress();
                Socket *clnt_sock=new Socket(serv_sock->accept(clnt_addr));
                std::cout << "new client fd " << clnt_sock->getFd() << "! IP: " << inet_ntoa(clnt_addr->addr.sin_addr) << " Port: " << ntohl(clnt_addr->addr.sin_port) << std::endl;
                clnt_sock->setnonblocking();
                Channel *clntChannel = new Channel(ep, clnt_sock->getFd());
                clntChannel->enableReading();
            }
            else if(activeChannels[i]->getRevents()&EPOLLIN)
            {
                handleReadEvent(activeChannels[i]->getFd());
            }else{
                printf("somethings else happending\n");
            }
        }
        
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}
void handleReadEvent(int sockfd)
{
    char buffer[READ_BUFFER];
    while (true)
    {
        bzero(&buffer,sizeof(buffer));
        ssize_t bytes_read=read(sockfd,buffer,sizeof(buffer));
        if(bytes_read>0)
        {
            printf("message from client fd %d :%s \n",sockfd,buffer);
            write(sockfd,buffer,sizeof(buffer));
        }else if(bytes_read==-1&&errno==EINTR)
        {
            printf("continue reading!");
            continue;
        }else if(bytes_read==0)
        {
            printf("EOF ,client fd %d disconnection !\n",sockfd);
            close(sockfd);
            break;
        }
    }
    
}