#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_kh.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"

bool cmd_kh(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
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
	case KH_ADD:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strKhmc = (pArray++)->as<std::string>();
		unsigned int nFatherid = (pArray++)->as<unsigned int>();
		if (nUsertype == 1)
		{
			nFatherid = 1;
		}
		std::string strLxfs = (pArray++)->as<std::string>();
		std::string strSsdq = (pArray++)->as<std::string>();
		std::string strJlxm = (pArray++)->as<std::string>();
		double nDj = (pArray++)->as<double>();
		std::string strBz = (pArray++)->as<std::string>();

		const TCHAR* pSql = _T("INSERT INTO kh_tbl (id,Khmc,Fatherid,Lxfs,Ssdq,Jlxm,Dj,Xgsj,Bz) VALUES(null,'%s',%u,'%s','%s','%s',%f,now(),'%s')");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, strKhmc.c_str(), nFatherid, strLxfs.c_str(), strSsdq.c_str(), strJlxm.c_str(), nDj, strBz.c_str());

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
				error_info(bobj, _T("客户已存在"));
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
		_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, (unsigned int)nIndex);

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
		ParserKh(_msgpack, row, KH_SELECT_SIZE);
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	case KH_QUERY:
	{
		bobj->nSubSubCmd = (pRootArray++)->as<int>();
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strKhid = (pArray++)->as<std::string>();
		unsigned int nKhid = 0;
		sscanf_s(strKhid.c_str(), "%u", &nKhid);

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		if (nUsertype == 1)
		{
			pSql = _T("SELECT Xsrq,COUNT(*) num FROM sim_tbl WHERE Khid01=%u GROUP BY Xsrq");
		}
		else if (nUsertype == 2)
		{
			pSql = _T("SELECT Xsrq,COUNT(*) num FROM sim_tbl WHERE Khid02=%u GROUP BY Xsrq");
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
		_msgpack.pack_array(7);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(bobj->nSubSubCmd);
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
	case KH_SIM_XSRQ:
	{
		bobj->nSubSubCmd = (pRootArray++)->as<int>();
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strKhid = (pArray++)->as<std::string>();
		unsigned int nKhid = 0;
		sscanf_s(strKhid.c_str(), "%u", &nKhid);
		std::string strXsrq = (pArray++)->as<std::string>();

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		if (nUsertype == 1)
		{
			pSql = _T("SELECT COUNT(*) num FROM sim_tbl WHERE Khid01=%u AND Xsrq='%s'");
		}
		else if (nUsertype == 2)
		{
			pSql = _T("SELECT COUNT(*) num FROM sim_tbl WHERE Khid02=%u AND Xsrq='%s'");
		}
		_stprintf_s(sql, sizeof(sql), pSql, nKhid, strXsrq.c_str());

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
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND Xsrq='%s' and id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, strXsrq.c_str(), nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u AND Xsrq='%s'and id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, strXsrq.c_str(), nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND Xsrq='%s'and id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, strXsrq.c_str(), nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u AND Xsrq='%s'ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, strXsrq.c_str(), nTemp);
			}
		}
		else if (nUsertype == 2)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND Xsrq='%s'and id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, strXsrq.c_str(), nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND Xsrq='%s'and id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, strXsrq.c_str(), nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND Xsrq='%s'and id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, strXsrq.c_str(), nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND Xsrq='%s'ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, strXsrq.c_str(), nTemp);
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
		_msgpack.pack_array(7);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(bobj->nSubSubCmd);
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
	case KH_LIST:
	{
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		std::string strKhmc = (pRootArray++)->as<std::string>();
		bool bType = _tcscmp(strKhmc.c_str(), "") == 0;

		const TCHAR* pSql = _T("SELECT COUNT(*) num FROM kh_tbl WHERE Fatherid=%u");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		if (nUsertype == 1) // 超级管理员
			nId = 1;

		if (!bType)
		{
			pSql = _T("SELECT COUNT(*) num FROM kh_tbl WHERE Khmc LIKE '%%%s%%'");
			_stprintf_s(sql, sizeof(sql), pSql, strKhmc.c_str());
		}
		else
		{
			pSql = _T("SELECT COUNT(*) num FROM kh_tbl WHERE Fatherid=%u");
			_stprintf_s(sql, sizeof(sql), pSql, nId);
		}

		//_stprintf_s(sql, sizeof(sql), pSql, nId);

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
			if (!bType)
			{
				pSql = _T("SELECT %s FROM kh_tbl WHERE Khmc LIKE '%%%s%%'");
				_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, strKhmc.c_str());
			}
			else
			{
				pSql = _T("SELECT %s FROM kh_tbl WHERE Fatherid=%u AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, nId, nKeyid, nPagesize);
			}
			/*pSql = _T("SELECT %s FROM kh_tbl WHERE Fatherid=%u AND id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, nId, nKeyid, nPagesize);*/
		}
		else if (nAB == 1)
		{
			if (!bType)
			{
				pSql = _T("SELECT %s FROM kh_tbl WHERE Khmc LIKE '%%%s%%'");
				_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, strKhmc.c_str());
			}
			else
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM kh_tbl WHERE Fatherid=%u AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, KH_SELECT, nId, nKeyid, nPagesize);
			}
			/*pSql = _T("SELECT %s FROM (SELECT %s FROM kh_tbl WHERE Fatherid=%u AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
			_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, KH_SELECT, nId, nKeyid, nPagesize);*/
		}
		else if (nAB == 2)
		{
			if (!bType)
			{
				pSql = _T("SELECT %s FROM kh_tbl WHERE Khmc LIKE '%%%s%%'");
				_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, strKhmc.c_str());
			}
			else
			{
				pSql = _T("SELECT %s FROM kh_tbl WHERE Fatherid=%u AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, nId, nKeyid, nPagesize);
			}
			/*pSql = _T("SELECT %s FROM kh_tbl WHERE Fatherid=%u AND id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, nId, nKeyid, nPagesize);*/
		}
		else
		{
			if (!bType)
			{
				pSql = _T("SELECT %s FROM kh_tbl WHERE Khmc LIKE '%%%s%%'");
				_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, strKhmc.c_str());
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM kh_tbl WHERE Fatherid=%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, KH_SELECT, nId, nTemp);
			}
			/*unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
			pSql = _T("SELECT %s FROM (SELECT %s FROM kh_tbl WHERE Fatherid=%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
			_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, KH_SELECT, nId, nTemp);*/
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
		_msgpack.pack_array(7);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(bobj->nSubSubCmd);
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
	case KH_SIM_LIST:
	{
		bobj->nSubSubCmd = (pRootArray++)->as<int>();
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strKhid = (pArray++)->as<std::string>();
		unsigned int nKhid = 0;
		sscanf_s(strKhid.c_str(), "%u", &nKhid);

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		//if (nUsertype == 1)
		//{
		//	//pSql = _T("SELECT COUNT(*) num FROM sim_tbl WHERE Khid01=%u");
		//	pSql = _T("SELECT Kzsl FROM kh_tbl WHERE id=%u");
		//}
		//else if (nUsertype == 2)
		//{
		//	//pSql = _T("SELECT COUNT(*) num FROM sim_tbl WHERE Khid02=%u");
		//	pSql = _T("SELECT Kzsl FROM kh_tbl WHERE id=%u");
		//}
		pSql = _T("SELECT Kzsl FROM kh_tbl WHERE id=%u");
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
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);

		unsigned int nNum = 0;
		sscanf_s(row[0], "%u", &nNum);

		if (nUsertype == 1)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u and id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u and id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u and id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nTemp);
			}
		}
		else if (nUsertype == 2)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u and id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u and id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u and id>%u LIMIT %d");
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
		_msgpack.pack_array(7);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(bobj->nSubSubCmd);
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
	case KH_SIM_ON15D:
	{
		bobj->nSubSubCmd = (pRootArray++)->as<int>();
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strKhid = (pArray++)->as<std::string>();
		unsigned int nKhid = 0;
		sscanf_s(strKhid.c_str(), "%u", &nKhid);

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		pSql = _T("SELECT On15d FROM kh_tbl WHERE id=%u");
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
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);

		unsigned int nNum = 0;
		sscanf_s(row[0], "%u", &nNum);

		if (nUsertype == 1)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 15 DAY) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u and Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 15 DAY) AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u and Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 15 DAY) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 15 DAY) ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nTemp);
			}
		}
		else if (nUsertype == 2)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 15 DAY) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 15 DAY) AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 15 DAY) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 15 DAY) ORDER BY id desc LIMIT %d) a ORDER BY id asc");
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
		_msgpack.pack_array(7);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(bobj->nSubSubCmd);
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
	case KH_SIM_ON1M:
	{
		bobj->nSubSubCmd = (pRootArray++)->as<int>();
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strKhid = (pArray++)->as<std::string>();
		unsigned int nKhid = 0;
		sscanf_s(strKhid.c_str(), "%u", &nKhid);

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		pSql = _T("SELECT On1m FROM kh_tbl WHERE id=%u");
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
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);

		unsigned int nNum = 0;
		sscanf_s(row[0], "%u", &nNum);

		if (nUsertype == 1)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 1 MONTH) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 1 MONTH) AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 1 MONTH) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 1 MONTH) ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nTemp);
			}
		}
		else if (nUsertype == 2)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 1 MONTH) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 1 MONTH) AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 1 MONTH) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq>CURDATE() AND Dqrq<DATE_ADD(CURDATE(),INTERVAL 1 MONTH) ORDER BY id desc LIMIT %d) a ORDER BY id asc");
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
		_msgpack.pack_array(7);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(bobj->nSubSubCmd);
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
	case KH_SIM_DU1M:
	{
		bobj->nSubSubCmd = (pRootArray++)->as<int>();
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strKhid = (pArray++)->as<std::string>();
		unsigned int nKhid = 0;
		sscanf_s(strKhid.c_str(), "%u", &nKhid);

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		pSql = _T("SELECT Du1m FROM kh_tbl WHERE id=%u");
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
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);

		unsigned int nNum = 0;
		sscanf_s(row[0], "%u", &nNum);

		if (nUsertype == 1)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 1 MONTH) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 1 MONTH) AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 1 MONTH) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 1 MONTH) ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nTemp);
			}
		}
		else if (nUsertype == 2)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 1 MONTH) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 1 MONTH) AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 1 MONTH) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 1 MONTH) ORDER BY id desc LIMIT %d) a ORDER BY id asc");
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
		_msgpack.pack_array(7);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(bobj->nSubSubCmd);
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
	case KH_SIM_DU15D:
	{
		bobj->nSubSubCmd = (pRootArray++)->as<int>();
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strKhid = (pArray++)->as<std::string>();
		unsigned int nKhid = 0;
		sscanf_s(strKhid.c_str(), "%u", &nKhid);

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		pSql = _T("SELECT Du15d FROM kh_tbl WHERE id=%u");
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
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);

		unsigned int nNum = 0;
		sscanf_s(row[0], "%u", &nNum);

		if (nUsertype == 1)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 15 DAY) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 15 DAY) AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 15 DAY) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid01=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 15 DAY) ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nTemp);
			}
		}
		else if (nUsertype == 2)
		{
			if (nAB == 0) // 首页
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 15 DAY) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 1)
			{
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 15 DAY) AND id<%u ORDER BY id desc LIMIT %d) a ORDER BY id asc");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else if (nAB == 2)
			{
				pSql = _T("SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 15 DAY) AND id>%u LIMIT %d");
				_stprintf_s(sql, sizeof(sql), pSql, SIM_SELECT, nKhid, nKeyid, nPagesize);
			}
			else
			{
				unsigned int nTemp = (nNum % nPagesize) == 0 ? nPagesize : (nNum % nPagesize);
				pSql = _T("SELECT %s FROM (SELECT %s FROM sim_tbl WHERE Khid02=%u AND Dqrq<CURDATE() AND Dqrq>DATE_SUB(CURDATE(),INTERVAL 15 DAY) ORDER BY id desc LIMIT %d) a ORDER BY id asc");
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
		_msgpack.pack_array(7);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(bobj->nSubSubCmd);
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
	case KH_SELECT_BY_NAME:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strKhmc = (pDataArray++)->as<std::string>();

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		pSql = _T("SELECT %s FROM kh_tbl WHERE Khmc LIKE '%%%s%%'");
		_stprintf_s(sql, sizeof(sql), pSql, KH_SELECT, strKhmc.c_str());

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

		unsigned int nRows = (unsigned int)mysql_num_rows(res);
		MYSQL_ROW row = mysql_fetch_row(res);
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(4);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(0);
		_msgpack.pack(1);
		ParserKh(_msgpack, row, KH_SELECT_SIZE);
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	default:
		break;
	}

	return true;
}