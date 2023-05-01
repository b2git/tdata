#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
#include <WinSock2.h>                //头文件
#pragma comment(lib,"ws2_32.lib")    //库文件

//宏定义
#define err(errMsg) printf("[error]%s failed,code %d line:%d",errMsg,WSAGetLastError(),__LINE__);

#define PORT 65520


//初始化网络库
bool init_Socket();
//关闭网络库
bool close_Socket();
//服务器： 创建服务器socket
SOCKET create_serverSocket();
//客户端：创建客户端 socket
SOCKET create_clientSocket();
#endif // !_TCPSOCKET_H_