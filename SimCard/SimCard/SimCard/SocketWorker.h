#pragma once
#include <WinSock2.h>
//#include ws2def.h"
const UINT BUF_LEN=5000000;//缓冲区最大长度 ,最多5万辆车
class CSocketWorker{
public:
	CSocketWorker(void);
	virtual ~CSocketWorker(void);	

public:
		// 套接字
	SOCKET m_socket;
	// 网络地址
	sockaddr_in m_sockaddr;
	//设置地址与端口
	int SetAddr(unsigned int nPort, const char* szIP, short family=AF_INET);
	//连接
	int Connect(bool bConEm=true);
	//连接
	int Connect(unsigned int nPort, char* szIP,bool bConEm=true, short family=AF_INET);
	// 重新连接
	int ReConnect(void);
	// 是否已经连接
	bool IsConnect(void);
	// 读取数据
	int virtual Read(void);
protected:
	// 接收缓冲区
	unsigned char *m_ucRecv;
	// 接收数据的长度
	int m_nDataLen;
public:
	
	// 数据处理
	int virtual DataProc(void);
	// 发送数据
	int Send(char* szSend, int nCount);
	// 连接后操作，用于继承
	virtual int OnConnected(void);
	// 关闭套接字,bKillThread:是否杀死读线程
	int Close(bool bKillThread=false);
private:
	// 线程句柄
	HANDLE m_hThread;
public:
	// 退出事件
	HANDLE m_hExitEvent;
	//等待事件
	HANDLE m_hWaitEvent;
	//重连间隔
	short m_nConnIntv;
	// 获得套接字信息
	//char* GetSocketInfo(void);
	virtual int SetStatus(int nStatus);
	// 线程是否已经创建
	bool IsThreadLive(void);
};

