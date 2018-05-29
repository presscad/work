// SocketWorker.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "SocketWorker.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#ifdef DEBUG
#include <typeinfo>
#endif

UINT WINAPI ReadThread(LPVOID lp)
{
	int nRt(0);//3秒重连
	CSocketWorker* socketworker=(CSocketWorker*)lp;
	if(!socketworker)
	{
		//WriteLog("传入线程的套接字错误…\n","error");
		return -1;
	}
	while(WaitForSingleObject(socketworker->m_hExitEvent,0) != WAIT_OBJECT_0)//事件为假
	{
		if(!(socketworker->IsConnect()))//连接断开
		{	
#ifdef _DEBUG
			OutputDebugString("断开重连\n");			
#endif
			if(WAIT_OBJECT_0 == WaitForSingleObject(socketworker->m_hExitEvent,
				socketworker->m_nConnIntv*1000))//为真就要退出线程了
				break;	
			socketworker->m_nConnIntv=10;//重连之后要重置
			if(-1 == socketworker->ReConnect())//重连失败
			{
				continue;					
			}			
		}
		socketworker->Read();
	}
	//TRACE1("%s socket thread is out\n",typeid(*socketworker).name());	
	SetEvent(socketworker->m_hWaitEvent);
	ResetEvent(socketworker->m_hExitEvent);
	return 0;
}
CSocketWorker::CSocketWorker()
: m_nDataLen(0)
,m_hThread(NULL)
,m_ucRecv(NULL)
,m_nConnIntv(10)
{
	m_socket=INVALID_SOCKET;		
	while(!m_ucRecv)
		m_ucRecv=new byte[BUF_LEN+1];
	memset(m_ucRecv,0,BUF_LEN); 
	m_hExitEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hWaitEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	return;
}
CSocketWorker::~CSocketWorker()
{
	//TRACE0("~CSocketWorker()\n");	
	Close(true);
	delete[] m_ucRecv;
	m_ucRecv=NULL;

	//closesocket(m_socket);
}


int CSocketWorker::SetAddr(unsigned int nPort, const char* szIP, short family)
{
	m_sockaddr.sin_family = family;
	m_sockaddr.sin_addr.S_un.S_addr = inet_addr(szIP);
	m_sockaddr.sin_port = htons(nPort);
	return 0;
}

int CSocketWorker::Connect(bool bConEm)
{
  	int nReturn = -1;
	if(IsThreadLive())
		Close(true);
	if(IsConnect())//句柄不为空
	{
		shutdown(m_socket,SD_BOTH);
		closesocket(m_socket);
		m_socket=INVALID_SOCKET;
	}
	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//套接类型
	int nFlag=1, sock_buf_size=USHRT_MAX;
	setsockopt(m_socket,IPPROTO_TCP,TCP_NODELAY,			//禁用Nagle算法
		(char*)&nFlag,sizeof(nFlag));
	setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF,				//设置接收缓冲区
		(char *)&sock_buf_size, sizeof(sock_buf_size) );
	ResetEvent(m_hExitEvent);
	ResetEvent(m_hWaitEvent);
	if(bConEm)
	{
		if(connect(m_socket, (SOCKADDR*) &m_sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) 
		{
			int nErr = GetLastError();
			shutdown(m_socket,SD_BOTH);
			closesocket(m_socket);
			m_socket=INVALID_SOCKET;
			
		}
          else
		{
			//TRACE0("连接成功…\n");	
			OnConnected();
			nReturn = 0;
			
		}	
	}
	else
		m_nConnIntv=0;//让线程来完成连接
	m_hThread=(HANDLE) _beginthreadex(NULL,NULL,ReadThread,LPVOID(this),NULL,NULL);
	return nReturn;
}

int CSocketWorker::Connect(unsigned int nPort, char* szIP, bool bConEm,short family)
{	
	SetAddr(nPort, szIP, family);
	return Connect(bConEm);
}

// 重新连接
int CSocketWorker::ReConnect(void)
{
	SetStatus(0);
	/*if(WaitForSingleObject(m_hExitEvent,0) != WAIT_OBJECT_0)
	{
		TRACE0("线程退出中，不重连…\n");
		return -1;
	}*/
	if(IsConnect())//句柄不为空
	{
		shutdown(m_socket,SD_BOTH);
		closesocket(m_socket);
		m_socket=INVALID_SOCKET;
	}
	//memset(m_ucRecv,0,m_nDataLen);
	m_nDataLen=0;
	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//套接类型
	int nFlag=1, sock_buf_size=USHRT_MAX;
	setsockopt(m_socket,IPPROTO_TCP,TCP_NODELAY,			//禁用Nagle算法
		(char*)&nFlag,sizeof(nFlag));
	setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF,				//设置接收缓冲区
		(char *)&sock_buf_size, sizeof(sock_buf_size) );
	if(connect(m_socket, (SOCKADDR*) &m_sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) 
	{	
		shutdown(m_socket,SD_BOTH);
		closesocket(m_socket);
		m_socket=INVALID_SOCKET;
		//TRACE1("%s重新连接失败\n",typeid(*this).name());	
		return -1;
	}
	else
	{
		//TRACE1("%s重新连接成功\n",typeid(*this).name());	
		OnConnected();
	}
	return 0;
}

// 是否已经连接
bool CSocketWorker::IsConnect(void)
{
	
	if(INVALID_SOCKET == m_socket)
		return false;
	return true;
}

// 读取数据
int CSocketWorker::Read(void)
{
	//char szRecv[10001]={0};
	//TRACE0("reading\n");
	if(!IsConnect())
		return -1;
	if(m_nDataLen < 0)
		m_nDataLen=0;
	int nLen=recv(m_socket,(char*)m_ucRecv+m_nDataLen,BUF_LEN-m_nDataLen,0);
  	if(nLen<1)//读数据错误，认为是网络错误
	{
		m_nDataLen=0;
		shutdown(m_socket,SD_BOTH);
		closesocket(m_socket);
		m_socket=INVALID_SOCKET;
		this->m_socket=INVALID_SOCKET;
		return -1;
	}
	m_nDataLen+=nLen;//长度控制
	this->DataProc();
	return 0;
}

// 数据处理
int CSocketWorker::DataProc(void)
{

	m_nDataLen=0;
	return 0;
}

// 发送数据
int CSocketWorker::Send(char* szSend, int nCount)
{
	if(IsConnect())
		return send(m_socket,szSend,nCount,0);
	else
	{
		//WriteLog("断开中，信息不发送…\n",GetSocketInfo());
		return -1;
	}
	return 0;
}

// 连接后操作，用于继承
int CSocketWorker::OnConnected(void)
{
	return 0;
}

// 关闭套接字,bKillThread:是否杀死读线程
int CSocketWorker::Close(bool bKillThread)
{
	//SetStatus(3);
	shutdown(m_socket,SD_BOTH);
	closesocket(m_socket);
	m_socket=INVALID_SOCKET;
	m_nDataLen=0;	
	DWORD nRt(0);
	if(bKillThread)
	{
		::SetEvent(m_hExitEvent);
		if(m_hThread)
		{
			nRt=WaitForSingleObject(m_hWaitEvent,1000);	//等待1秒
			if(nRt != WAIT_OBJECT_0)//线程没有结束
			{
				//TRACE0("socket thread is not out,kill!\n");
				TerminateThread(m_hThread,-1);
				ResetEvent(m_hExitEvent);
			}
			CloseHandle(m_hThread);
			m_hThread=0;
		}
		ResetEvent(m_hWaitEvent);	
	}
	return 0;
}

int CSocketWorker::SetStatus(int nStatus)
{
	return 0;
}


// 线程是否已经创建
bool CSocketWorker::IsThreadLive(void)
{
	if(m_hThread)
		return true;
	return false;
}
