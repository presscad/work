#pragma once

void InitClientSockMem();

CLIENT_SOCK* allocClientSock(DWORD dwSize);

void freeClientSock(CLIENT_SOCK* cleint_sock);

CLIENT_BUF* allocClientBuf();

void freeClientBuf(CLIENT_BUF* client_buf);

SOCKET_OBJ* allocSObj();

void freeSObj(SOCKET_OBJ* sobj);

BUFFER_OBJ* allocBObj(DWORD dwSize);

void freeBObj(BUFFER_OBJ* bobj);