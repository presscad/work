#pragma once
#include "IMessageHandler.h"

extern CSocketBussiness g_SockBuss;
class CUserHandler:public IMessageHandler
{
public:
	CUserHandler(void);
	~CUserHandler(void);
	void HandleClick(TNotifyUI& msg, CPaintManagerUI& pm);
};

