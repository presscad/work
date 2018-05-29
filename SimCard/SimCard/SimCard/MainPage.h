#pragma once

#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "UIUserManager.h"
#include "HTHandler.h"
#include "QueryHandler.h"
#include "UserHandler.h"
#include "OrderHandler.h"
#include "OperateHandler.h"
#include "YonghuCardPage.h"
#include "JingliKehuPage.h"
#include "XsrqCardPage.h"
#include "KhRqTjPage.h"
enum
{
	TIMER_XINCARD=10000,
	TIMMER_CHUCARD,
	TIMER_XUFEI,
	TIMER_TUICARD,
	TIMER_XIAOCARD,
	TIMER_KEHU,
};
enum E_FANYE
{
	en_first,
	en_next,
	en_last,
	en_pre
};

//#include "ControlEx.h"
class CMainPage;
extern CMainPage * g_MainPage;
extern CYonghuCardPage * g_YonghuCardPage;
extern CXsrqCardPage *g_XsrqCardPage;
extern CKhRqTjPage* g_pKhrqTjPage;
extern string g_strUserType;
class CMainPage : public CWindowWnd, public INotifyUI
{
public:
	CMainPage() ;
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	
	void SetVertion()
	{
		CTextUI* pVer = static_cast<CTextUI*>(m_pm.FindControl(_T("versiontext")));
		char szIP[1024];
		unsigned short nPort;
		TCHAR szFilePath[1024 ]={0};
		GetModuleFileName(NULL, szFilePath, 1024);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
		std::string  strPath = szFilePath;
		strPath += "app.ini";
		::GetPrivateProfileString("更新配置","version","201804181",szIP,1024,strPath.c_str());
		const char* szShow = "{c #386382}主程序版本：%s{/c}";
		char szmess[1024];
		sprintf_s(szmess,1024 ,szShow,szIP);
		pVer->SetText(szmess);
	}
	void SetAllListItemHeight(int h)
	{
		CListUI* plst;
		plst=static_cast<CListUI*>(m_pm.FindControl(_T("CX_kehucardquerylist")));
		plst->SetItemFixedHeight(h);
		plst=static_cast<CListUI*>(m_pm.FindControl(_T("CX_kehucardqueryxfrqlist")));
		plst->SetItemFixedHeight(h);
		plst=static_cast<CListUI*>(m_pm.FindControl(_T("CX_kehucardqueryxsjllist")));
		plst->SetItemFixedHeight(h);
		plst=static_cast<CListUI*>(m_pm.FindControl(_T("CX_kehulist")));
		plst->SetItemFixedHeight(h);
		plst=static_cast<CListUI*>(m_pm.FindControl(_T("CX_jinglilist")));
		plst->SetItemFixedHeight(h);
		plst=static_cast<CListUI*>(m_pm.FindControl(_T("CX_liuliangchiist")));
		plst->SetItemFixedHeight(h);
		plst=static_cast<CListUI*>(m_pm.FindControl(_T("CX_dxlist")));
		plst->SetItemFixedHeight(h);
		plst=static_cast<CListUI*>(m_pm.FindControl(_T("CX_tclist")));
		plst->SetItemFixedHeight(h);
		plst=static_cast<CListUI*>(m_pm.FindControl(_T("CX_queryuserlist")));
		plst->SetItemFixedHeight(h);
	
	}
	void SetUserType()
	{
		CComboUI* cbType=static_cast<CComboUI*>(m_pm.FindControl(_T("YH_createuserTypeCom")));
		CListLabelElementUI* pla = new CListLabelElementUI;
		pla->SetText("超级用户");
		cbType->Add(pla);
		CListLabelElementUI* pla1 = new CListLabelElementUI;
		pla1->SetText("代理商");
		cbType->Add(pla1);
		
	}
	void ShowhideTFj()
	{
			CButtonUI* pTingji = static_cast<CButtonUI*>(m_pm.FindControl(_T("CX_cardfujibtn")));
			CButtonUI* pFuji = static_cast<CButtonUI*>(m_pm.FindControl(_T("CX_cardfujibtn")));
			if (g_strUserType == "2")
			{
				pFuji->SetVisible(false);
				pFuji->Invalidate();
				if (g_strAuthority=="0")
				{
					pTingji->SetVisible(false);
					pTingji->Invalidate();
				}
			}
	}

	void Init() {
		m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
		m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
		m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
		m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
		m_pUserQueryBtn =static_cast<CButtonUI*>(m_pm.FindControl(_T("userquerybtn")));
		m_pSimQueryBtn=static_cast<CButtonUI*>(m_pm.FindControl(_T("cardquerybtn")));
		CComboUI* cbLLcLx=static_cast<CComboUI*>(m_pm.FindControl(_T("HT_addLLC_lxComb")));
		CListLabelElementUI* pLlx1 = new CListLabelElementUI;
		pLlx1->SetText("前向");
		cbLLcLx->Add(pLlx1);
		CListLabelElementUI* pLlx2 = new CListLabelElementUI;
		pLlx2->SetText("后向");
		cbLLcLx->Add(pLlx2);
		CComboUI* cbLLczt=static_cast<CComboUI*>(m_pm.FindControl(_T("HT_addcard_zt_com")));
		CListLabelElementUI* pLzt1 = new CListLabelElementUI;
		pLzt1->SetText("在用");
		cbLLczt->Add(pLzt1);
		CListLabelElementUI* pLzt2 = new CListLabelElementUI;
		pLzt2->SetText("用户报停");
		cbLLczt->Add(pLzt2);
		CListLabelElementUI* pLzt3 = new CListLabelElementUI;
		pLzt3->SetText("用户拆机");
		cbLLczt->Add(pLzt3);
		CListLabelElementUI* pLzt4 = new CListLabelElementUI;
		pLzt4->SetText("欠停(双向)");
		cbLLczt->Add(pLzt4);
		CListLabelElementUI* pLzt5 = new CListLabelElementUI;
		pLzt5->SetText("欠停(单向)");
		cbLLczt->Add(pLzt5);
		CListLabelElementUI* pLzt6 = new CListLabelElementUI;
		pLzt6->SetText("违章停机");
		cbLLczt->Add(pLzt6);
		CListLabelElementUI* pLzt7 = new CListLabelElementUI;
		pLzt7->SetText("挂机");
		cbLLczt->Add(pLzt7);
		CListLabelElementUI* pLzt8 = new CListLabelElementUI;
		pLzt8->SetText("活卡待激活");
		cbLLczt->Add(pLzt8);
		CListLabelElementUI* pLzt9 = new CListLabelElementUI;
		pLzt9->SetText("停机");
		cbLLczt->Add(pLzt9);
		CListLabelElementUI* pLzt10 = new CListLabelElementUI;
		pLzt10->SetText("注销");
		cbLLczt->Add(pLzt10);
		CListLabelElementUI* pLzt11 = new CListLabelElementUI;
		pLzt11->SetText("N/A");
		cbLLczt->Add(pLzt11);
		SetAllListItemHeight(30);
		SetVertion();
		SetUserType();
		m_pYHCardPage = new CYonghuCardPage;
		m_pYHCardPage->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_CLIENTEDGE);
		m_pYHCardPage->CenterWindow();
		m_pYHCardPage->ShowWindow(SW_HIDE);
		m_pJlkhPage = new CJingliKehuPage;
		m_pJlkhPage->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_CLIENTEDGE);
		m_pJlkhPage->CenterWindow();
		m_pJlkhPage->ShowWindow(SW_HIDE);
		g_YonghuCardPage = m_pYHCardPage;
		m_pXsrqCardPage = new CXsrqCardPage;
		m_pXsrqCardPage->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_CLIENTEDGE);
		m_pXsrqCardPage->CenterWindow();
		m_pXsrqCardPage->ShowWindow(SW_HIDE);
		g_XsrqCardPage = m_pXsrqCardPage;
		m_pKhrqTjPage = new CKhRqTjPage;
		m_pKhrqTjPage->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_CLIENTEDGE);
		m_pKhrqTjPage->CenterWindow();
		m_pKhrqTjPage->ShowWindow(SW_HIDE);
		g_pKhrqTjPage = m_pKhrqTjPage;
		ShowhideTFj();
		
	}

	void OnPrepare() {
		/*CDialogBuilder builder;
		CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("UsrsManager.xml"),(UINT)0));*/
	}

	void Notify(TNotifyUI& msg)
	{
		if( msg.sType == _T("windowinit") ) OnPrepare();
		else if( msg.sType == _T("click") ) {
			return HandleClick(msg);			
		}
		else if (msg.sType == _T("link"))
		{
			return HandleLink(msg);
		}
		
		else if(msg.sType==_T("selectchanged"))
		{
			return HandleSelected(msg);			
			
		}
		else if(msg.sType == DUI_MSGTYPE_HEADERCLICK || msg.sType == DUI_MSGTYPE_ITEMDBCLICK)
		{
			return HandleHeaderClick(msg);
		}
	}

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == TIMER_XINCARD)		
		{		
			KillTimer(*this,wParam);
			 m_clsHTHandler.HandleKaiCarExport(m_pm);

		    
			CGifAnimUI* pGif =static_cast<CGifAnimUI*>(m_pm.FindControl("HT_showing1"));
			pGif->SetVisible(false);
		}
		if (wParam == TIMMER_CHUCARD)		
		{	
			KillTimer(*this,wParam);
			m_clsHTHandler.HandleChuCarExport(m_pm);
			
			CGifAnimUI* pGif =static_cast<CGifAnimUI*>(m_pm.FindControl("HT_showing2"));
			pGif->SetVisible(false);
		}
		if (wParam == TIMER_XUFEI)		
		{		
			KillTimer(*this,wParam);
			m_clsHTHandler.HandleXufeiExport(m_pm);		
			CGifAnimUI* pGif =static_cast<CGifAnimUI*>(m_pm.FindControl("HT_showing3"));
			pGif->SetVisible(false);
		}
		if (wParam == TIMER_TUICARD)		
		{	
			KillTimer(*this,wParam);
			m_clsHTHandler.HandleTuiCarExport(m_pm);
			
			CGifAnimUI* pGif =static_cast<CGifAnimUI*>(m_pm.FindControl("HT_showing4"));
			pGif->SetVisible(false);
		}
		if (wParam == TIMER_XIAOCARD)		
		{		
			KillTimer(*this,wParam);
			m_clsHTHandler.HandleXiaoCarExport(m_pm);
			
			CGifAnimUI* pGif =static_cast<CGifAnimUI*>(m_pm.FindControl("HT_showing5"));
			pGif->SetVisible(false);
		}
		if (wParam == TIMER_KEHU)		
		{		
			KillTimer(*this,wParam);
			m_clsHTHandler.HandleKehuExport(m_pm);

			CGifAnimUI* pGif =static_cast<CGifAnimUI*>(m_pm.FindControl("HT_showing6"));
			pGif->SetVisible(false);
		}
		return 0;
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CDialogBuilderCallbackEx cb;
		CControlUI* pRoot = builder.Create(_T("首页.xml"), (UINT)0,  &cb, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		Init();
		g_MainPage = this;
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
		case WM_TIMER :         lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case  WM_GET_KHJL_MSG: lRes= OnKHJLMsg(uMsg,wParam,lParam,bHandled);break;
	    case  WM_GET_KH_MSG: lRes= OnKHMsg(uMsg,wParam,lParam,bHandled);break;
		case  WM_GET_DX_MSG: lRes= OnDXMsg(uMsg,wParam,lParam,bHandled);break;
		case  WM_GET_LLC_MSG: lRes= OnLLCMsg(uMsg,wParam,lParam,bHandled);break;
		case  WM_GET_SSDQ_MSG:lRes= OnSSDQMsg(uMsg,wParam,lParam,bHandled);break;
		case  WM_GET_LLTC_MSG:lRes= OnLLTCMsg(uMsg,wParam,lParam,bHandled);break;
		case  WM_GET_CARD_MSG:lRes= OnCardMsg(uMsg,wParam,lParam,bHandled);break;
		case  WM_TINGFUJI_MSG:lRes= OnTFJMsg(uMsg,wParam,lParam,bHandled);break;
		case  WM_UPDATECARDSTATE_MSG:lRes= OnUpdateStateMsg(uMsg,wParam,lParam,bHandled);break;
		case  WM_USER_MSG:lRes= OnUserMsg(uMsg,wParam,lParam,bHandled);break;
		default:
			bHandled = FALSE;
		}
		if( bHandled ) return lRes;
		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

public:
	CPaintManagerUI m_pm;
private:
	void HandleClick(TNotifyUI& msg);
	void HandleSelected(TNotifyUI& msg);
	void HandleHeaderClick(TNotifyUI& msg);
	void HandleLink(TNotifyUI& msg);
	LRESULT OnKHJLMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKHMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDXMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLLCMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSSDQMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLLTCMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCardMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTFJMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUpdateStateMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnImportCardMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pSimQueryBtn;
	CButtonUI* m_pUserQueryBtn;
	bool m_bSortAseByTitle;
	CHTHandler m_clsHTHandler;
	CQueryHandler m_clsQueryHandler;
	CUserHandler m_clsUserHandler;
	COrderHandler m_clsOrderHandler;
	COperateHandler m_clsOperteHandler;
	CYonghuCardPage* m_pYHCardPage;
	CJingliKehuPage* m_pJlkhPage;
	CXsrqCardPage* m_pXsrqCardPage;
	CKhRqTjPage* m_pKhrqTjPage;
public:
	ST_ID m_stcardID;
	ST_ID m_stKehuCardID;
	ST_ID m_stDianXin;
	ST_ID m_stLLC;
	ST_ID m_stKHCARD;
	ST_ID m_stJingli;
	ST_ID m_stLLTC;
	


	//...
};


