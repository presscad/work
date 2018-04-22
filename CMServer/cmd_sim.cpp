#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_sim.h"
#include "cm_mysql.h"
#include "global_data.h"

bool cmd_sim(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	switch (bobj->nCmd)
	{
	case SIM_ADD:
	{
		
	}
	break;
	default:
		break;
	}
	return true;
}