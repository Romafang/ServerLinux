#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include<iostream>
#include"utils.h"
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	errif(sockfd == -1, "sockfd ����ʧ��");
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(8888);
	errif(connect(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr))==-1,"�ͻ������ӷ����ʧ��");
	while (true)
	{
		char buf[1024];
		std::cin >> buf;
		size_t write_size = write(sockfd, buf, sizeof(buf));
		if (write_size == -1) {
			std::cout << "socket�Ѿ�ȡ�����ӣ������Լ���д��" << std::endl;
		}
		bzero(&buf, sizeof(buf));
		size_t read_size = read(sockfd, buf, sizeof(buf));
		if (read_size > 0)
		{
			std::cout << "message from client fd "  << " : " << buf << std::endl;
		}
		else if (read_size == 0)
		{
			std::cout  << "�ͻ���û������" << std::endl;
			
			break;
		}
		else if (read_size == -1)
		{
			errif(true, "��ȡʧ��");
		}
	}
	close(sockfd);
	return 0;
}