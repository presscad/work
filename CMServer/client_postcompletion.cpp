#include "stdafx.h"
#include "client_signaldata.h"
#include "client_postcompletion.h"
#include "init.h"
#include "global_data.h"
#include "client_mem.h"
#include "client_requestpost.h"
#include "client_cmd.h"

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

	freeClientBuf(bobj);
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

		freeClientBuf(bobj);
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
		BUFFER_OBJ* obj = allocBObj(g_dwPagesize);
		csock->Read(obj->data, len);
		obj->dwRecvedCount = len;
		obj->pRelateClientSock = csock;
		csock->AddRef();
		obj->SetIoRequestFunction(Client_CmdCompletionFailed, Client_CmdCompletionSuccess);
		if (0 == PostQueuedCompletionStatus(hComport, len, (ULONG_PTR)obj, &obj->ol))
		{
			csock->DecRef();
			freeBObj(obj);
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
		freeClientBuf(bobj);
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
	freeClientBuf(bobj);
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
	freeClientBuf(bobj);
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
		BUFFER_OBJ* obj = allocBObj(g_dwPagesize);
		csock->Read(obj->data, len);
		obj->dwRecvedCount = len;
		obj->pRelateClientSock = csock;
		csock->AddRef();
		obj->SetIoRequestFunction(Client_CmdCompletionFailed, Client_CmdCompletionSuccess);
		if (0 == PostQueuedCompletionStatus(hComport, len, (ULONG_PTR)obj, &obj->ol))
		{
			freeBObj(obj);
			if (0 == InterlockedDecrement(&csock->nRef))
			{
				CMCloseSocket(csock);
				freeClientSock(csock);
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
		freeClientBuf(bobj);
	}
}

void Client_SendCompletionFailed(void* _sobj, void* _bobj)
{
	SOCKET_OBJ* c_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

	BUFFER_OBJ* obj = c_sobj->GetNextData();
	freeBObj(c_bobj);
	if (obj)
	{
		InterlockedDecrement(&c_sobj->nRef);
		obj->SetIoRequestFunction(Client_SendCompletionFailed, Client_SendCompletionSuccess);
		if (!CLient_PostSend(c_sobj, obj))
		{
			Client_SendCompletionFailed(c_sobj, obj);
		}
	}
	else
	{
		if (0 == InterlockedDecrement(&c_sobj->nRef))
		{
			CMCloseSocket(c_sobj);
			freeSObj(c_sobj);
		}
	}
}
void Client_SendCompletionSuccess(DWORD dwTranstion, void* _sobj, void* _bobj)
{
	if (dwTranstion <= 0)
		return Client_SendCompletionFailed(_sobj, _bobj);

	SOCKET_OBJ* c_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

	c_bobj->dwSendedCount += dwTranstion;
	if (c_bobj->dwSendedCount < c_bobj->dwRecvedCount)
	{
		if (!CLient_PostSend(c_sobj, c_bobj))
		{
			Client_SendCompletionFailed(c_sobj, c_bobj);
			return;
		}
		return;
	}

	BUFFER_OBJ* obj = c_sobj->GetNextData();
	freeBObj(c_bobj);
	if (obj)
	{
		InterlockedDecrement(&c_sobj->nRef);
		obj->SetIoRequestFunction(Client_SendCompletionFailed, Client_SendCompletionSuccess);
		if (!CLient_PostSend(c_sobj, obj))
		{
			Client_SendCompletionFailed(c_sobj, obj);
		}
	}
	else
	{
		if (0 == InterlockedDecrement(&c_sobj->nRef))
		{
			CMCloseSocket(c_sobj);
			freeSObj(c_sobj);
		}
	}
}