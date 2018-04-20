#include "stdafx.h"
#include <msgpack.hpp>
#include "client_signaldata.h"
#include "client_cmd.h"

void Client_CmdCompletionFailed(void* bobj_, void* _bobj)
{

}

void Client_CmdCompletionSuccess(DWORD dwTranstion, void* bobj_, void* _bobj)
{
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;
	msgpack::unpacker unpack_;
	msgpack::object_handle result_;
	unpack_.reserve_buffer(bobj->dwRecvedCount - 2);
	memcpy_s(unpack_.buffer(), bobj->dwRecvedCount - 2, bobj->data, bobj->dwRecvedCount - 2);
	unpack_.buffer_consumed(bobj->dwRecvedCount - 2);
	unpack_.next(result_);
}