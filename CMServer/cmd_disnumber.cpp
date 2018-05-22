#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_disnumber.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"
#include "api_requestpost.h"
#include "cmd_error.h"

void DoDisNumData(msgpack::object* pRootArray, BUFFER_OBJ* bobj, const TCHAR* pData, const TCHAR* orderTypeId);

bool cmd_disnumber(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	std::string strId = (pRootArray++)->as<std::string>();
	//unsigned int nId = 0;
	sscanf_s(strId.c_str(), "%u", &bobj->nUserId);
	std::string strUsertype = (pRootArray++)->as<std::string>();
	unsigned int nUsertype = 0;
	sscanf_s(strUsertype.c_str(), "%u", &nUsertype);
	bobj->nSubSubCmd = (pRootArray++)->as<int>();
	bobj->pfndoApiResponse = doDisNumberResponse;
	switch (bobj->nSubCmd)
	{
	case DN_DISABLE:
	{
		const TCHAR* pSql = _T("SELECT a.id,a.Llchm,a.Kzsl,b.User,b.Password,b.MKey FROM (SELECT id,Dxzh,Llchm,Kzsl FROM Llc_tbl WHERE Llclx='后向' LIMIT 1) a LEFT JOIN Dxzh_tbl b ON a.Dxzh=b.Dxzh");
		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("连接数据库失败"));
			return Api_error(bobj);
		}

		MYSQL_RES* res = NULL;
		if (!SelectFromTbl(pSql, pMysql, bobj, &res))
		{
			//error_info(bobj, _T("连接数据库失败"));
			Mysql_BackToPool(pMysql);
			return Api_error(bobj);
		}

		MYSQL_ROW row = mysql_fetch_row(res);

		_tprintf_s(_T("%s,%s,%s,%s,%s,%s"), row[0], row[1], row[2], row[3], row[4], row[5]);

		bobj->nPerLogID = _tstoi(row[0]);

		bobj->pfndoApiResponse = doLlcQryResponse;
		const TCHAR* pMethod = _T("poolQry");
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=poolQry&user_id=%s&passWord=%s&sign=%s&poolNbr=%s\r\n\r\n");

		std::string key(row[5]);

		WOTEDUtils::EncInterfacePtr ep(__uuidof(DesUtils));
		_variant_t varPwd = ep->strEnc(row[4], key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());
		_variant_t varSign = ep->strEncSign4(row[3], row[4], pMethod, row[1], key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

		_stprintf_s(bobj->data, bobj->datalen, pData, row[3], (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign, row[1]);
		bobj->dwRecvedCount = (DWORD)strlen(bobj->data);

		bobj->pTempData = new std::list<LLC_QRY_S*>;
		std::list<LLC_QRY_S*>* p = (std::list<LLC_QRY_S*>*)(bobj->pTempData);

		LLC_QRY_S * pLlcQry = new LLC_QRY_S;
		_stscanf_s(row[2], _T("%u"), &pLlcQry->nCount);
		pLlcQry->llchm = row[1];
		p->push_back(pLlcQry);

		doApi(bobj);

		mysql_free_result(res);
		//const TCHAR* pData = _T("GET /m2m_ec/query.do?method=disabledNumber&user_id=%s&access_number=%s&acctCd=&passWord=%s&sign=%s&orderTypeId=19\r\n\r\n");
		//DoDisNumData(pRootArray, bobj, pData, "19");
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
	bobj->strTemp = (pArray++)->as<std::string>();
	std::string dxzh = (pArray++)->as<std::string>();
	const TCHAR* pSql = _T("SELECT User,Password,MKey FROM dxzh_tbl WHERE Dxzh LIKE '%%%s%%'");
	TCHAR sql[256];
	memset(sql, 0, 256);
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
	mysql_free_result(res);

	std::string key(row[2]);
	std::string method = _T("disabledNumber");
	WOTEDUtils::EncInterfacePtr ep(__uuidof(DesUtils));
	_variant_t varPwd = ep->strEnc(row[1], key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());
	_variant_t varSign = ep->strEncSign6(bobj->strTemp.c_str(), row[0], row[1], method.c_str(), "", orderTypeId,
		key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

	_stprintf_s(bobj->data, bobj->datalen, pData, row[0], bobj->strTemp.c_str(), (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign);
	bobj->dwRecvedCount = (DWORD)strlen(bobj->data);

	int nType = _tstoi(orderTypeId);
	if (nType == 19)
	{
		pSql = _T("INSERT INTO log_tbl (id,Userid,jrhm,acceptmsg,senddt) values(null,%u,'%s','停机-%s',now())");
	}
	else if (nType == 20)
	{
		pSql = _T("INSERT INTO log_tbl (id,Userid,jrhm,acceptmsg,senddt) values(null,%u,'%s','复机-%s',now())");
	}

	memset(sql, 0x00, sizeof(sql));
	_stprintf_s(sql, sizeof(sql), pSql, bobj->nUserId, bobj->strTemp.c_str(), bobj->strTemp.c_str());
	if (!InsertIntoTbl(sql, pMysql, bobj))
	{
		Mysql_BackToPool(pMysql);
		Api_error(bobj);
		return;
	}

	bobj->nPerLogID = (unsigned int)mysql_insert_id(pMysql);

	Mysql_BackToPool(pMysql);

	doApi(bobj);
}