#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_ncdata.h"
//#include "tinyxml2.h"
#include "global_data.h"
#include "cmd_error.h"
#include "nc_requestpost.h"
#include "nc_postcompletion.h"
#include "client_mem.h"

extern TCHAR* Utf8ConvertAnsi(const TCHAR* strIn, int inLen);

bool DoNotifyContractRoot(tinyxml2::XMLElement* root);
bool DoWanrningContractRoot(tinyxml2::XMLElement* RootElment);
void doNcResponse(BUFFER_OBJ* bobj);

bool doNcData(const TCHAR* pData)
{
	_tprintf_s(_T("NC--%s\n"), pData);
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(pData))
	{
		delete pData;
		_tprintf_s(_T("NC--%s\n"), _T("格式化xml失败"));
		return false;
	}
	delete pData;
	tinyxml2::XMLElement* RootElment = doc.RootElement();
	const TCHAR* rootName = RootElment->Value();
	if (_tcscmp(rootName, _T("NotifyContractRoot")) == 0)
	{
		return DoNotifyContractRoot(RootElment);
	}
	else if (_tcscmp(rootName, _T("NotifyContractRoot")) == 0)
	{
		return DoWanrningContractRoot(RootElment);
	}

	return true;
}

//<?xml version = "1.0" encoding = "utf-8"?>
//<NotifyContractRoot>
//<TYPE>1< / TYPE>
//<GROUP_TRANSACTIONID>1000000252201605131983898593< / GROUP_TRANSACTIONID>
//<CUSTOMER_TRANSACTIONID></CUSTOMER_TRANSACTIONID>
//<STATUSINFO>竣工</STATUSINFO>
//<ACCNBR>1064915901733</ACCNBR>
//<ACCEPTTYPE>8</ACCEPTTYPE>
//<ACCEPTMSG>号码:1064915901733，办理停机保号</ACCEPTMSG>
//<STATUSDT>20160513153905</STATUSDT>
//<RESULTMSG>成功</RESULTMSG>
//</NotifyContractRoot>

bool DoNotifyContractRoot(tinyxml2::XMLElement* root)
{
	tinyxml2::XMLElement* GROUP_TRANSACTIONID = root->FirstChildElement("GROUP_TRANSACTIONID");
	tinyxml2::XMLElement* STATUSINFO = root->FirstChildElement("STATUSINFO");
	tinyxml2::XMLElement* ACCNBR = root->FirstChildElement("ACCNBR");
	tinyxml2::XMLElement* ACCEPTTYPE = root->FirstChildElement("ACCEPTTYPE");
	tinyxml2::XMLElement* STATUSDT = root->FirstChildElement("STATUSDT");
	if (NULL == GROUP_TRANSACTIONID || NULL == STATUSINFO || NULL == ACCNBR || NULL == ACCEPTTYPE || NULL == STATUSDT)
	{
		_tprintf_s(_T("接收到的报竣数据格式有误\n"));
		return false;
	}

	const TCHAR* pGROUP_TRANSACTIONID = GROUP_TRANSACTIONID->GetText();
	const TCHAR* pSTATUSINFO = STATUSINFO->GetText();
	const TCHAR* pACCNBR = ACCNBR->GetText();
	const TCHAR* pACCEPTTYPE = ACCEPTTYPE->GetText();
	const TCHAR* pSTATUSDT = STATUSDT->GetText();

	int nType = _tstoi(pACCEPTTYPE);

	const TCHAR* pSql = NULL;
	TCHAR* sql = new TCHAR[256 * sizeof(TCHAR)];
	memset(sql, 0x00, 256 * sizeof(TCHAR));

	pSql = _T("UPDATE log_tbl SET statusinfo='%s',statusdt='%s' WHERE lshm='%s'");
	_stprintf_s(sql, 256 * sizeof(TCHAR), pSql, pSTATUSINFO, pSTATUSDT, pGROUP_TRANSACTIONID);
	PostThreadMessage(mysqlThreadId, MYSQL_UPDATE, (WPARAM)sql, NULL);

	// 根据流水号匹配停复机响应信息
	switch (nType)
	{
	case 1:
	{}
	break;
	case 2:
	{}
	break;
	case 3:
	{}
	break;
	case 4:
	{}
	break;
	case 5:
	{}
	break;
	case 7:// 活卡激活
	{
		
	}
	break;
	case 8:// 停机
	{
		if (_tcscmp(pSTATUSINFO, _T("竣工")) == 0) // 修改状态
		{
			pSql = _T("UPDATE sim_tbl SET zt=2 WHERE Jrhm='%s'");
			TCHAR* sql = new TCHAR[256 * sizeof(TCHAR)];
			memset(sql, 0x00, 256 * sizeof(TCHAR));
			_stprintf_s(sql, 256 * sizeof(TCHAR), pSql, pACCNBR);
			PostThreadMessage(mysqlThreadId, MYSQL_UPDATE, (WPARAM)sql, NULL);
		}
	}
	break;
	case 9:// 复机
	{
		if (_tcscmp(pSTATUSINFO, _T("竣工")) == 0) // 修改状态
		{
			pSql = _T("UPDATE sim_tbl SET zt=1 WHERE Jrhm='%s'");
			TCHAR* sql = new TCHAR[256 * sizeof(TCHAR)];
			memset(sql, 0x00, 256 * sizeof(TCHAR));
			_stprintf_s(sql, 256 * sizeof(TCHAR), pSql, pACCNBR);
			PostThreadMessage(mysqlThreadId, MYSQL_UPDATE, (WPARAM)sql, NULL);
		}
	}
	break;
	case 10:
	{}
	break;
	case 11:
	{}
	break;
	case 12:
	{}
	break;
	case 13:
	{}
	break;
	case 14:
	{}
	break;
	default:
		break;
	}

	return true;
}

bool DoWanrningContractRoot(tinyxml2::XMLElement* RootElment)
{
	tinyxml2::XMLElement* pType = RootElment->FirstChildElement("TYPE");
	if (NULL == pType)
	{
		_tprintf(_T("接收到的xml节点RspType格式错误\n"));
		return false;
	}
	const TCHAR* pcType = pType->GetText();
	_tprintf(_T("%s\n"), pcType);
	if (_tstoi(pcType) != 2)
	{
		return false;
	}
	tinyxml2::XMLElement* pAccnbr = pType->NextSiblingElement("ACCNBR");
	tinyxml2::XMLElement* pWarningtype = pAccnbr->NextSiblingElement("WARNINGTYPE");
	tinyxml2::XMLElement* pWarningtypemsg = pWarningtype->NextSiblingElement("WARNINGTYPEMSG");
	return true;
}

void doNcResponse(BUFFER_OBJ* bobj)
{
	//const TCHAR* pData = _T("HTTP/1.1 200 OK\r\n<?xml version = \"1.0\" encoding = \"UTF8\"?><ContractRoot><SUCCESS>true</SUCCESS><NOTIFY_CODE>000000</NOTIFY_CODE><RESULTMSG>调用成功</RESULTMSG></ContractRoot>\r\n\r\n");
	const TCHAR* pData = _T("HTTP/1.1 200 OK\r\n\r\n");
	bobj->dwRecvedCount = (DWORD)strlen(pData);
	memcpy_s(bobj->data, bobj->datalen, pData, bobj->dwRecvedCount);
	bobj->SetIoRequestFunction(NC_SendCompFailed, NC_SendCompSuccess);
	if (!Nc_PostSend((SOCKET_OBJ*)bobj->pRelateClientSock, bobj))
	{
		CMCloseSocket(bobj->pRelateClientSock);
		freeSObj((SOCKET_OBJ*)bobj->pRelateClientSock);
		freeBObj(bobj);
	}
}