#pragma once

#define CLIENT_PORT	6086
#define API_PORT	7086
#define ACCEPT_NUM	10
#define MAX_ACCEPT_NUM	100

extern HANDLE hComport;
extern DWORD g_dwPagesize;

extern LPFN_ACCEPTEX lpfnAccpetEx;
extern LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockaddrs;
extern LPFN_CONNECTEX lpfnConnectEx;

int GetRand();

void CMCloseSocket(void* client_sock);

void DoSend(BUFFER_OBJ* bobj);

void DealTail(msgpack::sbuffer& sBuf, BUFFER_OBJ* bobj);

int error_info(BUFFER_OBJ* buffer, const TCHAR* format, ...);