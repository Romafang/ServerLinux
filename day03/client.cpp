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
	errif(sockfd == -1, "socket����ʧ��");
	
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(8888);
	
	errif(connect(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) == -1, "socket����ʧ��");
	while (true)
	{
		char buffer[BUFFER_SIZE];
		bzero(&buffer, sizeof(buffer));
		std::cin >> buffer;
		ssize_t write_bytes = write(sockfd, buffer, sizeof(buffer));
		if (write_bytes == -1)
		{
			std::cout << "socket�Ѿ��Ͽ����ӣ������Լ���д��" << std::endl;
			break;
		}
		bzero(&buffer, sizeof(buffer));
		ssize_t read_bytes = read(sockfd, buffer, sizeof(buffer));
		if (read_bytes > 0)
		{
			std::cout << "�ӷ���˴�������Ϣ:" << buffer << std::endl;
		}
		else if (read_bytes == 0)
		{
			std::cout << "������Ѿ��Ͽ�����" << std::endl;
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