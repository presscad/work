#pragma once
#include "IMessageHandler.h"
extern CSocketBussiness g_SockBuss;
class COrderHandler
{
public:
	COrderHandler(void);
	~COrderHandler(void);
	void HandleClick(TNotifyUI& msg, CPaintManagerUI& pm);
};

