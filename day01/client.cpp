#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(8888);
	connect(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
	return 0;
}