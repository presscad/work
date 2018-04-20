#pragma once

void InitClientSockMem();

CLIENT_SOCK* allocClientSock(DWORD dwSize);

void freeClientSock(CLIENT_SOCK* cleint_sock);