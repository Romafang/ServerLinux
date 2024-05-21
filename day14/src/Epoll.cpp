#include "Epoll.h"

#include <unistd.h>

#include <cstring>

#include "Channel.h"
#include "util.h"

#define MAX_EVENTS 1000
Epoll::Epoll()
{
    epfd_=epoll_create1(0);
    errif(epfd_==-1,"epoll创建失败");
    events_=new epoll_event[MAX_EVENTS];
    memset(events_,0,sizeof(*events_)*MAX_EVENTS);
}
Epoll::~Epoll()
{
    if (epfd_!=-1)
    {
        close(epfd_);
        epfd_=-1;
    }
    delete [] events_;
}
std::vector<Channel*>Epoll::Poll(int timeout)
{
    std::vector<Channel*>active_channels;
    int nfds=epoll_wait(epfd_,events_,MAX_EVENTS,timeout);
    errif(nfds==-1,"epoll等待事件失败");
    for (int i = 0; i < nfds; i++)
    {
        Channel* ch=(Channel*)events_[i].data.ptr;
        ch->SetReadyEvents(events_[i].events);
        active_channels.push_back(ch);
    }
    return active_channels;
    
}
void Epoll::UpdateChannel(Channel*ch)
{
    int fd=ch->GetFd();
    struct epoll_event ev{};
    ev.data.ptr=ch;
    ev.events=ch->GetListenEvents();
    if (!ch->GetInEpoll())
    {
        errif(epoll_ctl(epfd_,EPOLL_CTL_ADD,fd,&ev)==-1,"epoll事件添加失败");
        ch->SetInEpoll();
    }
    else{
        errif(epoll_ctl(epfd_,EPOLL_CTL_MOD,fd,&ev)==-1,"epoll事件修改失败");
    }
    
}
void Epoll::DeleteChannel(Channel*ch)
{
    int fd=ch->GetFd();
    errif(epoll_ctl(epfd_,EPOLL_CTL_DEL,fd,nullptr)==-1,"epoll事件删除失败");
    ch->SetInEpoll(false);
}