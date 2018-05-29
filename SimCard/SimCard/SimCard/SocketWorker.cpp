// SocketWorker.cpp : ���� DLL Ӧ�ó���ĵ���������
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
	int nRt(0);//3������
	CSocketWorker* socketworker=(CSocketWorker*)lp;
	if(!socketworker)
	{
		//WriteLog("�����̵߳��׽��ִ���\n","error");
		return -1;
	}
	while(WaitForSingleObject(socketworker->m_hExitEvent,0) != WAIT_OBJECT_0)//�¼�Ϊ��
	{
		if(!(socketworker->IsConnect()))//���ӶϿ�
		{	
#ifdef _DEBUG
			OutputDebugString("�Ͽ�����\n");			
#endif
			if(WAIT_OBJECT_0 == WaitForSingleObject(socketworker->m_hExitEvent,
				socketworker->m_nConnIntv*1000))//Ϊ���Ҫ�˳��߳���
				break;	
			socketworker->m_nConnIntv=10;//����֮��Ҫ����
			if(-1 == socketworker->ReConnect())//����ʧ��
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
	if(IsConnect())//�����Ϊ��
	{
		shutdown(m_socket,SD_BOTH);
		closesocket(m_socket);
		m_socket=INVALID_SOCKET;
	}
	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//�׽�����
	int nFlag=1, sock_buf_size=USHRT_MAX;
	setsockopt(m_socket,IPPROTO_TCP,TCP_NODELAY,			//����Nagle�㷨
		(char*)&nFlag,sizeof(nFlag));
	setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF,				//���ý��ջ�����
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
			//TRACE0("���ӳɹ���\n");	
			OnConnected();
			nReturn = 0;
			
		}	
	}
	else
		m_nConnIntv=0;//���߳����������
	m_hThread=(HANDLE) _beginthreadex(NULL,NULL,ReadThread,LPVOID(this),NULL,NULL);
	return nReturn;
}

int CSocketWorker::Connect(unsigned int nPort, char* szIP, bool bConEm,short family)
{	
	SetAddr(nPort, szIP, family);
	return Connect(bConEm);
}

// ��������
int CSocketWorker::ReConnect(void)
{
	SetStatus(0);
	/*if(WaitForSingleObject(m_hExitEvent,0) != WAIT_OBJECT_0)
	{
		TRACE0("�߳��˳��У���������\n");
		return -1;
	}*/
	if(IsConnect())//�����Ϊ��
	{
		shutdown(m_socket,SD_BOTH);
		closesocket(m_socket);
		m_socket=INVALID_SOCKET;
	}
	//memset(m_ucRecv,0,m_nDataLen);
	m_nDataLen=0;
	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//�׽�����
	int nFlag=1, sock_buf_size=USHRT_MAX;
	setsockopt(m_socket,IPPROTO_TCP,TCP_NODELAY,			//����Nagle�㷨
		(char*)&nFlag,sizeof(nFlag));
	setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF,				//���ý��ջ�����
		(char *)&sock_buf_size, sizeof(sock_buf_size) );
	if(connect(m_socket, (SOCKADDR*) &m_sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) 
	{	
		shutdown(m_socket,SD_BOTH);
		closesocket(m_socket);
		m_socket=INVALID_SOCKET;
		//TRACE1("%s��������ʧ��\n",typeid(*this).name());	
		return -1;
	}
	else
	{
		//TRACE1("%s�������ӳɹ�\n",typeid(*this).name());	
		OnConnected();
	}
	return 0;
}

// �Ƿ��Ѿ�����
bool CSocketWorker::IsConnect(void)
{
	
	if(INVALID_SOCKET == m_socket)
		return false;
	return true;
}

// ��ȡ����
int CSocketWorker::Read(void)
{
	//char szRecv[10001]={0};
	//TRACE0("reading\n");
	if(!IsConnect())
		return -1;
	if(m_nDataLen < 0)
		m_nDataLen=0;
	int nLen=recv(m_socket,(char*)m_ucRecv+m_nDataLen,BUF_LEN-m_nDataLen,0);
  	if(nLen<1)//�����ݴ�����Ϊ���������
	{
		m_nDataLen=0;
		shutdown(m_socket,SD_BOTH);
		closesocket(m_socket);
		m_socket=INVALID_SOCKET;
		this->m_socket=INVALID_SOCKET;
		return -1;
	}
	m_nDataLen+=nLen;//���ȿ���
	this->DataProc();
	return 0;
}

// ���ݴ���
int CSocketWorker::DataProc(void)
{

	m_nDataLen=0;
	return 0;
}

// ��������
int CSocketWorker::Send(char* szSend, int nCount)
{
	if(IsConnect())
		return send(m_socket,szSend,nCount,0);
	else
	{
		//WriteLog("�Ͽ��У���Ϣ�����͡�\n",GetSocketInfo());
		return -1;
	}
	return 0;
}

// ���Ӻ���������ڼ̳�
int CSocketWorker::OnConnected(void)
{
	return 0;
}

// �ر��׽���,bKillThread:�Ƿ�ɱ�����߳�
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
			nRt=WaitForSingleObject(m_hWaitEvent,1000);	//�ȴ�1��
			if(nRt != WAIT_OBJECT_0)//�߳�û�н���
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


// �߳��Ƿ��Ѿ�����
bool CSocketWorker::IsThreadLive(void)
{
	if(m_hThread)
		return true;
	return false;
}
