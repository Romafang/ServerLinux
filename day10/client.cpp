#include <iostream>
#include <unistd.h>
#include <string.h>
#include "src/util.h"
#include "src/Buffer.h"
#include "src/InetAddress.h"
#include "src/Socket.h"
using namespace std;
int main() {
    Socket*sock=new Socket();
    InetAddress*addr=new InetAddress("127.0.0.1",1234);
    sock->connect(addr);
    int sockfd=sock->getFd();
    Buffer *sentBuffer=new Buffer();
    Buffer *readBuffer=new Buffer();
    while (true)    
    {
        sentBuffer->getline();
        ssize_t write_bytes=write(sockfd,sentBuffer->c_str(),sentBuffer->size());
        if(write_bytes==-1)
        {
            cout<<"连接已断开，不可以继续写入"<<endl;
            break;
        }
        int readed=0;
        char buf[1024];
        while (true)
        {
            bzero(&buf,sizeof(buf));
            ssize_t read_bytes=read(sockfd,buf,sizeof(buf));
            if(read_bytes>0)
            {
                readBuffer->append(buf,read_bytes);
                readed+=read_bytes;
            }
            else if(read_bytes==0)
            {
                cout<<"服务端断开"<<endl;
                exit(EXIT_SUCCESS);
            }
            if(readed>=sentBuffer->size())
            {
                cout<<"来自服务端的消息:"<<readBuffer->c_str()<<endl;
                break;
            }
        }
        readBuffer->clear();
    }
    delete addr;
    delete sock;
    
    return 0;
}