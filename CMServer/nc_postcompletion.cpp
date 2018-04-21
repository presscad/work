#include "stdafx.h"
#include "client_signaldata.h"
#include "init.h"
#include "global_data.h"
#include "nc_postcompletion.h"
#include "client_mem.h"

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

	if (NULL == strstr(bobj->data, "</NotifyContractRoot>") && NULL == strstr(bobj->data, "</WanrningContractRoot>"))
	{
		//c_bobj->SetIoRequestFunction(NC_RecvZeroCompFailed, NC_RecvZeroCompSuccess);
		//if (!PostZeroRecv(c_sobj, c_bobj))
		{
			CMCloseSocket(sobj);
			freeSObj(sobj);
			freeBObj(bobj);
			return;
		}
	}
	else
	{
		//doNcData(c_bobj);
	}
}