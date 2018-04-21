#include "stdafx.h"
#include "client_signaldata.h"
#include "client_mem.h"
#include <vector>

std::vector<CLIENT_SOCK*> vctClientSock;
CRITICAL_SECTION csClientSock;

std::vector<CLIENT_BUF*> vctClientBuf;
CRITICAL_SECTION csClientBuf;

std::vector<SOCKET_OBJ*> vctSockObj;
CRITICAL_SECTION csSockObj;

std::vector<BUFFER_OBJ*> vctBufObj;
CRITICAL_SECTION csBufObj;

void InitClientSockMem()
{
	InitializeCriticalSection(&csClientSock);
	vctClientSock.clear();
	InitializeCriticalSection(&csClientBuf);
	vctClientBuf.clear();
	InitializeCriticalSection(&csSockObj);
	vctSockObj.clear();
	InitializeCriticalSection(&csBufObj);
	vctBufObj.clear();
}

CLIENT_SOCK* allocClientSock(DWORD dwSize)
{
	CLIENT_SOCK* client_sock = NULL;

	EnterCriticalSection(&csClientSock);
	if (vctClientSock.empty())
	{
		client_sock = (CLIENT_SOCK*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
		client_sock->lstSend = new std::list<BUFFER_OBJ*>;
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

CLIENT_BUF* allocClientBuf()
{
	CLIENT_BUF* client_buf = NULL;
	EnterCriticalSection(&csClientBuf);
	if (vctClientBuf.empty())
	{
		client_buf = (CLIENT_BUF*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, SIZE_OF_CLIENT_BUF);
	}
	else
	{
		client_buf = vctClientBuf.back();
		vctClientBuf.pop_back();
	}
	LeaveCriticalSection(&csClientBuf);

	if (client_buf)
		client_buf->Init();

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

SOCKET_OBJ* allocSObj()
{
	SOCKET_OBJ* sobj = NULL;
	EnterCriticalSection(&csSockObj);
	if (vctSockObj.empty())
		sobj = (SOCKET_OBJ*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, SIZE_OF_SOCKET_OBJ);
	else
	{
		sobj = vctSockObj.back();
		vctSockObj.pop_back();
	}
	LeaveCriticalSection(&csSockObj);

	if (sobj)
		sobj->Init();
	return sobj;
}

void freeSObj(SOCKET_OBJ* sobj)
{
	EnterCriticalSection(&csSockObj);
	if (vctSockObj.size() < 1000)
	{
		vctSockObj.push_back(sobj);
	}
	else
	{
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, sobj);
	}
	LeaveCriticalSection(&csSockObj);
}

BUFFER_OBJ* allocBObj(DWORD dwSize)
{
	BUFFER_OBJ* bobj = NULL;
	EnterCriticalSection(&csBufObj);
	if (vctBufObj.empty())
	{
		bobj = (BUFFER_OBJ*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
	}
	else
	{
		bobj = vctBufObj.back();
		vctBufObj.pop_back();
	}
	LeaveCriticalSection(&csBufObj);

	if (bobj)
		bobj->Init(dwSize - SIZE_BUFFER_OBJ_T);

	return bobj;
}

void freeBObj(BUFFER_OBJ* bobj)
{
	EnterCriticalSection(&csBufObj);
	if (vctBufObj.size() < 1000)
	{
		vctBufObj.push_back(bobj);
	}
	else
	{
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, bobj);
	}
	LeaveCriticalSection(&csBufObj);
}