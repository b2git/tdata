#include "tcpSocket.h"
#include <stdio.h>

//初始化网络库
bool init_Socket()
{
	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata)) {
		err("WSAStartup");
		return false;
	}
	return true;
}
//关闭网络库
bool close_Socket()
{
	if (0 != WSACleanup()) {
		err("WSACleanup");
	}
	return true;
}

SOCKET create_serverSocket()
{
	//创建一个空的scoket
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == fd) {
	
		err("scoket");
		return INVALID_SOCKET;
	}
	//给socket绑定本地ip地址和端口号
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT); //把本地字节序转成网络字节序
	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	if (SOCKET_ERROR == bind(fd, (struct sockaddr*)&addr, sizeof(addr))) {
	
		err("bind")
			return INVALID_SOCKET;
	}
	//开始监听
	listen(fd, 10);
	return fd;
}

SOCKET create_clientSocket()
{
	//创建一个空的scoket
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == fd) {

		err("scoket");
		return INVALID_SOCKET;
	}
	//给socket绑定服务器的ip地址和端口号
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT); //htons：host to network，由16位本地字节序转换成网络字节序。
	addr.sin_addr.S_un.S_addr = htonl(1920103026); //htonl(主机字节序) 绑定服务器ip htonl：host to network，由32位主机字节序转换成网络字节序。

	if (INVALID_SOCKET == connect(fd, (struct sockaddr*)&addr, sizeof(addr))) {
		err("connect")
			return INVALID_SOCKET;
	}

	return fd;
}


