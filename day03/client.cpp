#include<iostream>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include"util.h"
#define BUFFER_SIZE 1024
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	errif(sockfd == -1, "socket创建失败");
	
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(8888);
	
	errif(connect(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) == -1, "socket连接失败");
	while (true)
	{
		char buffer[BUFFER_SIZE];
		bzero(&buffer, sizeof(buffer));
		std::cin >> buffer;
		ssize_t write_bytes = write(sockfd, buffer, sizeof(buffer));
		if (write_bytes == -1)
		{
			std::cout << "socket已经断开连接，不可以继续写入" << std::endl;
			break;
		}
		bzero(&buffer, sizeof(buffer));
		ssize_t read_bytes = read(sockfd, buffer, sizeof(buffer));
		if (read_bytes > 0)
		{
			std::cout << "从服务端传来的信息:" << buffer << std::endl;
		}
		else if (read_bytes == 0)
		{
			std::cout << "服务端已经断开连接" << std::endl;
			break;
		}
		else if (read_bytes == -1)
		{
			close(sockfd);
			errif(true, "socket read error");
		}
	}
	close(sockfd);
	return 0;
}