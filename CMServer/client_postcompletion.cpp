#include "stdafx.h"
#include "client_signaldata.h"
#include "client_postcompletion.h"
#include "init.h"
#include "global_data.h"
#include "client_mem.h"
#include "client_requestpost.h"

struct tcp_keepalive alive_in = { TRUE, 1000 * 10, 1000 };
struct tcp_keepalive alive_out = { 0 };
unsigned long ulBytesReturn = 0;

void Client_AcceptCompletionFailed(void* _lsock, void* _bobj)
{
	LISTEN_SOCK* lsock = (LISTEN_SOCK*)_lsock;
	CLIENT_BUF* bobj = (CLIENT_BUF*)_bobj;
	CLIENT_SOCK* csock = bobj->pRelateClientSock;

	lsock->DeleteFromPendingMap(csock);

	CMCloseSocket(csock);

	delete bobj;
	freeClientSock(csock);
}

void Client_AcceptCompletionSuccess(DWORD dwTranstion, void* _lsock, void* _bobj)
{
	if (dwTranstion <= 0)
		return Client_AcceptCompletionFailed(_lsock, _bobj);

	LISTEN_SOCK* lsock = (LISTEN_SOCK*)_lsock;
	CLIENT_BUF* bobj = (CLIENT_BUF*)_bobj;
	CLIENT_SOCK* csock = bobj->pRelateClientSock;

	lsock->DeleteFromPendingMap(csock);

	WSAIoctl(csock->sock, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
		&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);

	if (NULL == CreateIoCompletionPort((HANDLE)csock->sock, hComport, (ULONG_PTR)csock, 0))
	{
		_tprintf(_T("客户端socket绑定完成端口失败, errCode = %d\n"), WSAGetLastError());

		CMCloseSocket(csock);

		delete bobj;
		freeClientSock(csock);
		return;
	}

	SOCKADDR* localAddr,
		*remoteAddr;
	localAddr = NULL;
	remoteAddr = NULL;
	int localAddrlen,
		remoteAddrlen;

	lpfnGetAcceptExSockaddrs(csock->buf, csock->dwBufsize - ((sizeof(sockaddr_in) + 16) * 2),
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		&localAddr, &localAddrlen,
		&remoteAddr, &remoteAddrlen);

	csock->InitWRpos(dwTranstion);
	DWORD len = csock->GetCmdDataLength();
	while (len)
	{
		char buf[256];
		DWORD dwRecvedCount = len;
		csock->Read(buf, len);
		csock->AddRef();
		if (0 == PostQueuedCompletionStatus(hComport, len, (ULONG_PTR)buf, NULL))
		{
			//freeBObj(obj);
			if (0 == InterlockedDecrement(&csock->nRef))
			{
			//	CSCloseSocket(c_sobj);
				//freeSObj(c_sobj);
				return;
			}
		}
		len = csock->GetCmdDataLength();
	}

	bobj->SetIoRequestFunction(Client_ZeroRecvCompletionFailed, Client_ZeroRecvCompletionSuccess);
	if (!Client_PostZeroRecv(csock, bobj))
	{
		if (0 == InterlockedDecrement(&csock->nRef))
		{
			CMCloseSocket(csock);
			freeClientSock(csock);
		}
		delete bobj;
	}
}

void Client_ZeroRecvCompletionFailed(void* _csock, void* _bobj)
{
	CLIENT_SOCK* csock = (CLIENT_SOCK*)_csock;
	CLIENT_BUF* bobj = (CLIENT_BUF*)_bobj;
	if (0 == InterlockedDecrement(&csock->nRef))
	{
		CMCloseSocket(csock);
		freeClientSock(csock);
	}
	delete bobj;
}

void Client_ZeroRecvCompletionSuccess(DWORD dwTranstion, void* _csock, void* _bobj)
{
	CLIENT_SOCK* csock = (CLIENT_SOCK*)_csock;
	CLIENT_BUF* bobj = (CLIENT_BUF*)_bobj;

	bobj->SetIoRequestFunction(Client_RecvCompletionFailed, Client_RecvCompletionSuccess);
	if (!Client_PostRecv(csock, bobj))
	{
		if (0 == InterlockedDecrement(&csock->nRef))
		{
			CMCloseSocket(csock);
			freeClientSock(csock);
		}
		delete bobj;
	}
}

void Client_RecvCompletionFailed(void* _csock, void* _bobj)
{
	CLIENT_SOCK* csock = (CLIENT_SOCK*)_csock;
	CLIENT_BUF* bobj = (CLIENT_BUF*)_bobj;

	if (0 == InterlockedDecrement(&csock->nRef))
	{
		CMCloseSocket(csock);
		freeClientSock(csock);
	}
	delete bobj;
}
void Client_RecvCompletionSuccess(DWORD dwTranstion, void* _csock, void* _bobj)
{
	if (dwTranstion <= 0)
		return Client_RecvCompletionFailed(_csock, _bobj);

	CLIENT_SOCK* csock = (CLIENT_SOCK*)_csock;
	CLIENT_BUF* bobj = (CLIENT_BUF*)_bobj;

	csock->InitWRpos(dwTranstion);
	DWORD len = csock->GetCmdDataLength();
	while (len)
	{

	}

	bobj->SetIoRequestFunction(Client_ZeroRecvCompletionFailed, Client_ZeroRecvCompletionSuccess);
	if (!Client_PostZeroRecv(csock, bobj))
	{
		if (0 == InterlockedDecrement(&csock->nRef))
		{
			CMCloseSocket(csock);
			freeClientSock(csock);
		}
		delete bobj;
	}
}

void Client_SendCompletionFailed(void* _csock, void* _bobj)
{

}
void Client_SendCompletionSuccess(DWORD dwTranstion, void* _csock, void* _bobj)
{

}