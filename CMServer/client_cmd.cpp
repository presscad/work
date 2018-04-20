#include "stdafx.h"
#include <msgpack.hpp>
#include "client_signaldata.h"
#include "client_cmd.h"
#include "global_data.h"
#include "client_mem.h"
#include "cmd_user.h"

void Client_CmdCompletionFailed(void* bobj_, void* _bobj)
{
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;
	SOCKET_OBJ* sobj = bobj->pRelateClientSock;
	
	if (0 == InterlockedDecrement(&sobj->nRef))
	{
		CMCloseSocket(sobj);
		freeSObj(sobj);
	}
	freeBObj(bobj);
}

void Client_CmdCompletionSuccess(DWORD dwTranstion, void* bobj_, void* _bobj)
{
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;
	try
	{
		msgpack::unpacker unpack_;
		msgpack::object_handle result_;
		unpack_.reserve_buffer(bobj->dwRecvedCount - 2);
		memcpy_s(unpack_.buffer(), bobj->dwRecvedCount - 2, bobj->data, bobj->dwRecvedCount - 2);
		unpack_.buffer_consumed(bobj->dwRecvedCount - 2);
		unpack_.next(result_);
		if (msgpack::type::ARRAY != result_.get().type)
			goto error;

		msgpack::object* pArray = result_.get().via.array.ptr;
		bobj->nCmd = (pArray++)->as<int>();
		switch (bobj->nCmd)
		{
		case 1:
		{
			cmd_user(pArray, bobj);
		}
		break;
		default:
			break;
		}
	}
	catch (const std::exception&)
	{

	}

error:
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> msgPack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	msgPack.pack_array(3);
	msgPack.pack(0xbb);
	msgPack.pack(0xbb);
	msgPack.pack(0);

	DealTail(sbuf, bobj);
}