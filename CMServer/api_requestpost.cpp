#include "stdafx.h"
#include <tbb\concurrent_hash_map.h>
#include "client_signaldata.h"
#include "global_data.h"
#include "nc_requestpost.h"
#include "api_requestpost.h"
#include "api_postcompletion.h"
#include "client_mem.h"
#include "tinyxml2.h"
#include "cJSON.h"
#include "cm_mysql.h"

extern void API_ConnectCompFailed(void* _sobj, void* _bobj);
extern void API_ConnectCompSuccess(DWORD dwTransion, void* _sobj, void* _bobj);

tbb::concurrent_hash_map<int, SOCKET_OBJ*> connMap;

void InsertIntoPendingConnMap(SOCKET_OBJ* sobj)
{
	tbb::concurrent_hash_map<int, SOCKET_OBJ*>::accessor a_ad;
	connMap.insert(a_ad, sobj->nKey);
	a_ad->second = sobj;
}

void DeleteFromPendingConnMap(SOCKET_OBJ* sobj)
{
	tbb::concurrent_hash_map<int, SOCKET_OBJ*>::accessor a_rm;
	if (connMap.find(a_rm, sobj->nKey))
		connMap.erase(a_rm);
}

unsigned int _stdcall checkconnectex(LPVOID pVoid)
{
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	while (WaitForSingleObject(hEvent, 60 * 1000) == WAIT_TIMEOUT)
	{
		DWORD dwCurTick = GetTickCount();
		for (tbb::concurrent_hash_map<int, SOCKET_OBJ*>::iterator i = connMap.begin(); i != connMap.end(); ++i)
		{
			if (dwCurTick - i->second->dwTick > 60 * 1000)
				CMCloseSocket(i->second);
		}
	}

	return 0;
}

bool doApi(BUFFER_OBJ* bobj)
{
	int nError = 0;
	DWORD dwBytes = 0;

	ADDRINFOT hints, *sAddrInfo = NULL;
	SOCKET_OBJ* a_sobj = NULL;

	a_sobj = allocSObj();
	if (NULL == a_sobj)
		goto error;

	memset(&hints, 0x00, sizeof(hints));
	hints.ai_flags = 0;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	nError = GetAddrInfo(_T("api.ct10649.com"), _T("9001"), &hints, &sAddrInfo);
	if (0 != nError)
		goto error;

	//char ipbuf[16];
	//struct addrinfo *res, *cur;
	//for (cur = sAddrInfo; cur != NULL; cur = cur->ai_next) {
	//	struct sockaddr_in* addr = (struct sockaddr_in *)cur->ai_addr;
	//	printf("%s\n", inet_ntop(AF_INET,&addr->sin_addr, ipbuf, 16));
	//}

	a_sobj->sAddrInfo = sAddrInfo;
	a_sobj->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == a_sobj->sock)
		goto error;

	struct sockaddr_in taddr;
	taddr.sin_family = AF_INET;
	taddr.sin_port = htons(0);
	taddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (SOCKET_ERROR == bind(a_sobj->sock, (sockaddr*)&taddr, sizeof(taddr)))
		goto error;

	if (NULL == CreateIoCompletionPort((HANDLE)a_sobj->sock, hComport, (ULONG_PTR)a_sobj, 0))
		goto error;

	bobj->SetIoRequestFunction(API_ConnectCompFailed, API_ConnectCompSuccess);

	a_sobj->nKey = GetRand();
	a_sobj->dwTick = GetTickCount();
	InsertIntoPendingConnMap(a_sobj);

	if (!lpfnConnectEx(a_sobj->sock, (sockaddr*)sAddrInfo->ai_addr, (int)sAddrInfo->ai_addrlen, bobj->data, bobj->dwRecvedCount, &dwBytes, &bobj->ol))
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			DeleteFromPendingConnMap(a_sobj);
			goto error;
		}
	}

	return true;

error:
	if (NULL != a_sobj)
	{
		CMCloseSocket(a_sobj);
		if (NULL != a_sobj->sAddrInfo)
			FreeAddrInfo(a_sobj->sAddrInfo);
		freeSObj(a_sobj);
	}

	error_info(bobj, _T("初始化API连接失败"));

	API_Failed(bobj);

	return false;
}

void API_Failed(BUFFER_OBJ* bobj)
{
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);

	_msgpack.pack_array(6);
	_msgpack.pack(bobj->nCmd);
	_msgpack.pack(bobj->nSubCmd);
	_msgpack.pack(bobj->nSubSubCmd);
	_msgpack.pack(1);
	_msgpack.pack(bobj->strTemp);
	_msgpack.pack_array(1);
	_msgpack.pack_array(1);
	_msgpack.pack(std::string(bobj->data));

	DealTail(sbuf, bobj);
}

void DoReturnData(BUFFER_OBJ* bobj)
{
	if (!bobj->pfndoApiResponse(bobj))
	{
		return API_Failed(bobj);
	}
}

TCHAR* Utf8ConvertAnsi(const TCHAR* strIn, int inLen)
{
	//int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, strIn, -1, NULL, 0); // 如果字符串不是以空字符中止，设置为-1可能失败，可能成功
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, strIn, inLen, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, strIn, inLen, (LPWSTR)pUnicode, unicodeLen);
	int targetLen = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)pUnicode, -1, NULL, 0, NULL, NULL);
	BYTE* pTargetData = new BYTE[targetLen + 1];
	memset(pTargetData, 0, targetLen + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)pUnicode, -1, (char*)pTargetData, targetLen, NULL, NULL);

	delete pUnicode;
	return (TCHAR*)pTargetData;
}

bool Api_error(BUFFER_OBJ* bobj)
{
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);

	_msgpack.pack_array(6);
	_msgpack.pack(bobj->nCmd);
	_msgpack.pack(bobj->nSubCmd);
	_msgpack.pack(bobj->nSubSubCmd);
	_msgpack.pack(1);
	_msgpack.pack(bobj->strTemp);
	_msgpack.pack_array(1);
	_msgpack.pack_array(1);
	_msgpack.pack(std::string(bobj->data));

	DealTail(sbuf, bobj);
	return false;
}

//	static const char* test = "<?xml version = \"1.0\" encoding = \"utf-8\"?>"
//	"<businessServiceResponse>" // 业务根节点
//		"<RspType>0</RspType>"	// 请求状态 返回0，标识请求成功
//		"<result>0</result>"	// 状态响应码 返回0，标识成功接收消息
//		"<resultMsg>成功接收消息</resultMsg>"	// 返回消息信息
//		"<GROUP_TRANSACTIONID>1000000252201606149170517340</GROUP_TRANSACTIONID>" // 流水号 请求流水
//	"</businessServiceResponse>";
bool doDisNumberResponse(void* _bobj)
{
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;
	TCHAR* pResponData = Utf8ConvertAnsi(bobj->data, bobj->dwRecvedCount);
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(pResponData))
	{
		error_info(bobj, _T("%s"), pResponData);
		delete pResponData;
		return false;
	}
	delete pResponData;

	tinyxml2::XMLElement* root = doc.RootElement();
	tinyxml2::XMLElement* RspType = root->FirstChildElement(); // <RspType>0</RspType>
	const TCHAR* pRspType = RspType->GetText();

	tinyxml2::XMLElement* result = RspType->NextSiblingElement(); // <result>0</result>
	const TCHAR* presult = result->GetText();

	tinyxml2::XMLElement* resultMsg = result->NextSiblingElement(); // <resultMsg>成功接收消息</resultMsg>
	const TCHAR* presultMsg = resultMsg->GetText();

	tinyxml2::XMLElement* GROUP_TRANSACTIONID = resultMsg->NextSiblingElement(); // <GROUP_TRANSACTIONID>1000000252201606149170517340</GROUP_TRANSACTIONID>
	const TCHAR* pGROUP_TRANSACTIONID = GROUP_TRANSACTIONID->GetText();

	const TCHAR* pSql = NULL;
	TCHAR* sql = new TCHAR[256 * sizeof(TCHAR)];
	memset(sql, 0x00, 256 * sizeof(TCHAR));

	pSql = _T("UPDATE log_tbl SET lshm='%s',reslutmsg='%s' WHERE id=%u");
	_stprintf_s(sql, 256 * sizeof(TCHAR), pSql, pGROUP_TRANSACTIONID, presultMsg, bobj->nPerLogID);

	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);

	_msgpack.pack_array(6);
	_msgpack.pack(bobj->nCmd);
	_msgpack.pack(bobj->nSubCmd);
	_msgpack.pack(bobj->nSubSubCmd);
	_msgpack.pack(0);
	_msgpack.pack(bobj->strTemp);
	_msgpack.pack_array(1);
	_msgpack.pack_array(4);
	_msgpack.pack(std::string(pRspType));
	_msgpack.pack(std::string(presult));
	_msgpack.pack(std::string(presultMsg));
	_msgpack.pack(std::string(pGROUP_TRANSACTIONID));

	DealTail(sbuf, bobj);

	PostThreadMessage(mysqlThreadId, MYSQL_UPDATE, (WPARAM)sql, NULL);

	return true;
}

//<?xml version = "1.0" encoding = "utf-8"?>
//<root>
//	<Query_response>
//		<BasicInfo>
//			<result>0</result>
//			<resultMsg>处理成功!</resultMsg>
//		</BasicInfo>
//		<prodRecords>
//			<prodRecord>
//<				productInfo>
//					<productStatusCd>1</productStatusCd>
//					<productStatusName>在用</productStatusName>
//					<servCreateDate>20160531</servCreateDate>
//				</productInfo>
//			</prodRecord>
//		</prodRecords>
//		<number>14910000000</number>
//		<GROUP_TRANSACTIONID>1000000252201609302104391983</GROUP_TRANSACTIONID>
//	</Query_response>
//</root>
bool doCardStatusResponse(void* _bobj)
{
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;
	TCHAR* pResponData = Utf8ConvertAnsi(bobj->data, bobj->dwRecvedCount);
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(pResponData))
	{
		error_info(bobj, _T("%s"), pResponData);
		delete pResponData;
		return false;
	}
	delete pResponData;

	tinyxml2::XMLElement* root = doc.RootElement();
	tinyxml2::XMLElement* BasicInfo = root->FirstChildElement()->FirstChildElement();
	tinyxml2::XMLElement* result = BasicInfo->FirstChildElement(); // <result>0</result>
	const TCHAR* presult = result->GetText();

	tinyxml2::XMLElement* resultMsg = result->NextSiblingElement(); // <resultMsg>处理成功!</resultMsg>
	const TCHAR* presultMsg = resultMsg->GetText();

	tinyxml2::XMLElement* prodRecords = BasicInfo->NextSiblingElement();
	tinyxml2::XMLElement* productInfo = prodRecords->FirstChildElement()->FirstChildElement();
	tinyxml2::XMLElement* productStatusCd = productInfo->FirstChildElement(); // <productStatusCd>1</productStatusCd>
	const TCHAR* pproductStatusCd = productStatusCd->GetText();

	tinyxml2::XMLElement* productStatusName = productStatusCd->NextSiblingElement(); // <productStatusName>在用</productStatusName>
	const TCHAR* pproductStatusName = productStatusName->GetText();

	tinyxml2::XMLElement* servCreateDate = productStatusName->NextSiblingElement(); // <servCreateDate>20160531</servCreateDate>
	const TCHAR* pservCreateDate = servCreateDate->GetText();

	tinyxml2::XMLElement* number = prodRecords->NextSiblingElement(); // <number>14910000000</number>
	const TCHAR* pnumber = number->GetText();

	tinyxml2::XMLElement* GROUP_TRANSACTIONID = number->NextSiblingElement(); // <GROUP_TRANSACTIONID>1000000252201609302104391983</GROUP_TRANSACTIONID>
	const TCHAR* pGROUP_TRANSACTIONID = GROUP_TRANSACTIONID->GetText();

	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);

	_msgpack.pack_array(6);
	_msgpack.pack(bobj->nCmd);
	_msgpack.pack(bobj->nSubCmd);
	_msgpack.pack(bobj->nSubSubCmd);
	_msgpack.pack(0);
	_msgpack.pack(bobj->strTemp);
	_msgpack.pack_array(1);
	_msgpack.pack_array(6);
	_msgpack.pack(std::string(presult));
	_msgpack.pack(std::string(presultMsg));
	_msgpack.pack(std::string(pproductStatusCd));
	_msgpack.pack(std::string(pproductStatusName));
	_msgpack.pack(std::string(pservCreateDate));
	//_msgpack.pack(pcs->strNumber);
	_msgpack.pack(std::string(pGROUP_TRANSACTIONID));

	DealTail(sbuf, bobj);

	// 150001未实名制违规停机
	const TCHAR* pSql = _T("UPDATA sim_tbl SET Zt=%s WHERE Jrhm='%s'");
	TCHAR sql[256];
	memset(sql, 0x00, sizeof(sql));
	_stprintf_s(sql, sizeof(sql), pSql, pproductStatusCd, pnumber);


	MYSQL* pMysql = Mysql_AllocConnection();
	if (NULL == pMysql)
	{
		return true;;
	}
	size_t len = _tcslen(sql);
	if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
	{
		mysql_rollback(pMysql);
		return true;
	}
	mysql_rollback(pMysql);

	return true;
}

//	<?xml version = "1.0" encoding = "utf-8"?>
//	<poolList>
//		<pool>
//			<create_date>2016-02-19</create_date>
//			<pool_infoUnit>GB</pool_infoUnit>
//			<state>在用</state>
//			<pool_info>10</pool_info>
//			<exp_date>3000-01-01</exp_date>
//			<acc_nbr>50LLC04244</acc_nbr>
//			<eff_date>2016-02-19</eff_date>
//		</pool>
//	</poolList>
bool doPoolList01Response(void* _bobj)
{
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;
	TCHAR* pResponData = Utf8ConvertAnsi(bobj->data, bobj->dwRecvedCount);
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(pResponData))
	{
		error_info(bobj, _T("%s"), pResponData);
		delete pResponData;
		return false;
	}
	delete pResponData;

	tinyxml2::XMLElement* poollist = doc.RootElement();
	tinyxml2::XMLElement* pool = poollist->FirstChildElement();
	int nElemCount = 0;
	while (pool)
	{
		++nElemCount;
		pool = pool->NextSiblingElement();
	}
	pool = poollist->FirstChildElement();

	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(4);
	_msgpack.pack(bobj->nCmd);
	_msgpack.pack(bobj->nSubCmd);
	_msgpack.pack(0);
	_msgpack.pack_array(nElemCount);
	while (pool)
	{
		_msgpack.pack_array(7);
		tinyxml2::XMLElement* create_data = pool->FirstChildElement(); // <create_date>2016 - 02 - 19< / create_date>
		_msgpack.pack(std::string(create_data->GetText()));
		tinyxml2::XMLElement* pool_infoUnit = create_data->NextSiblingElement(); // <pool_infoUnit>GB</pool_infoUnit>
		_msgpack.pack(std::string(pool_infoUnit->GetText()));
		tinyxml2::XMLElement* state = pool_infoUnit->NextSiblingElement(); // <state>在用</state>
		_msgpack.pack(std::string(state->GetText()));
		tinyxml2::XMLElement* pool_info = state->NextSiblingElement(); // <pool_info>10</pool_info>
		_msgpack.pack(std::string(pool_info->GetText()));
		tinyxml2::XMLElement* exp_date = pool_info->NextSiblingElement(); // <exp_date>3000-01-01</exp_date>
		_msgpack.pack(std::string(exp_date->GetText()));
		tinyxml2::XMLElement* acc_nbr = exp_date->NextSiblingElement(); // <acc_nbr>50LLC04244</acc_nbr>
		_msgpack.pack(std::string(acc_nbr->GetText()));
		tinyxml2::XMLElement* eff_date = acc_nbr->NextSiblingElement(); // <eff_date>2016-02-19</eff_date>
		_msgpack.pack(std::string(eff_date->GetText()));

		pool = pool->NextSiblingElement();
	}

	DealTail(sbuf, bobj);

	return true;
}

//<?xml version = "1.0" encoding = "utf-8"?>
//<SvcCont>
//	<pageIndex>1</pageIndex>
//	<OutProdInfos>
//		<acc_nbr>1064910000000</acc_nbr>
//		<eff_date>2016/03/08 15:03:28</eff_date>
//		<flow_quota>0</flow_quota>
//		<org_id>8320100</org_id>
//		<quota_type>1</quota_type>
//		<state>在用</state>
//	</OutProdInfos>
//	<resultCode>0</resultCode>
//	<resultMsg>成功</resultMsg>
//	<totalPage>360</totalPage>
//	<GROUP_TRANSACTIONID>1000000252201606165545289689</GROUP_TRANSACTIONID>
//</SvcCont>
bool doPoolList02Response(void* _bobj)
{
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;
	TCHAR* pResponData = Utf8ConvertAnsi(bobj->data, bobj->dwRecvedCount);
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(pResponData))
	{
		error_info(bobj, _T("%s"), pResponData);
		delete pResponData;
		return false;
	}
	delete pResponData;

	return true;
}

//<?xml version = "1.0"?>
//<SvcCont>
//	<IRESULT>0</IRESULT>
//	<pool_already>458422599</pool_already>
//	<pool_left>71108281</pool_left>
//	<pool_total>529530880</pool_total>
//	<GROUP_TRANSACTIONID>1000000190201804216859677554</GROUP_TRANSACTIONID>
//</SvcCont>
bool dopoolQryResponse(void* _bobj)
{
	BUFFER_OBJ* bobj = (BUFFER_OBJ*)_bobj;
	TCHAR* pResponData = Utf8ConvertAnsi(bobj->data, bobj->dwRecvedCount);
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(pResponData))
	{
		delete pResponData;
		return false;
	}
	delete pResponData;

	tinyxml2::XMLElement* SvcCont = doc.RootElement();
	tinyxml2::XMLElement* IRESULT = SvcCont->FirstChildElement();
	tinyxml2::XMLElement* pool_already = IRESULT->FirstChildElement();
	tinyxml2::XMLElement* pool_left = pool_already->NextSiblingElement();
	tinyxml2::XMLElement* pool_total = pool_left->NextSiblingElement();
	tinyxml2::XMLElement* GROUP_TRANSACTIONID = pool_total->NextSiblingElement();

	const TCHAR* pSql = _T("SELECT COUNT(*) as num FROM sim_tbl WHERE Llchm='%s'");
	TCHAR sql[256];
	memset(sql, 0x00, sizeof(pSql));
	_stprintf_s(sql, sizeof(sql), pSql, bobj->strTemp.c_str());
	MYSQL* pMysql = Mysql_AllocConnection();
	if (NULL == pMysql)
	{
		error_info(bobj, _T("连接数据库失败"));
		return false;
	}

	MYSQL_RES* res = NULL;
	if (!SelectFromTbl(sql, pMysql, bobj, &res))
	{
		Mysql_BackToPool(pMysql);
		return false;
	}
	MYSQL_ROW row = mysql_fetch_row(res);
	mysql_free_result(res);

	unsigned int nNum = 0;
	sscanf_s(row[0], "%u", &nNum);

	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);

	_msgpack.pack_array(4);
	_msgpack.pack(bobj->nCmd);
	_msgpack.pack(bobj->nSubCmd);
	_msgpack.pack(0);
	_msgpack.pack_array(1);
	_msgpack.pack_array(5);
	_msgpack.pack(nNum);
	_msgpack.pack(std::string(IRESULT->GetText()));// 执行成功
	_msgpack.pack(std::string(pool_already->GetText())); // 已经使用的流量
	_msgpack.pack(std::string(pool_left->GetText())); // 剩余的流量
	_msgpack.pack(std::string(pool_total->GetText())); // 总流量

	DealTail(sbuf, bobj);

	return true;
}