#include<iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	errif(sockfd == -1, "sockfd 创建失败");
	struct  sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(8888);
	errif(bind(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr))==-1,"socket bind绑定失败");
	errif(listen(sockfd, SOMAXCONN)==-1,"socket 监听失败");
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len = sizeof(clientaddr);
	bzero(&clientaddr, sizeof(clientaddr));
	int client_sockfd = accept(sockfd, (sockaddr*)&clientaddr, &clientaddr_len);
	errif(client_sockfd == -1, "服务端获取信息失败");
	std::cout << "new client fd " << client_sockfd << "! IP:" << inet_ntoa(clientaddr.sin_addr) << " Port:" << ntohs(clientaddr.sin_port);
	while (true)
	{
		char buf[1024];
		bzero(&buf, sizeof(buf));
		size_t read_size = read(client_sockfd, buf, sizeof(buf));
		if (read_size > 0)
		{
			std::cout << "message from client fd " << client_sockfd << " : " << buf << std::endl;
			write(client_sockfd, buf, sizeof(buf));
		}
		else if (read_size == 0)
		{
			std::cout << client_sockfd << "客户端没有连接" << std::endl;
			close(client_sockfd);
			break;
		}
		else if (read_size == -1)
		{
			close(client_sockfd);
			errif(true, "读取失败");
		}
	}
	close(sockfd);
	return 0;
}