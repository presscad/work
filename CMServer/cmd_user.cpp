#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_user.h"
#include "cm_mysql.h"
#include "global_data.h"

bool cmd_user(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();

	switch (bobj->nSubCmd)
	{
	case USER_ADD:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strUsername = (pArray++)->as<std::string>();
		std::string strPassword = (pArray++)->as<std::string>();
		int nAuthority = (pArray++)->as<int>();
		double dDj = (pArray++)->as<double>();

		const TCHAR* pSql = _T("INSERT INTO user_tbl (id,username,password,authority,dj,xgsj) VALUES(null,'%s','%s',%d,%lf,now())");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, strUsername.c_str(), strPassword.c_str(), nAuthority, dDj);

		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			_tprintf(_T("连接数据库失败\n"));
			return false;
		}

		size_t len = _tcslen(sql);
		if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
		{
			UINT uError = mysql_errno(pMysql);
			mysql_error(pMysql);
			Mysql_BackToPool(pMysql);
			return false;
		}

		if (mysql_affected_rows(pMysql) != 1)
		{ 
			_tprintf(_T("数据操作异常\n"));
			UINT uError = mysql_errno(pMysql);
			mysql_error(pMysql);
			Mysql_BackToPool(pMysql);
			return false;
		}

		my_ulonglong nIndex = mysql_insert_id(pMysql);

		Mysql_BackToPool(pMysql);

		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(4);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(0);
		_msgpack.pack_array(1);
		_msgpack.pack_array(5);
		_msgpack.pack((int)nIndex);
		_msgpack.pack(strUsername);
		_msgpack.pack(strPassword);
		_msgpack.pack(nAuthority);
		_msgpack.pack(dDj);

		DealTail(sbuf, bobj);
	}
	break;

	case USER_LOGIN:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strUsername = (pArray++)->as<std::string>();
		std::string strPassword = (pArray++)->as<std::string>();

		const TCHAR* pSql = _T("SELECT id,authority,dj,xgsj FROM user_tbl WHERE username='%s' AND password='%s'");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, strUsername.c_str(), strPassword.c_str());

		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			_tprintf(_T("连接数据库失败\n"));
			return false;
		}

		size_t len = _tcslen(sql);
		if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
		{
			UINT uError = mysql_errno(pMysql);
			mysql_error(pMysql);
			Mysql_BackToPool(pMysql);
			return false;
		}

		MYSQL_RES* res = mysql_store_result(pMysql);
		if (NULL == res)
		{
			if (mysql_field_count(pMysql) != 0)
			{
				_tprintf(_T("查询失败\n"));
				Mysql_BackToPool(pMysql);
				return false;
			}
			else
			{
				_tprintf(_T("成功，但数据未找到\n"));
			}
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
		_msgpack.pack_array(6);
		_msgpack.pack(row[0]);
		_msgpack.pack(strUsername);
		_msgpack.pack(strPassword);
		_msgpack.pack(row[1]);
		_msgpack.pack(row[2]);
		_msgpack.pack(row[3]);

		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	default:
		break;
	}

	return true;
}