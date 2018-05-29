#pragma once
#include "IMessageHandler.h"
extern CSocketBussiness g_SockBuss;
class COperateHandler
{
public:
	COperateHandler(void);
	~COperateHandler(void);
	void HandleClick(TNotifyUI& msg, CPaintManagerUI& pm);
};

