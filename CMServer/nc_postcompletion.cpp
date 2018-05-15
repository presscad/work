#include "stdafx.h"
#include "client_signaldata.h"
#include "init.h"
#include "global_data.h"
#include "nc_postcompletion.h"
#include "client_mem.h"
#include "nc_requestpost.h"
#include "cmd_ncdata.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

extern TCHAR* Utf8ConvertAnsi(const TCHAR* strIn, int inLen);

extern struct tcp_keepalive alive_in;
extern struct tcp_keepalive alive_out;
extern unsigned long ulBytesReturn;

int CheckNCData(BUFFER_OBJ* bobj)
{
	TCHAR* pHeader = NULL;
	if (NULL == (pHeader = _tcsstr(bobj->data, _T("\r\n\r\n"))))
	{
		return 0;
	}
	else
	{
		int HeaderLen = (int)(pHeader - bobj->data);
		HeaderLen += (int)_tcslen(_T("\r\n\r\n"));
		int len = (int)_tcslen(_T("Content-Length: "));
		TCHAR* pContentLength = StrStrI(bobj->data, _T("Content-Length: "));
		if (NULL == pContentLength)
			return 0;
		TCHAR* pContentLengthEnd = _tcsstr(pContentLength, _T("\r\n"));
		if (NULL == pContentLengthEnd)
			return 0;
		int nLengthLen = (int)(pContentLengthEnd - pContentLength) - len;
		TCHAR Length[8] = { 0 };
		memcpy_s(Length, sizeof(Length), pContentLength + len, nLengthLen);
		len = _tstoi(Length);
		_tprintf_s("Content-Length: %d\n", len);
		if ((HeaderLen + len) > (int)bobj->dwRecvedCount)
			return 0;
		else if ((HeaderLen + len) == (int)bobj->dwRecvedCount)
			return HeaderLen;
		else
		{
			CMCloseSocket(bobj);
			return 0;
		}
	}
	return 0;
}

void Nc_AcceptCompletionFailed(void* _lsock, void* _bobj)
{
	LISTEN_SOCK* lobj = (LISTEN_SOCK*)_lsock;
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;
	SOCKET_OBJ* sobj = (SOCKET_OBJ*)bobj->pRelateClientSock;

	lobj->DeleteFromPendingMap(sobj);

	CMCloseSocket(sobj);
	freeSObj(sobj);
	freeBObj(bobj);
}

void Nc_AcceptCompletionSuccess(DWORD dwTranstion, void* _lsock, void* _bobj)
{
	if (dwTranstion <= 0)
		return Nc_AcceptCompletionFailed(_lsock, _bobj);

	LISTEN_SOCK* lobj = (LISTEN_SOCK*)_lsock;
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;
	SOCKET_OBJ* sobj = (SOCKET_OBJ*)bobj->pRelateClientSock;

	lobj->DeleteFromPendingMap(sobj);

	WSAIoctl(sobj->sock, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
		&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
	if (NULL == CreateIoCompletionPort((HANDLE)sobj->sock, hComport, (ULONG_PTR)sobj, 0))
	{
		CMCloseSocket(sobj);
		freeSObj(sobj);
		freeBObj(bobj);
		return;
	}

	bobj->dwRecvedCount += dwTranstion;

	SOCKADDR* localAddr,
		*remoteAddr;
	localAddr = NULL;
	remoteAddr = NULL;
	int localAddrlen,
		remoteAddrlen;

	lpfnGetAcceptExSockaddrs(bobj->data, bobj->datalen - ((sizeof(sockaddr_in) + 16) * 2),
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		&localAddr, &localAddrlen,
		&remoteAddr, &remoteAddrlen);

	int Headerlen = CheckNCData(bobj);
	if (0 == Headerlen)
	{
		bobj->SetIoRequestFunction(NC_RecvZeroCompFailed, NC_RecvZeroCompSuccess);
		if (!Nc_PostZeroRecv(sobj, bobj))
		{
			CMCloseSocket(sobj);
			freeSObj(sobj);
			freeBObj(bobj);
			return;
		}
	}
	else
	{
		TCHAR* pResponData = Utf8ConvertAnsi(bobj->data + Headerlen, bobj->dwRecvedCount - Headerlen);
		doNcResponse(bobj); // 返回推送反馈报文
		doNcData(pResponData);
	}
}

void NC_RecvZeroCompFailed(void* _sobj, void* _bobj)
{
	SOCKET_OBJ* c_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

#ifdef _DEBUG
	DWORD dwTranstion = 0;
	DWORD dwFlags = 0;
	if (!WSAGetOverlappedResult(c_sobj->sock, &c_bobj->ol, &dwTranstion, FALSE, &dwFlags))
		_tprintf(_T("函数:%s ErrorCode = %d\n"), __FUNCTION__, WSAGetLastError());
#endif

	CMCloseSocket(c_sobj);
	freeSObj(c_sobj);
	freeBObj(c_bobj);
}

void NC_RecvZeroCompSuccess(DWORD dwTransion, void* _sobj, void* _bobj)
{
	SOCKET_OBJ* c_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

	c_bobj->SetIoRequestFunction(NC_RecvCompFailed, NC_RecvCompSuccess);
	if (!Nc_PostRecv(c_sobj, c_bobj))
	{
		CMCloseSocket(c_sobj);
		freeSObj(c_sobj);
		freeBObj(c_bobj);
		return;
	}
}

void NC_RecvCompFailed(void* _sobj, void* _bobj)
{
	SOCKET_OBJ* c_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

#ifdef _DEBUG
	DWORD dwTranstion = 0;
	DWORD dwFlags = 0;
	if (!WSAGetOverlappedResult(c_sobj->sock, &c_bobj->ol, &dwTranstion, FALSE, &dwFlags))
		_tprintf(_T("函数:%s ErrorCode = %d\n"), __FUNCTION__, WSAGetLastError());
#endif

	CMCloseSocket(c_sobj);
	freeSObj(c_sobj);
	freeBObj(c_bobj);
}

void NC_RecvCompSuccess(DWORD dwTransion, void* _sobj, void* _bobj)
{
	if (dwTransion <= 0)
		return NC_RecvCompFailed(_sobj, _bobj);

	SOCKET_OBJ* sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;

	bobj->dwRecvedCount += dwTransion;

	int Headerlen = CheckNCData(bobj);
	if (0 == Headerlen)
	{
		bobj->SetIoRequestFunction(NC_RecvZeroCompFailed, NC_RecvZeroCompSuccess);
		if (!Nc_PostZeroRecv(sobj, bobj))
		{
			CMCloseSocket(sobj);
			freeSObj(sobj);
			freeBObj(bobj);
			return;
		}
	}
	else
	{
		TCHAR* pResponData = Utf8ConvertAnsi(bobj->data + Headerlen, bobj->dwRecvedCount - Headerlen);
		doNcResponse(bobj); // 返回推送反馈报文
		doNcData(pResponData);
	}
}

void NC_SendCompFailed(void* _sobj, void* _bobj)
{
	SOCKET_OBJ* c_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

#ifdef _DEBUG
	DWORD dwTranstion = 0;
	DWORD dwFlags = 0;
	if (!WSAGetOverlappedResult(c_sobj->sock, &c_bobj->ol, &dwTranstion, FALSE, &dwFlags))
		_tprintf(_T("函数:%s ErrorCode = %d\n"), __FUNCTION__, WSAGetLastError());
#endif

	CMCloseSocket(c_sobj);
	freeSObj(c_sobj);
	freeBObj(c_bobj);
}

void NC_SendCompSuccess(DWORD dwTransion, void* _sobj, void* _bobj)
{
	if (dwTransion <= 0)
		return NC_SendCompFailed(_sobj, _bobj);

	SOCKET_OBJ* c_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

	c_bobj->dwSendedCount += dwTransion;
	if (c_bobj->dwSendedCount < c_bobj->dwRecvedCount)
	{
		if (!Nc_PostSend(c_sobj, c_bobj))
		{
			CMCloseSocket(c_sobj);
			freeSObj(c_sobj);
			freeBObj(c_bobj);
			return;
		}
		return;
	}

	CMCloseSocket(c_sobj);
	freeSObj(c_sobj);
	freeBObj(c_bobj);
	return;
	
	c_bobj->dwRecvedCount = 0;
	c_bobj->dwSendedCount = 0;
	c_bobj->SetIoRequestFunction(NC_RecvZeroCompFailed, NC_RecvZeroCompSuccess);
	if (!Nc_PostZeroRecv(c_sobj, c_bobj))
	{
		CMCloseSocket(c_sobj);
		freeSObj(c_sobj);
		freeBObj(c_bobj);
		return;
	}
}