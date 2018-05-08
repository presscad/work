#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_sim.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"

unsigned int GetIndexByName(const TCHAR* name, BUFFER_OBJ* bobj);

bool cmd_sim(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	std::string strId = (pRootArray++)->as<std::string>();
	unsigned int nId = 0;
	sscanf_s(strId.c_str(), "%u", &nId);
	std::string strUsertype = (pRootArray++)->as<std::string>();
	unsigned int nUsertype = 0;
	sscanf_s(strUsertype.c_str(), "%u", &nUsertype);
	switch (bobj->nSubCmd)
	{
	case SIM_ADD:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strJrhm = (pArray++)->as<std::string>();
		std::string strKhmc = (pArray++)->as<std::string>();
		std::string strJlxm = (pArray++)->as<std::string>();
		std::string strXsrq = (pArray++)->as<std::string>();
		std::string strBz = (pArray++)->as<std::string>();

		unsigned int Khid = GetIndexByName(strKhmc.c_str(), bobj);

		if (0 == Khid)
		{
			
		}

		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			return cmd_error(bobj);
		}

		const TCHAR* pSql = _T("UPDATE sim_tbl SET Khid01=%u,Jlxm='%s',Xsrq='%s',Bz='%s' WHERE Jrhm='%s'");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, Khid, strJlxm.c_str(), strXsrq.c_str(), strBz.c_str(), strJrhm.c_str());
		if (!UpdateTbl(sql, pMysql, bobj))
		{
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj);
		}
	}
	break;
	case SIM_LIST:
	{
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		unsigned int nKhid = (pArray++)->as<unsigned int>();
		std::string strKhmc = (pArray++)->as<std::string>();

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		if (nUsertype == 1)
		{
			pSql = _T("SELECT COUNT(*) AS num FROM sim_tbl");
			_stprintf_s(sql, sizeof(sql), pSql);
		}
		else if (nUsertype == 2)
		{
			pSql = _T("SELECT COUNT(*) AS num FROM sim_tbl WHERE Khid01=%u");
			_stprintf_s(sql, sizeof(sql), pSql, nKhid);
		}
		else if (nUsertype == 3)
		{
			pSql = _T("SELECT COUNT(*) AS num FROM sim_tbl WHERE Khid02=%u");
			_stprintf_s(sql, sizeof(sql), pSql, nKhid);
		}

		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			return cmd_error(bobj, nIndex);
		}

		MYSQL_RES* res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj, nIndex);
		}
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);

		unsigned int nNum = 0;
		sscanf_s(row[0], "%u", &nNum);

		if (nUsertype == 1)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nTemp);
			}
		}
		else if (nUsertype == 2)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nTemp);
			}
		}
		else if (nUsertype == 3)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nTemp);
			}
		}

		res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj, nIndex);
		}

		Mysql_BackToPool(pMysql);

		unsigned int nRows = (unsigned int)mysql_num_rows(res);
		row = mysql_fetch_row(res);
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(6);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(nIndex);
		_msgpack.pack(0);
		_msgpack.pack(nNum);
		_msgpack.pack_array(nRows);
		while (row)
		{
			ParserSim(_msgpack, row, SIM_SELECT_SIZE);
			row = mysql_fetch_row(res);
		}
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	case SIM_JRHM:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strJrhm = (pArray++)->as<std::string>();

		const TCHAR* pSql = _T("SELECT a.id,a.Jrhm,a.Iccid,a.Dxzh,b.Khmc,a.Jlxm,a.Zt,a.Llchm,a.Llclx,b.Dj,a.Xsrq,a.Jhrq,a.Xfrq,a.Dqrq,a.Zxrq,a.Bz FROM (SELECT %s,Jlxm,Khid01 FROM sim_tbl WHERE Jrhm='%s') a LEFT JOIN kh_tbl b ON a.Khid01=b.id");
		TCHAR sql[512];
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, strJrhm.c_str());
		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{ 
			error_info(bobj, _T("连接数据库失败"));
			return cmd_error(bobj);
		}

		MYSQL_RES* res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj);
		}
		MYSQL_ROW row = mysql_fetch_row(res);

		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(4);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(0);
		_msgpack.pack_array(1);
		ParserSim(_msgpack, row, 16);
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	default:
		break;
	}
	return true;
}