#include "stdafx.h"
#include "client_signaldata.h"
#include "client_thread.h"
#include "global_data.h"

typedef struct _obj_value
{
	WSAOVERLAPPED ol;
	PTIoRequestFailed pfnFailed;
	PTIoRequestSuccess pfnSuccess;
}OBJ_VALUE;

unsigned int _stdcall client_thread(LPVOID pVoid)
{
	::CoInitialize(NULL);
	ULONG_PTR key;
	OBJ_VALUE* bobj;
	LPOVERLAPPED lpol;
	DWORD dwTranstion;
	BOOL bSuccess;

	while (true)
	{
		bSuccess = GetQueuedCompletionStatus(hComport, &dwTranstion, &key, &lpol, INFINITE);
		if (NULL == lpol)
		{
			printf("���������Ϊ�趨NULLΪ�˳��źţ���ô���Ƿ����ش����ֱ���˳�\n");
			return 0;
		}

		bobj = CONTAINING_RECORD(lpol, OBJ_VALUE, ol);

		if (!bSuccess)
			bobj->pfnFailed((void*)key, bobj);
		else
			bobj->pfnSuccess(dwTranstion, (void*)key, bobj);
	}
	return 0;
}