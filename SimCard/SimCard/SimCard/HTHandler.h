#pragma once
#include "IMessageHandler.h"


extern CSocketBussiness g_SockBuss;
class CHTHandler:public IMessageHandler
{
public:
	CHTHandler(void);
	~CHTHandler(void);
	void HandleClick(TNotifyUI& msg, CPaintManagerUI& pm);
public:
	void HandleKaiCarExport(CPaintManagerUI& pm);
	void HandleChuCarExport(CPaintManagerUI& pm);
	void HandleXufeiExport(CPaintManagerUI& pm);
	void HandleTuiCarExport(CPaintManagerUI& pm);
	void HandleXiaoCarExport(CPaintManagerUI& pm);
	void HandleKehuExport(CPaintManagerUI& pm);
	void HandleExportAll(CPaintManagerUI& pm);
	void HandleCardSave(CPaintManagerUI& pm);
	CSocketBussiness m_SockBuss;
};

