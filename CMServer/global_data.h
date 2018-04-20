#pragma once

extern HANDLE hComport;
extern DWORD g_dwPagesize;

extern LPFN_ACCEPTEX lpfnAccpetEx;
extern LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockaddrs;
extern LPFN_CONNECTEX lpfnConnectEx;

int GetRand();

void CMCloseSocket(CLIENT_SOCK* client_sock);