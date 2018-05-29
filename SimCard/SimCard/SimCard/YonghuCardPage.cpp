#include "StdAfx.h"
#include "YonghuCardPage.h"
#include "MainPage.h"
#include "ListEx.h"

CYonghuCardPage::CYonghuCardPage(void)
{
}


CYonghuCardPage::~CYonghuCardPage(void)
{
}



void CYonghuCardPage::HandleSelected(TNotifyUI& msg)
{
	
}
void CYonghuCardPage::HandleClick(TNotifyUI& msg)
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
		if (strBtnName == "YHK_cardnextpage")
		{
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("YHK_cardcurrentpage"));
			string strpage =pCurPage->GetText();
			replace_all(strpage,"{p}{y 20}第{c #F27D30}","");
			replace_all(strpage,"{/c}页{/p}","");
			int nPage= atoi(strpage.c_str());
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("YHK_cardsumtxt"));

			string strTotalPage=pTotalPage->GetText();
			replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
			replace_all(strTotalPage,"{/c}页){/p}","");

			int nToPage= atoi(strTotalPage.c_str());
			int nSendPage = nPage +1;
			if (nSendPage >= nToPage)
			{
				nSendPage = nToPage;
			}

			CEditUI* pCard=static_cast<CEditUI*>(m_pm.FindControl(_T("YHK_cardIDedit")));
			CListUIEx* pCardList =static_cast<CListUIEx*>(m_pm.FindControl(_T("YHK_cardquerylist")));
			pCardList->RemoveAll();
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
			{
				if (nSendPage == nToPage)
					g_SockBuss.SendQueryKehuCardBylx(1,m_strkehuid,m_nCmdType,nSendPage,3,0);
				else g_SockBuss.SendQueryKehuCardBylx(1,m_strkehuid,m_nCmdType,nSendPage,2,atoi(m_id.strLast.c_str()));
			}

		}
		if (strBtnName == "YHK_cardprepage")
		{
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("YHK_cardcurrentpage"));
			string strpage =pCurPage->GetText();
			replace_all(strpage,"{p}{y 20}第{c #F27D30}","");  
			replace_all(strpage,"{/c}页{/p}","");
			int nPage= atoi(strpage.c_str());
			
			CListUIEx* pCardList =static_cast<CListUIEx*>(m_pm.FindControl(_T("YHK_cardquerylist")));
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
				g_SockBuss.SendQueryKehuCardBylx(1,m_strkehuid,m_nCmdType,nPre);
				}
				else
					g_SockBuss.SendQueryKehuCardBylx(1,m_strkehuid,m_nCmdType,nPre,1,atoi(m_id.strFirst.c_str()));
			}

		}
		else if (strBtnName == "YHK_cardlastpage")
		{
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("YHK_cardsumtxt"));

			string strTotalPage=pTotalPage->GetText();
			replace_all(strTotalPage,"{p}{y 20}(共{c #F27D30}","");
			replace_all(strTotalPage,"{/c}页){/p}","");

			int nPage= atoi(strTotalPage.c_str());
			CEditUI* pCard=static_cast<CEditUI*>(m_pm.FindControl(_T("YHK_cardIDedit")));
			CListUIEx* pCardList =static_cast<CListUIEx*>(m_pm.FindControl(_T("YHK_cardquerylist")));
			pCardList->RemoveAll();
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
				g_SockBuss.SendQueryKehuCardBylx(1,m_strkehuid,m_nCmdType,nPage,3,0);

		}
		else if ( strBtnName== "YHK_cardfirstpage")
		{
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("YHK_cardcurrentpage"));
			string strpage ="{p}{y 20}第{c #F27D30}";
			strpage += "0";
			strpage +="{/c}页{/p}";
			pCurPage->SetText(strpage.c_str());
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("YHK_cardsumtxt"));

			string strTotalPage="{p}{y 20}(共{c #F27D30}";
			strTotalPage += "0";
			strTotalPage+="{/c}页){/p}";
			pTotalPage->SetText(strTotalPage.c_str());
			//CEditUI* pCard=static_cast<CEditUI*>(m_pm.FindControl(_T("YHK_cardIDedit")));
			CListUIEx* pCardList =static_cast<CListUIEx*>(m_pm.FindControl(_T("YHK_cardquerylist")));
			pCardList->RemoveAll();
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
				g_SockBuss.SendQueryKehuCardBylx(1,m_strkehuid,m_nCmdType,1);
		}

		/////////////////停复机//////////////////////////////////////////////////////////////////////////////////

		else if (strBtnName == "YHK_cardtingjibtn")
		{
			CListUIEx* pUserList =static_cast<CListUIEx*>(m_pm.FindControl(_T("YHK_cardquerylist")));
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

						g_SockBuss.SendTingfuji(1,strjrhm,strdx);
					}
				}
			}


			return;
		}

		else if (strBtnName == "YHK_cardfujibtn")
		{
			CListUIEx* pUserList =static_cast<CListUIEx*>(m_pm.FindControl(_T("YHK_cardquerylist")));
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
						g_SockBuss.SendTingfuji(1,strjrhm,strdx,false);
					}
				}
			}


			return;
		}

		else if (strBtnName == "YHK_cardupdatstatebtn")
		{
			CListUIEx* pUserList =static_cast<CListUIEx*>(m_pm.FindControl(_T("YHK_cardquerylist")));

			for (int i = 0; i<pUserList->GetCount(); i++)
			{
				CListContainerElementUI* pLine = (CListContainerElementUI*)pUserList->GetItemAt(i);
				CCheckBoxUI* pla =static_cast<CCheckBoxUI*>(pLine->FindSubControl("cardquerycheckbox"));
			    string strjrhm = pla->GetText();
				CLabelUI* pla2=static_cast<CLabelUI*>(pLine->FindSubControl("domain2"));
				string strdx = pla2->GetText();
				if (pla->GetCheck())
				{
					g_SockBuss.SendUpdateSimState(1,strjrhm,strdx);
					
				}
			}


			return;
		}
		////////////////////////////////////////////////////////////////////
	}


}

void CYonghuCardPage::HandleHeaderClick(TNotifyUI& msg)
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

void CYonghuCardPage::HandleLink(TNotifyUI& msg)
{
	/*CListUI* pSimList=static_cast<CListUIEx*>(m_pm.FindControl(_T("cardquerylist")));
	if (msg.pSender->GetParent()->GetParent() == pSimList)
	{
		if (msg.wParam == 0)
		{
			CListTextElementUI* pEle = (CListTextElementUI*)msg.pSender;
			string strSim = pEle->GetText(0);
			replace_all(strSim,"{a}","");
			replace_all(strSim,"{/a}","");
		}
		else if (msg.wParam == 1)
		{
			CListTextElementUI* pEle = (CListTextElementUI*)msg.pSender;
			string strSim = pEle->GetText(1);
		}

	}*/
}

void CYonghuCardPage::InitInfo(string strType, string strKehu,string strkehuid,int nCmdType)
{
	m_strType = strType;
	m_strKehu = strKehu;
	m_strkehuid = strkehuid;
	m_nCmdType = nCmdType;
	CListUI* pList =static_cast<CListUI*>(m_pm.FindControl(_T("YHK_cardquerylist")));
	pList->SetItemFixedHeight(30);
	pList->RemoveAll();
	CLabelUI* pKehumin =static_cast<CLabelUI*>(m_pm.FindControl(_T("YHK_khm_label")));	
	string strKehuShow ="客户名:";
	strKehuShow+= "{c #F27D30}";
	strKehuShow +=strKehu;
	strKehuShow+="{\c}";
	pKehumin->SetText(strKehuShow.c_str());
	CLabelUI* pLX =static_cast<CLabelUI*>(m_pm.FindControl(_T("YHK_lx_label")));	
	string strlxShow ="类型:";
	strlxShow += "{c #F27D30}";
	strlxShow+= strType;
	strlxShow +="{\c}";
	pLX->SetText(strlxShow.c_str());
	CListUIEx* pCardList =static_cast<CListUIEx*>(m_pm.FindControl(_T("YHK_cardTFJlist")));
	//pCardList->SetItemFixedHeight(30);
	pCardList->RemoveAll();
	
}


LRESULT  CYonghuCardPage::OnKehudatecardmsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj =(object*)lParam;
	
	CListUIEx* pCardList =static_cast<CListUIEx*>(m_pm.FindControl(_T("YHK_cardquerylist")));
		//pCardList->SetItemFixedHeight(30);
		pCardList->RemoveAll();
		pObj++;
		int nReq = pObj->as<int>();
		pObj++;
		int nTag = pObj->as<int>();
		char sz[20];
		itoa(nTag,sz,10);
		CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("YHK_cardcurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += sz;
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());

		CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("YHK_cardsumtxt"));
		pObj++;
		int nResult=pObj->as<int>();
		if (nResult != 0)
		{
			return 0;
		}
		pObj++;
		int nCount =pObj->as<int>();
		int nPage = nCount/PAGE_COUNT;
		if (nCount%PAGE_COUNT != 0)
			nPage+=1;	 
		itoa(nPage,sz,10);
		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		strTotalPage += sz;
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());
		pObj++;
		if (nResult== 0)
		{
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			
			for (int t = 0; t< i; t++)
			{//id,Jrhm,Iccid,Dxzh,Llchm,Llclx,Xsrq,Jhrq,Xfrq,Dqrq,Zxrq,Bz]
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				if (t==0)
					m_id.strFirst = id;
				if (t==i-1)
					m_id.strLast =id;
				pItem ++;
				string  strjrhm = pItem->as<string >();
				
				pItem++;
				string striccd = pItem->as<string>();
				pItem++;
				string strZT = pItem->as<string>();
				pItem++;
				string strdxzh = pItem->as<string>();
				pItem++;
				string strllcm= pItem->as<string>();
				pItem++;
				string strllclx= pItem->as<string>();
				strllcm +="(";
				strllcm += strllclx;
				strllcm += ")";			

				
				pItem++;
				//xsrq,jhrq,dqrq,zxrq
				string strxsrq = pItem->as<string>();
				pItem++;
				string strjhrq = pItem->as<string>();
				pItem++;
				string strxfrq = pItem->as<string>();
				pItem++;
				string strdqrq = pItem->as<string>();
				pItem++;
				string strzxrq = pItem->as<string>();
				pItem++;
				string strbz = pItem->as<string>();
				
				/*CListTextElementUI* pEle = new CListTextElementUI;
				pCardList->Add(pEle);*/
			
				string strSim = strjrhm;
				string strstate;
				string strtest ="test";
				char szNum[16];
				CDialogBuilder builder;
				CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("card_list_item_column _yh.xml"),(UINT)0));
				CCheckBoxUI* pla =static_cast<CCheckBoxUI*>(pLine->FindSubControl("cardquerycheckbox"));
				pla->SetText(strjrhm.c_str());
				CLabelUI* pla1 =static_cast<CLabelUI*>(pLine->FindSubControl("domain1"));
				pla1->SetText(striccd.c_str());
				CLabelUI* pla2 =static_cast<CLabelUI*>(pLine->FindSubControl("domain2"));
				pla2->SetText(strdxzh.c_str());
				CLabelUI* pla3 =static_cast<CLabelUI*>(pLine->FindSubControl("domain3"));
				pla3->SetText(strllcm.c_str());
				CLabelUI* pla4 =static_cast<CLabelUI*>(pLine->FindSubControl("domain4"));
				pla4->SetText(strxsrq.c_str());
				CLabelUI* pla5 =static_cast<CLabelUI*>(pLine->FindSubControl("domain5"));
				pla5->SetText(strjhrq.c_str());
				CLabelUI* pla6 =static_cast<CLabelUI*>(pLine->FindSubControl("domain6"));
				pla6->SetText(strxfrq.c_str());
				CLabelUI* pla7 =static_cast<CLabelUI*>(pLine->FindSubControl("domain7"));
				pla7->SetText(strdqrq.c_str());
				CLabelUI* pla8 =static_cast<CLabelUI*>(pLine->FindSubControl("domain8"));
				pla8->SetText(strzxrq.c_str());
				CLabelUI* pla9 =static_cast<CLabelUI*>(pLine->FindSubControl("domain9"));
				pla9->SetText(strZT.c_str());
				CLabelUI* pla10 =static_cast<CLabelUI*>(pLine->FindSubControl("domain10"));
				pla10->SetText(strbz.c_str());
			
				if( pLine != NULL ) 
				{			

					pCardList->InsertItem(pCardList->GetCount(), 30, pLine);
				}			
				
			}
		}
	return 0;
}


LRESULT CYonghuCardPage::OnTFJimsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj = (object*)lParam;
	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	if (nsubcmd == DN_DISABLE  || nsubcmd == DN_ABLE)
	{
		pObj++;
		int nReq = pObj->as<int>();
		
		pObj++;
		int nResult = pObj->as<int>();
		pObj++;
		string strjrhm = pObj->as<string>();
		pObj++;
		string strMsg;
		if (nResult == 1)
		{
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;

			for (int t = 0; t< i; t++)
			{//result,resultMsg,productStatusCd,productStatusName,servCreateDate,GROUP_TRANSACTIONID]]]
				object* pItem = (parray++)->via.array.ptr;
				strMsg = pItem->as<string>();
			}
		}
		if (nResult== 0)
		{
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			
			for (int t = 0; t< i; t++)
			{//jrhm,dxzh,khmc,jlxm,xsrq,jhrq,xfrq,xqrq,zxrq,bz,dj
				//RspType,result,resultMsg,GROUP_TRANSACTIONID]
				object* pItem = (parray++)->via.array.ptr;
				string  irsp = pItem->as<string >();
				pItem++;
				string nResult = pItem->as<string>();
				pItem++;
				strMsg = pItem->as<string>();
			}
		}
	
		if (strMsg.empty())
		{
			if (nResult == 0)
				strMsg ="请求成功";
			else
				strMsg = "请求失败";
		}
		CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("YHK_cardTFJlist")));
		//pCardList->RemoveAll();
		bool bFind = false;
		for (int i = 0 ; i<pCardList->GetCount(); i++)
		{
			CListTextElementUI* pele = (CListTextElementUI*)pCardList->GetItemAt(i);
			string strSim = pele->GetText(0);
			if (strSim == strjrhm)
			{
				pele->SetText(1,strMsg.c_str());
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			CListTextElementUI* pele = new CListTextElementUI;
			pCardList->Add(pele);
			pele->SetText(0,strjrhm.c_str());
			pele->SetText(1,strMsg.c_str());
			pele->SetText(2,"");
			pele->SetText(3,"");

		}
	}
	
	return 0;
	
}


LRESULT  CYonghuCardPage::OnUpdatestatemsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{


	object* pObj = (object*)lParam;
	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	
	if (nsubcmd == CS_QUERY_JRHM)
	{
		pObj++;
		int nReq = pObj->as<int>();

		pObj++;
		int nResult = pObj->as<int>();
		pObj++;
		string strjrhm = pObj->as<string>();
		pObj++;
		string strMsg,strProStatus,strTime;
		if (nResult == 1)
		{
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;

			for (int t = 0; t< i; t++)
			{//result,resultMsg,productStatusCd,productStatusName,servCreateDate,GROUP_TRANSACTIONID]]]
				object* pItem = (parray++)->via.array.ptr;
				strMsg = pItem->as<string>();
			}
		}
		if (nResult ==0)
		{
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			
			for (int t = 0; t< i; t++)
			{//result,resultMsg,productStatusCd,productStatusName,servCreateDate,GROUP_TRANSACTIONID]]]
				object* pItem = (parray++)->via.array.ptr;
				string nResult = pItem->as<string>();
				pItem++;
				strMsg = pItem->as<string>();
				pItem++;
				pItem++;
				strProStatus = pItem->as<string>();
				strTime = pItem->as<string>();
			}
		}
	
		if (strMsg.empty())
		{
			if (nResult == 0)
				strMsg ="请求成功";
			else
				strMsg = "请求失败";
		}
		CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("YHK_cardTFJlist")));

		

		bool bFind = false;
		for (int i = 0 ; i<pCardList->GetCount(); i++)
		{
			CListTextElementUI* pele = (CListTextElementUI*)pCardList->GetItemAt(i);
			string strSim = pele->GetText(0);
			if (strSim == strjrhm)
			{
				pele->SetText(1,strMsg.c_str());
				pele->SetText(2,strProStatus.c_str());
				pele->SetText(3,strTime.c_str());
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			CListTextElementUI* pele = new CListTextElementUI;
			pCardList->Add(pele);
			pele->SetText(0,strjrhm.c_str());
			pele->SetText(1,strMsg.c_str());
			pele->SetText(2,strProStatus.c_str());
			pele->SetText(3,strTime.c_str());

		}
	}
	return 0;
}