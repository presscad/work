#pragma once

extern HANDLE hComport;
extern DWORD g_dwPagesize;

extern LPFN_ACCEPTEX lpfnAccpetEx;
extern LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockaddrs;
extern LPFN_CONNECTEX lpfnConnectEx;

int GetRand();

void CMCloseSocket(CLIENT_SOCK* client_sock);

void DoSend(BUFFER_OBJ* bobj);

void DealTail(msgpack::sbuffer& sBuf, BUFFER_OBJ* bobj);