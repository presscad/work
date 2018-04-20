#pragma once

bool Client_PostAcceptEx(LISTEN_SOCK* lsock);

BOOL Client_PostZeroRecv(CLIENT_SOCK *_csock, CLIENT_BUF* _bobj);

BOOL Client_PostRecv(CLIENT_SOCK* _sobj, CLIENT_BUF* _bobj);

BOOL CLient_PostSend(SOCKET_OBJ* _sobj,  BUFFER_OBJ* _bobj);