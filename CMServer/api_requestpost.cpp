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

	if (!lpfnConnectEx(a_sobj->sock, (sockaddr*)sAddrInfo->ai_addr, sAddrInfo->ai_addrlen, bobj->data, bobj->dwRecvedCount, &dwBytes, &bobj->ol))
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

//	PostThreadMessage(g_HelpThreadID, MSG_DIS_NUMBER, (WPARAM)pds, 0);// 进行数据库操作
	_T("UPDATE log_tbl set log_state='完成',log_msg='%s',log_etime=now()");

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
	int n = strlen(pResponData);
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

//	PostThreadMessage(g_HelpThreadID, MSG_CARD_STATUS, (WPARAM)pcs, 0);// 进行数据库操作

	return true;
}