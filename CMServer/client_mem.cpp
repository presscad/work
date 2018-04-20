#include "stdafx.h"
#include "client_signaldata.h"
#include "client_mem.h"
#include <vector>

std::vector<CLIENT_SOCK*> vctClientSock;
CRITICAL_SECTION csClientSock;

std::vector<CLIENT_BUF*> vctClientBuf;
CRITICAL_SECTION csClientBuf;

void InitClientSockMem()
{
	InitializeCriticalSection(&csClientSock);
	vctClientSock.clear();
	InitializeCriticalSection(&csClientBuf);
	vctClientBuf.clear();
}

CLIENT_SOCK* allocClientSock(DWORD dwSize)
{
	CLIENT_SOCK* client_sock = NULL;

	EnterCriticalSection(&csClientSock);
	if (vctClientSock.empty())
	{
		client_sock = (CLIENT_SOCK*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
		client_sock->lstSend = new std::list<CLIENT_BUF*>;
	}
	else
	{
		client_sock = vctClientSock.back();
		vctClientSock.pop_back();
	}
	LeaveCriticalSection(&csClientSock);

	if (client_sock)
		client_sock->Init(dwSize - SIZE_OF_CLIENT_SOCK_T);

	return client_sock;
}

void freeClientSock(CLIENT_SOCK* client_sock)
{
	EnterCriticalSection(&csClientSock);
	if (vctClientSock.size() < 1000)
	{
		vctClientSock.push_back(client_sock);
	}
	else
	{
		client_sock->Clear();
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, client_sock);
	}
	LeaveCriticalSection(&csClientSock);
}

CLIENT_BUF* allocClientBuf(DWORD dwSize)
{
	CLIENT_BUF* client_buf = NULL;
	EnterCriticalSection(&csClientBuf);
	if (vctClientBuf.empty())
	{
		client_buf = (CLIENT_BUF*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
	}
	else
	{
		client_buf = vctClientBuf.back();
		vctClientBuf.pop_back();
	}
	LeaveCriticalSection(&csClientBuf);

	if (client_buf)
		client_buf->Init(dwSize - SIZE_OF_CLIENT_BUF_T);

	return client_buf;
}

void freeClientBuf(CLIENT_BUF* client_buf)
{
	EnterCriticalSection(&csClientBuf);
	if (vctClientBuf.size() < 1000)
	{
		vctClientBuf.push_back(client_buf);
	}
	else
	{
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, client_buf);
	}
	LeaveCriticalSection(&csClientBuf);
}