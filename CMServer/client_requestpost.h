#pragma once

bool Client_PostAcceptEx(void* _lsock);

BOOL Client_PostZeroRecv(CLIENT_SOCK *_csock, CLIENT_BUF* _bobj);

BOOL Client_PostRecv(CLIENT_SOCK* _sobj, CLIENT_BUF* _bobj);

BOOL CLient_PostSend(CLIENT_SOCK* _sobj,  BUFFER_OBJ* _bobj);