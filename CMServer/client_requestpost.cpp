#include "stdafx.h"
#include "init.h"
#include "client_signaldata.h"
#include "global_data.h"
#include "client_requestpost.h"
#include "client_mem.h"

extern void Client_AcceptCompletionFailed(void* _lsock, void* _bobj);
extern void Client_AcceptCompletionSuccess(DWORD dwTranstion, void* _lsock, void* _bobj);

bool Client_PostAcceptEx(LISTEN_SOCK* lsock)
{
	DWORD dwBytes = 0;
	CLIENT_BUF* bobj = new CLIENT_BUF;
	if (NULL == bobj)
		return false;

	CLIENT_SOCK* csock = allocClientSock(g_dwPagesize);
	if (NULL == csock)
	{
		delete bobj;
		return false;
	}

	csock->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == csock->sock)
	{
		delete bobj;
		freeClientSock(csock);
		return false;
	}

	bobj->pRelateClientSock = csock;
	bobj->SetIoRequestFunction(Client_AcceptCompletionFailed, Client_AcceptCompletionSuccess);
	// ÉèÖÃ»Øµô
	csock->nKey = GetRand();
	lsock->InsertIntoPendingMap(csock);

	if (!lpfnAccpetEx(lsock->sListenSock, csock->sock, csock->buf, csock->dwBufsize - ((sizeof(sockaddr_in) + 16) * 2),
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dwBytes, &bobj->ol))
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			lsock->DeleteFromPendingMap(csock);
			closesocket(csock->sock);
			csock->sock = INVALID_SOCKET;
			delete bobj;
			freeClientSock(csock);
			return false;
		}
	}

	return true;
}

BOOL Client_PostZeroRecv(CLIENT_SOCK *_csock, CLIENT_BUF* _bobj)
{
	DWORD dwBytes = 0,
		dwFlags = 0;

	int err = 0;

	_csock->wsabuf[0].buf = NULL;
	_csock->wsabuf[0].len = 0;
	_csock->wsabuf[1].buf = NULL;
	_csock->wsabuf[1].len = 0;

	err = WSARecv(_csock->sock, _csock->wsabuf, 2, &dwBytes, &dwFlags, &_bobj->ol, NULL);
	if (SOCKET_ERROR == err && WSA_IO_PENDING != WSAGetLastError())
		return FALSE;

	return TRUE;
}

BOOL Client_PostRecv(CLIENT_SOCK* _csock, CLIENT_BUF* _bobj)
{
	DWORD dwBytes = 0,
		dwFlags = 0;

	int err = 0;

	_csock->InitWSABUFS();

	err = WSARecv(_csock->sock, _csock->wsabuf, 2, &dwBytes, &dwFlags, &_bobj->ol, NULL);
	if (SOCKET_ERROR == err && WSA_IO_PENDING != WSAGetLastError())
		return FALSE;

	return TRUE;
}

//BOOL CLient_PostSend(CLIENT_SOCK* _csock, CLIENT_BUF* _bobj)
//{
//	DWORD dwBytes = 0;
//	int err = 0;
//
//	//_bobj->wsaBuf.buf = _bobj->data + _bobj->dwSendedCount;
//	//_bobj->wsaBuf.len = _bobj->dwRecvedCount - _bobj->dwSendedCount;
//
//	//err = WSASend(_sobj->sock, &_bobj->wsaBuf, 1, &dwBytes, 0, &_bobj->ol, NULL);
//	//if (SOCKET_ERROR == err && WSA_IO_PENDING != WSAGetLastError())
//	//	return FALSE;
//
//	return TRUE;
//}