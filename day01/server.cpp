#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct  sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(8888);
	bind(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
	listen(sockfd, SOMAXCONN);
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len = sizeof(clientaddr);
	bzero(&clientaddr, sizeof(clientaddr));
	int client_sockfd = accept(sockfd, (sockaddr*)&clientaddr, &clientaddr_len);
	std::cout << "new client fd " << client_sockfd << "! IP:" << inet_ntoa(clientaddr.sin_addr) << " Port:" << ntohs(clientaddr.sin_port);
	return 0;
}