#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<iostream>
#include"utils.h"
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	errif(sockfd == -1, "sockfd 创建失败");
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(8888);
	errif(connect(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr))==-1,"客户端连接服务端失败");
	while (true)
	{
		char buf[1024];
		std::cin >> buf;
		size_t write_size = write(sockfd, buf, sizeof(buf));
		if (write_size == -1) {
			std::cout << "socket已经取消连接，不可以继续写入" << std::endl;
		}
		bzero(&buf, sizeof(buf));
		size_t read_size = read(sockfd, buf, sizeof(buf));
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