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