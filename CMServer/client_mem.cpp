#include "stdafx.h"
#include "client_signaldata.h"
#include "client_mem.h"
#include <vector>

std::vector<CLIENT_SOCK*> vctClientSock;
CRITICAL_SECTION csClientSock;

void InitClientSockMem()
{
	InitializeCriticalSection(&csClientSock);
	vctClientSock.clear();
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