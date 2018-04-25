#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_kh.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"

bool cmd_kh(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();

	switch (bobj->nSubCmd)
	{
	case KH_ADD:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strKhmc = (pArray++)->as<std::string>();
		unsigned int nFatherid = (pArray++)->as<unsigned int>();
		std::string strLxfs = (pArray++)->as<std::string>();
		std::string strSsdq = (pArray++)->as<std::string>();
		std::string strJlxm = (pArray++)->as<std::string>();
		std::string strBz = (pArray++)->as<std::string>();

		const TCHAR* pSql = _T("INSERT INTO kh_tbl (id,Khmc,Fatherid,Lxfs,Ssdq,Jlxm,Bz) VALUES(null,'%s',%u,'%s','%s','%s','%s')");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, strKhmc.c_str(), nFatherid, strLxfs.c_str(), strSsdq.c_str(), strJlxm.c_str(), strBz.c_str());

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
				error_info(bobj, _T("已经存在的用户名"));
			}
			else
			{
				error_info(bobj, _T("数据库异常 ErrorCode = %08x, ErrorMsg = %s"), uError, mysql_error(pMysql));
			}
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj);
		}
		my_ulonglong nIndex = mysql_insert_id(pMysql); // 新添加的用户的id

		pSql = _T("SELECT %s FROM kh_tbl WHERE id=%u");
		memset(sql, 0x00, sizeof(sql));
		//_stprintf_s(sql, sizeof(sql), pSql, USER_ITEM, (unsigned int)nIndex);

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
		//ParserUser(_msgpack, row);
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;

	case KH_LIST:
	{
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		unsigned int nId = (pArray++)->as<unsigned int>();
		unsigned int nUsertype = (pArray++)->as<unsigned int>();

		const TCHAR* pSql = pSql = _T("SELECT COUNT(*) AS num FROM kh_tbl WHERE nFatherid=%u");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		if (nUsertype == 1) // 超级管理员
			nId = 1;

		_stprintf_s(sql, sizeof(sql), pSql, nId);

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

		if (nAB == 3) // 尾页
		{

			pSql = _T("SELECT %s FROM user_tbl WHERE ");
		}
		else if (nAB == 1) // 上一页
		{
			pSql = _T("SELECT %s FROM user_tbl WHERE nFatherid=%u AND id<%u OERDER BY id LIMIT %d");
		}
		else// 下一页
		{
			pSql = _T("SELECT %s FROM user_tbl WHERE nFatherid=%u AND id>%u OERDER BY id LIMIT %d");
		}

		//_stprintf_s(sql, sizeof(sql), pSql, USER_ITEM, nId, nKeyid, nPagesize);

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
			//ParserUser(_msgpack, row);
			row = mysql_fetch_row(res);
		}
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	case KH_SIM_LIST:
	{
		_T("SELECT * FROM sim_tbl WHERE ");
	}
	break;
	default:
		break;
	}

	return true;
}