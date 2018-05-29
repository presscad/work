#pragma once
#include "stdafx.h"
#include "Base64Coder.h"
class CMainPage;


//窗口实例及消息响应部分
class CLoginDlg : public CWindowWnd, public INotifyUI
{
	public:
    CLoginDlg()
	{
		m_nTimeCount = 0;
		m_nTimeReConn = 60*2;//两分钟没收到数据，重连
		m_nTimeSendActive = 30;//30秒若没收到数据发送一次心跳
		
		m_pMainDlg = NULL;
	};
    LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
    UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
    void OnFinalMessage(HWND /*hWnd*/) { delete this; };
    void Init();	
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Notify(TNotifyUI& msg);
	void setTimeCount0(){m_nTimeCount = 0;};
    void OnPrepare() 
    {
	
		char szUser[1024],szPass[1024];
		//unsigned short nPort;
		TCHAR szFilePath[1024 ]={0};
		GetModuleFileName(NULL, szFilePath, 1024);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
		std::string  strPath = szFilePath;
		strPath += "login.ini";
		::GetPrivateProfileString("clientconfig","username","",szUser,1024,strPath.c_str());
		std::string strUser= szUser;
		CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("useredit")));
		pEdit->SetText(szUser);
		CEditUI* pEditPass = static_cast<CEditUI*>(m_pm.FindControl(_T("passwordedit")));
		COptionUI* pRemberCheck = static_cast<COptionUI*>(m_pm.FindControl(_T("remberPWD")));
		m_bSave =	::GetPrivateProfileInt("clientconfig","save",0,strPath.c_str());
		::GetPrivateProfileString("clientconfig","pass","",szPass,1024,strPath.c_str());
		if (m_bSave)
		{
			pRemberCheck->Selected(true);
			CBase64Coder bc;
			m_strPwd = szPass;
			bc.Decode(m_strPwd.c_str());
			m_strPwd=bc.DecodedMessage();
			pEditPass->SetText(m_strPwd.c_str());
		}
		if (!strUser.empty())
		{

			pEditPass->SetFocus();
		}
		else
		{
			pEdit->SetFocus();
		}
    }
	void OnLogin();
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnReturnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//自定义消息
	LRESULT OnShowMainDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowErrorMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	
	
	
public:
    CPaintManagerUI m_pm;
	short m_nTimeCount, m_nTimeReConn, m_nTimeSendActive;
	bool m_bSave;
	string m_strPwd;
	CMainPage *m_pMainDlg;
};