#pragma once
#include <tbb\concurrent_hash_map.h>

typedef struct _key_value
{
	int nKey;
	SOCKET sock;
}KEY_VALUE;

typedef struct _listen_sock
{
	SOCKET sListenSock;
	HANDLE hPostAcceptExEvent;
	tbb::concurrent_hash_map<int, void*> acceptPendingMap;

	void Init()
	{
		if (INVALID_SOCKET != sListenSock)
		{
			closesocket(sListenSock);
			sListenSock = INVALID_SOCKET;
		}
		hPostAcceptExEvent = NULL;
		acceptPendingMap.clear();
	}

	void InsertIntoPendingMap(void* sock_obj)
	{
		tbb::concurrent_hash_map<int, void*>::accessor a;
		acceptPendingMap.insert(a, ((KEY_VALUE*)sock_obj)->nKey);
		a->second = sock_obj;
	}

	void DeleteFromPendingMap(void* sock_obj)
	{
		tbb::concurrent_hash_map<int, void*>::accessor a;
		if (acceptPendingMap.find(a, ((KEY_VALUE*)sock_obj)->nKey))
			acceptPendingMap.erase(a);
	}
}LISTEN_SOCK;

bool UniqueInstance();

bool InitWinsock2();

bool GetExtensionFunctionPointer();

bool InitClientListenSock(LISTEN_SOCK* lsock, USHORT nPort);

extern LISTEN_SOCK* LSock_Array[];
extern HANDLE hEvent_Array[];
extern DWORD g_dwListenCount;

#define CLIENT_PORT	6086
#define API_PORT	7086
#define ACCEPT_NUM	10
#define MAX_ACCEPT_NUM	100