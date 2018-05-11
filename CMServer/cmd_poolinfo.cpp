#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_poolinfo.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"
#include "api_requestpost.h"
#include "cmd_error.h"

bool cmd_poolinfo(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	std::string strId = (pRootArray++)->as<std::string>();
	sscanf_s(strId.c_str(), "%u", &bobj->nUserId);
	std::string strUsertype = (pRootArray++)->as<std::string>();
	unsigned int nUsertype = 0;
	sscanf_s(strUsertype.c_str(), "%u", &nUsertype);
	bobj->nSubSubCmd = (pRootArray++)->as<int>();

	switch (bobj->nSubCmd)
	{
	case PL_GET_LIST:
	{
		bobj->pfndoApiResponse = NULL;
		const TCHAR* pMethod = _T("getPoolList");
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=getPoolList&user_id=%s&passWord=%s&sign=%s\r\n\r\n");

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string dxzh = (pArray++)->as<std::string>();
		const TCHAR* pSql = _T("SELECT User,Password,MKey FROM dxzh_tbl WHERE Dxzh LIKE '%%%s%%'");
		TCHAR sql[256];
		memset(sql, 0x00, 256);
		_stprintf_s(sql, 256, pSql, dxzh.c_str());
		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			return Api_error(bobj);
		}

		MYSQL_RES* res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return Api_error(bobj);
		}
		MYSQL_ROW row = mysql_fetch_row(res);
		std::string key(row[2]);

		WOTEDUtils::EncInterfacePtr ep(__uuidof(DesUtils));
		_variant_t varPwd = ep->strEnc(row[1], key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());
		_variant_t varSign = ep->strEncSign3(row[0], row[1], pMethod,key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

		_stprintf_s(bobj->data, bobj->datalen, pData, row[0], (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign);
		bobj->dwRecvedCount = (DWORD)strlen(bobj->data);

		doApi(bobj);
	}
	break;
	case PL_LLC_INFO:
	{
		bobj->pfndoApiResponse = NULL;
		const TCHAR* pMethod = _T("getPoolMemberList");
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=getPoolMemberList&user_id=%s&passWord=%s&sign=%s&poolNbr=%s&currentPage=%s\r\n\r\n");

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strLlchm = (pArray++)->as<std::string>();
		std::string strCurrentPage = (pArray++)->as<std::string>();
		std::string dxzh = (pArray++)->as<std::string>();

		const TCHAR* pSql = _T("SELECT User,Password,MKey FROM dxzh_tbl WHERE Dxzh LIKE '%%%s%%'");
		TCHAR sql[256];
		memset(sql, 0x00, 256);
		_stprintf_s(sql, 256, pSql, dxzh.c_str());
		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			return Api_error(bobj);
		}

		MYSQL_RES* res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return Api_error(bobj);
		}
		MYSQL_ROW row = mysql_fetch_row(res);
		std::string key(row[2]);

		WOTEDUtils::EncInterfacePtr ep(__uuidof(DesUtils));
		_variant_t varPwd = ep->strEnc(row[1], key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());
		_variant_t varSign = ep->strEncSign5(row[0], row[1], pMethod, strLlchm.c_str(), strCurrentPage.c_str(), key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

		_stprintf_s(bobj->data, bobj->datalen, pData, row[0], (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign, strLlchm.c_str(), strCurrentPage.c_str());
		bobj->dwRecvedCount = (DWORD)strlen(bobj->data);

		doApi(bobj);
	}
	break;
	case PL_LLC_POOLQRY:
	{
		bobj->pfndoApiResponse = NULL;
		const TCHAR* pMethod = _T("poolQry");
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=poolQry&user_id=%s&passWord=%s&sign=%s&poolNbr=%s\r\n\r\n");

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		bobj->strTemp = (pArray++)->as<std::string>();
		std::string dxzh = (pArray++)->as<std::string>();
		const TCHAR* pSql = _T("SELECT User,Password,MKey FROM dxzh_tbl WHERE Dxzh LIKE '%%%s%%'");
		TCHAR sql[256];
		memset(sql, 0x00, 256);
		_stprintf_s(sql, 256, pSql, dxzh.c_str());
		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			return Api_error(bobj);
		}

		MYSQL_RES* res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return Api_error(bobj);
		}
		MYSQL_ROW row = mysql_fetch_row(res);
		std::string key(row[2]);

		WOTEDUtils::EncInterfacePtr ep(__uuidof(DesUtils));
		_variant_t varPwd = ep->strEnc(row[1], key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());
		_variant_t varSign = ep->strEncSign4(row[0], row[1], pMethod, bobj->strTemp.c_str(), key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

		_stprintf_s(bobj->data, bobj->datalen, pData, row[0], (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign, bobj->strTemp.c_str());
		bobj->dwRecvedCount = (DWORD)strlen(bobj->data);

		doApi(bobj);
	}
	break;
	default:
		break;
	}

	return true;
}