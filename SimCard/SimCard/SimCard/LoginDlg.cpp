#pragma once
#include "stdafx.h"
#include "LoginDlg.h"
#include "MainPage.h"
#include "msgpack.hpp"
#include "SocketBussiness.h"
#include "Messagebox.h "
#include "RegePage.h"
#include "Base64Coder.h"
using namespace msgpack;
CSocketBussiness g_SockBuss;
extern CSocketBussiness g_llcsock;
extern CSocketBussiness g_dxzhsock;
extern CSocketBussiness g_sudqsock;
extern CSocketBussiness g_lltcsock;
void CLoginDlg::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) OnPrepare();
	else if (msg.sType =="link")
	{
		CRegePage* pMainDlg = new CRegePage();
		pMainDlg->Create(NULL, _T(""), UI_WNDSTYLE_DIALOG , WS_EX_CLIENTEDGE);
		pMainDlg->CenterWindow();
		pMainDlg->ShowWindow(true);
	}
	else if (msg.sType== "return")
	{

		CEditUI* pUserEdit = static_cast<CEditUI*>(m_pm.FindControl("useredit"));
		CEditUI* pPassEdit = static_cast<CEditUI*>(m_pm.FindControl("passwordedit"));
		if (msg.pSender->GetName() == "useredit")
		{
			string strUser = pUserEdit->GetText().GetData();
			if (!strUser.empty())
				pPassEdit->SetFocus();
		}
		else if (msg.pSender->GetName() == "passwordedit")
		{
			string strPass = pPassEdit->GetText().GetData();
			if (!strPass.empty())
			{
				OnLogin();
			}
		}
		
	}
	else if( msg.sType == _T("click") )
	{
		if( msg.pSender->GetName() == _T("closebtn") ) 
		{
			exit(0);
		}
		else if (msg.pSender->GetName() == _T("cancelbtn"))
		{
			exit(0);
		}
		else if (msg.pSender->GetName() == _T("loginbtn"))
		{				
			//MyMessageBox(*this, "测试Messagebox对话框！");
			//SendMessage( WM_SHOWMAINDLG_MSG,0,0);
			OnLogin();				
		}

	}
	
}

void CLoginDlg::OnLogin()
{
	/*CMainPage* pdlg = new CMainPage();
	pdlg->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_CLIENTEDGE);
	pdlg->CenterWindow();
	pdlg->ShowWindow(true);*/
	char szIP[1024];
	unsigned short nPort;
	TCHAR szFilePath[1024 ]={0};
	GetModuleFileName(NULL, szFilePath, 1024);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
	std::string  strPath = szFilePath;
	strPath += "login.ini";
	::GetPrivateProfileString("clientconfig","ServerIP","192.168.1.188",szIP,1024,strPath.c_str());
	nPort = ::GetPrivateProfileInt("clientconfig","ServerPort",6086,strPath.c_str());
	g_SockBuss.SetLoginDlg(this);

	std::string strUser, strPass;
	CControlUI* pControlUser = static_cast<CControlUI*>(m_pm.FindControl(_T("useredit")));
	if( pControlUser ) 
	{
		
		CDuiString strDuiUser = pControlUser->GetText();
		strUser = strDuiUser.GetData();
	}
	CControlUI* pControlPass = static_cast<CControlUI*>(m_pm.FindControl(_T("passwordedit")));
	if( pControlPass ) 
	{

		CDuiString strDuiPass = pControlPass->GetText();
		strPass = strDuiPass.GetData();
		if (strDuiPass.IsEmpty())
		{
			MyMessageBox(NULL, "密码不能为空！");
			return;
		}
	}
	if (strUser.empty() )
	{
		MyMessageBox(NULL,_T("用户名不能为空！"));
		return;
	}
	else
	{
		//SendMessage(WM_SHOWMAINDLG_MSG);

		g_SockBuss.SetUserInfo(strUser, strPass);
		CControlUI* pLink = static_cast<CControlUI*>(m_pm.FindControl("LabelLinking"));
		pLink->SetText("连接服务器中......");
		pLink->SetVisible();
		g_SockBuss.SetAddr(nPort,szIP);
		g_llcsock.SetAddr(nPort,szIP);
		g_dxzhsock.SetAddr(nPort,szIP);
		g_sudqsock.SetAddr(nPort,szIP);
		g_lltcsock.SetAddr(nPort,szIP);
		
		int iErr =g_SockBuss.Connect();
  		if (iErr == -1)
		{
			pLink->SetText("连接服务器失败！");
		}
		else
		{
			pLink->SetText("");
			g_SockBuss.Login();
		}

	}
}


LRESULT CLoginDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	//case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	//case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	//case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	//case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	
	//case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	//case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_TIMER:         lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	case WM_SHOWMAINDLG_MSG: lRes=OnShowMainDlg(uMsg, wParam, lParam, bHandled);break;
	case  WM_SHOWERROR_MSG: lRes = OnShowErrorMsg(uMsg, wParam, lParam, bHandled);break;
	case  WM_NCLBUTTONDBLCLK: bHandled = TRUE; break;
	
	
	
	default:
		bHandled = FALSE;
	}
    if (bHandled )
		return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	/*if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);*/

}
void CLoginDlg::Init()
{
	::SetTimer(m_hWnd,1, 1000, NULL);
}


LRESULT CLoginDlg:: OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_pm.Init(m_hWnd);
	CDialogBuilder builder;	
	CControlUI* pRoot = builder.Create(_T("登录.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	Init();
	return 0;
}
LRESULT CLoginDlg::OnReturnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	OnLogin();
	bHandled = TRUE;
	return 0;

}

LRESULT CLoginDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CLoginDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SendMessage(WM_CLOSE);

	bHandled = FALSE;
	return 0;
}
LRESULT CLoginDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_nTimeCount ++;
	if (m_nTimeCount == m_nTimeReConn)
	{
		g_SockBuss.Close(false);
		m_nTimeCount = 0;
	}
	if (m_nTimeCount == m_nTimeSendActive)
	{
		g_SockBuss.SendActive();
	}
	return 0;
}

LRESULT CLoginDlg::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CLoginDlg::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CLoginDlg::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CLoginDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	/*if( !::IsZoomed(*this) ) {
	RECT rcSizeBox = m_pm.GetSizeBox();
	if( pt.y < rcClient.top + rcSizeBox.top ) {
	if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
	if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
	return HTTOP;
	}
	else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
	if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
	if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
	return HTBOTTOM;
	}
	if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
	if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}*/
	
	RECT rcCaption = m_pm.GetCaptionRect();

	if (pt.x > rcCaption.left && pt.x < rcCaption.right && pt.y >rcCaption.top && pt.y <rcCaption.bottom)		
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
		if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CLoginDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	bHandled = TRUE;
	return 0;
}

LRESULT CLoginDlg::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CLoginDlg::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CLoginDlg::OnShowMainDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("useredit")));
	CDuiString strUser= pEdit->GetText();
    CEditUI* pPass = static_cast<CEditUI*>(m_pm.FindControl(_T("passwordedit")));
	string strPass = pPass->GetText();
	char szFilePath[1024];
	GetModuleFileName(NULL, szFilePath, 1024);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
	std::string  strPath = szFilePath;
	strPath += "login.ini";
	::WritePrivateProfileString("clientconfig","username",strUser.GetData(),strPath.c_str());
	COptionUI* pRemberCheck = static_cast<COptionUI*>(m_pm.FindControl(_T("remberPWD")));
	if (pRemberCheck->IsSelected())
	{
		CBase64Coder bc;
		bc.Encode(strPass.c_str());
		string strEncryPass=bc.EncodedMessage();
		::WritePrivateProfileString("clientconfig","pass",strEncryPass.c_str(),strPath.c_str());
		::WritePrivateProfileString("clientconfig","save","1",strPath.c_str());
	}
	else
	{
		::WritePrivateProfileString("clientconfig","save","0",strPath.c_str());
	}
	ShowWindow(false);
	if (m_pMainDlg)
	{
		m_pMainDlg->ShowWindow();
	}
	else
	{
		m_pMainDlg = new CMainPage();
		m_pMainDlg->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_CLIENTEDGE);
		m_pMainDlg->CenterWindow();
		m_pMainDlg->ShowWindow(true);
	}

	/*if (g_SockBuss.Connect()== -1)
	{
		MyMessageBox(*m_pMainDlg,"连接服务器失败");
		return 0;
	}*/
	g_SockBuss.SendKhjlQueryAll("",0);
	if (g_llcsock.Connect() != -1)
		g_llcsock.SendQueryLlcAll(0);
	if (g_dxzhsock.Connect() != -1)
		g_dxzhsock.SendGetDXList(0);
	if (g_sudqsock.Connect() != -1)
		g_sudqsock.SendSsdqQueryAll(0);
	if (g_lltcsock.Connect() != -1)
		g_lltcsock.SendLltcQueryAll(0);
	g_SockBuss.SendTotalSum();
	return 0;
}

LRESULT CLoginDlg::OnShowErrorMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
	LPCTSTR pMsg = (LPCTSTR)lParam;
	MyMessageBox(*this,"登录失败，请检查用户名和密码是否正确！");
	return 0;
}

