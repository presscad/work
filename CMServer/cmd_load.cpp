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

std::string GetLlchmByJrhm(const TCHAR* jrhm, BUFFER_OBJ* bobj)
{
	const TCHAR* pSql = NULL;
	TCHAR sql[256];
	memset(sql, 0x00, sizeof(sql));

	pSql = _T("SELECT Llchm FROM sim_tbl WHERE Jrhm='%s'");
	_stprintf_s(sql, sizeof(sql), pSql, jrhm);

	MYSQL* pMysql = Mysql_AllocConnection();
	if (NULL == pMysql)
	{
		error_info(bobj, _T("连接数据库失败"));
		return "";
	}

	MYSQL_RES* res = NULL;
	if (!SelectFromTbl(sql, pMysql, bobj, &res))
	{
		Mysql_BackToPool(pMysql);
		return "";
	}
	MYSQL_ROW row = mysql_fetch_row(res);
	mysql_free_result(res);

	Mysql_BackToPool(pMysql);

	return std::string(row[0]);
}

bool cmd_load(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	std::string strId = (pRootArray++)->as<std::string>();
	unsigned int nId = 0;
	sscanf_s(strId.c_str(), "%u", &nId);
	std::string strUsertype = (pRootArray++)->as<std::string>();
	unsigned int nUsertype = 0;
	sscanf_s(strUsertype.c_str(), "%u", &nUsertype);
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
			goto error;
		}
		mysql_autocommit(pMysql, 0);

		const TCHAR* pSql = _T("INSERT INTO sim_tbl (id,Jrhm,Iccid,Dxzh,Llchm,Llclx) VALUES(null,'%s','%s','%s','%s','%s')  ON DUPLICATE KEY UPDATE Iccid='%s'");
		TCHAR sql[256];
		int nNewData = 0;

		for (int i = 0; i < nArraySize; i++)
		{
			msgpack::object* pArray = (pDataArray++)->via.array.ptr;
			std::string strJrhm = (pArray++)->as<std::string>();
			std::string strIccid = (pArray++)->as<std::string>();
			std::string strDxzh = (pArray++)->as<std::string>();
			memset(sql, 0x00, sizeof(sql));
			_stprintf_s(sql, sizeof(sql), pSql, strJrhm.c_str(), strIccid.c_str(), strDxzh.c_str(), strLlchm.c_str(), strLlclx.c_str(), strIccid.c_str());

			if (!Trans_InsertIntoTbl(sql, pMysql, bobj))
			{
				mysql_rollback(pMysql);
				mysql_autocommit(pMysql, 1);
				Mysql_BackToPool(pMysql);
				goto error;
			}

			// nAffectedRows = 0重复导入,但是没有更新
			// nAffectedRows = 1新插入数据
			// nAffectedRows = 2更新数据
			int nAffectedRows = (int)mysql_affected_rows(pMysql);
			if (1 == nAffectedRows)
				++nNewData;
		}

		pSql = _T("UPDATE llc_tbl SET Kzsl=Kzsl+%d WHERE Llchm='%s'");
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, nNewData, strLlchm.c_str());
		if (!Trans_UpdateTbl(sql, pMysql, bobj))
		{
			mysql_rollback(pMysql);
			mysql_autocommit(pMysql, 1);
			Mysql_BackToPool(pMysql);
			goto error;
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
			goto error;
		}
		mysql_autocommit(pMysql, 0);

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		for (int i = 0; i < nArraySize; i++)
		{
			msgpack::object* pArray = (pDataArray++)->via.array.ptr;
			std::string strJrhm = (pArray++)->as<std::string>();
			std::string strKhmc = (pArray++)->as<std::string>();
			std::string strXsrq = (pArray++)->as<std::string>();
			std::string strJlxm = (pArray++)->as<std::string>();
			unsigned int Khid = GetIndexByName(strKhmc.c_str(), bobj);
			std::string strBz = (pArray++)->as<std::string>();
			std::string strLlchm = GetLlchmByJrhm(strJrhm.c_str(), bobj);
			if (0 == Khid || "" == strLlchm)
			{
				mysql_rollback(pMysql);
				mysql_autocommit(pMysql, 1);
				Mysql_BackToPool(pMysql);
				goto error;
			}
			pSql = _T("UPDATE sim_tbl SET Khid01=%u,Jlxm='%s',Xsrq='%s',Bz='%s' WHERE Jrhm='%s'");
			memset(sql, 0x00, sizeof(sql));
			_stprintf_s(sql, sizeof(sql), pSql, Khid, strJlxm.c_str(), strXsrq.c_str(), strBz.c_str(), strJrhm.c_str());
			if (!Trans_UpdateTbl(sql, pMysql, bobj))
			{
				mysql_rollback(pMysql);
				mysql_autocommit(pMysql, 1);
				Mysql_BackToPool(pMysql);
				goto error;
			}

			// nAffectedRows = 0 更新数据与数据库中数据完全一样
			// nAffectedRows = 1 数据被跟新
			int nAffectedRows = (int)mysql_affected_rows(pMysql);
			if (1 == nAffectedRows)
			{
				pSql = _T("UPDATE kh_tbl SET Kzsl=Kzsl+1 WHERE id=%u");
				memset(sql, 0x00, sizeof(sql));
				_stprintf_s(sql, sizeof(sql), pSql, Khid);
				if (!Trans_UpdateTbl(sql, pMysql, bobj))
				{
					mysql_rollback(pMysql);
					mysql_autocommit(pMysql, 1);
					Mysql_BackToPool(pMysql);
					goto error;
				}

				pSql = _T("UPDATE llc_tbl SET Kysl=Kysl+1 WHERE Llchm='%s'");
				memset(sql, 0x00, sizeof(sql));
				_stprintf_s(sql, sizeof(sql), pSql, strLlchm.c_str());
				if (!Trans_UpdateTbl(sql, pMysql, bobj))
				{
					mysql_rollback(pMysql);
					mysql_autocommit(pMysql, 1);
					Mysql_BackToPool(pMysql);
					goto error;
				}
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
			goto error;
		}
		mysql_autocommit(pMysql, 0);

		const TCHAR* pSql = _T("UPDATE sim_tbl SET Xfrq='%s',Dqrq=DATE_ADD(IF('%s'>IFNULL(Dqrq,'1988-01-01'),'%s',Dqrq),INTERVAL %d MONTH) WHERE Jrhm='%s'");
		TCHAR sql[256];

		for (int i = 0; i < nArraySize; i++)
		{
			msgpack::object* pArray = (pDataArray++)->via.array.ptr;
			std::string strJrhm = (pArray++)->as<std::string>();
			std::string strXfrq = (pArray++)->as<std::string>();
			int nMonth = (pArray++)->as<int>();
//			std::string strBz = (pArray++)->as<std::string>();
			memset(sql, 0x00, sizeof(sql));
			_stprintf_s(sql, sizeof(sql), pSql, strXfrq.c_str(), strXfrq.c_str(), strXfrq.c_str(), nMonth, strJrhm.c_str());
			if (!Trans_UpdateTbl(sql, pMysql, bobj))
			{
				mysql_rollback(pMysql);
				mysql_autocommit(pMysql, 1);
				Mysql_BackToPool(pMysql);
				goto error;
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
	case EXCEL_TKQD:
	{
		// 流量池中Kysl - 1;
		// 客户名下 Kzsl - 1
		// 卡状态 Zt = 20,Khid01=0,Khid02=0,Rq=null,Jlxm=null
		int nArraySize = pRootArray->via.array.size;
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			goto error;
		}
		mysql_autocommit(pMysql, 0);

		//const TCHAR* pSql = NULL;
		TCHAR sql[256];
		for (int i = 0; i < nArraySize; i++)
		{
			msgpack::object* pArray = (pDataArray++)->via.array.ptr;
			std::string strJrhm = (pArray++)->as<std::string>();
			std::string strKhmc = (pArray++)->as<std::string>();
			unsigned int Khid = GetIndexByName(strKhmc.c_str(), bobj);
			std::string strBz = (pArray++)->as<std::string>();
			std::string strLlchm = GetLlchmByJrhm(strJrhm.c_str(), bobj);
			if (0 == Khid || "" == strLlchm)
			{
				mysql_rollback(pMysql);
				mysql_autocommit(pMysql, 1);
				Mysql_BackToPool(pMysql);
				goto error;
			}
			const TCHAR* pSql = _T("UPDATE sim_tbl SET Zt=20,Khid01=0,Khid02=0,Jlxm=null,Xsrq=null,Jhrq=null,Xfrq=null,Dqrq=null,Zxrq=null,Bz='%s' WHERE Jrhm='%s'");
			memset(sql, 0x00, sizeof(sql));
			_stprintf_s(sql, sizeof(sql), pSql, strJrhm.c_str());
			if (!Trans_UpdateTbl(sql, pMysql, bobj))
			{
				mysql_rollback(pMysql);
				mysql_autocommit(pMysql, 1);
				Mysql_BackToPool(pMysql);
				goto error;
			}

			// nAffectedRows = 0 更新数据与数据库中数据完全一样
			// nAffectedRows = 1 数据被跟新
			int nAffectedRows = (int)mysql_affected_rows(pMysql);
			if (1 == nAffectedRows)
			{
				pSql = _T("UPDATE kh_tbl SET Kzsl=Kzsl-1 WHERE id=%u");
				memset(sql, 0x00, sizeof(sql));
				_stprintf_s(sql, sizeof(sql), pSql, Khid);
				if (!Trans_UpdateTbl(sql, pMysql, bobj))
				{
					mysql_rollback(pMysql);
					mysql_autocommit(pMysql, 1);
					Mysql_BackToPool(pMysql);
					goto error;
				}

				pSql = _T("UPDATE llc_tbl SET Kysl=Kysl-1 WHERE Llchm='%s'");
				memset(sql, 0x00, sizeof(sql));
				_stprintf_s(sql, sizeof(sql), pSql, strLlchm.c_str());
				if (!Trans_UpdateTbl(sql, pMysql, bobj))
				{
					mysql_rollback(pMysql);
					mysql_autocommit(pMysql, 1);
					Mysql_BackToPool(pMysql);
					goto error;
				}
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

error:
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(4);
	_msgpack.pack(bobj->nCmd);
	_msgpack.pack(bobj->nSubCmd);
	_msgpack.pack(nIndex);
	_msgpack.pack(1);
	DealTail(sbuf, bobj);
	return false;
}