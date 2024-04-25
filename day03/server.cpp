#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<errno.h>
#include"util.h"
#include<iostream>
#define MAX_EVENTS 1024
#define READ_BUFFER 1024
void setnonblocking(int fd)
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	errif(sockfd == -1, "socket创建失败");

	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(8888);

	errif(bind(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) == -1, "socket绑定失败");

	errif(listen(sockfd, SOMAXCONN) == -1, "socket监听失败");

	int epfd = epoll_create1(0);
	errif(epfd == -1, "epoll创建失败");

	struct epoll_event events[MAX_EVENTS], ev;
	bzero(&events, sizeof(events));

	bzero(&ev, sizeof(ev));

	ev.data.fd = sockfd;
	ev.events = EPOLLIN | EPOLLET;
	setnonblocking(sockfd);

	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

	while (true)
	{
		int nfds = epoll_wait(epfd, events, MAX_EVENTS,- 1);
		errif(nfds == -1, "epoll wait error");
		for (int i = 0; i < nfds; ++i)
		{
			if (events[i].data.fd == sockfd)
			{
				struct sockaddr_in clientaddr;
				bzero(&clientaddr, sizeof(clientaddr));
				socklen_t client_addr_len = sizeof(clientaddr);

				int client_sockfd = accept(sockfd, (sockaddr*)&clientaddr, &client_addr_len);
				errif(client_sockfd == -1, "socket accept error");
				std::cout << "new client fd " << client_sockfd << "! IP: " << inet_ntoa(clientaddr.sin_addr) << " Port: " << ntohl(clientaddr.sin_port) << std::endl;

				bzero(&ev, sizeof(ev));
				ev.data.fd = client_sockfd;
				ev.events = EPOLLIN | EPOLLET;
				setnonblocking(client_sockfd);
				epoll_ctl(epfd, EPOLL_CTL_ADD, client_sockfd, &ev);
			}
			else if (events[i].events & EPOLLIN)
			{
				char buffer[READ_BUFFER];
				while (true)
				{
					bzero(&buffer, sizeof(buffer));
					ssize_t bytes_read = read(events[i].data.fd, buffer, sizeof(buffer));
					if (bytes_read > 0)
					{
						std::cout << "message from client fd " << events[i].data.fd << ": " << buffer << std::endl;
						write(events[i].data.fd, buffer, sizeof(buffer));
					}else if (bytes_read == -1 && errno ==EINTR)
					{
						std::cout << "持续读取中" << std::endl;
						continue;
					}
					else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
					{
						std::cout << "已经完成读取一次 errno: " << errno << std::endl;
						break;
					}
					else if (bytes_read == 0)
					{
						std::cout << "EOF 客户端 :" << events[i].data.fd << "已经断开连接" << std::endl;
						close(events[i].data.fd);
						break;
					}
				}
			}
			else
			{
				std::cout << "其他事件发生" << std::endl;
			}
		}
	}
	close(sockfd);
	return 0;
}