#include "stdafx.h"
#include "client_signaldata.h"
#include "global_data.h"
#include <random>
#include "init.h"
#include "client_requestpost.h"
#include "client_postcompletion.h"

HANDLE hComport = NULL;
DWORD g_dwPagesize = 0;

std::random_device randdev;
std::default_random_engine randeg(randdev());
std::uniform_int_distribution<int> urand(0);

int GetRand()
{
	return urand(randeg);
}

void CMCloseSocket(CLIENT_SOCK* client_sock)
{
	if (INVALID_SOCKET != client_sock->sock)
	{
		closesocket(client_sock->sock);
		client_sock->sock = INVALID_SOCKET;
	}
}

void DoSend(BUFFER_OBJ* bobj)
{
	SOCKET_OBJ* c_sobj = bobj->pRelateClientSock;
	if (c_sobj->CheckSend(bobj))
	{
		bobj->SetIoRequestFunction(Client_SendCompletionFailed, Client_SendCompletionSuccess);
		if (!CLient_PostSend(c_sobj, bobj))
		{
			Client_SendCompletionFailed(c_sobj, bobj);
		}
	}
}

byte csum(unsigned char *addr, int count)
{
	byte sum = 0;
	for (int i = 0; i< count; i++)
	{
		sum += (byte)addr[i];
	}
	return sum;
}

void DealTail(msgpack::sbuffer& sBuf, BUFFER_OBJ* bobj)
{
	char* pCh = sBuf.data();
	int nLen = sBuf.size();
	byte pData[1024 * 32];
	memset(pData, 0, 1024 * 32);
	memcpy(pData, pCh + 6, nLen - 6);
	byte nSum = csum(pData, nLen - 6);
	sBuf.write("\x00", 1);
	memcpy(pCh + nLen, &nSum, 1);
	sBuf.write("\x0d", 1);
	nLen = sBuf.size();
	nLen -= 8;
	memcpy(pCh + 2, &nLen, 4);
	nLen += 8;

	memcpy(bobj->data, pCh, nLen);// �Ż�
	bobj->dwRecvedCount = nLen;
	DoSend(bobj);
}