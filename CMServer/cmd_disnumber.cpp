#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_disnumber.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"
#include "api_requestpost.h"

void DoDisNumData(msgpack::object* pObj, BUFFER_OBJ* bobj, const TCHAR* pData, const TCHAR* orderTypeId);

bool cmd_disnumber(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	unsigned int nId = (pRootArray++)->as<unsigned int>();
	unsigned int nUsertype = (pRootArray++)->as<unsigned int>();
	bobj->pfndoApiResponse = NULL;
	switch (bobj->nSubCmd)
	{
	case DN_DISABLE:
	{
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=disabledNumber&user_id=%s&access_number=%s&acctCd=&passWord=%s&sign=%s&orderTypeId=19\r\n\r\n");
		//DoDisNumData(pObj, bobj, pData, "19");
	}
	break;
	case DN_ABLE:
	{
		const TCHAR* pData = _T("GET /m2m_ec/query.do?method=disabledNumber&user_id=%s&access_number=%s&acctCd=&passWord=%s&sign=%s&orderTypeId=20\r\n\r\n");
		//DoDisNumData(pObj, bobj, pData, "20");
	}
	break;
	default:
		break;
	}
	return true;
}

void DoDisNumData(msgpack::object* pObj, BUFFER_OBJ* bobj, const TCHAR* pData, const TCHAR* orderTypeId)
{
	//msgpack::object* pArray = (pObj++)->via.array.ptr;
	//msgpack::object* pDataObj = (pArray++)->via.array.ptr;
	//bobj->strJrhm = (pDataObj++)->as<std::string>();
	//std::string dxzh = (pDataObj++)->as<std::string>();

	////const TCHAR* pSql = _T("SELECT userid,pwd,skey FROM dxzh_tbl WHERE dxzh='%s'");
	//const TCHAR* pSql = _T("SELECT userid,pwd,skey FROM dxzh_tbl WHERE dxzh LIKE '%%%s%%'");
	//TCHAR sql[256];
	//memset(sql, 0x00, 256);
	//_stprintf_s(sql, 256, pSql, dxzh.c_str());
	//if (!Select_From_Tbl(sql, bobj->pRecorder))
	//{
	//	API_Failed(bobj);
	//	return;
	//}
	//_variant_t var_userid = bobj->pRecorder->GetCollect("userid");
	//_variant_t var_pwd = bobj->pRecorder->GetCollect("pwd");
	//_variant_t var_key = bobj->pRecorder->GetCollect("skey");
	//std::string key((const char*)(_bstr_t)var_key);
	//std::string method = _T("disabledNumber");
	//WOTEDUtils::EncInterfacePtr ep(__uuidof(DesUtils));
	//_variant_t varPwd = ep->strEnc((const char*)(_bstr_t)var_pwd, key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());
	//_variant_t varSign = ep->strEncSign6(bobj->strJrhm.c_str(), (const char*)(_bstr_t)var_userid, (const char*)(_bstr_t)var_pwd, method.c_str(), "", orderTypeId,
	//	key.substr(0, 3).c_str(), key.substr(3, 3).c_str(), key.substr(6, 3).c_str());

	//_stprintf_s(bobj->data, bobj->datalen, pData, (const char*)(_bstr_t)var_userid, bobj->strJrhm.c_str(), (const char*)(_bstr_t)varPwd, (const char*)(_bstr_t)varSign);
	//bobj->dwRecvedCount = strlen(bobj->data);
	doApi(bobj);
}