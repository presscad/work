#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_khjl.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"

bool cmd_khjl(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	unsigned int nId = (pRootArray++)->as<unsigned int>();
	unsigned int nUsertype = (pRootArray++)->as<unsigned int>();
	switch (bobj->nSubCmd)
	{
	case KHJL_ADD:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strJlxm = (pArray++)->as<std::string>();
		std::string strLxfs = (pArray++)->as<std::string>();
		std::string strBz = (pArray++)->as<std::string>();

		if (nUsertype != 1)
		{
			error_info(bobj, _T("无权限"));
			return cmd_error(bobj);
		}

		const TCHAR* pSql = _T("INSERT INTO khjl_tbl (id,Jlxm,Lxfs,Xgsj,Bz) VALUES(null,'%s','%s',now(),'%s')");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, strJlxm.c_str(), strLxfs.c_str(), strBz.c_str());

		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			return cmd_error(bobj);
		}

		if (!InsertIntoTbl(sql, pMysql, bobj))
		{
			UINT uError = mysql_errno(pMysql);
			if (uError == 1062)
			{
				error_info(bobj, _T("客户经理已存在"));
			}
			else
			{
				error_info(bobj, _T("数据库异常 ErrorCode = %08x, ErrorMsg = %s"), uError, mysql_error(pMysql));
			}
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj);
		}
		my_ulonglong nIndex = mysql_insert_id(pMysql); // 新添加的用户的id

		pSql = _T("SELECT %s FROM khjl_tbl WHERE id=%u");
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, KHJL_SELECT, (unsigned int)nIndex);

		MYSQL_RES* res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj);
		}

		Mysql_BackToPool(pMysql);

		MYSQL_ROW row = mysql_fetch_row(res);
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(4);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(0);
		_msgpack.pack_array(1);
		ParserLlc(_msgpack, row, KHJL_SELECT_SIZE);
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	case KHJL_LIST:
	{
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

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
			pSql = _T("SELECT %s FROM khjl_tbl WHERE id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, KHJL_SELECT, nKeyid, nPagesize);
		}
		else if (nAB == 1)
		{
			pSql = _T("SELECT %s FROM khjl_tbl WHERE id<%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, KHJL_SELECT, nKeyid, nPagesize);
		}
		else if (nAB == 2)
		{
			pSql = _T("SELECT %s FROM khjl_tbl WHERE id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, KHJL_SELECT, nKeyid, nPagesize);
		}
		else
		{
			unsigned int nTemp = nNum % nPagesize;
			pSql = _T("SELECT %s FROM khjl_tbl ORDER BY id desc LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, KHJL_SELECT, nPagesize);
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
			ParserKhjl(_msgpack, row, KHJL_SELECT_SIZE);
			row = mysql_fetch_row(res);
		}
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	case KHJL_KH:
	{
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		const TCHAR* pSql = pSql = _T("SELECT COUNT(*) num FROM kh_tbl WHERE Jlxm='%s'");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strJlxm = (pArray++)->as<std::string>();

		if (nUsertype == 1) // 超级管理员
			nId = 1;

		_stprintf_s(sql, sizeof(sql), pSql, strJlxm.c_str());

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
			pSql = _T("SELECT %s FROM kh_tbl WHERE Jlxm='%s' AND id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, strJlxm.c_str(), nKeyid, nPagesize);
		}
		else if (nAB == 1)
		{
			pSql = _T("SELECT %s FROM kh_tbl WHERE Jlxm='%s' AND id<%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, strJlxm.c_str(), nKeyid, nPagesize);
		}
		else if (nAB == 2)
		{
			pSql = _T("SELECT %s FROM kh_tbl WHERE Jlxm='%s' AND id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, strJlxm.c_str(), nKeyid, nPagesize);
		}
		else
		{
			unsigned int nTemp = nNum % nPagesize;
			pSql = _T("SELECT %s FROM kh_tbl WHERE Jlxm='%s' ORDER BY id desc LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, strJlxm.c_str(), nPagesize);
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
			ParserKh(_msgpack, row, KH_SELECT_SIZE);
			row = mysql_fetch_row(res);
		}
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	case KHJL_QUERY:
	{
		_T("SELECT Xsrq,COUNT(*) num FROM sim_tbl WHERE Khid IN (SELECT id FROM kh_tbl WHERE Jlxm='%s') GROUP BY Xsrq)");
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		unsigned int nKhid = (pArray++)->as<unsigned int>();

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		if (nUsertype == 1)
		{
			pSql = _T("SELECT Xsrq,COUNT(*) num FROM sim_tbl WHERE Jlxm='%s' GROUP BY Xsrq)");
		}
		else
		{
			error_info(bobj, _T("无权限"));
			return cmd_error(bobj);
		}
		_stprintf_s(sql, sizeof(sql), pSql, nKhid);

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

		Mysql_BackToPool(pMysql);

		unsigned int nRows = (unsigned int)mysql_num_rows(res);
		MYSQL_ROW row = mysql_fetch_row(res);
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(6);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(nIndex);
		_msgpack.pack(0);
		_msgpack.pack(nRows);
		_msgpack.pack_array(nRows);
		while (row)
		{
			ParserXsrq(_msgpack, row, XSRQ_SELECT_SIZE);
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