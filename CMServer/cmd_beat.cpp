#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_beat.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"

bool cmd_beat(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(1);
	_msgpack.pack(0);
	_tprintf_s(_T("ÐÄÌø\n"));
	DealTail(sbuf, bobj);
	return 0;
}