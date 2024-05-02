#include"Epoll.h"
#include"Channel.h"
#include<unistd.h>
#include<string.h>
#include"util.h"
#define MAX_EVENTS 1024
Epoll::Epoll():epfd(-1),event(nullptr)
{
    epfd=epoll_create1(0);
    errif(epfd==-1,"epoll创建失败");
    event=new epoll_event[MAX_EVENTS];
    bzero(event,sizeof(epoll_event)*MAX_EVENTS);
}
Epoll::~Epoll(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete [] event;
}
void Epoll::updateChannel(Channel*channel)
{
    int fd=channel->getFd();
    struct epoll_event ev;
    bzero(&ev,sizeof(ev));
    ev.data.ptr=channel;
    ev.events=channel->getEvents();
    if(!channel->getInEpoll())
    {
        errif(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev)==-1,"epoll添加事件失败");
        channel->setInEpoll();
    }
    else{
        errif(epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev)==-1,"epoll更新事件失败");
    }
}
std::vector<Channel*> Epoll::poll(int timeout)
{
    std::vector<Channel*>activateChannel;
    int nfds=epoll_wait(epfd,event,MAX_EVENTS,timeout);
    errif(nfds==-1,"epoll等待失败");
    for(int i=0;i<nfds;i++)
    {
        Channel*ch=(Channel*)event[i].data.ptr;
        ch->setRevents(event[i].events);
        activateChannel.emplace_back(ch);
    }
    return activateChannel;
}