#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_user.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"

#define USER_ITEM	_T("id,User,Password,Authority,Usertype,Fatherid,Xgsj")

void ParserUser(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);

bool cmd_user(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();

	switch (bobj->nSubCmd)
	{
	case USER_ADD:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strUser = (pArray++)->as<std::string>();
		std::string strPassword = (pArray++)->as<std::string>();
		std::string strKhmc = (pArray++)->as<std::string>();
		unsigned int nAuthority = (pArray++)->as<unsigned int>();
		unsigned int nUsertype = (pArray++)->as<unsigned int>();
		unsigned int nFatherid = (pArray++)->as<unsigned int>();

		const TCHAR* pSql = _T("SELECT Userid From kh_tbl WHERE Khmc='%s'");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, strKhmc.c_str());
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
		unsigned int nNum = 0;
		sscanf_s(row[0], "%u", &nNum);
		if (nNum != 0)
		{
			mysql_free_result(res);
			res = NULL;
			Mysql_BackToPool(pMysql);
			error_info(bobj, _T("客户已经分配过账号信息"));
			return cmd_error(bobj);
		}
		mysql_free_result(res);
		res = NULL;

		pSql = _T("INSERT INTO user_tbl (%s) VALUES(null,'%s','%s',%u,%u,%u,now())");
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, USER_ITEM, strUser.c_str(), strPassword.c_str(), nAuthority, nFatherid, nUsertype);

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

		pSql = _T("UPDATE kh_tbl SET Userid=%u WHERE khmc='%s'");
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, (unsigned int)nIndex, strKhmc.c_str());
		if (!UpdateTbl(sql, pMysql, bobj))
		{
			// 删除刚添加的用户
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj);
		}

		pSql = _T("SELECT %s FROM user_tbl WHERE id=%u");
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, USER_ITEM, (unsigned int)nIndex);

		res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj);
		}

		Mysql_BackToPool(pMysql);

		row = mysql_fetch_row(res);
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(4);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(0);
		_msgpack.pack_array(1);
		ParserUser(_msgpack, row);
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;

	case USER_LOGIN:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strUser = (pArray++)->as<std::string>();
		std::string strPassword = (pArray++)->as<std::string>();

		const TCHAR* pSql = _T("SELECT %s FROM user_tbl WHERE User='%s' AND Password='%s'");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, USER_ITEM, strUser.c_str(), strPassword.c_str());

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
		ParserUser(_msgpack, row);
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	case USER_LIST:
	{
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		unsigned int nId  = (pArray++)->as<unsigned int>();
		unsigned int nUsertype = (pArray++)->as<unsigned int>();

		const TCHAR* pSql = pSql = _T("SELECT COUNT(*) AS num FROM user_tbl WHERE nFatherid=%u");
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

		if (nAB == 0) // 首页
		{
			pSql = _T("SELECT * FROM user_tbl WHERE Fatherid=%u AND id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, USER_ITEM, nId, nKeyid, nPagesize);
		}
		else if (nAB == 1)
		{
			pSql = _T("SELECT * FROM user_tbl WHERE Fatherid=%u AND id<%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, USER_ITEM, nId, nKeyid, nPagesize);
		}
		else if (nAB == 2)
		{
			pSql = _T("SELECT * FROM user_tbl WHERE Fatherid=%u AND id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, USER_ITEM, nId, nKeyid, nPagesize);
		}
		else
		{
			unsigned int nTemp = nNum % nPagesize;
			pSql = _T("SELECT * FROM user_tbl WHERE Fatherid=%u ORDER BY id desc LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, USER_ITEM, nId, nPagesize);
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
			ParserUser(_msgpack, row);
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

// row[0] = id;
// row[1] = User;
// row[2] = Password;
// row[3] = Authority;
// row[4] = Usertype;
// row[5] = Dj;
// row[6] = Fatherid;
// row[7] = Xgsj;
void ParserUser(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row)
{
	_msgpack.pack_array(8);
	_msgpack.pack(row[0]);
	_msgpack.pack(row[1]);
	_msgpack.pack(row[2]);
	_msgpack.pack(row[3]);
	_msgpack.pack(row[4]);
	_msgpack.pack(row[5]);
	_msgpack.pack(row[6]);
	_msgpack.pack(row[7]);
}