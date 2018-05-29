#pragma once
#include "stdafx.h"
class CRegePage: public CWindowWnd, public INotifyUI
{
public:
	CRegePage(void);
	~CRegePage(void);
public:
	
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Init() {
		m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	
	}

	void OnPrepare() {
		m_pUserEdit =static_cast<CEditUI*>(m_pm.FindControl(_T("useredit")));
		m_pPassEdit =static_cast<CEditUI*>(m_pm.FindControl(_T("passwordedit")));
		m_pPassConfirmEdit =static_cast<CEditUI*>(m_pm.FindControl(_T("passwordconfedit")));
		
	}

	void Notify(TNotifyUI& msg)
	{
		if( msg.sType == _T("windowinit") ) OnPrepare();
		else if( msg.sType == _T("click") ) 
		{
			if( msg.pSender->GetName() == _T("closebtn") ) 
			{
				Close();
			}
			else if (msg.pSender->GetName() == _T("cancelbtn"))
			{
				Close();
			}
			else if (msg.pSender->GetName() == _T("regbtn"))
			{				
				string strUser = m_pUserEdit->GetText();
				string strPass = m_pPassConfirmEdit->GetText();
				string strConPass= m_pPassEdit->GetText();
				if (strUser.length()== 0 || strPass.length()== 0 || strConPass.length()== 0)
				{
					MyMessageBox(*this,"用户名和密码不能为空！");
					return;
				}
				if (strPass != strConPass)
				{
					MyMessageBox(*this,"密码不一致！","错误");
				   return;
				}
				char szIP[1024];
				unsigned short nPort;
				TCHAR szFilePath[1024 ]={0};
				GetModuleFileName(NULL, szFilePath, 1024);
				(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
				std::string  strPath = szFilePath;
				strPath += "login.ini";
				::GetPrivateProfileString("clientconfig","ServerIP","192.168.1.188",szIP,1024,strPath.c_str());
				nPort = ::GetPrivateProfileInt("clientconfig","ServerPort",6086,strPath.c_str());
				g_SockBuss.SetAddr(nPort,szIP);
			/*	if (g_SockBuss.Connect()== -1)
				{
					MyMessageBox(*this,"连接服务器失败！");
					return;
				}*/
				g_SockBuss.SendRegInfo(strUser,strPass);
			}
					
		}
	
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("注册.xml"), (UINT)0,  0, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		Init();
		return 0;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//::PostQuitMessage(0L);
		SendMessage(WM_CLOSE);

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( ::IsIconic(*this) ) bHandled = FALSE;
		return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		// 		if( !::IsZoomed(*this) ) {
		// 			RECT rcSizeBox = m_pm.GetSizeBox();
		// 			if( pt.y < rcClient.top + rcSizeBox.top ) {
		// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
		// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
		// 				return HTTOP;
		// 			}
		// 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
		// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
		// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
		// 				return HTBOTTOM;
		// 			}
		// 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		// 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
		// 		}

		RECT rcCaption = m_pm.GetCaptionRect();
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
				if( pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 && 
					_tcscmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
					_tcscmp(pControl->GetClass(), DUI_CTR_TEXT) != 0 )
					return HTCAPTION;
		}

		return HTCLIENT;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SIZE szRoundCorner = m_pm.GetRoundCorner();
		if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
			CDuiRect rcWnd;
			::GetWindowRect(*this, &rcWnd);
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);
			rcWnd.right++; rcWnd.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
			::SetWindowRgn(*this, hRgn, TRUE);
			::DeleteObject(hRgn);
		}

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

		LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
		lpMMI->ptMaxPosition.x	= rcWork.left;
		lpMMI->ptMaxPosition.y	= rcWork.top;
		lpMMI->ptMaxSize.x		= rcWork.right;
		lpMMI->ptMaxSize.y		= rcWork.bottom;

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) {
			if( !bZoomed ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch( uMsg ) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = FALSE;
		}
		if( bHandled ) return lRes;
		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

public:
	CPaintManagerUI m_pm;
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pRegBtn;
	CEditUI* m_pUserEdit;
	CEditUI* m_pPassEdit;
	CEditUI* m_pPassConfirmEdit;

};

