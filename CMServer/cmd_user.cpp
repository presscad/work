#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_user.h"

bool cmd_user(msgpack::object* pArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pArray++)->as<int>();

	switch (bobj->nSubCmd)
	{
	default:
		break;
	}

	return true;
}