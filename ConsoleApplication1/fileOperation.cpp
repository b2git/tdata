#define _CRT_SECURE_NO_WARNINGS
#include "fileOperation.h"
#include <cstdio>
#include <iostream>
using namespace std;

long g_filesize = 3145728; //获取文件大小（默认2m）
char* g_fileBuf;  //保存文件数据

/*客户端操作*/
bool readFile(const char* fileName)
{
	//打开文件
	FILE* read = fopen(fileName,"rb");
	if (!read) {
		perror("file open failed: \n");
		return false;
		
	}
	//获取文件大小
	fseek(read, 0, SEEK_END);  
	g_filesize  = ftell(read);
	fseek(read, 0, SEEK_SET);
	printf("文件大小为: %d\n", g_filesize);
	
	//保存文件数据
	g_fileBuf = (char*)calloc(g_filesize,sizeof(char));
	if (!g_fileBuf) {
		return false;
	}
	//把文件读到内存中来
	fread(g_fileBuf, sizeof(char), g_filesize, read);

	fclose(read);
	return true;
}


bool sendFile(SOCKET s, const char* filePath)
{
	string::size_type pos;
	std::string fileName;
	//截取文件名 const char*转std::string
	pos = std::string(filePath).rfind("\\");
	//文件名称 ret.substr(pos + 1)
	fileName = std::string(filePath).substr(pos + 1);

	//发送文件名称
	if (send(s, (char*)fileName.data(), sizeof(fileName), 0) < 0) {
		perror("send filename error");
		exit(-1);
	}
	//读文件
	readFile(filePath);
	//发送文件
	int ret = send(s, g_fileBuf, g_filesize, 0);
	if (ret == SOCKET_ERROR) {
		err("sendFile");
		return false;
	}
	printf("发送文件成功: %d", g_filesize);
	return true;
}


/*服务端操作*/


bool saveFile(const char* fileName)
{
	//打开文件
	FILE* write = fopen(fileName, "wb");
	if (!write) {
		perror("file open failed: \n");
		return false;

	}
	//写文件
	fwrite(g_fileBuf, sizeof(char), g_filesize, write);

	fclose(write);
	return true;
}

bool recvFile(SOCKET s, const char* fileName)
{
	//分配内存
	if (g_fileBuf == NULL) {
	
		g_fileBuf = (char*)calloc(g_filesize, sizeof(char));
		if (!g_fileBuf) { return false; }
	}

		
	//接受文件
	int ret = recv(s, g_fileBuf, g_filesize, 0);
	if (ret == 0) {

		printf("客户端正常下线...\n");
	}
	else if (ret < 0) {

		err("recv");
	}
	//保存文件
	saveFile(fileName);
	return true;
}
