#pragma once

bool Nc_PostAcceptEx(void* lsock);

BOOL Nc_PostZeroRecv(SOCKET_OBJ* _sobj, BUFFER_OBJ* _bobj);

BOOL Nc_PostRecv(SOCKET_OBJ* _sobj, BUFFER_OBJ* _bobj);

BOOL Nc_PostSend(SOCKET_OBJ* _sobj, BUFFER_OBJ* _bobj);