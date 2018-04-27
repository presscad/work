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

	API_Failed(bobj);

	return false;
}

void API_Failed(BUFFER_OBJ* bobj)
{
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);

	_msgpack.pack_array(3);
	_msgpack.pack(bobj->nCmd);
	_msgpack.pack(bobj->nSubCmd);
	_msgpack.pack(1);

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
		delete pResponData;
		return false;
	}
	delete pResponData;

	//DIS_NUMBER* pds = new DIS_NUMBER;
	//pds->strJrhm = bobj->strJrhm;
	tinyxml2::XMLElement* root = doc.RootElement();
	tinyxml2::XMLElement* RspType = root->FirstChildElement(); // <RspType>0</RspType>
	//pds->strRsptype = RspType->GetText();

	tinyxml2::XMLElement* result = RspType->NextSiblingElement(); // <result>0</result>
	//pds->strResult = result->GetText();

	tinyxml2::XMLElement* resultMsg = result->NextSiblingElement(); // <resultMsg>成功接收消息</resultMsg>
	//pds->strResultMsg = resultMsg->GetText();

	tinyxml2::XMLElement* GROUP_TRANSACTIONID = resultMsg->NextSiblingElement(); // <GROUP_TRANSACTIONID>1000000252201606149170517340</GROUP_TRANSACTIONID>
	//pds->strGROUP_TRANSACTIONID = GROUP_TRANSACTIONID->GetText();

	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);

	_msgpack.pack_array(6);
	_msgpack.pack(bobj->nCmd);
	_msgpack.pack(bobj->nSubCmd);
	_msgpack.pack(bobj->nSubSubCmd);
	_msgpack.pack(0);
//	_msgpack.pack(bobj->strJrhm);
	_msgpack.pack_array(1);
	_msgpack.pack_array(4);
	//_msgpack.pack(atoi(pds->strRsptype.c_str()));
	//_msgpack.pack(atoi(pds->strResult.c_str()));
	//_msgpack.pack(pds->strResultMsg);
	//_msgpack.pack(pds->strGROUP_TRANSACTIONID);

	DealTail(sbuf, bobj);

//	PostThreadMessage(g_HelpThreadID, MSG_DIS_NUMBER, (WPARAM)pds, 0);// 进行数据库操作

	return true;
}