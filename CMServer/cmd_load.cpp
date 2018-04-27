#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_load.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"

unsigned int GetIndexByName(const TCHAR* name , BUFFER_OBJ* bobj)
{
	const TCHAR* pSql = NULL;
	TCHAR sql[256];
	memset(sql, 0x00, sizeof(sql));

	pSql = _T("SELECT id FROM kh_tbl WHERE Khmc='%s'");
	_stprintf_s(sql, sizeof(sql), pSql, name);

	MYSQL* pMysql = Mysql_AllocConnection();
	if (NULL == pMysql)
	{
		error_info(bobj, _T("连接数据库失败"));
		return 0;
	}

	MYSQL_RES* res = NULL;
	if (!SelectFromTbl(sql, pMysql, bobj, &res))
	{
		Mysql_BackToPool(pMysql);
		return 0;
	}
	MYSQL_ROW row = mysql_fetch_row(res);
	mysql_free_result(res);

	unsigned int nNum = 0;
	sscanf_s(row[0], "%u", &nNum);
	Mysql_BackToPool(pMysql);

	return nNum;
}

bool cmd_load(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	unsigned int nId = (pRootArray++)->as<unsigned int>();
	unsigned int nUsertype = (pRootArray++)->as<unsigned int>();
	int nIndex = (pRootArray++)->as<int>();

	switch (bobj->nSubCmd)
	{
	case EXCEL_SIM:
	{
		std::string strLlchm = (pRootArray++)->as<std::string>();
		std::string strLlclx = (pRootArray++)->as<std::string>();
		int nArraySize = pRootArray->via.array.size;
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			return cmd_error(bobj, nIndex);
		}
		mysql_autocommit(pMysql, 0);

		for (int i = 0; i < nArraySize; i++)
		{
			msgpack::object* pArray = (pDataArray++)->via.array.ptr;
			std::string strJrhm = (pArray++)->as<std::string>();
			std::string strIccid = (pArray++)->as<std::string>();
			std::string strDxzh = (pArray++)->as<std::string>();
			//std::string strLlchm = (pArray++)->as<std::string>();
			//std::string strLlclx = (pArray++)->as<std::string>();
			// insert into sim_tbl (id,jrhm,iccid,dxzh,llchm,llclx) value(null,'%s','%s','%s','%s','%s') ON DUPLICATE KEY UPDATE iccid='%s'
			const TCHAR* pSql = _T("INSERT INTO sim_tbl (id,Jrhm,Iccid,Dxzh,Llchm,Llclx) VALUES(null,'%s','%s','%s','%s','%s')  ON DUPLICATE KEY UPDATE Iccid='%s'");
			TCHAR sql[256];
			memset(sql, 0x00, sizeof(sql));
			_stprintf_s(sql, sizeof(sql), pSql, strJrhm.c_str(), strIccid.c_str(), strDxzh.c_str(), strLlchm.c_str(), strLlclx.c_str(), strIccid.c_str());
			if (!InsertIntoTbl(sql, pMysql, bobj))
			{
				mysql_rollback(pMysql);
				mysql_autocommit(pMysql, 1);
				Mysql_BackToPool(pMysql);
				return cmd_error(bobj, nIndex);
			}
		}

		mysql_commit(pMysql);
		mysql_autocommit(pMysql, 1);
		Mysql_BackToPool(pMysql);

		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(4);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(nIndex);
		_msgpack.pack(0);
		DealTail(sbuf, bobj);
	}
	break;

	case EXCEL_XSQD:
	{
		int nArraySize = pRootArray->via.array.size;
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			return cmd_error(bobj, nIndex);
		}
		mysql_autocommit(pMysql, 0);

		for (int i = 0; i < nArraySize; i++)
		{
			msgpack::object* pArray = (pDataArray++)->via.array.ptr;
			std::string strJrhm = (pArray++)->as<std::string>();
			std::string strKhmc = (pArray++)->as<std::string>();
			std::string strXsrq = (pArray++)->as<std::string>();
			std::string strJlxm = (pArray++)->as<std::string>();
			unsigned int Khid = GetIndexByName(strKhmc.c_str(), bobj);
			if (0 == Khid)
			{
				mysql_rollback(pMysql);
				mysql_autocommit(pMysql, 1);
				Mysql_BackToPool(pMysql);
				return cmd_error(bobj, nIndex);
			}
			const TCHAR* pSql = _T("UPDATE sim_tbl SET Khid01=%u,Jlxm='%s',Xsrq='%s' WHERE Jrhm='%s'");
			TCHAR sql[256];
			memset(sql, 0x00, sizeof(sql));
			_stprintf_s(sql, sizeof(sql), pSql, Khid, strJlxm.c_str(), strXsrq.c_str(), strJrhm.c_str());
			if (!UpdateTbl(sql, pMysql, bobj))
			{
				mysql_rollback(pMysql);
				mysql_autocommit(pMysql, 1);
				Mysql_BackToPool(pMysql);
				return cmd_error(bobj, nIndex);
			}
		}

		mysql_commit(pMysql);
		mysql_autocommit(pMysql, 1);
		Mysql_BackToPool(pMysql);

		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(4);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(nIndex);
		_msgpack.pack(0);
		DealTail(sbuf, bobj);
	}
	break;
	case EXCEL_XFQD:
	{
		int nArraySize = pRootArray->via.array.size;
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			return cmd_error(bobj, nIndex);
		}
		mysql_autocommit(pMysql, 0);

		for (int i = 0; i < nArraySize; i++)
		{
			msgpack::object* pArray = (pDataArray++)->via.array.ptr;
			std::string strJrhm = (pArray++)->as<std::string>();
			std::string strXfrq = (pArray++)->as<std::string>();
			int nMonth = (pArray++)->as<int>();
			std::string strBz = (pArray++)->as<std::string>();
			const TCHAR* pSql = _T("UPDATE sim_tbl SET Xfrq='%s',Dqrq=DATE_ADD(IF('%s'>IFNULL(Dqrq,'1988-01-01'),'%s',Dqrq),INTERVAL %d MONTH),Bz='%s' WHERE Jrhm='%s'");
			TCHAR sql[256];
			memset(sql, 0x00, sizeof(sql));
			_stprintf_s(sql, sizeof(sql), pSql, strXfrq.c_str(), strXfrq.c_str(), strXfrq.c_str(), nMonth, strBz.c_str(), strJrhm.c_str());
			if (!UpdateTbl(sql, pMysql, bobj))
			{
				mysql_rollback(pMysql);
				mysql_autocommit(pMysql, 1);
				Mysql_BackToPool(pMysql);
				return cmd_error(bobj, nIndex);
			}
		}

		mysql_commit(pMysql);
		mysql_autocommit(pMysql, 1);
		Mysql_BackToPool(pMysql);

		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(4);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(nIndex);
		_msgpack.pack(0);
		DealTail(sbuf, bobj);
	}
	break;
	default:
		break;
	}

	return true;
}