#pragma once
#include "IMessageHandler.h"
extern CSocketBussiness g_SockBuss;
class CQueryHandler
{
public:
	CQueryHandler(void);
	~CQueryHandler(void);
	void HandleClick(TNotifyUI& msg, CPaintManagerUI& pm);
};

