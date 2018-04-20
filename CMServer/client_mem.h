#pragma once

void InitClientSockMem();

CLIENT_SOCK* allocClientSock(DWORD dwSize);

#define allocSObj allocClientSock

void freeClientSock(CLIENT_SOCK* cleint_sock);

#define freeSObj freeClientSock

CLIENT_BUF* allocClientBuf(DWORD dwSize);

#define allocBObj allocClientBuf

void freeClientBuf(CLIENT_BUF* client_buf);

#define freeBObj freeClientBuf