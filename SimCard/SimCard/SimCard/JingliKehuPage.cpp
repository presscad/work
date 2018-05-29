#include "StdAfx.h"
#include "JingliKehuPage.h"
#include "XsrqCardPage.h"
extern CXsrqCardPage* g_XsrqCardPage;
extern string addUrlTag(string & str);
CJingliKehuPage::CJingliKehuPage(void)
{
}


CJingliKehuPage::~CJingliKehuPage(void)
{
}

void CJingliKehuPage::HandleClick(TNotifyUI& msg)
{
	if( msg.pSender == m_pCloseBtn ) {
		ShowWindow(SW_HIDE);
		return; 
	}
	else if( msg.pSender == m_pMinBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
	else if( msg.pSender == m_pMaxBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
	else if( msg.pSender == m_pRestoreBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
	else
	{
		string strBtnName= msg.pSender->GetName();
		//卡查询界面开始////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (strBtnName == "JL_KH_cardnextpage")
		{
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("JL_KH_cardcurrentpage"));
			string strpage =pCurPage->GetText();
			replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
			replace_all(strpage,"{/c}页{/p}","");
			int nPage= atoi(strpage.c_str());
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("JL_KH_cardsumtxt"));

			string strTotalPage=pTotalPage->GetText();
			replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
			replace_all(strTotalPage,"{/c}页){/p}","");

			int nToPage= atoi(strTotalPage.c_str());
			int nSendPage = nPage +1;
			if (nSendPage >= nToPage)
			{
				nSendPage = nToPage;
			}

			CEditUI* pCard=static_cast<CEditUI*>(m_pm.FindControl(_T("JL_KH_cardIDedit")));
			CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("JL_KH_cardquerylist")));
			pCardList->RemoveAll();
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
			{
				if (nSendPage == nToPage)
					g_SockBuss.SendGetJlKh(1,m_strJL,nSendPage,3,0);
				else g_SockBuss.SendGetJlKh(1,m_strJL,nSendPage,2,atoi(m_id.strLast.c_str()));
			}

		}
		if (strBtnName == "JL_KH_cardprepage")
		{
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("JL_KH_cardcurrentpage"));
			string strpage =pCurPage->GetText();
			replace_all(strpage,"{p}{y 20}第{c #F27D30}","");  
			replace_all(strpage,"{/c}页{/p}","");
			int nPage= atoi(strpage.c_str());

			CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("JL_KH_cardquerylist")));
			pCardList->RemoveAll();

			int nPre = nPage-1;
			if (nPage -1 <= 0)
			{
				nPre = 1;
			}
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
			{
				if (nPre==1)
				{
					g_SockBuss.SendGetJlKh(1,m_strJL,nPre);
				}
				else
					g_SockBuss.SendGetJlKh(1,m_strJL,nPre,1,atoi(m_id.strFirst.c_str()));
			}

		}
		else if (strBtnName == "JL_KH_cardlastpage")
		{
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("JL_KH_cardsumtxt"));

			string strTotalPage=pTotalPage->GetText();
			replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
			replace_all(strTotalPage,"{/c}页){/p}","");

			int nPage= atoi(strTotalPage.c_str());
			CEditUI* pCard=static_cast<CEditUI*>(m_pm.FindControl(_T("JL_KH_cardIDedit")));
			CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("JL_KH_cardquerylist")));
			pCardList->RemoveAll();
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
				g_SockBuss.SendGetJlKh(1,m_strJL,nPage,3,0);

		}
		else if ( strBtnName== "JL_KH_cardfirstpage")
		{
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("JL_KH_cardcurrentpage"));
			string strpage ="{p}{y 20}第{c #F27D30}";
			strpage += "0";
			strpage +="{/c}页{/p}";
			pCurPage->SetText(strpage.c_str());
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("JL_KH_cardsumtxt"));

			string strTotalPage="{p}{y 20}(共{c #F27D30}";
			strTotalPage += "0";
			strTotalPage+="{/c}页){/p}";
			pTotalPage->SetText(strTotalPage.c_str());
			//CEditUI* pCard=static_cast<CEditUI*>(m_pm.FindControl(_T("JL_KH_cardIDedit")));
			CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("JL_KH_cardquerylist")));
			pCardList->RemoveAll();
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
				g_SockBuss.SendGetJlKh(1,m_strJL,1);
		}		
  }
}
void CJingliKehuPage::HandleSelected(TNotifyUI& msg)
{
	
}
void CJingliKehuPage::HandleHeaderClick(TNotifyUI& msg)
{

}
void CJingliKehuPage::HandleLink(TNotifyUI& msg)
{
	
	CListUI* pXSList=static_cast<CListUI*>(m_pm.FindControl(_T("JL_KH_cardXSJUList")));
	if (msg.pSender->GetParent()->GetParent() == pXSList)
	{
		string strdate;
		CListTextElementUI* pEle = (CListTextElementUI*)msg.pSender;
		strdate = pEle->GetText(0);
		g_SockBuss.SendGetJLXSDataCards(1,m_strJL,strdate);
		g_XsrqCardPage->InitInfo(m_strJL, strdate);
		g_XsrqCardPage->ShowWindow(SW_SHOW);
	
	}

}
LRESULT  CJingliKehuPage::OnJLKHmsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj =(object*)lParam;
	pObj++;
	int nReq = pObj->as<int>();
	pObj++;
	int nTag = pObj->as<int>();
	pObj++;
	int nResult = pObj->as<int>();
	if (nResult != 0)
	{
		
		return 0;
	}
	else
	{
		pObj++;
		int nCount = pObj->as<int>();
		pObj++;
		object* parray = pObj->via.array.ptr;
		int i = pObj->via.array.size;
		CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("JL_KH_cardcurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		char sz[16];
		strpage += itoa(nTag,sz,10);
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("JL_KH_cardsumtxt"));

		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		int nPage = nCount/PAGE_COUNT;
		if (nCount%PAGE_COUNT != 0)
			nPage+=1;	
		strTotalPage += itoa(nPage,sz,10);
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());		
		CListUI* pLst1= static_cast<CListUI*>(m_pm.FindControl(_T("JL_KH_cardquerylist")));
		pLst1->RemoveAll();
		for (int t = 0; t< i; t++)
		{		
			object* pItem = (parray++)->via.array.ptr;
			string  id = pItem->as<string >();
			if (t== 0 && nReq != 0)
			{
				m_id.strFirst = id;
			}
			if (t== i-1&& nReq != 0)
			{
				m_id.strLast = id;
			}
			pItem ++;
			string  strkhxm = pItem->as<string >();
			pItem++;
			string strlxfs = pItem->as<string>();
			pItem++;
			string strssdq = pItem->as<string>();
			pItem++;
			string strjingli = pItem->as<string>();
			pItem++;
			string strdj = pItem->as<string>();
			pItem++;
			
			CListTextElementUI* pEle= new CListTextElementUI;
			char sz[16];
			if (nReq== 1)
				pLst1->Add(pEle);
			pEle->SetText(0,strkhxm.c_str());
			pEle->SetText(1,strlxfs.c_str());
			pEle->SetText(2,strssdq.c_str());
			pEle->SetText(3,strjingli.c_str());
			pEle->SetText(4,strdj.c_str());			

		}

	}
	return 0;

}
LRESULT  CJingliKehuPage::OnJLQuerymsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj =(object*)lParam;
	pObj++;
	int nReq = pObj->as<int>();
	pObj++;
	int nTag = pObj->as<int>();
	pObj++;
	int nResult = pObj->as<int>();
	if (nResult != 0)
	{
		return 0;
	}
	else
	{
		pObj++;
		int nCount = pObj->as<int>();
		pObj++;
		object* parray = pObj->via.array.ptr;
		int i = pObj->via.array.size;
		CListUI* pLst1= static_cast<CListUI*>(m_pm.FindControl(_T("JL_KH_cardXSJUList")));
		pLst1->RemoveAll();
		for (int t = 0; t< i; t++)
		{		
			object* pItem = (parray++)->via.array.ptr;		
			string  strXSRQ = pItem->as<string >();
			pItem++;
			string strSL = pItem->as<string>();			
			CListTextElementUI* pEle= new CListTextElementUI;			
			if (nReq== 1)
				pLst1->Add(pEle);
			pEle->SetText(0,strXSRQ.c_str());
			addUrlTag(strSL);
			pEle->SetText(1,strSL.c_str());

		}

	}
	return 0;
}
LRESULT  CJingliKehuPage::OnUpdatestatemsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
void CJingliKehuPage::InitInfo(string strJL )
{
	m_strJL= strJL;
	CLabelUI* pJingli = static_cast<CLabelUI*>(m_pm.FindControl(_T("JL_KH_khm_label")));
	string strJingli = "经理名：{c #F27D30}";
	strJingli += m_strJL;
	strJingli +="{/c}";
	pJingli->SetText(strJingli.c_str());
	CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("JL_KH_cardquerylist")));
	pCardList->RemoveAll();
	CListUI* pLst1= static_cast<CListUI*>(m_pm.FindControl(_T("JL_KH_cardXSJUList")));
	pLst1->RemoveAll();
}