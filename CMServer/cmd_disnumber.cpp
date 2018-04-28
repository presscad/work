#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_disnumber.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"
#include "api_requestpost.h"
#include "cmd_error.h"

void DoDisNumData(msgpack::object* pObj, BUFFER_OBJ* bobj, const TCHAR* pData, const TCHAR* orderTypeId);

bool cmd_disnumber(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	bobj->nSubSubCmd = (pRootArray++)->as<int>();
	unsigned int nId = (pRootArray++)->as<unsigned int>();
	unsigned int nUsertype = (pRootArray++)->as<unsigned int>();
	bobj->pfndoApiResponse = doDisNumberResponse;
	switch (bobj->nSubCmd)
	{
	case DN_DISABLE:
	{
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=disabledNumber&user_id=%s&access_number=%s&acctCd=&passWord=%s&sign=%s&orderTypeId=19\r\n\r\n");
		DoDisNumData(pRootArray, bobj, pData, "19");
	}
	break;
	case DN_ABLE:
	{
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=disabledNumber&user_id=%s&access_number=%s&acctCd=&passWord=%s&sign=%s&orderTypeId=20\r\n\r\n");
		DoDisNumData(pRootArray, bobj, pData, "20");
	}
	break;
	default:
		break;
	}
	return true;
}

void DoDisNumData(msgpack::object* pRootArray, BUFFER_OBJ* bobj, const TCHAR* pData, const TCHAR* orderTypeId)
{
	msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
	msgpack::object* pArray = (pDataArray++)->via.array.ptr;
	std::string strJrhm = (pArray++)->as<std::string>();
	std::string dxzh = (pArray++)->as<std::string>();

	const TCHAR* pSql = _T("SELECT User,Password,MKey FROM dxzh_tbl WHERE User='yunhu2017'");
	TCHAR sql[256];
	memset(sql, 0x00, 256);
	_stprintf_s(sql, 256, pSql, dxzh.c_str());
	MYSQL* pMysql = Mysql_AllocConnection();
	if (NULL == pMysql)
	{
		error_info(bobj, _T("连接数据库失败"));
		cmd_error(bobj);
		return;
	}

	MYSQL_RES* res = NULL;
	if (!SelectFromTbl(sql, pMysql, bobj, &res))
	{
		Mysql_BackToPool(pMysql);
		cmd_error(bobj);
		return;
	}
	MYSQL_ROW row = mysql_fetch_row(res);
	std::string key(row[2]);
	std::string method = _T("disabledNumber");
	//_EncInterfacePtr ep(__uuidof(EDesUtils));
	//_variant_t varPwd = ep->strEnc(row[1], key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());
	//_variant_t varSign = ep->strEncSign6(strJrhm.c_str(), row[0], row[1], method.c_str(), "", orderTypeId,
	//	key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

	//_stprintf_s(bobj->data, bobj->datalen, pData, row[0], strJrhm.c_str(), (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign);
	bobj->dwRecvedCount = strlen(bobj->data);
	doApi(bobj);
}