#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_traffic.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"
#include "api_requestpost.h"
#include "cmd_error.h"

bool cmd_traffic(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	std::string strId = (pRootArray++)->as<std::string>();
	sscanf_s(strId.c_str(), "%u", &bobj->nUserId);
	std::string strUsertype = (pRootArray++)->as<std::string>();
	unsigned int nUsertype = 0;
	sscanf_s(strUsertype.c_str(), "%u", &nUsertype);
	bobj->nSubSubCmd = (pRootArray++)->as<int>();

	switch (bobj->nSubSubCmd)
	{
	case TD_QUERY_TRAFFIC_TODAY:
	{
		bobj->pfndoApiResponse = NULL;
		const TCHAR* pMethod = _T("queryTrafficOfToday");
		// needDtl=0表示只返回流量总使用量，不返回使用明细
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=queryTrafficOfToday&access_number=%s&user_id=%s&passWord=%s&sign=%s&needDtl=%s\r\n\r\n");

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		bobj->strTemp = (pArray++)->as<std::string>();
		std::string strneedDtl = (pArray++)->as<std::string>();
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
		_variant_t varSign = ep->strEncSign4(bobj->strTemp.c_str(), row[0], row[1], pMethod, key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

		_stprintf_s(bobj->data, bobj->datalen, pData, bobj->strTemp.c_str(), row[0], (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign, strneedDtl.c_str());
		bobj->dwRecvedCount = (DWORD)strlen(bobj->data);

		doApi(bobj);
	}
	break;
	case TD_QUERY_TRAFFIC:
	{
		bobj->pfndoApiResponse = NULL;
		const TCHAR* pMethod = _T("queryTraffic");
		// needDtl=0表示只返回流量总使用量，不返回使用明细
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=queryTraffic&access_number=%s&user_id=%s&passWord=%s&sign=%s&needDtl=%s\r\n\r\n");

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		bobj->strTemp = (pArray++)->as<std::string>();
		std::string strneedDtl = (pArray++)->as<std::string>();
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
		_variant_t varSign = ep->strEncSign4(bobj->strTemp.c_str(), row[0], row[1], pMethod, key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

		_stprintf_s(bobj->data, bobj->datalen, pData, bobj->strTemp.c_str(), row[0], (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign, strneedDtl.c_str());
		bobj->dwRecvedCount = (DWORD)strlen(bobj->data);

		doApi(bobj);
	}
	break;
	case TD_QUERY_TRAFFIC_DATE:
	{
		bobj->pfndoApiResponse = NULL;
		const TCHAR* pMethod = _T("queryTrafficByDate");
		// needDtl=0表示只返回流量总使用量，不返回使用明细
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=queryTrafficByDate&access_number=%s&user_id=%s&passWord=%s&sign=%s&startDate=%s&endDate=%s&needDtl=%s\r\n\r\n");

		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		bobj->strTemp = (pArray++)->as<std::string>();
		std::string strstartDate = (pArray++)->as<std::string>();
		std::string strendDate = (pArray++)->as<std::string>();
		std::string strneedDtl = (pArray++)->as<std::string>();
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
		_variant_t varSign = ep->strEncSign4(bobj->strTemp.c_str(), row[0], row[1], pMethod, key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

		_stprintf_s(bobj->data, bobj->datalen, pData, bobj->strTemp.c_str(), row[0], (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign, strstartDate.c_str(), strendDate.c_str(), strneedDtl.c_str());
		bobj->dwRecvedCount = (DWORD)strlen(bobj->data);

		doApi(bobj);
	}
	break;
	default:
		break;
	}

	return true;
}