#pragma once

#ifdef _DEBUG
#import "..\WOTEDUtils\bin\Debug\WOTEDUtils.tlb"
#else
#import "..\WOTEDUtils\bin\Release\WOTEDUtils.tlb"
#endif // _DEBUG
using namespace WOTEDUtils;

void InsertIntoPendingConnMap(SOCKET_OBJ* sobj);

void DeleteFromPendingConnMap(SOCKET_OBJ* sobj);

bool doApi(BUFFER_OBJ* bobj);

void API_Failed(BUFFER_OBJ* bobj);

void DoReturnData(BUFFER_OBJ* bobj);

TCHAR* Utf8ConvertAnsi(const TCHAR* strIn, int inLen);

bool Api_error(BUFFER_OBJ* bobj);

bool doDisNumberResponse(void* _bobj);

bool doCardStatusResponse(void* _bobj);

bool doPoolList01Response(void* _bobj);

bool doPoolList02Response(void* _bobj);

bool doPoolList03Response(void* _bobj);