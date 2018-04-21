#include "stdafx.h"
#include "init.h"
#include "client_signaldata.h"
#include "global_data.h"
#include "nc_requestpost.h"
#include "client_mem.h"

extern void Nc_AcceptCompletionFailed(void* _lsock, void* _bobj);
extern void Nc_AcceptCompletionSuccess(DWORD dwTranstion, void* _lsock, void* _bobj);

bool Nc_PostAcceptEx(void* _lsock)
{
	LISTEN_SOCK* lsock = (LISTEN_SOCK*)_lsock;
	DWORD dwBytes = 0;
	BUFFER_OBJ* bobj = allocBObj(g_dwPagesize);
	if (NULL == bobj)
		return false;

	SOCKET_OBJ* csock = allocSObj();
	if (NULL == csock)
	{
		freeBObj(bobj);
		return false;
	}

	csock->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == csock->sock)
	{
		freeBObj(bobj);
		freeSObj(csock);
		return false;
	}

	bobj->pRelateClientSock = csock;
	bobj->SetIoRequestFunction(Nc_AcceptCompletionFailed, Nc_AcceptCompletionSuccess);
	// ÉèÖÃ»Øµô
	csock->nKey = GetRand();
	lsock->InsertIntoPendingMap(csock);

	if (!lpfnAccpetEx(lsock->sListenSock, csock->sock, bobj->data, bobj->datalen - ((sizeof(sockaddr_in) + 16) * 2),
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dwBytes, &bobj->ol))
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			lsock->DeleteFromPendingMap(csock);
			CMCloseSocket(csock);
			freeBObj(bobj);
			freeSObj(csock);
			return false;
		}
	}

	return true;
}

BOOL Nc_PostZeroRecv(SOCKET_OBJ* _sobj, BUFFER_OBJ* _bobj)
{
	DWORD dwBytes = 0,
		dwFlags = 0;

	int err = 0;

	_bobj->wsaBuf.buf = NULL;
	_bobj->wsaBuf.len = 0;

	err = WSARecv(_sobj->sock, &_bobj->wsaBuf, 1, &dwBytes, &dwFlags, &_bobj->ol, NULL);
	if (SOCKET_ERROR == err && WSA_IO_PENDING != WSAGetLastError())
		return FALSE;

	return TRUE;
}

BOOL Nc_PostRecv(SOCKET_OBJ* _sobj, BUFFER_OBJ* _bobj)
{
	DWORD dwBytes = 0,
		dwFlags = 0;

	int err = 0;

	_bobj->wsaBuf.buf = _bobj->data + _bobj->dwRecvedCount;
	_bobj->wsaBuf.len = _bobj->datalen - _bobj->dwRecvedCount;

	err = WSARecv(_sobj->sock, &_bobj->wsaBuf, 1, &dwBytes, &dwFlags, &_bobj->ol, NULL);
	if (SOCKET_ERROR == err && WSA_IO_PENDING != WSAGetLastError())
		return FALSE;

	return TRUE;
}

BOOL Nc_PostSend(SOCKET_OBJ* _sobj, BUFFER_OBJ* _bobj)
{
	DWORD dwBytes = 0;
	int err = 0;

	_bobj->wsaBuf.buf = _bobj->data + _bobj->dwSendedCount;
	_bobj->wsaBuf.len = _bobj->dwRecvedCount - _bobj->dwSendedCount;

	err = WSASend(_sobj->sock, &_bobj->wsaBuf, 1, &dwBytes, 0, &_bobj->ol, NULL);
	if (SOCKET_ERROR == err && WSA_IO_PENDING != WSAGetLastError())
		return FALSE;

	return TRUE;
}