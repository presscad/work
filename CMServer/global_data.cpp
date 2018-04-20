#include "stdafx.h"
#include "client_signaldata.h"
#include "global_data.h"
#include <random>

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