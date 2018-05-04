#include "stdafx.h"
#include "client_signaldata.h"
#include "global_data.h"
#include "api_postcompletion.h"
#include "nc_requestpost.h"
#include "api_requestpost.h"
#include "client_mem.h"

void API_ConnectCompFailed(void* _sobj, void* _bobj)
{
	SOCKET_OBJ* a_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

	DeleteFromPendingConnMap(a_sobj);
	FreeAddrInfo(a_sobj->sAddrInfo);

#ifdef _DEBUG
	DWORD dwTranstion = 0;
	DWORD dwFlags = 0;
	if (!WSAGetOverlappedResult(a_sobj->sock, &c_bobj->ol, &dwTranstion, FALSE, &dwFlags))
		_tprintf(_T("函数:%s ErrorCode = %d\n"), __FUNCTION__, WSAGetLastError());
#endif

	CMCloseSocket(a_sobj);
	freeSObj(a_sobj);
	error_info(c_bobj, _T("连接API失败"));
	API_Failed(c_bobj);
}

void API_ConnectCompSuccess(DWORD dwTransion, void* _sobj, void* _bobj)
{
	if (dwTransion <= 0)
		return API_ConnectCompFailed(_sobj, _bobj);

	SOCKET_OBJ* a_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

	DeleteFromPendingConnMap(a_sobj);
	FreeAddrInfo(a_sobj->sAddrInfo);

	c_bobj->dwSendedCount += dwTransion;
	if (c_bobj->dwSendedCount < c_bobj->dwRecvedCount)
	{
		c_bobj->SetIoRequestFunction(API_SendCompFailed, API_SendCompSuccess);
		if (!Nc_PostSend(a_sobj, c_bobj))
		{
			CMCloseSocket(a_sobj);
			freeSObj(a_sobj);
			goto error;
		}
		return;
	}

	memset(c_bobj->data, 0x00, c_bobj->dwRecvedCount);
	c_bobj->dwRecvedCount = 0;
	c_bobj->dwSendedCount = 0;
	c_bobj->SetIoRequestFunction(API_RecvZeroCompFailed, API_RecvZeroCompSuccess);
	if (!Nc_PostZeroRecv(a_sobj, c_bobj))
	{
		CMCloseSocket(a_sobj);
		freeSObj(a_sobj);
		goto error;;
	}

	return;
error:
	error_info(c_bobj, _T("连接API失败"));
	API_Failed(c_bobj);
	return;
}

void API_SendCompFailed(void* _sobj, void* _bobj)
{
	SOCKET_OBJ* a_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

#ifdef _DEBUG
	DWORD dwTranstion = 0;
	DWORD dwFlags = 0;
	if (!WSAGetOverlappedResult(a_sobj->sock, &c_bobj->ol, &dwTranstion, FALSE, &dwFlags))
		_tprintf(_T("函数:%s ErrorCode = %d\n"), __FUNCTION__, WSAGetLastError());
#endif

	CMCloseSocket(a_sobj);
	freeSObj(a_sobj);
	error_info(c_bobj, _T("发送API数据失败"));
	API_Failed(c_bobj);
}

void API_SendCompSuccess(DWORD dwTransion, void* _sobj, void* _bobj)
{
	if (dwTransion <= 0)
		return API_SendCompFailed(_sobj, _bobj);

	SOCKET_OBJ* a_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

	c_bobj->dwSendedCount += dwTransion;
	if (c_bobj->dwSendedCount < c_bobj->dwRecvedCount)
	{
		if (!Nc_PostSend(a_sobj, c_bobj))
		{
			CMCloseSocket(a_sobj);
			freeSObj(a_sobj);
			goto error;
		}
		return;
	}

	c_bobj->dwRecvedCount = 0;
	c_bobj->dwSendedCount = 0;
	c_bobj->SetIoRequestFunction(API_RecvZeroCompFailed, API_RecvZeroCompSuccess);
	if (!Nc_PostZeroRecv(a_sobj, c_bobj))
	{
		CMCloseSocket(a_sobj);
		freeSObj(a_sobj);
		goto error;;
	}
	return;

error:
	error_info(c_bobj, _T("发送API数据失败"));
	API_Failed(c_bobj);
	return;
}

void API_RecvZeroCompFailed(void* _sobj, void* _bobj)
{
	SOCKET_OBJ* a_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

#ifdef _DEBUG
	DWORD dwTranstion = 0;
	DWORD dwFlags = 0;
	if (!WSAGetOverlappedResult(a_sobj->sock, &c_bobj->ol, &dwTranstion, FALSE, &dwFlags))
		_tprintf(_T("函数:%s ErrorCode = %d\n"), __FUNCTION__, WSAGetLastError());
#endif

	CMCloseSocket(a_sobj);
	freeSObj(a_sobj);
	error_info(c_bobj, _T("接收API数据失败"));
	API_Failed(c_bobj);
}

void API_RecvZeroCompSuccess(DWORD dwTransion, void* _sobj, void* _bobj)
{
	SOCKET_OBJ* c_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

	c_bobj->SetIoRequestFunction(API_RecvCompFailed, API_RecvCompSuccess);
	if (!Nc_PostRecv(c_sobj, c_bobj))
	{
		CMCloseSocket(c_sobj);
		freeSObj(c_sobj);
		error_info(c_bobj, _T("接收API数据失败"));
		API_Failed(c_bobj);
	}
}

void API_RecvCompFailed(void* _sobj, void* _bobj)
{
	SOCKET_OBJ* a_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

#ifdef _DEBUG
	DWORD dwTranstion = 0;
	DWORD dwFlags = 0;
	if (!WSAGetOverlappedResult(a_sobj->sock, &c_bobj->ol, &dwTranstion, FALSE, &dwFlags))
		_tprintf(_T("函数:%s ErrorCode = %d\n"), __FUNCTION__, WSAGetLastError());
#endif

	CMCloseSocket(a_sobj);
	freeSObj(a_sobj);
	error_info(c_bobj, _T("接收API数据失败"));
	API_Failed(c_bobj);
}

void API_RecvCompSuccess(DWORD dwTransion, void* _sobj, void* _bobj)
{
	if (dwTransion < 0)
		return API_RecvCompFailed(_sobj, _bobj);

	SOCKET_OBJ* a_sobj = (SOCKET_OBJ*)_sobj;
	BUFFER_OBJ* c_bobj = (BUFFER_OBJ*)_bobj;

	if (dwTransion == 0) // 对方关闭连接
	{
		_tprintf(_T("客户端关闭\n"));
		CMCloseSocket(a_sobj);
		freeSObj(a_sobj);
		DoReturnData(c_bobj);
		return;
	}

	c_bobj->dwRecvedCount += dwTransion;
	c_bobj->SetIoRequestFunction(API_RecvZeroCompFailed, API_RecvZeroCompSuccess);
	if (!Nc_PostZeroRecv(a_sobj, c_bobj))
	{
		CMCloseSocket(a_sobj);
		freeSObj(a_sobj);
		error_info(c_bobj, _T("接收API数据失败"));
		API_Failed(c_bobj);
		return;
	}
}