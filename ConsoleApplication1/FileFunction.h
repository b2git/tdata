#include <stdlib.h>
#include <atlstr.h>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <psapi.h>
#include <typeinfo>
#include <iostream>
#include <vector>
#include <random>
#include <iostream>
#include <io.h>
#include <direct.h>
#include <fstream>
using namespace std;

void CopyFiles(char* SourceFile, char* NewFile);
void GetFileNames(string path, vector<string>& files);
void CreateFolder(std::string folderPath);
std::string GetRandomFileName(int sz);
std::string GetExeFilePath();
BOOL IsDirectory(const char* pDir);
BOOL DeleteDirectory(const char* DirName);
int  DeleteFiles(const char* fileName);