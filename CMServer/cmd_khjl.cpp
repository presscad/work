#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_khjl.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"

void ParserKhjl(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);

bool cmd_khjl(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	switch (bobj->nSubCmd)
	{
	case KHJL_ADD:
	{
		
	}
	break;
	case KHJL_LIST:
	{
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		unsigned int nId = (pArray++)->as<unsigned int>();
		unsigned int nUsertype = (pArray++)->as<unsigned int>();

		if (nUsertype != 1)
		{
			error_info(bobj, _T("无权限"));
			return cmd_error(bobj, nIndex);
		}

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));

		pSql = _T("SELECT COUNT(*) AS num FROM khjl_tbl");
		_stprintf_s(sql, sizeof(sql), pSql);

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

		if (nAB == 0) // 首页
		{
			pSql = _T("SELECT id,Khmc,Lxfs,Ssdq,Jlxm,Dj FROM khjl_tbl WHERE id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, nKeyid, nPagesize);
		}
		else if (nAB == 1)
		{
			pSql = _T("SELECT id,Khmc,Lxfs,Ssdq,Jlxm,Dj FROM khjl_tbl WHERE id<%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, nKeyid, nPagesize);
		}
		else if (nAB == 2)
		{
			pSql = _T("SELECT id,Khmc,Lxfs,Ssdq,Jlxm,Dj FROM khjl_tbl WHERE id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, nKeyid, nPagesize);
		}
		else
		{
			unsigned int nTemp = nNum % nPagesize;
			pSql = _T("SELECT id,Khmc,Lxfs,Ssdq,Jlxm,Dj FROM khjl_tbl ORDER BY id desc LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, nPagesize);
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
			ParserKhjl(_msgpack, row);
			row = mysql_fetch_row(res);
		}
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	default:
		break;
	}
	return true;
}

void ParserKhjl(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row)
{

}