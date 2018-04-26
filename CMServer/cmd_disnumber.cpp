#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_disnumber.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"

bool cmd_disnumber(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	unsigned int nId = (pRootArray++)->as<unsigned int>();
	unsigned int nUsertype = (pRootArray++)->as<unsigned int>();
	switch (bobj->nSubCmd)
	{
	case DN_DISABLE:
	{}
	break;
	case DN_ABLE:
	{}
	break;
	default:
		break;
	}
	return true;
}