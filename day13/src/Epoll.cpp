#include"Epoll.h"
#include"Channel.h"
#include"util.h"
#include<string.h>
#include<unistd.h>
using std::vector;
#define MAX_EVENTS 1024
Epoll::Epoll():epfd(-1),events(nullptr)
{
    epfd=epoll_create1(0);
    errif(epfd==-1,"epoll创建失败");
    events=new epoll_event[MAX_EVENTS];
    bzero(events,sizeof(*events)*MAX_EVENTS);
}
Epoll::~Epoll()
{
    if(epfd!=-1)
    {
        close(epfd);
        epfd=-1;
    }
    delete []events;
}
vector<Channel*>Epoll::poll(int timeout)
{
    vector<Channel*>activeChannels;
    int nfds=epoll_wait(epfd,events,MAX_EVENTS,timeout);
    //epoll将事件放进内核区，将就绪的事件的序列返回
    errif(nfds==-1,"epoll等待失败");
    for (size_t i = 0; i < nfds; i++)
    {
        Channel*ch=(Channel*)events[i].data.ptr;
        ch->setReady(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
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
        errif(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev)==-1,"epoll添加失败");
        channel->setInEpoll();
    }
    else{
        errif(epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev)==-1,"epoll更改失败");
    }
}
void Epoll::deleteChannel(Channel*channel)
{
    int fd=channel->getFd();
    errif(epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL)==-1,"epoll删除失败");
    channel->setInEpoll(false);
}