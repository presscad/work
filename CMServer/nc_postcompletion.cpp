#include "stdafx.h"
#include "client_signaldata.h"
#include "init.h"
#include "global_data.h"
#include "nc_postcompletion.h"
#include "client_mem.h"
#include "nc_requestpost.h"
#include "cmd_ncdata.h"

extern TCHAR* Utf8ConvertAnsi(const TCHAR* strIn, int inLen);

extern struct tcp_keepalive alive_in;
extern struct tcp_keepalive alive_out;
extern unsigned long ulBytesReturn;

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
	_tprintf_s(_T("NC--%s\n"), "testtest");
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

	TCHAR* pResponData = Utf8ConvertAnsi(bobj->data, bobj->dwRecvedCount);
	_tprintf_s(_T("NC--%s\n"), pResponData);
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(pResponData))
	{
		delete pResponData;
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
		delete pResponData;
		doNcResponse(bobj); // 返回推送反馈报文
		doNcData(doc);
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

	SOCKET_OBJ* c_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

	c_bobj->dwRecvedCount += dwTransion;

	TCHAR* pResponData = Utf8ConvertAnsi(c_bobj->data, c_bobj->dwRecvedCount);
	_tprintf_s(_T("NC--%s\n"), pResponData);
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(pResponData))
	//if (NULL == strstr(c_bobj->data, "</NotifyContractRoot>") && NULL == strstr(c_bobj->data, "</WanrningContractRoot>"))
	{
		delete pResponData;
		c_bobj->SetIoRequestFunction(NC_RecvZeroCompFailed, NC_RecvZeroCompSuccess);
		if (!Nc_PostZeroRecv(c_sobj, c_bobj))
		{
			CMCloseSocket(c_sobj);
			freeSObj(c_sobj);
			freeBObj(c_bobj);
			return;
		}
	}
	else
	{
		delete pResponData;
		doNcResponse(c_bobj); // 返回推送反馈报文
		doNcData(doc);
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