#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_state.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"
#include "api_requestpost.h"
#include "cmd_error.h"

void DoCSData(msgpack::object* pRootArray, BUFFER_OBJ* bobj, const TCHAR* pData);

bool cmd_cardstatus(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	std::string strId = (pRootArray++)->as<std::string>();
	//unsigned int nId = 0;
	sscanf_s(strId.c_str(), "%u", &bobj->nUserId);
	std::string strUsertype = (pRootArray++)->as<std::string>();
	unsigned int nUsertype = 0;
	sscanf_s(strUsertype.c_str(), "%u", &nUsertype);
	bobj->nSubSubCmd = (pRootArray++)->as<int>();
	bobj->pfndoApiResponse = doCardStatusResponse;

	switch (bobj->nSubCmd)
	{
	case CS_QUERY_JRHM:
	{
		const TCHAR* pData = "GET /m2m_ec/query.do?method=queryCardStatus&access_number=%s&user_id=%s&passWord=%s&sign=%s\r\n\r\n";
		DoCSData(pRootArray, bobj, pData);
	}
	break;
	case CS_QUERY_ICCID:
	{
		const TCHAR* pData = "GET /m2m_ec/query.do?method=queryCardStatus&iccid=%s&user_id=%s&passWord=%s&sign=%s\r\n\r\n";
		DoCSData(pRootArray, bobj, pData);
	}
	break;
	default:
		break;
	}
	return true;

	return true;
}

void DoCSData(msgpack::object* pRootArray, BUFFER_OBJ* bobj, const TCHAR* pData)
{
	msgpack::object* pArray = (pRootArray++)->via.array.ptr;
	msgpack::object* pDataObj = (pArray++)->via.array.ptr;
	bobj->strTemp = (pDataObj++)->as<std::string>();
	std::string dxzh = (pDataObj++)->as<std::string>();
	const TCHAR* pSql = _T("SELECT User,Password,MKey FROM dxzh_tbl WHERE Dxzh LIKE '%%%s%%'");
	TCHAR sql[256];
	memset(sql, 0x00, 256);
	_stprintf_s(sql, 256, pSql, dxzh.c_str());
	MYSQL* pMysql = Mysql_AllocConnection();
	if (NULL == pMysql)
	{
		error_info(bobj, _T("连接数据库失败"));
		Api_error(bobj);
		return;
	}

	MYSQL_RES* res = NULL;
	if (!SelectFromTbl(sql, pMysql, bobj, &res))
	{
		Mysql_BackToPool(pMysql);
		Api_error(bobj);
		return;
	}
	MYSQL_ROW row = mysql_fetch_row(res);
	std::string key(row[2]);
	std::string method = _T("queryCardStatus");
	WOTEDUtils::EncInterfacePtr ep(__uuidof(DesUtils));
	_variant_t varPwd = ep->strEnc(row[1], key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());
	_variant_t varSign = ep->strEncSign4(bobj->strTemp.c_str(), row[0], row[1], method.c_str(),
		key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

	_stprintf_s(bobj->data, bobj->datalen, pData, bobj->strTemp.c_str(), row[0], (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign);
	bobj->dwRecvedCount = (DWORD)strlen(bobj->data);
	doApi(bobj);
}