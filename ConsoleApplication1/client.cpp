#include <WinSock2.h>                //头文件
#pragma comment(lib,"ws2_32.lib")    //库文件
#include "tcpSocket.h"
#include "fileOperation.h"
#include <stdio.h>

void ClientSocketUpload(const char* filePath) {
	init_Socket();
	SOCKET fd = create_clientSocket();
	if (fd == INVALID_SOCKET) {
		printf("\n客户端连接服务端失败,压缩文件生成在本地...\n");
	}else {
		printf("\n客户端连接服务端成功...\n");
	}
	//客户端给服务端发送文件
	sendFile(fd, filePath);
	closesocket(fd);
	close_Socket();
}