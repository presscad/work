#pragma once

#ifdef _DEBUG
#import "..\BLEDUtils\bin\Debug\BLEDUtils.tlb"
#else
#import "..\BLEDUtils\bin\Release\BLEDUtils.tlb"
#endif // _DEBUG
using namespace BLEDUtils;

void InsertIntoPendingConnMap(SOCKET_OBJ* sobj);

void DeleteFromPendingConnMap(SOCKET_OBJ* sobj);

bool doApi(BUFFER_OBJ* bobj);

void API_Failed(BUFFER_OBJ* bobj);

void DoReturnData(BUFFER_OBJ* bobj);

TCHAR* Utf8ConvertAnsi(const TCHAR* strIn, int inLen);

bool doDisNumberResponse(void* _bobj);