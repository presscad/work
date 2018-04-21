#include "stdafx.h"
#include "init.h"
#include "client_signaldata.h"
#include "global_data.h"
#include "api_requestpost.h"
#include "client_mem.h"

extern void API_AcceptCompletionFailed(void* _lsock, void* _bobj);
extern void API_AcceptCompletionSuccess(DWORD dwTranstion, void* _lsock, void* _bobj);

bool Api_PostAcceptEx(void* _lsock)
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
	bobj->SetIoRequestFunction(API_AcceptCompletionFailed, API_AcceptCompletionSuccess);
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