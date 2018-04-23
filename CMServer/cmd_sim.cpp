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
	case SIM_LIST:
	{
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		unsigned int nId = (pArray++)->as<unsigned int>();
		unsigned int nUsertype = (pArray++)->as<unsigned int>();

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		if (nUsertype == 1) // 超级管理员
		{
			pSql = _T("SELECT COUNT(*) AS num FROM sim_tbl WHERE Fatherid=%u");
		}
		else if (nUsertype == 2)
		{
			pSql = _T("SELECT COUNT(*) AS num FROM user_tbl WHERE Fatherid=%u");
		}
		else
		{
			pSql = _T("SELECT COUNT(*) AS num FROM sim_tbl WHERE proxy01=%u");
		}
		_stprintf_s(sql, sizeof(sql), pSql, nId);
	}
	break;
	default:
		break;
	}
	return true;
}