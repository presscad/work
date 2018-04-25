#include "stdafx.h"
#include <msgpack.hpp>
#include "client_signaldata.h"
#include "cmd_error.h"
#include "global_data.h"

bool cmd_error(BUFFER_OBJ* bobj, int nTag/* = 0*/)
{
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);

	if (nTag == 0)
	{
		_msgpack.pack_array(4);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(1);
		_msgpack.pack(bobj->data);
	}
	else
	{
		_msgpack.pack_array(6);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		//_msgpack.pack(bobj->nSubSubCmd);
		_msgpack.pack(nTag);
		_msgpack.pack(1);
		_msgpack.pack(0);
		_msgpack.pack(bobj->data);
	}

	DealTail(sbuf, bobj);
	return false;
}