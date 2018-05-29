#include "StdAfx.h"
#include "KhRqTjPage.h"
#include "XsrqCardPage.h"
#include "ListEx.h"
extern string addUrlTag(string & str);
extern CXsrqCardPage* g_XsrqCardPage;
CKhRqTjPage::CKhRqTjPage(void)
{
}


CKhRqTjPage::~CKhRqTjPage(void)
{
}


void CKhRqTjPage::HandleSelected(TNotifyUI& msg)
{
	
}
void CKhRqTjPage::HandleClick(TNotifyUI& msg)
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
		if (strBtnName == "XSRQ_cardnextpage")
		{
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("XSRQ_cardcurrentpage"));
			string strpage =pCurPage->GetText();
			replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
			replace_all(strpage,"{/c}页{/p}","");
			int nPage= atoi(strpage.c_str());
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("XSRQ_cardsumtxt"));

			string strTotalPage=pTotalPage->GetText();
			replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
			replace_all(strTotalPage,"{/c}页){/p}","");

			int nToPage= atoi(strTotalPage.c_str());
			int nSendPage = nPage +1;
			if (nSendPage >= nToPage)
			{
				nSendPage = nToPage;
			}

			CEditUI* pCard=static_cast<CEditUI*>(m_pm.FindControl(_T("XSRQ_cardIDedit")));
			CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("XSRQ_cardquerylist")));
			pCardList->RemoveAll();
		
			{
				/*if (nSendPage == nToPage)
					g_SockBuss.SendGetJLXSDataCards(2,m_strjl,m_strdate,nSendPage,3,0);
				else g_SockBuss.SendGetJLXSDataCards(2,m_strjl,m_strdate,nSendPage,2,atoi(m_id.strLast.c_str()));*/
			}

		}
		if (strBtnName == "XSRQ_cardprepage")
		{
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("XSRQ_cardcurrentpage"));
			string strpage =pCurPage->GetText();
			replace_all(strpage,"{p}{y 20}第{c #F27D30}","");  
			replace_all(strpage,"{/c}页{/p}","");
			int nPage= atoi(strpage.c_str());
			
			CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("XSRQ_cardquerylist")));
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
				/*if (nPre==1)
				{
				g_SockBuss.SendGetJLXSDataCards(2,m_strjl,m_strdate,nPre);
				}
				else
					g_SockBuss.SendGetJLXSDataCards(2,m_strjl,m_strdate,nPre,1,atoi(m_id.strFirst.c_str()));*/
			}

		}
		else if (strBtnName == "XSRQ_cardlastpage")
		{
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("XSRQ_cardsumtxt"));

			string strTotalPage=pTotalPage->GetText();
			replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
			replace_all(strTotalPage,"{/c}页){/p}","");

			int nPage= atoi(strTotalPage.c_str());
			CEditUI* pCard=static_cast<CEditUI*>(m_pm.FindControl(_T("XSRQ_cardIDedit")));
			CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("XSRQ_cardquerylist")));
			pCardList->RemoveAll();
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
				//g_SockBuss.SendGetJLXSDataCards(1,m_strjl,m_strdate,nPage,3,0);

		}
		else if ( strBtnName== "XSRQ_cardfirstpage")
		{
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("XSRQ_cardcurrentpage"));
			string strpage ="{p}{y 20}第{c #F27D30}";
			strpage += "0";
			strpage +="{/c}页{/p}";
			pCurPage->SetText(strpage.c_str());
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("XSRQ_cardsumtxt"));

			string strTotalPage="{p}{y 20}(共{c #F27D30}";
			strTotalPage += "0";
			strTotalPage+="{/c}页){/p}";
			pTotalPage->SetText(strTotalPage.c_str());
			//CEditUI* pCard=static_cast<CEditUI*>(m_pm.FindControl(_T("XSRQ_cardIDedit")));
			CListUIEx* pCardList =static_cast<CListUIEx*>(m_pm.FindControl(_T("XSRQ_cardquerylist")));
			pCardList->RemoveAll();
	
				//g_SockBuss.SendGetJLXSDataCards(1,m_strjl,m_strdate,1);
		}

		/////////////////停复机//////////////////////////////////////////////////////////////////////////////////

		else if (strBtnName == "XSRQ_cardtingjibtn")
		{
			CListUIEx* pUserList =static_cast<CListUIEx*>(m_pm.FindControl(_T("XSRQ_cardquerylist")));
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}*/
			
			for (int i = 0; i<pUserList->GetCount(); i++)
			{
				CListContainerElementUI* pLine = (CListContainerElementUI*)pUserList->GetItemAt(i);
				CCheckBoxUI* pla =static_cast<CCheckBoxUI*>(pLine->FindSubControl("cardquerycheckbox"));
				string strjrhm = pla->GetText();
			
				CLabelUI* pla2=static_cast<CLabelUI*>(pLine->FindSubControl("domain2"));
				string strdx = pla2->GetText();
				if (pla->GetCheck())
				{

					int b = 0;
				
					{

						//g_SockBuss.SendTingfuji(2,strjrhm,strdx);
					}
				}
			}


			return;
		}

		else if (strBtnName == "XSRQ_cardfujibtn")
		{
			CListUIEx* pUserList =static_cast<CListUIEx*>(m_pm.FindControl(_T("XSRQ_cardquerylist")));
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
			}*/
			
			for (int i = 0; i<pUserList->GetCount(); i++)
			{
				CListContainerElementUI* pLine = (CListContainerElementUI*)pUserList->GetItemAt(i);
				CCheckBoxUI* pla =static_cast<CCheckBoxUI*>(pLine->FindSubControl("cardquerycheckbox"));
				
				string strjrhm = pla->GetText();

				CLabelUI* pla2=static_cast<CLabelUI*>(pLine->FindSubControl("domain2"));
				string strdx = pla2->GetText();
				if (pla->GetCheck())
				{
					int b = 0;			
					{
						//g_SockBuss.SendTingfuji(2,strjrhm,strdx,false);
					}
				}
			}


			return;
		}

		else if (strBtnName == "XSRQ_cardupdatstatebtn")
		{
			CListUIEx* pUserList =static_cast<CListUIEx*>(m_pm.FindControl(_T("XSRQ_cardquerylist")));

			for (int i = 0; i<pUserList->GetCount(); i++)
			{
				CListContainerElementUI* pLine = (CListContainerElementUI*)pUserList->GetItemAt(i);
				CCheckBoxUI* pla =static_cast<CCheckBoxUI*>(pLine->FindSubControl("cardquerycheckbox"));
			    string strjrhm = pla->GetText();
				CLabelUI* pla2=static_cast<CLabelUI*>(pLine->FindSubControl("domain2"));
				string strdx = pla2->GetText();
				if (pla->GetCheck())
				{
					//g_SockBuss.SendUpdateSimState(2,strjrhm,strdx);
					
				}
			}


			return;
		}
		////////////////////////////////////////////////////////////////////
	}


}

void CKhRqTjPage::HandleHeaderClick(TNotifyUI& msg)
{
	//return;//暂时屏蔽此功能
	//CListUI* pList= NULL;
	//CListUI* pUserList =static_cast<CListUI*>(m_pm.FindControl(_T("userlist")));
	//COptionUI* pOptionUser = static_cast<COptionUI*>(m_pm.FindControl(_T("userqueryopt")));
	//if (pOptionUser->IsSelected())
	//{
	//	pList = pUserList;
	//}
	//if (pList)
	//{

	//	CListHeaderUI* pHeader = pList->GetHeader();
	//	int nNum = pHeader->GetCount();
	//	for (int i = 0; i < pHeader->GetCount(); i++)
	//	{
	//		CListHeaderItemUI* pC = static_cast<CListHeaderItemUI*>(pHeader->GetItemAt(i));
	//		LPWSTR sz;
	//		CDuiString strTit = pC->GetText();
	//		if (strTit == msg.pSender->GetText())
	//		{
	//			/*g_nListItemClick = i;*/
	//			CDuiString strA = /*CPaintManagerUI::GetResourcePath()  + */"list_header_orderA.png";
	//			CDuiString strD = /*CPaintManagerUI::GetResourcePath()  + */"list_header_orderD.png";

	//			if (m_bSortAseByTitle)
	//				pC->SetBkImage(strD);
	//			else
	//				pC->SetBkImage(strA);
	//			m_bSortAseByTitle =!m_bSortAseByTitle;

	//		}
	//		else
	//		{
	//			CDuiString str = /*CPaintManagerUI::GetResourcePath()  +*/ "list_header_bg.png";

	//			pC->SetBkImage(str);
	//		}
	//	}
	//	//DoSort(pList,msg.pSender->GetText()); 
	//}
}

void CKhRqTjPage::HandleLink(TNotifyUI& msg)
{
	CListUI* pSimList=static_cast<CListUIEx*>(m_pm.FindControl(_T("KH_RQ_cardXSJUList")));
	if (msg.pSender->GetParent()->GetParent() == pSimList)
	{
		if (msg.wParam == 0)
		{
			CListTextElementUI* pEle = (CListTextElementUI*)msg.pSender;
			string strdata = pEle->GetText(0);
			g_SockBuss.SendGetKhXsrqCard(1,m_strkhid,strdata);	
			g_XsrqCardPage->InitInfo(m_strJL,strdata,en_kehu);
			g_XsrqCardPage->SetKhid(m_strkhid);
			g_XsrqCardPage->ShowWindow(SW_SHOW);
		}
	

	}
}

void CKhRqTjPage::InitInfo(string strku,string strkhid)
{
	m_strJL = strku;
	m_strkhid = strkhid;
	//m_strjl = strJL;
	//m_strdate = strdate;
	//m_enType = type;
	//
	CListUI* pList =static_cast<CListUI*>(m_pm.FindControl(_T("KH_RQ_cardXSJUList")));
	pList->SetItemFixedHeight(30);
	pList->RemoveAll();
	CLabelUI* pKehumin =static_cast<CLabelUI*>(m_pm.FindControl(_T("KH_RQ_khm_label")));	
	string strKehuShow ="客户名:";

	strKehuShow+= "{c #F27D30}";
	strKehuShow +=strku;
	strKehuShow+="{\c}";
	pKehumin->SetText(strKehuShow.c_str());
	//CLabelUI* pLX =static_cast<CLabelUI*>(m_pm.FindControl(_T("XSRQ_lx_label")));	
	//string strlxShow ="销售时间:";
	//strlxShow += "{c #F27D30}";
	//strlxShow+= strdate;
	//strlxShow +="{\c}";
	//pLX->SetText(strlxShow.c_str());
	//CListUIEx* pCardList =static_cast<CListUIEx*>(m_pm.FindControl(_T("XSRQ_cardTFJlist")));
	////pCardList->SetItemFixedHeight(30);
	//pCardList->RemoveAll();
	//
}


LRESULT CKhRqTjPage::OnKhRqtjmsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("KH_RQ_cardcurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		char sz[16];
		strpage += itoa(nTag,sz,10);
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("KH_RQ_cardsumtxt"));

		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		int nPage = nCount/PAGE_COUNT;
		if (nCount%PAGE_COUNT != 0)
			nPage+=1;	
		strTotalPage += itoa(nPage,sz,10);
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());		
		CListUI* pLst1= static_cast<CListUI*>(m_pm.FindControl(_T("KH_RQ_cardXSJUList")));
		pLst1->RemoveAll();
		for (int t = 0; t< i; t++)
		{		
			object* pItem = (parray++)->via.array.ptr;
	/*		string  id = pItem->as<string >();
			if (t== 0 && nReq != 0)
			{
				m_id.strFirst = id;
			}
			if (t== i-1&& nReq != 0)
			{
				m_id.strLast = id;
			}
			pItem ++;*/
			string  strkhxm = pItem->as<string >();
			pItem++;
			string strsl = pItem->as<string>();
		

			CListTextElementUI* pEle= new CListTextElementUI;
			char sz[16];
			if (nReq== 1)
				pLst1->Add(pEle);
			pEle->SetText(0,strkhxm.c_str());
			addUrlTag(strsl);
			pEle->SetText(1,strsl.c_str());
			
		}

	}
	return 0;
}