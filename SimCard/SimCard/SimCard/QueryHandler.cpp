#include "StdAfx.h"
#include "QueryHandler.h"
#include "ListEx.h"
#include "MainPage.h"
extern CMainPage* g_MainPage;
CQueryHandler::CQueryHandler(void)
{
}


CQueryHandler::~CQueryHandler(void)
{
}

void CQueryHandler::HandleClick(TNotifyUI& msg, CPaintManagerUI& pm)
{
  	string strBtnName= msg.pSender->GetName();
	//卡查询界面开始////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (strBtnName == "CX_cardnextpage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_cardcurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_cardsumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		int nSendPage = nPage +1;
		if (nSendPage >= nToPage)
		{
			nSendPage = nToPage;
		}

		CEditUI* pCard=static_cast<CEditUI*>(pm.FindControl(_T("CX_cardIDedit")));
		CListUIEx* pCardList =static_cast<CListUIEx*>(pm.FindControl(_T("CX_cardquerylist")));
		pCardList->RemoveAll();
		string strID = pCard->GetText();
		if (strID.empty())
		{
			if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return ;
			}
		   else
		    g_SockBuss.SendQueryAllCard(1,nSendPage);
		}
		else
		{
			if (!AllisNum(strID))
			{
				/*if (g_SockBuss.Connect() == -1)
				{
					MyMessageBox(NULL,"连接服务器失败");
					return ;
				}
				else*/
					g_SockBuss.SendQueryCardByKehu( strID,nSendPage);
			}
			else
			{
				/*if (g_SockBuss.Connect() == -1)
				{
				MyMessageBox(NULL,"连接服务器失败");
				return ;
				}
				else
				g_SockBuss.SendQueryCardByJRHM( strID,nSendPage)*/
			}
		}
		
	}
	if (strBtnName == "CX_cardprepage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_cardcurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");  
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		CEditUI* pCard=static_cast<CEditUI*>(pm.FindControl(_T("CX_cardIDedit")));
		CListUIEx* pCardList =static_cast<CListUIEx*>(pm.FindControl(_T("CX_cardquerylist")));
		pCardList->RemoveAll();
		string strID = pCard->GetText();
		int nPre = nPage-1;
		if (nPage -1 <= 0)
		{
			nPre = 1;
		}
		if (strID.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return ;
			}
			else*/
				g_SockBuss.SendQueryAllCard(1,nPre);
		}
		else
		{
			if (!AllisNum(strID))
			{/*
				if (g_SockBuss.Connect() == -1)
				{
					MyMessageBox(NULL,"连接服务器失败");
					return ;
				}
				else*/
					g_SockBuss.SendQueryCardByKehu( strID,nPre);
			}
		}

	}
	else if (strBtnName == "CX_cardlastpage")
	{
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_cardsumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");
		
		int nPage= atoi(strTotalPage.c_str());
		CEditUI* pCard=static_cast<CEditUI*>(pm.FindControl(_T("CX_cardIDedit")));
		CListUIEx* pCardList =static_cast<CListUIEx*>(pm.FindControl(_T("CX_cardquerylist")));
		pCardList->RemoveAll();
		string strID = pCard->GetText();
		if (strID.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return ;
			}
			else*/
				g_SockBuss.SendQueryAllCard(1,nPage);
		}
		else
		{
			if (!AllisNum(strID))
			{
				/*if (g_SockBuss.Connect() == -1)
				{
					MyMessageBox(NULL,"连接服务器失败");
					return ;
				}
				else*/
					g_SockBuss.SendQueryCardByKehu( strID,nPage);
			}
		}

	}
	else if (strBtnName == "CX_cardquerybtn" || strBtnName== "CX_cardfirstpage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_cardcurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += "0";
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_cardsumtxt"));

		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		strTotalPage += "0";
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());
		CEditUI* pCard=static_cast<CEditUI*>(pm.FindControl(_T("CX_cardIDedit")));
		CListUIEx* pCardList =static_cast<CListUIEx*>(pm.FindControl(_T("CX_cardquerylist")));
		pCardList->RemoveAll();
		string strID = pCard->GetText();
		CListUI* ptfjList =static_cast<CListUI*>(pm.FindControl(_T("CX_cardTFJlist")));
		ptfjList->RemoveAll();
		if (strID.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return ;
			}
			else*/
				g_SockBuss.SendQueryAllCard(1);
		}
		else
		{
			if (AllisNum(strID))
			{
				/*if (g_SockBuss.Connect() == -1)
				{
					MyMessageBox(NULL,"连接服务器失败");
					return ;
				}
				else*/
					g_SockBuss.SendQueryCardbyJrhm(strID);
			}
			else
			{/*
				if (g_SockBuss.Connect() == -1)
				{
					MyMessageBox(NULL,"连接服务器失败");
					return ;
				}
				else*/
					g_SockBuss.SendQueryCardByKehu(strID);
			}
		}
	}
//卡查询界面结束////////////////////////////////////////////////////////////////////////////////////////////////////////////


//客户卡查询界面开始//////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (strBtnName == "CX_kehucardquerybtn"|| strBtnName== "CX_kehucardfirstpage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_kehucardcurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += "0";
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_kehucardsumtxt"));

		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		strTotalPage += "0";
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());
		CListUI* pkehuSimList=static_cast<CListUIEx*>(pm.FindControl(_T("CX_kehucardquerylist")));
		pkehuSimList->RemoveAll();
		//pkehuSimList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_kehucardIDedit")));
		string strKehu = pkehuEdit->GetText();
		//if (!strKehu.empty())
		{
			if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else
			{
				g_SockBuss.SendKhQueryAll(strKehu,0,1,0,0);
			}
		}
		

	}

	else if (strBtnName == "CX_kehucardprepage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_kehucardcurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		nPage -=1;
		if (nPage<=0)
		{
			nPage = 1;
		}
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_kehucardquerylist")));
		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_kehucardIDedit")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				if (nPage == 1)
			       g_SockBuss.SendKhQueryAll(strKehu,1,1,0);
			    else
				   g_SockBuss.SendKhQueryAll(strKehu,1,nPage,1,atoi(g_MainPage->m_stKHCARD.strFirst.c_str()));
			}
		}
	}

	else if (strBtnName == "CX_kehucardnextpage")
	{

		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_kehucardcurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_kehucardsumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		int nSendPage = nPage +1;
		if (nSendPage >= nToPage)
		{
			nSendPage = nToPage;
		}
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_kehucardquerylist")));

		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_kehucardIDedit")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				if (nSendPage == nToPage)
				{
					g_SockBuss.SendKhQueryAll(strKehu,1,nSendPage,3,0);
				}
				else
					g_SockBuss.SendKhQueryAll(strKehu,1,nSendPage,2,atoi(g_MainPage->m_stKHCARD.strLast.c_str()));
			}
		}
	}
	else if (strBtnName == "CX_kehucardlastpage")
	{
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_kehucardsumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_kehucardquerylist")));
		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_kehucardIDedit")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendKhQueryAll(strKehu,1,nToPage,3,0);
			}
		}
	}
//客户卡查询界面结束//////////////////////////////////////////////////////////////////////////////////////////////////////////

//客户查询界面开始//////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (strBtnName == "CX_kehuBtn" || strBtnName == "CX_kehufirstpageBtn")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_kehucurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += "0";
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_kehusumTxt"));

		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		strTotalPage += "0";
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());
		CListUI* pkehuSimList=static_cast<CListUIEx*>(pm.FindControl(_T("CX_kehulist")));
		pkehuSimList->RemoveAll();
		//pkehuSimList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_kehunameEdt")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendKhQueryAll(strKehu,1);
			}
		}
	}
	else if (strBtnName == "CX_kehunextpageBtn")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_kehucurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_kehusumTxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		int nSendPage = nPage +1;
		if (nSendPage >= nToPage)
		{
			nSendPage = nToPage;
		}

		CListUI* pkehuSimList=static_cast<CListUIEx*>(pm.FindControl(_T("CX_kehulist")));
		pkehuSimList->RemoveAll();
		//pkehuSimList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_kehunameEdt")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendKhQueryAll(strKehu,1,nSendPage);
			}
		}
	}

	else if (strBtnName == "CX_kehuprepageBtn")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_kehucurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_kehusumTxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		
		int nPre = nPage-1;
		if (nPage -1 <= 0)
		{
			nPre = 1;
		}

		CListUI* pkehuSimList=static_cast<CListUIEx*>(pm.FindControl(_T("CX_kehulist")));
		pkehuSimList->RemoveAll();
		//pkehuSimList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_kehunameEdt")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendKhQueryAll(strKehu,1,nPre);
			}
		}
	}

	else if (strBtnName == "CX_kehulastpageBtn")
	{
	
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_kehusumTxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		

		CListUI* pkehuSimList=static_cast<CListUIEx*>(pm.FindControl(_T("CX_kehulist")));
		pkehuSimList->RemoveAll();
		//pkehuSimList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_kehunameEdt")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendKhQueryAll(strKehu,1,nToPage);
			}
		}
	}

//客户查询界面结束//////////////////////////////////////////////////////////////////////////////////////////////////////////

//客户经理查询界面开始//////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (strBtnName == "CX_jinglibtn" || strBtnName == "CX_jinglifirstpage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_jinglicurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += "0";
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_jinglisumtxt"));

		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		strTotalPage += "0";
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());
		CListUI* pkehuSimList=static_cast<CListUI*>(pm.FindControl(_T("CX_jinglilist")));
		pkehuSimList->RemoveAll();
		//pkehuSimList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_jingliedit")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendKhjlQueryAll(strKehu,1,1,0,0);
			}
		}
	}

	else if (strBtnName == "CX_jingliprepage" )
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_jinglicurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());;
		
				
		nPage -=1;
		if (nPage<=0)
		{
			nPage = 1;
		}
		CListUI* pkehuSimList=static_cast<CListUIEx*>(pm.FindControl(_T("CX_jinglilist")));
		pkehuSimList->RemoveAll();
		//pkehuSimList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_jingliedit")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
			MyMessageBox(NULL,"连接服务器失败");
			return;
			}
			else*/
			{
				if (nPage == 1)
					g_SockBuss.SendKhjlQueryAll(strKehu,1,1,0);
				else
					g_SockBuss.SendKhjlQueryAll(strKehu,1,nPage,1,atoi(g_MainPage->m_stJingli.strFirst.c_str()));
				
			}
		}
	}
	else if (strBtnName == "CX_jinglinextpage" )
	{
		

		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_jinglicurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_jinglisumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		int nSendPage = nPage +1;
		if (nSendPage >= nToPage)
		{
			nSendPage = nToPage;
		}
		CListUI* pkehuSimList=static_cast<CListUI*>(pm.FindControl(_T("CX_jinglilist")));
		pkehuSimList->RemoveAll();
		//pkehuSimList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_jingliedit")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				if (nSendPage == nToPage)
				{
					g_SockBuss.SendKhjlQueryAll(strKehu,1,nSendPage,3,0);
				}
				else
					g_SockBuss.SendKhjlQueryAll(strKehu,1,nSendPage,2,atoi(g_MainPage->m_stKHCARD.strLast.c_str()));
			}
		}
	}
	else if (strBtnName == "CX_jinglilastpage" )
	{

		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_jinglisumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
	
		CListUI* pkehuSimList=static_cast<CListUI*>(pm.FindControl(_T("CX_jinglilist")));
		pkehuSimList->RemoveAll();
		//pkehuSimList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_jingliedit")));
		string strKehu = pkehuEdit->GetText();
		//if (strKehu.empty())
		{
			if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else
			{
				g_SockBuss.SendKhjlQueryAll(strKehu,1,nToPage,3,0);
			}
		}
	}
//客户经理查询界面结束//////////////////////////////////////////////////////////////////////////////////////////////////////////

//流量池查询界面结束//////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (strBtnName == "CX_liuliangchibtn"||strBtnName == "CX_liuliangchifirstpage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_liuliangchicurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += "1";
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_liuliangchisumtxt"));

		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		strTotalPage += "1";
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_liuliangchiist")));
		pList->RemoveAll();
		//pList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_liuliangchiedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendQueryLlcAll(1,1,0,0);
			}
		}
	}

	else if (strBtnName == "CX_liuliangchiprepage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_liuliangchicurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		nPage -=1;
		if (nPage<=0)
		{
			nPage = 1;
		}
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_liuliangchiist")));
		pList->RemoveAll();
		//pList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_liuliangchiedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{/*
			if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				if (nPage == 1)
					g_SockBuss.SendQueryLlcAll(1,1,0);
				else
					g_SockBuss.SendQueryLlcAll(1,nPage,1,atoi(g_MainPage->m_stLLC.strFirst.c_str()));
			}
		}
	}

	else if (strBtnName == "CX_liuliangchinextpage")
	{

		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_liuliangchicurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_liuliangchisumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		int nSendPage = nPage +1;
		if (nSendPage >= nToPage)
		{
			nSendPage = nToPage;
		}
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_liuliangchiist")));
		pList->RemoveAll();
		//pList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_liuliangchiedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				if (nSendPage == nToPage)
				{
					g_SockBuss.SendQueryLlcAll(1,nSendPage,3,0);
				}
				else
					g_SockBuss.SendQueryLlcAll(1,nSendPage,2,atoi(g_MainPage->m_stLLC.strLast.c_str()));
			}
		}
	}
	else if (strBtnName == "CX_liuliangchilastpage")
	{
		
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_liuliangchisumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_liuliangchiist")));
		pList->RemoveAll();
		//pList->SetItemFixedHeight(30);
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_liuliangchiedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendQueryLlcAll(1,nToPage,3,0);
			}
		}
	}
//流量池查询界面结束//////////////////////////////////////////////////////////////////////////////////////////////////////////
//电信账号查询界面开始//////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (strBtnName == "CX_dxbtn" || strBtnName == "CX_dxfirstpage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_dxcurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += "0";
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_dxsumtxt"));

		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		strTotalPage += "0";
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_dxlist")));
		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_dxedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendGetDXList(1);
			}
		}
	}
	else if (strBtnName == "CX_dxprepage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_dxcurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		nPage -=1;
		if (nPage<=0)
		{
			nPage = 1;
		}
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_dxlist")));
		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_dxedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{ if (nPage == 1)
			   g_SockBuss.SendGetDXList(1,1,0);
			else
				g_SockBuss.SendGetDXList(1,nPage,1,atoi(g_MainPage->m_stDianXin.strFirst.c_str()));
			}
		}
	}

	else if (strBtnName == "CX_dxnextpage")
	{

		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_dxcurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_dxsumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		int nSendPage = nPage +1;
		if (nSendPage >= nToPage)
		{
			nSendPage = nToPage;
		}
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_dxlist")));
	   
		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_dxedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				if (nSendPage == nToPage)
				{
					g_SockBuss.SendGetDXList(1,nSendPage,3,0);
				}
				else
					g_SockBuss.SendGetDXList(1,nSendPage,2,atoi(g_MainPage->m_stDianXin.strLast.c_str()));
			}
		}
	}
	else if (strBtnName == "CX_dxlastpage")
	{
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_dxsumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_dxlist")));
		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_dxedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendGetDXList(1,nToPage,3,0);
			}
		}
	}
 //电信账号查询界面结束//////////////////////////////////////////////////////////////////////////////////////////////////////////
 //流量套餐查询开始//////////////////////////////////////////////////////////
	else if (strBtnName == "CX_tcbtn" || strBtnName == "CX_tcfirstpage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_tccurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += "0";
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_tcsumtxt"));

		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		strTotalPage += "0";
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_tclist")));
		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_tcedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendLltcQueryAll(1,1,0,0);
			}
		}
	}
	else if (strBtnName == "CX_tcprepage")
	{
		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_tccurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		nPage -=1;
		if (nPage<=0)
		{
			nPage = 1;
		}
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_tclist")));
		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_tcedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				if (nPage == 1)
					g_SockBuss.SendLltcQueryAll(1,1,0,0);
				else
					g_SockBuss.SendLltcQueryAll(1,nPage,1, atoi(g_MainPage->m_stLLC.strFirst.c_str()));
			}
		}
	}

	else if (strBtnName == "CX_tcnextpage")
	{

		CTextUI* pCurPage = static_cast<CTextUI*>(pm.FindControl("CX_tccurrentpage"));
		string strpage =pCurPage->GetText();
		replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
		replace_all(strpage,"{/c}页{/p}","");
		int nPage= atoi(strpage.c_str());
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_tcsumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		int nSendPage = nPage +1;
		if (nSendPage >= nToPage)
		{
			nSendPage = nToPage;
		}
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_tclist")));
		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_tcedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				if (nToPage == nSendPage)
				{
					g_SockBuss.SendLltcQueryAll(1,nSendPage,3,0);
				}
				else
					g_SockBuss.SendLltcQueryAll(1,nSendPage,2,atoi(g_MainPage->m_stLLTC.strLast.c_str()));
			}
		}
	}
	else if (strBtnName == "CX_tclastpage")
	{
		CTextUI* pTotalPage = static_cast<CTextUI*>(pm.FindControl("CX_tcsumtxt"));

		string strTotalPage=pTotalPage->GetText();
		replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
		replace_all(strTotalPage,"{/c}页){/p}","");

		int nToPage= atoi(strTotalPage.c_str());
		CListUI* pList=static_cast<CListUI*>(pm.FindControl(_T("CX_tclist")));
		pList->RemoveAll();
		/*pList->SetItemFixedHeight(30);*/
		CEditUI* pkehuEdit = static_cast<CEditUI*>(pm.FindControl(_T("CX_tcedit")));
		string strKehu = pkehuEdit->GetText();
		if (strKehu.empty())
		{
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败");
				return;
			}
			else*/
			{
				g_SockBuss.SendLltcQueryAll(1,nToPage,3,0);
			}
		}
	}
//流量套餐查询结束//////////////////////////////////////////////////////////

	else if (strBtnName == "CX_userquerybtn")
	{
			CListUIEx* pUserList =static_cast<CListUIEx*>(pm.FindControl(_T("CX_queryuserlist")));

			CDialogBuilder builder;
			CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("user_list_item_column.xml"),(UINT)0));
			CLabelUI* pla =static_cast<CLabelUI*>(pLine->FindSubControl("domain"));
			pla->SetText("mytest");
			if( pLine != NULL ) 
			{			
			
				pUserList->InsertItem(pUserList->GetCount(), 30, pLine);
			}
			pUserList->SetPos(pUserList->GetPos());
			pUserList->EndDown();
			return;
	}

	/////////////////停复机//////////////////////////////////////////////////////////////////////////////////

	else if (strBtnName == "CX_cardtingjibtn")
	{
		CListUIEx* pUserList =static_cast<CListUIEx*>(pm.FindControl(_T("CX_cardquerylist")));
		/*if (g_SockBuss.Connect() == -1)
		{
			MyMessageBox(NULL,"连接服务器失败");
		}
		*/
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
					
					g_SockBuss.SendTingfuji(0,strjrhm,strdx);
				}
			}
		}

	
		return;
	}

	else if (strBtnName == "CX_cardfujibtn")
	{
		CListUIEx* pUserList =static_cast<CListUIEx*>(pm.FindControl(_T("CX_cardquerylist")));
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
					
					g_SockBuss.SendTingfuji(0,strjrhm,strdx,false);
				}
			}
		}


		return;
	}

	else if (strBtnName == "CX_cardupdatstatebtn")
	{
		CListUIEx* pUserList =static_cast<CListUIEx*>(pm.FindControl(_T("CX_cardquerylist")));

		for (int i = 0; i<pUserList->GetCount(); i++)
		{
			CListContainerElementUI* pLine = (CListContainerElementUI*)pUserList->GetItemAt(i);
			CCheckBoxUI* pla =static_cast<CCheckBoxUI*>(pLine->FindSubControl("cardquerycheckbox"));
			CLabelUI* pla3 =static_cast<CLabelUI*>(pLine->FindSubControl("domain3"));
			string strdx = pla3->GetText();
			CLabelUI* pla2=static_cast<CLabelUI*>(pLine->FindSubControl("domain2"));
			string sim = pla2->GetText();
			if (pla->GetCheck())
			{

				int b = 0;
				/*if (g_SockBuss.Connect() == -1)
				{
					MyMessageBox(NULL,"连接服务器失败");
				}
				else*/
				{

					g_SockBuss.SendUpdateSimState(0,sim,strdx);
				}
			}
		}


		return;
	}

	
 }