#ifndef __FILEOP_H_
#define __FILEOP_H_
#include "tcpSocket.h"

/***客户端***/
//读取文件
bool readFile(const char* fileName);
//发文件
bool sendFile(SOCKET s, const char* fileName);
/**服务端***/
//接受文件
bool recvFile(SOCKET s, const char* fileName);
//保存文件（写）
bool saveFile(const char* fileName);
#endif // ! __FILEOP_H_
