#pragma once
#include <WinSock2.h>
//#include ws2def.h"
const UINT BUF_LEN=5000000;//��������󳤶� ,���5������
class CSocketWorker{
public:
	CSocketWorker(void);
	virtual ~CSocketWorker(void);	

public:
		// �׽���
	SOCKET m_socket;
	// �����ַ
	sockaddr_in m_sockaddr;
	//���õ�ַ��˿�
	int SetAddr(unsigned int nPort, const char* szIP, short family=AF_INET);
	//����
	int Connect(bool bConEm=true);
	//����
	int Connect(unsigned int nPort, char* szIP,bool bConEm=true, short family=AF_INET);
	// ��������
	int ReConnect(void);
	// �Ƿ��Ѿ�����
	bool IsConnect(void);
	// ��ȡ����
	int virtual Read(void);
protected:
	// ���ջ�����
	unsigned char *m_ucRecv;
	// �������ݵĳ���
	int m_nDataLen;
public:
	
	// ���ݴ���
	int virtual DataProc(void);
	// ��������
	int Send(char* szSend, int nCount);
	// ���Ӻ���������ڼ̳�
	virtual int OnConnected(void);
	// �ر��׽���,bKillThread:�Ƿ�ɱ�����߳�
	int Close(bool bKillThread=false);
private:
	// �߳̾��
	HANDLE m_hThread;
public:
	// �˳��¼�
	HANDLE m_hExitEvent;
	//�ȴ��¼�
	HANDLE m_hWaitEvent;
	//�������
	short m_nConnIntv;
	// ����׽�����Ϣ
	//char* GetSocketInfo(void);
	virtual int SetStatus(int nStatus);
	// �߳��Ƿ��Ѿ�����
	bool IsThreadLive(void);
};

