#include "stdafx.h"
#include "MainPage.h"
#include "ListEx.h"
#include "YonghuCardPage.h"
extern string g_strAuthority;//权限
#include <shellapi.h> 
CSocketBussiness g_llcsock;
CSocketBussiness g_dxzhsock;
CSocketBussiness g_sudqsock;
CSocketBussiness g_lltcsock;
CMainPage::CMainPage() {g_MainPage=NULL; };
string addUrlTag(string & str)
{
	str = "{a}" +str;
	str+="{/a}";
	return str;
}
void CMainPage::HandleSelected(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	CTabLayoutUI* pSwitch = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
	if(name==_T("query"))
		pSwitch->SelectItem(0);
	else if(name==_T("backgroud"))
		pSwitch->SelectItem(1);
	else if(name==_T("import"))
		pSwitch->SelectItem(2);
	else if(name==_T("order"))
		pSwitch->SelectItem(3);
	else if(name==_T("usermana"))
	{
		pSwitch->SelectItem(4);
	/*	if (g_SockBuss.Connect() != -1)
		{
			g_SockBuss.SendKhQueryAll(0,1);
		}*/
	}
	/*else if(name==_T("cardmana"))
		pSwitch->SelectItem(4);*/
	else if(name==_T("oper"))
		pSwitch->SelectItem(5);

	CTabLayoutUI* pQuery = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("query")));
	if(name==_T("cardqueryopt"))
		pQuery->SelectItem(0);
	else if(name==_T("kehucardqueryopt"))
		pQuery->SelectItem(1);
	
	else if(name==_T("kehuqueryopt"))
		pQuery->SelectItem(2);
	else if(name==_T("jingliqueryopt"))
		pQuery->SelectItem(3);
	else if(name==_T("liliangchiqueryopt"))
		pQuery->SelectItem(4);
	else if (name == _T("dxqueryopt"))
		pQuery->SelectItem(5);
	else if(name==_T("tcqueryopt"))
		pQuery->SelectItem(6);
	else if(name==_T("userqueryopt"))
		pQuery->SelectItem(7);
	CTabLayoutUI* pbackadd = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("backadd1")));
	if(name==_T("cardaddopt"))
		pbackadd->SelectItem(0);
	else if(name==_T("liuliangchiaddopt"))
		pbackadd->SelectItem(1);
	else if(name==_T("kehuaddopt"))
		pbackadd->SelectItem(2);
	else if(name==_T("jingliaddopt"))
		pbackadd->SelectItem(3);
	else if(name==_T("areaaddopt"))
	    pbackadd->SelectItem(4);
	else if(name==_T("dianyinuseraddopt"))
	   pbackadd->SelectItem(5);
	else if (name == _T("TCaddopt"))
		pbackadd->SelectItem(6);
	CTabLayoutUI* porder = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("ordertab")));
	if(name==_T("orderaddopt"))
		porder->SelectItem(0);
	else if(name==_T("orderoperopt"))
		porder->SelectItem(1);
}
void CMainPage::HandleClick(TNotifyUI& msg)
{
	if( msg.pSender == m_pCloseBtn ) {
		PostQuitMessage(0);
		return; 
	}
	else if( msg.pSender == m_pMinBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
	else if( msg.pSender == m_pMaxBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
	else if( msg.pSender == m_pRestoreBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
	//else if (msg.pSender == m_pUserQueryBtn)
	//{
	//	CListUIEx* pUserList =static_cast<CListUIEx*>(m_pm.FindControl(_T("userlist")));

	//	CDialogBuilder builder;
	//	CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("user_list_item_column.xml"),(UINT)0));
	//	CLabelUI* pla =static_cast<CLabelUI*>(pLine->FindSubControl("domain"));
	//	pla->SetText("mytest");
	//	if( pLine != NULL ) 
	//	{			
	//	
	//		pUserList->InsertItem(pUserList->GetCount(), 30, pLine);
	//	}
	//	pUserList->SetPos(pUserList->GetPos());
	//	pUserList->EndDown();
	//	return;
	//}
	//else if (msg.pSender == m_pSimQueryBtn)
	//{
	//	CListUI* pSimList=static_cast<CListUIEx*>(m_pm.FindControl(_T("cardquerylist")));
	//	CListTextElementUI* pEle = new CListTextElementUI;
	//	pSimList->Add(pEle);
	//	string strSim = "{a}";
	//	strSim+= "12345678899888";
	//	strSim+= "{/a}";
	//	string strtest ="test";
	//	pEle->SetText(0, strSim.c_str());
	//	pEle->SetText(1, "{a}654321{/a}");
	//	pSimList->SetItemFixedHeight(30);
	//	for (int i = 2; i<13; i++)
	//	{
	//		pEle->SetText(i, strtest.c_str());
	//	}

	//}
	//else if (msg.pSender->GetName() =="domain3" )
	//{
	//	CControlUI* pH = msg.pSender->GetParent();
	//	CControlUI* pH2 = pH->GetParent();
	//	/*CListContainerElementUI* pLine = (CListContainerElementUI*)pLine;*/
	//	CListUIEx* pUserList =static_cast<CListUIEx*>(m_pm.FindControl(_T("userlist")));
	//	pUserList->Remove(pH2);

	//	int a  = 0;

	//}
	else if (msg.pSender->GetName().Find("ournet")!= -1)
	{
		::ShellExecute(NULL, _T("open"), _T("http://simcard.chinaptt.net/simcard/simcard/82472/"), NULL, NULL, SW_SHOW);
	}
	else if (msg.pSender->GetName().Find("about")!= -1)
	{
		char szIP[1024];
		unsigned short nPort;
		TCHAR szFilePath[1024 ]={0};
		GetModuleFileName(NULL, szFilePath, 1024);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
		std::string  strPath = szFilePath;
		strPath += "app.ini";
		::GetPrivateProfileString("更新配置","version","201804181",szIP,1024,strPath.c_str());
		 const char* szShow = "版本号:  %s\n\n版权:  河南宝蓝有限公司";
		 char szmess[1024];
		 sprintf_s(szmess,1024 ,szShow,szIP);
		MyMessageBox(NULL,szmess,"关于");
	}
	else if (msg.pSender->GetName().Find("HT_")!= -1)
	{
		m_clsHTHandler.HandleClick(msg, m_pm);
	}
	else if (msg.pSender->GetName().Find("DD_")!= -1)
	{
		m_clsOrderHandler.HandleClick(msg, m_pm);
	}
	else if (msg.pSender->GetName().Find("CX_")!= -1)
	{
		m_clsQueryHandler.HandleClick(msg, m_pm);
	}
	else if (msg.pSender->GetName().Find("YH_")!= -1)
	{
		m_clsUserHandler.HandleClick(msg, m_pm);
	}
	else if (msg.pSender->GetName().Find("CC_")!= -1)
	{
		m_clsUserHandler.HandleClick(msg, m_pm);
	}
}

void CMainPage::HandleHeaderClick(TNotifyUI& msg)
{
	return;//暂时屏蔽此功能
	CListUI* pList= NULL;
	CListUI* pUserList =static_cast<CListUI*>(m_pm.FindControl(_T("userlist")));
	COptionUI* pOptionUser = static_cast<COptionUI*>(m_pm.FindControl(_T("userqueryopt")));
	if (pOptionUser->IsSelected())
	{
		pList = pUserList;
	}
	if (pList)
	{
		
		CListHeaderUI* pHeader = pList->GetHeader();
		int nNum = pHeader->GetCount();
		for (int i = 0; i < pHeader->GetCount(); i++)
		{
			CListHeaderItemUI* pC = static_cast<CListHeaderItemUI*>(pHeader->GetItemAt(i));
			LPWSTR sz;
			CDuiString strTit = pC->GetText();
			if (strTit == msg.pSender->GetText())
			{
				/*g_nListItemClick = i;*/
				CDuiString strA = /*CPaintManagerUI::GetResourcePath()  + */"list_header_orderA.png";
				CDuiString strD = /*CPaintManagerUI::GetResourcePath()  + */"list_header_orderD.png";
				
				if (m_bSortAseByTitle)
					pC->SetBkImage(strD);
				else
					pC->SetBkImage(strA);
				m_bSortAseByTitle =!m_bSortAseByTitle;

			}
			else
			{
				CDuiString str = /*CPaintManagerUI::GetResourcePath()  +*/ "list_header_bg.png";

				pC->SetBkImage(str);
			}
		}
		//DoSort(pList,msg.pSender->GetText()); 
	}
}

void CMainPage::HandleLink(TNotifyUI& msg)
{
    CListUI* pSimList=static_cast<CListUIEx*>(m_pm.FindControl(_T("CX_kehucardquerylist")));
   CListUI* pJLList=static_cast<CListUIEx*>(m_pm.FindControl(_T("CX_jinglilist")));
	if (msg.pSender->GetParent()->GetParent() == pSimList)
	{
		string strKehuid,strType;
		CListTextElementUI* pEle = (CListTextElementUI*)msg.pSender;
		strKehuid = pEle->GetText(0);
		string strkehu =pEle->GetText(1);
		replace_all(strkehu,"{a}","");
		replace_all(strkehu,"{/a}","");
		int nCmdType;
		if (msg.wParam == 0)
		{
			string strkhid,strkh;
			CListTextElementUI* pEle = (CListTextElementUI*)msg.pSender;
			strkhid = pEle->GetText(0);
			strkh = pEle->GetText(1);
			replace_all(strkh,"{a}","");
			replace_all(strkh,"{/a}","");
			g_SockBuss.SendGetKhRqTj(1,strKehuid);
			m_pKhrqTjPage->InitInfo(strkehu,strKehuid);
			m_pKhrqTjPage->ShowWindow(SW_SHOW);
			return;
		}
		else if (msg.wParam == 1)
		{
			strType="全部";
			nCmdType=KH_SIM_LIST;
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
				g_SockBuss.SendQueryKehuCardBylx(1,strKehuid,KH_SIM_LIST,1);
		}
		else if (msg.wParam == 2)
		{
			strType="1个月到期";
			nCmdType=KH_SIM_U1M;
		/*	if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
				g_SockBuss.SendQueryKehuCardBylx(1,strKehuid,KH_SIM_U1M,1);
		}
		else if (msg.wParam == 3)
		{
			nCmdType=KH_SIM_U15D;
			strType="15天到期";
		/*	if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
				g_SockBuss.SendQueryKehuCardBylx(1,strKehuid,KH_SIM_U15D,1);
		}
		else if (msg.wParam == 4)
		{
			nCmdType=KH_SIM_D15D;
			strType="已到期15天";
			/*if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else*/
				g_SockBuss.SendQueryKehuCardBylx(1,strKehuid,KH_SIM_D15D,1);
		}
		else if (msg.wParam == 5)
		{
			nCmdType=KH_SIM_D1M;
			strType="已到期1个月";
			if (g_SockBuss.Connect() == -1)
			{
				MyMessageBox(NULL,"连接服务器失败！");
				return;
			}
			else
				g_SockBuss.SendQueryKehuCardBylx(1,strKehuid,KH_SIM_D1M,1);
		}
		//m_pYHCardPage->Close();
		//m_pYHCardPage->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_CLIENTEDGE);
		m_pYHCardPage->InitInfo(strType,strkehu,strKehuid,nCmdType);
	
		m_pYHCardPage->ShowWindow(SW_SHOW);
		
	
	}

	if (msg.pSender->GetParent()->GetParent() == pJLList)
	{
		string strJlname,strType;
		CListTextElementUI* pEle = (CListTextElementUI*)msg.pSender;
		strJlname = pEle->GetText(0);
	
		replace_all(strJlname,"{a}","");
		replace_all(strJlname,"{/a}","");
		/*if (m_pYHCardPage->GetHWND() == NULL)*/
		//	m_pYHCardPage->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_CLIENTEDGE);
		g_SockBuss.SendGetJlKh(1,strJlname);
		g_SockBuss.SendGetJlXSJL(1,strJlname);
		if (msg.wParam == 0)
		{
			m_pJlkhPage->InitInfo(strJlname);
			m_pJlkhPage->ShowWindow(SW_SHOW);
		}
		int nCmdType;
		
	}
	
	
}

LRESULT CMainPage::OnKHMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    	object* pObj = (object*)lParam;
	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	if (nsubcmd == KH_ADD)
	{		
		pObj++;
		int nResult = pObj->as<int>();
		if (nResult != 0)
		{
			MyMessageBox(*this,"添加客户失败，请检查名称是否已经存在");
			return 0;
		}
		else
		{
			CEditUI* pName= static_cast<CEditUI*>(m_pm.FindControl("HT_addkehu_kehunameEdit"));
			CEditUI* pLxfs= static_cast<CEditUI*>(m_pm.FindControl("HT_addkehu_lxfsEdit"));
			CComboUI* pJl= static_cast<CComboUI*>(m_pm.FindControl("HT_addkehu_jingliCombo"));
			CEditUI* pRemark= static_cast<CEditUI*>(m_pm.FindControl("HT_addkehu_remarkEdit"));
			pName->SetText("");
			pLxfs->SetText("");
			pJl->SetText("");
			pRemark->SetText("");
			pObj++;
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			
			CListUI* pLst= static_cast<CListUI*>(m_pm.FindControl(_T("kehuaddlist")));
			//[nCmd,nSubCmd,0/1,[[id,Khmc,Lxfs,Ssdq,Jlxm,Dj,On1m,On15d,Du15d,Du1m,Xgsj,Bz]]]
			for (int t = 0; t< i; t++)
			{		
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				pItem ++;
				string  strxm = pItem->as<string >();
				CListLabelElementUI* lab = new CListLabelElementUI;
				lab->SetText(strxm.c_str());
				
				pItem++;
				string strlxfs = pItem->as<string>();
				pItem++;
				string strssdq = pItem->as<string>();
				pItem++;
				string strjl = pItem->as<string>();
				pItem++;
				string dj = pItem->as<string>();
			
				pItem++;
				pItem++;
				pItem++;
				pItem++;
				pItem++;
				string strscsj = pItem->as<string>();
				
				pItem++;
				string strbz = pItem->as<string>();
				CListTextElementUI* pEle = new CListTextElementUI;
				pLst->Add(pEle);
				pEle->SetText(0, strxm.c_str());
				pEle->SetText(1, strlxfs.c_str());
				pEle->SetText(2,strjl.c_str());
				pEle->SetText(3, strssdq.c_str());	
				pEle->SetText(4, dj.c_str());
				pEle->SetText(5, strscsj.c_str());	
				pEle->SetText(6, strbz.c_str());
			}
		}
	}
	
	else if (nsubcmd == KH_LIST)
	{			
		pObj++;
		int nReq = pObj->as<int>();
		CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("CX_kehucardquerylist")));
		//[nCmd,nSubCmd,nTag,0/1,ncount,[[id,Khmc,Lxfs,Ssdq,Jlxm,Dj,On1m,On15d,Du15d,Du1m,Xgsj,Bz]]]
		pCardList->RemoveAll();
		pObj++;
		int nTag = pObj->as<int>();
		char sz[20];
		itoa(nTag,sz,10);
		CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("CX_kehucardcurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += sz;
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());

		CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("CX_kehucardsumtxt"));
		pObj++;
		int nResult=pObj->as<int>();
		if (nResult!=0)
		{
			MyMessageBox(NULL,"查询失败");
			return 1;
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
			{//id,Khmc,Lxfs,Ssdq,Jlxm,Dj,On1m,On15d,Du15d,Du1m,Xgsj,Bz
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				 if (t == 0 )
					 g_MainPage->m_stKHCARD.strFirst = id;
				 if ( t== i-1)
				     g_MainPage->m_stKHCARD.strLast = id;
				pItem ++;
				string strkhmc = pItem->as<string>();
				pItem ++;
				string strlxfs = pItem->as<string>();
				pItem ++;
				string strssdq = pItem->as<string>();
				pItem ++;
				string strjlxm= pItem->as<string>();
				pItem ++;
				string strdj= pItem->as<string>();
				pItem ++;
				string strsuoyo= pItem->as<string>();
				
				addUrlTag(strsuoyo);
					
				pItem ++;
				string stron15 = pItem->as<string>();
				addUrlTag(stron15);
				pItem ++;
				string stron1m = pItem->as<string>();
				addUrlTag(stron1m);
				pItem ++;
				string stryi15 = pItem->as<string>();
				addUrlTag(stryi15);
				pItem ++;
				string stryi1m = pItem->as<string>();
				addUrlTag(stryi1m);
				;
				pItem ++;
				string strcreattime = pItem->as<string>();
				pItem ++;
				string strbz = pItem->as<string>();
				CListTextElementUI* pEle = new CListTextElementUI;
				pCardList->Add(pEle);
				pEle->SetText(0, id.c_str());
				string strtagkehu = strkhmc;
				addUrlTag(strtagkehu);
				pEle->SetText(1, strtagkehu.c_str());
				pEle->SetText(2, strjlxm.c_str());
				pEle->SetText(3, strlxfs.c_str());
				pEle->SetText(4, strssdq.c_str());
				pEle->SetText(5, strdj.c_str());
				pEle->SetText(6, strsuoyo.c_str());
				pEle->SetText(7, stron1m.c_str());
				pEle->SetText(8, stron15.c_str());
				pEle->SetText(9, stryi15.c_str());
				pEle->SetText(10, stryi1m.c_str());

				pEle->SetText(11, strcreattime.c_str());
				pEle->SetText(12, strbz.c_str());

					
			}
		}
		
	}
	else if (nsubcmd>=KH_SIM_USING && nsubcmd<=KH_SIM_D15D ||nsubcmd == KH_SIM_LIST)
	{
		::SendMessage(*m_pYHCardPage, WM_DATECARD_MSG,0,(LPARAM)pObj);
	}
	else if(nsubcmd == KH_QUERY)
	{
		::SendMessage(*m_pKhrqTjPage, WM_KH_RQTJ_MSG,0,(LPARAM)pObj);
	}

	else if(nsubcmd == KH_SIM_XSRQ)
	{
		::SendMessage(*m_pXsrqCardPage, WM_DATECARD_MSG,0,(LPARAM)pObj);
	}

	
}

LRESULT CMainPage::OnDXMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj = (object*)lParam;
	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	if (nsubcmd == DXZH_ADD)
	{
		
		pObj++;
		int nResult = pObj->as<int>();
		if (nResult != 0)
		{
			MyMessageBox(*this,"添加电信账户失败，请检查名称是否已经存在");
			return 0;
		}
		else
		{
			CEditUI* pName= static_cast<CEditUI*>(m_pm.FindControl("HT_addDX_nameEdit"));
			CEditUI* pID= static_cast<CEditUI*>(m_pm.FindControl("HT_addDX_idEdit"));
			CEditUI* pPass= static_cast<CEditUI*>(m_pm.FindControl("HT_addDX_passEdit"));
			CEditUI* pMiyao= static_cast<CEditUI*>(m_pm.FindControl("HT_addDX_miyaoEdit"));
			CEditUI* pRemark= static_cast<CEditUI*>(m_pm.FindControl("HT_addDX_remarkEdit"));
			pName->SetText("");
			pID->SetText("");
			pPass->SetText("");
			pMiyao->SetText("");
			pRemark->SetText("");
			pObj++;
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;

			CListUI* pLst= static_cast<CListUI*>(m_pm.FindControl(_T("DXaddlist")));

			for (int t = 0; t< i; t++)
			{		
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				pItem ++;
				string  strxm = pItem->as<string >();
				
				pItem++;
				string strID = pItem->as<string>();
				pItem++;
				string strPass = pItem->as<string>();
				pItem++;
				string strMiyao = pItem->as<string>();

				pItem++;
				string strbz = pItem->as<string>();
				CListTextElementUI* pEle = new CListTextElementUI;
				pLst->Add(pEle);
				pEle->SetText(0, strxm.c_str());
				pEle->SetText(1, strID.c_str());
				pEle->SetText(2,strPass.c_str());
				pEle->SetText(3, strMiyao.c_str());	
				pEle->SetText(4, strbz.c_str());
			}
			g_SockBuss.SendGetDXList(0);
		}
	}
	else if (nsubcmd == DXZH_LIST)
	{
		pObj++;
		int nReq = pObj->as<int>();
		pObj++;
		int nTag = pObj->as<int>();
		pObj++;
		int nResult = pObj->as<int>();
		if (nResult != 0)
		{
 			//MyMessageBox(*this,"获取电信账户失败");
			return 0;
		}
		else
		{
			pObj++;
			int nCount = pObj->as<int>();
			pObj++;
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("CX_dxcurrentpage"));
			string strpage ="{p}{y 20}第{c #F27D30}";
			char sz[16];
			strpage += itoa(nTag,sz,10);
			strpage +="{/c}页{/p}";
			pCurPage->SetText(strpage.c_str());
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("CX_dxsumtxt"));

			string strTotalPage="{p}{y 20}(共{c #F27D30}";
			int nPage = nCount/PAGE_COUNT;
			if (nCount%PAGE_COUNT != 0)
				nPage+=1;	
			strTotalPage += itoa(nPage,sz,10);
			strTotalPage+="{/c}页){/p}";
			pTotalPage->SetText(strTotalPage.c_str());

			CComboUI* pCom1= static_cast<CComboUI*>(m_pm.FindControl(_T("HT_addLLC_dxzhComb")));
			if (nReq == 0)
			pCom1->RemoveAll();
			CComboUI* pCom2= static_cast<CComboUI*>(m_pm.FindControl(_T("HT_addcard_dxzh_Edit")));
			if (nReq == 0)
			pCom2->RemoveAll();
			CListUI* pLst1= static_cast<CListUI*>(m_pm.FindControl(_T("CX_dxlist")));
			for (int t = 0; t< i; t++)
			{		
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				if (t== 0 && nReq != 0)
				{
					g_MainPage->m_stDianXin.strFirst = id;
				}
				if (t== i-1&& nReq != 0)
				{
					g_MainPage->m_stDianXin.strLast = id;
				}
				pItem ++;
				string  strxm = pItem->as<string >();
				CListLabelElementUI* lab = new CListLabelElementUI;
				CListLabelElementUI* lab1 = new CListLabelElementUI;
				CListTextElementUI* pEle= new CListTextElementUI;
				char sz[16];
			    if (nReq== 1)
				pLst1->Add(pEle);
				lab->SetText(strxm.c_str());
				lab1->SetText(strxm.c_str());
				if (nReq == 0)
				{
					pCom1->Add(lab);
					pCom2->Add(lab1);
				}
				
				pEle->SetText(0,id.c_str());
				pEle->SetText(1,strxm.c_str());

				pItem++;
				string strID = pItem->as<string>();
				pItem++;
				string strPass = pItem->as<string>();
				pItem++;
				string strMiyao = pItem->as<string>();

				pItem++;
			
			}
			
		}
	}
	return 0;
}

LRESULT CMainPage::OnSSDQMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj = (object*)lParam;
	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	if (nsubcmd == LLC_ADD)
	{
		pObj++;
		int iResult = pObj->as<int>();


		if (iResult== 0)
		{
			CEditUI* pdq= static_cast<CEditUI*>(m_pm.FindControl("HT_addareaEdit"));
			pdq->SetText("");
			pObj++;
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			CListUI* pLst=static_cast<CListUIEx*>(m_pm.FindControl(_T("areaaddlist")));
			pLst->RemoveAll();
			
			for (int t = 0; t< i; t++)
			{//id,llchm,llclx,dxzh,xgsj,bz]
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				pItem ++;
				string  strArea = pItem->as<string >();
			
				string strid =id;
				CListTextElementUI* pEle = new CListTextElementUI;
				pLst->Add(pEle);
				pEle->SetText(0, strid.c_str());
				pEle->SetText(1, strArea.c_str());

			
				
			}
			pLst->SetPos(pLst->GetPos());
			pLst->EndDown();
			CEditUI* pName= static_cast<CEditUI*>(m_pm.FindControl("HT_addareaEdit"));			
			pName->SetText("");		
			pName->SetFocus();
			g_SockBuss.SendSsdqQueryAll();
		}
		else
		{
			MyMessageBox(*this,"添加失败，检查是否已经有该流量池编号");
		}
		int b = 0;
	}
	else if (nsubcmd == SSDQ_LIST)
	{
		pObj++;
		int nReq = pObj->as<int>();
 		pObj++;
 		int nTag = pObj->as<int>();
		pObj++;
		int nResult= pObj->as<int>();
	
		if (nResult== 0)
		{
			pObj++;
			int nCount =pObj->as<int>();
			pObj++;
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;

			CComboUI* pCb=static_cast<CComboUI*>(m_pm.FindControl(_T("HT_addcard_ssdq_com")));
			pCb->RemoveAll();
			CComboUI* pCom=static_cast<CComboUI*>(m_pm.FindControl(_T("HT_addkh_ssdq_com")));
			pCom->RemoveAll();
			for (int t = 0; t< i; t++)
			{		
				//id,llchm,llclx,dxzh,xgsj,bz
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				pItem ++;
				string  strssdq = pItem->as<string >();
				pItem++;
				string strcjsj = pItem->as<string>();
				
				CListLabelElementUI * ele = new CListLabelElementUI;
				ele->SetText(strssdq.c_str());
				pCb->Add(ele);
				CListLabelElementUI * ele2 = new CListLabelElementUI;
				ele2->SetText(strssdq.c_str());
				pCom->Add(ele2);
				/*ele->SetText(0,strllchm.c_str());
				ele->SetText(1,strllclx.c_str());
				ele->SetText(2,strdxzh.c_str());
				ele->SetText(3,strxgsj.c_str());
				ele->SetText(4,strbz.c_str());*/
			}
		}
	}
	return 0;
}

LRESULT CMainPage::OnLLCMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj = (object*)lParam;
	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	if (nsubcmd == LLC_LIST)
	{
		pObj++;
		int nReq = pObj->as<int>();
		pObj++;
		int nTag = pObj->as<int>();
		pObj++;
		int nResult= pObj->as<int>();
		
		if (nResult== 0)
		{
			pObj++;
			int nCount =pObj->as<int>();
			pObj++;
			
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("CX_liuliangchicurrentpage"));
			string strpage ="{p}{y 20}第{c #F27D30}";
			char sz[16];
			strpage += itoa(nTag,sz,10);
			strpage +="{/c}页{/p}";
			pCurPage->SetText(strpage.c_str());
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("CX_liuliangchisumtxt"));

			string strTotalPage="{p}{y 20}(共{c #F27D30}";
			int nPage = nCount/PAGE_COUNT;
			if (nCount%PAGE_COUNT != 0)
				nPage+=1;	
			strTotalPage += itoa(nPage,sz,10);
			strTotalPage+="{/c}页){/p}";
			pTotalPage->SetText(strTotalPage.c_str());
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			
			CListUI* pList=static_cast<CListUI*>(m_pm.FindControl(_T("CX_liuliangchiist")));
			
			pList->RemoveAll();
			CComboUI* paddcarllc=static_cast<CComboUI*>(m_pm.FindControl(_T("HT_addcard_llc_Edit")));
			if (nReq == 0)
			paddcarllc->RemoveAll();
			for (int t = 0; t< i; t++)
			{		
				//id,llchm,llclx,dxzh,xgsj,bz
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				if (t == 0&& nReq != 0)
				{
					g_MainPage->m_stLLC.strFirst =id;
				}
				if (t == i-1&& nReq != 0)
				{
					g_MainPage->m_stLLC.strLast = id;
				}
				pItem ++;
				string  strllchm = pItem->as<string >();
				pItem++;
				CListLabelElementUI * plab = new CListLabelElementUI;
				plab->SetText(strllchm.c_str());
				if (nReq == 0)
				paddcarllc->Add(plab);
				string strllclx = pItem->as<string>();
				pItem++;
				string strdxzh = pItem->as<string>();
				pItem++;
				string strxgsj = pItem->as<string>();
				pItem++;
				string strbz = pItem->as<string>();
				CListTextElementUI * ele = new CListTextElementUI;
				if (nReq==1)
				{
					pList->Add(ele);
				}
			
				ele->SetText(0,strllchm.c_str());
				ele->SetText(1,strllclx.c_str());
				ele->SetText(2,strdxzh.c_str());
				ele->SetText(3,strxgsj.c_str());
				ele->SetText(4,strbz.c_str());
			}
		}
		//if (g_SockBuss.Connect()== -1)
		//{
		//	MyMessageBox(*g_MainPage,"连接服务器失败");
		//	return 0;
		//}
		//g_SockBuss.SendGetDXList();

	}
	else if (nsubcmd == LLC_ADD)
	{
		pObj++;
		int iResult = pObj->as<int>();

		
		if (iResult== 0)
		{
			CComboUI* pLX= static_cast<CComboUI*>(m_pm.FindControl("HT_addLLC_lxComb"));
			CEditUI* pID= static_cast<CEditUI*>(m_pm.FindControl("HT_addLLC_nameEdit"));
			CComboUI* pDX= static_cast<CComboUI*>(m_pm.FindControl("HT_addLLC_dxzhComb"));
			CEditUI* pBZ= static_cast<CEditUI*>(m_pm.FindControl("HT_addLLC_bzEdit"));
			pLX->SetText("");
			pID->SetText("");
			pDX->SetText("");
			pBZ->SetText("");
			pObj++;
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			CListUI* pLst=static_cast<CListUIEx*>(m_pm.FindControl(_T("lingliangchiaddlist")));
			for (int t = 0; t< i; t++)
			{//id,llchm,llclx,dxzh,xgsj,bz]
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				pItem ++;
				string  strllchm = pItem->as<string >();
				pItem++;
				string  strllclx = pItem->as<string >();
				pItem++;
				string strdxzh = pItem->as<string>();
				pItem++;
				string strxgsj = pItem->as<string>();
				pItem++;
				string strbz = pItem->as<string>();
				CListTextElementUI* pEle = new CListTextElementUI;
				pLst->Add(pEle);
				pEle->SetText(0, strllchm.c_str());
				pEle->SetText(1, strllclx.c_str());
				pEle->SetText(2,strdxzh.c_str());
				pEle->SetText(3, strxgsj.c_str());	
				pEle->SetText(4, strbz.c_str());
			}
			pLst->SetPos(pLst->GetPos());
			pLst->EndDown();
			CEditUI* pName= static_cast<CEditUI*>(m_pm.FindControl("HT_jl_name_edit"));
			CEditUI* pLxfs= static_cast<CEditUI*>(m_pm.FindControl("HT_jl_lxfs_edit"));
			CEditUI* pRemark= static_cast<CEditUI*>(m_pm.FindControl("HT_jl_remark"));
			pName->SetText("");
			pLxfs->SetText("");
			pRemark->SetText("");
			pName->SetFocus();
			g_SockBuss.SendQueryLlcAll(0);
		}
		else
		{
			MyMessageBox(*this,"添加失败，检查是否已经有该流量池编号");
		}
		int b = 0;
	}
	return 0;
}
LRESULT CMainPage::OnKHJLMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj = (object*)lParam;
	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	if (nsubcmd == KHJL_LIST)
	{
		//[KHJL_DATA,KHJL_LIST,nTag,0/1,nCount,[[id,jlxm,lxfs,xgsj,bz],...]]
		pObj++;
		int nReq = pObj->as<int>();
		pObj++;
		int nTag = pObj->as<int>();
		pObj++;
		char sz[20];
		itoa(nTag,sz,10);
		CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("CX_jinglicurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += sz;
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());
		int nResult= pObj->as<int>();
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
		CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("CX_jinglisumtxt"));
		string strTotalPage="{p}{y 20}(共{c #F27D30}";
		strTotalPage += sz;
		strTotalPage+="{/c}页){/p}";
		pTotalPage->SetText(strTotalPage.c_str());
		pObj++;
		if (nResult== 0)
		{
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			CComboUI* pkehujinglicom=static_cast<CComboUI*>(m_pm.FindControl(_T("HT_addkehu_jingliCombo")));
			if (nReq == 0)
			pkehujinglicom->RemoveAll();
			CComboUI* paddcarjlcom=static_cast<CComboUI*>(m_pm.FindControl(_T("HT_addcard_jl_com")));
			if (nReq == 0)
			paddcarjlcom->RemoveAll();
		    CListUI* pList=static_cast<CListUI*>(m_pm.FindControl(_T("CX_jinglilist")));
			if (nReq == 1)
			pList->RemoveAll();
			for (int t = 0; t< i; t++)
			{		
				//
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				if (t== 0 && nReq != 0)
				{
					g_MainPage->m_stJingli.strFirst = id;
				}
				if ( t == i-1 && nReq != 0)
				{
					g_MainPage->m_stJingli.strLast = id;
				}
				pItem ++;
				string  strjlxm = pItem->as<string >();
				CListLabelElementUI* lab = new CListLabelElementUI;
				string strurljl = strjlxm;
				addUrlTag(strurljl);
				lab->SetText(strjlxm.c_str());
				CListLabelElementUI* lab1 = new CListLabelElementUI;
				lab1->SetText(strjlxm.c_str());
				if (nReq == 0)
				{
					pkehujinglicom->Add(lab);
					paddcarjlcom->Add(lab1);
				}
				

				pItem++;
				string strlxfs = pItem->as<string>();
				pItem++;
				string strxgsj = pItem->as<string>();
				pItem++;
				string strbz = pItem->as<string>();
				CListTextElementUI* pLe = new CListTextElementUI;
				if (nReq == 1)
				pList->Add(pLe);
				pLe->SetText(0,strurljl.c_str());
				pLe->SetText(1,strlxfs.c_str());
				pLe->SetText(2,strxgsj.c_str());
				pLe->SetText(3,strbz.c_str());
			}
		}
	/*	if (g_SockBuss.Connect()== -1)
		{
			MyMessageBox(*g_MainPage,"连接服务器失败");
			return 0;
		}*/
		//g_SockBuss.SendGetDXList();

	}
	else if (nsubcmd == KHJL_ADD)
	{
		pObj++;
		int iResult = pObj->as<int>();
		
		pObj++;
		if (iResult== 0)
		{
			CEditUI* pName= static_cast<CEditUI*>(m_pm.FindControl("HT_jl_name_edit"));
			CEditUI* pLxfs= static_cast<CEditUI*>(m_pm.FindControl("HT_jl_lxfs_edit"));
			CEditUI* pRemark= static_cast<CEditUI*>(m_pm.FindControl("HT_jl_remark"));
			pName->SetText("");
			pLxfs->SetText("");
			pRemark->SetText("");
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			CListUI* pLst=static_cast<CListUIEx*>(m_pm.FindControl(_T("HT_jingliaddlist")));
			for (int t = 0; t< i; t++)
			{		
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				pItem ++;
				string  strjlxm = pItem->as<string >();
				pItem++;
				string strlxfs = pItem->as<string>();
				pItem++;
				string strxgsj = pItem->as<string>();
				pItem++;
				string strbz = pItem->as<string>();
				CListTextElementUI* pEle = new CListTextElementUI;
				pLst->Add(pEle);
				pEle->SetText(0, strjlxm.c_str());
				pEle->SetText(1, strlxfs.c_str());
				pEle->SetText(2, strxgsj.c_str());	
				pEle->SetText(3, strbz.c_str());
			}
			pLst->SetPos(pLst->GetPos());
			pLst->EndDown();
			g_SockBuss.SendKhjlQueryAll("",0);
	
		}
		else
		{
			MyMessageBox(*this,"添加失败，检查是否已经有该经理名");
		}
		int b = 0;
		
	}
	else if (nsubcmd == KHJL_KH)
	{
		::SendMessage(*m_pJlkhPage, WM_JLKH_LIST_MSG,0,(LPARAM)pObj);
	}
	else if (nsubcmd == KH_QUERY)
	{
		::SendMessage(*m_pJlkhPage, WM_JL_QUERY_MSG,0,(LPARAM)pObj);
	}
	else if (nsubcmd == KHJL_SIM_XSRQ)
	{
		::SendMessage(*m_pXsrqCardPage, WM_DATECARD_MSG,0,(LPARAM)pObj);
	}
	return 0;
}


LRESULT CMainPage::OnLLTCMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj = (object*)lParam;
	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	if (nsubcmd == LLTC_LIST)
	{
		pObj++;
		int nReq = pObj->as<int>();

 		CComboUI* ptc=static_cast<CComboUI*>(m_pm.FindControl(_T("HT_addcard_lltc_com")));
		if (nReq == 0)
		ptc->RemoveAll();
		CListUI* pLst=static_cast<CListUI*>(m_pm.FindControl(_T("CX_tclist")));
		if (nReq == 1)
		pLst->RemoveAll();
 		pObj++;
		int nTag = pObj->as<int>();
		
 		pObj++;
 		int nResult=pObj->as<int>();
		if (nResult != 0)
		{
			return 0;
		}
		pObj++;
		int nCount =pObj->as<int>();
		
		pObj++;
		if (nResult== 0)
		{
			CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("CX_tccurrentpage"));
			string strpage ="{p}{y 20}第{c #F27D30}";
			char sz[16];
			strpage += itoa(nTag,sz,10);
			strpage +="{/c}页{/p}";
			pCurPage->SetText(strpage.c_str());
			CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("CX_tcsumtxt"));

			string strTotalPage="{p}{y 20}(共{c #F27D30}";
			int nPage = nCount/PAGE_COUNT;
			if (nCount%PAGE_COUNT != 0)
				nPage+=1;	
			strTotalPage += itoa(nPage,sz,10);
			strTotalPage+="{/c}页){/p}";
			pTotalPage->SetText(strTotalPage.c_str());
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			
			for (int t = 0; t< i; t++)
			{
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
				if (t == 0 && nReq != 0)
				{
					g_MainPage->m_stLLTC.strFirst = id;
				}
				if (t== i-1 && nReq != 0)
				{
					g_MainPage->m_stLLTC.strLast = id;
				}
				pItem ++;
				string  strtcmc = pItem->as<string >();
				CListLabelElementUI* pla = new CListLabelElementUI;
				CListTextElementUI* pele= new CListTextElementUI;
				if (nReq ==1)
				pLst->Add(pele);
				pla->SetText(strtcmc.c_str());
				if (nReq == 0)
				ptc->Add(pla);
				pItem++;
				string strtclx = pItem->as<string>();
				char sz[16];
				pele->SetText(0,id.c_str());
				pele->SetText(1,strtcmc.c_str());
				pele->SetText(2,strtclx.c_str());
				
				
			
			/*	pEle->SetText(0, strSim.c_str());
				pEle->SetText(1, "");
				pEle->SetText(2, striccd.c_str());
				pEle->SetText(3, strdxzh.c_str());
				pEle->SetText(4, strkhmc.c_str());
				pEle->SetText(5, strkhmc.c_str());*/
				
			}
		}
	}
	else if (nsubcmd == LLTC_ADD)
	{
		CEditUI* pLX= static_cast<CEditUI*>(m_pm.FindControl("HT_addTC_LXEdit"));
		CEditUI* pName= static_cast<CEditUI*>(m_pm.FindControl("HT_addTC_NAMEEdit"));
		pLX->SetText("");
		pName->SetText("");
		pObj++;
		int iResult = pObj->as<int>();

		pObj++;
		if (iResult== 0)
		{
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			CListUI* pLst=static_cast<CListUIEx*>(m_pm.FindControl(_T("TCaddlist")));
			
			
			for (int t = 0; t< i; t++)
			{		
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string>();
				
				pItem ++;
				string  strtcmc = pItem->as<string >();
				pItem++;
				string strtcfl = pItem->as<string>();
				pItem++;
				string strxgsj = pItem->as<string>();
				
			
				CListTextElementUI* pEle = new CListTextElementUI;
				pLst->Add(pEle);
				pEle->SetText(0, id.c_str());
				pEle->SetText(1, strtcmc.c_str());
				pEle->SetText(2, strtcfl.c_str());	
				pEle->SetText(3, strxgsj.c_str());
			}
			pLst->SetPos(pLst->GetPos());
			pLst->EndDown();
			CEditUI* pName= static_cast<CEditUI*>(m_pm.FindControl("HT_jl_name_edit"));
			CEditUI* pLxfs= static_cast<CEditUI*>(m_pm.FindControl("HT_jl_lxfs_edit"));
			CEditUI* pRemark= static_cast<CEditUI*>(m_pm.FindControl("HT_jl_remark"));
			pName->SetText("");
			pLxfs->SetText("");
			pRemark->SetText("");
			pName->SetFocus();
			g_SockBuss.SendLltcQueryAll(0);
		}
		else
		{
			MyMessageBox(*this,"添加失败，检查是否已经有该流量套餐名");
		}
		
	}
	return 0;
}

LRESULT CMainPage::OnTFJMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		else if (nResult == 0)
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
		CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("CX_cardTFJlist")));
		
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

LRESULT CMainPage::OnUpdateStateMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		CListUI* pCardList =static_cast<CListUI*>(m_pm.FindControl(_T("CX_cardTFJlist")));

		

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

LRESULT CMainPage::OnImportCardMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainPage::OnUserMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj = (object*)lParam;
 	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	if (nsubcmd == USER_ADD )
	{
		pObj++;
		int nResult = pObj->as<int>();
		
		if (nResult == 0)
		{
			MyMessageBox(0,"添加成功！");
		}
		else
		{
			MyMessageBox(0,"添加失败，请检查是否用户名已经存在或者客户名是否存在！");
		}
		
	}
	else if (nsubcmd == USER_LIST)
	{
		//[nCmd,nSubCmd,nreq,nTag,0/1,ncount,[[id,User,Password,Authority,Usertype,Xgsj]]]
		pObj++;
		int nReq = pObj->as<int>();
		pObj++;
		int nTag = pObj->as<int>();
		pObj++;
		int nResult = pObj->as<int>();
		pObj++;
	
		if (nResult== 0)
		{
			int nCount = pObj->as<int>();
			pObj++;
			CListUI* pLst=static_cast<CListUIEx*>(m_pm.FindControl(_T("YH_usermanalist")));
			pLst->RemoveAll();
			object* parray = pObj->via.array.ptr; 
			int i = pObj->via.array.size;

			for (int t = 0; t< i; t++)
			{
				object* pItem = (parray++)->via.array.ptr;
				string id = pItem->as<string >();
				pItem++;
				string User = pItem->as<string >();
				pItem++;
				string Password = pItem->as<string >();
				pItem++;
				string Authority = pItem->as<string >();
				string strAuth="无停机权限";
				if (Authority == "1")
					strAuth ="有停机权限";
					pItem++;
				string Usertype = pItem->as<string >();
					pItem++;
				string Xgsj = pItem->as<string >();
				CListTextElementUI* pEle = new CListTextElementUI;
				pLst->Add(pEle);
				pEle->SetText(0,id.c_str());
				pEle->SetText(1,User.c_str());
				pEle->SetText(2,Password.c_str());
				pEle->SetText(3,strAuth.c_str());
				string strType;
				if (Usertype=="1")
				{
					strType = "超级管理员";
					strAuth ="有停机权限";
				}
				else
				 strType = "代理商";
				pEle->SetText(4,strType.c_str());
				pEle->SetText(5,Xgsj.c_str());
			}
		}

	}
	return 0;
}
LRESULT CMainPage::OnCardMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	object* pObj = (object*)lParam;
	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	if (nsubcmd == SIM_LIST )
	{
		pObj++;
		int nreq = pObj->as<int>();
		CListUIEx* pCardList =static_cast<CListUIEx*>(m_pm.FindControl(_T("CX_cardquerylist")));
		//pCardList->SetItemFixedHeight(30);
		pCardList->RemoveAll();
		pObj++;
		int nTag = pObj->as<int>();
		char sz[20];
		itoa(nTag,sz,10);
		CTextUI* pCurPage = static_cast<CTextUI*>(m_pm.FindControl("CX_cardcurrentpage"));
		string strpage ="{p}{y 20}第{c #F27D30}";
		strpage += sz;
		strpage +="{/c}页{/p}";
		pCurPage->SetText(strpage.c_str());

		CTextUI* pTotalPage = static_cast<CTextUI*>(m_pm.FindControl("CX_cardsumtxt"));
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
			{//jrhm,dxzh,khmc,jlxm,xsrq,jhrq,xfrq,xqrq,zxrq,bz,dj
				object* pItem = (parray++)->via.array.ptr;
				int  id = pItem->as<int >();
				if (t == 0)
				{
					m_stcardID.strFirst = id;
				}
				if (t=i-1)
				{
					m_stcardID.strLast = id;
				}
				pItem ++;
				string  strjrhm = pItem->as<string >();
				
				pItem++;
				string striccd = pItem->as<string>();
				pItem++;
				string strdxzh = pItem->as<string>();
				pItem++;
				string strkhmc = pItem->as<string>();
				pItem++;
				string strjlxm = pItem->as<string>();
				pItem++;
				string strzt= pItem->as<string>();
				pItem++;
				string strllcm= pItem->as<string>();
				pItem++;
				string strllclx= pItem->as<string>();
				strllcm +="(";
				strllcm += strllclx;
				strllcm += ")";

				pItem++;
				string strdj = pItem->as<string>();
				pItem++;
				//xsrq,jhrq,dqrq,zxrq
				string strxsrq = pItem->as<string>();
				pItem++;
				string strjhrq = pItem->as<string>();
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
				CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("card_list_item_column.xml"),(UINT)0));
				CCheckBoxUI* pla =static_cast<CCheckBoxUI*>(pLine->FindSubControl("cardquerycheckbox"));
				pla->SetText(strSim.c_str());
				CLabelUI* pla1 =static_cast<CLabelUI*>(pLine->FindSubControl("domain1"));
				pla1->SetText(strzt.c_str());
				CLabelUI* pla2 =static_cast<CLabelUI*>(pLine->FindSubControl("domain2"));
				pla2->SetText(striccd.c_str());
				CLabelUI* pla3 =static_cast<CLabelUI*>(pLine->FindSubControl("domain3"));
				pla3->SetText(strdxzh.c_str());
				CLabelUI* pla4 =static_cast<CLabelUI*>(pLine->FindSubControl("domain4"));
				pla4->SetText(strkhmc.c_str());
				CLabelUI* pla5 =static_cast<CLabelUI*>(pLine->FindSubControl("domain5"));
				pla5->SetText(strjlxm.c_str());
				CLabelUI* pla6 =static_cast<CLabelUI*>(pLine->FindSubControl("domain6"));
				pla6->SetText(strllcm.c_str());
				CLabelUI* pla7 =static_cast<CLabelUI*>(pLine->FindSubControl("domain7"));
				pla7->SetText(strdqrq.c_str());
				CLabelUI* pla8 =static_cast<CLabelUI*>(pLine->FindSubControl("domain8"));
				pla8->SetText(strxsrq.c_str());
				CLabelUI* pla9 =static_cast<CLabelUI*>(pLine->FindSubControl("domain9"));
				pla9->SetText(strdj.c_str());
				CLabelUI* pla10 =static_cast<CLabelUI*>(pLine->FindSubControl("domain10"));
				pla10->SetText(strjhrq.c_str());
				CLabelUI* pla11 =static_cast<CLabelUI*>(pLine->FindSubControl("domain11"));
				pla11->SetText(strzxrq.c_str());
				CLabelUI* pla12 =static_cast<CLabelUI*>(pLine->FindSubControl("domain12"));
				pla12->SetText(strbz.c_str());
				if( pLine != NULL ) 
				{			

					pCardList->InsertItem(pCardList->GetCount(), 30, pLine);
				}
				
			
			/*	pEle->SetText(0, strSim.c_str());
				pEle->SetText(1, "");
				pEle->SetText(2, striccd.c_str());
				pEle->SetText(3, strdxzh.c_str());
				pEle->SetText(4, strkhmc.c_str());
				pEle->SetText(5, strkhmc.c_str());*/
				
			}
		}
	}
	else if ( nsubcmd==SIM_JRHM)
	{
		//[nCmd,nSubCmd,0/1,[id,Jrhm,Iccid,Dxzh,Llchm,Llclx,Xsrq,Jhrq,Xfrq,Dqrq,Zxrq,Bz]]
		CListUIEx* pCardList =static_cast<CListUIEx*>(m_pm.FindControl(_T("CX_cardquerylist")));
		//pCardList->SetItemFixedHeight(30);
		pCardList->RemoveAll();
		
		pObj++;
		int nResult= pObj->as<int>();
	    pObj++;
		if (nResult== 0)
		{
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;
			
			for (int t = 0; t< i; t++)
			{//id,Jrhm,Iccid,Dxzh,Khmc,Jlxm,Zt,Llchm,Llclx,Dj,Jhrq,Xfrq,Dqrq,Zxrq,Bz
				object* pItem = (parray++)->via.array.ptr;
				string  id = pItem->as<string >();
			
				pItem ++;
				string  strjrhm = pItem->as<string >();
				
				pItem++;
				string striccd = pItem->as<string>();
				
				pItem++;
				string strdxzh = pItem->as<string>();
			
				pItem++;
				string strkh = pItem->as<string>();
				pItem++;
				string strjlxm = pItem->as<string>();
				pItem++;
				string strzt = pItem->as<string>();
				pItem++;
				string strllcm= pItem->as<string>();
				pItem++;
				string strllclx= pItem->as<string>();
				strllcm +="(";
				strllcm += strllclx;
				strllcm += ")";			
				pItem++;
				//xsrq,jhrq,dqrq,zxrq
				string strdj = pItem->as<string>();
				
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
			//id,Jrhm,Iccid,Dxzh,Khmc,Jlxm,Zt,Llchm,Llclx,Dj,Jhrq,Xfrq,Dqrq,Zxrq,Bz
				string strSim = strjrhm;
				string strstate;
				string strtest ="test";
				char szNum[16];
				CDialogBuilder builder;
				CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("card_list_item_column.xml"),(UINT)0));
				CCheckBoxUI* pla =static_cast<CCheckBoxUI*>(pLine->FindSubControl("cardquerycheckbox"));
				pla->SetText(strjrhm.c_str());
				CLabelUI* pla1 =static_cast<CLabelUI*>(pLine->FindSubControl("domain1"));
				pla1->SetText(striccd.c_str());
				CLabelUI* pla2 =static_cast<CLabelUI*>(pLine->FindSubControl("domain2"));
				pla2->SetText(strdxzh.c_str());
				CLabelUI* pla3 =static_cast<CLabelUI*>(pLine->FindSubControl("domain3"));
				pla3->SetText(strkh.c_str());
				CLabelUI* pla4 =static_cast<CLabelUI*>(pLine->FindSubControl("domain4"));
				pla4->SetText(strjlxm.c_str());
				CLabelUI* pla5 =static_cast<CLabelUI*>(pLine->FindSubControl("domain5"));
				pla5->SetText(strzt.c_str());
				CLabelUI* pla6 =static_cast<CLabelUI*>(pLine->FindSubControl("domain6"));
				pla6->SetText(strllcm.c_str());
				CLabelUI* pla7 =static_cast<CLabelUI*>(pLine->FindSubControl("domain7"));
				pla7->SetText(strdj.c_str());
				CLabelUI* pla8 =static_cast<CLabelUI*>(pLine->FindSubControl("domain8"));
				pla8->SetText(strxsrq.c_str());
				CLabelUI* pla9 =static_cast<CLabelUI*>(pLine->FindSubControl("domain9"));
				pla9->SetText(strjhrq.c_str());
			
				CLabelUI* pla10 =static_cast<CLabelUI*>(pLine->FindSubControl("domain10"));
				pla10->SetText(strxfrq.c_str());
				CLabelUI* pla11 =static_cast<CLabelUI*>(pLine->FindSubControl("domain11"));
				pla11->SetText(strdqrq.c_str());
				CLabelUI* pla12=static_cast<CLabelUI*>(pLine->FindSubControl("domain12"));
				pla12->SetText(strzxrq.c_str());
				CLabelUI* pla13=static_cast<CLabelUI*>(pLine->FindSubControl("domain13"));
				pla13->SetText(strbz.c_str());
				
				if( pLine != NULL ) 
				{			

					pCardList->InsertItem(pCardList->GetCount(), 30, pLine);
				}			
				
			}
		}
	}
	else if ( nsubcmd==SIM_ADD)
	{
		pObj++;
		int nResult= pObj->as<int>();
		if (nResult == 0)
		{
			CEditUI* pEtsim = static_cast<CEditUI*>(m_pm.FindControl("HT_savecard_qishisim_et"));
			CEditUI* pEticcid = static_cast<CEditUI*>(m_pm.FindControl("HT_savecard_qishiiccid_et"));
			CComboUI* pDXZH = static_cast<CComboUI*>(m_pm.FindControl("HT_addcard_dxzh_Edit"));
			CComboUI* pSSDQ = static_cast<CComboUI*>(m_pm.FindControl("HT_addcard_ssdq_com"));
			//CComboUI* pDXZH = static_cast<CComboUI*>(pm.FindControl("HT_addcard_dxzh_Edit"));
			//CComboUI* pSSDQ = static_cast<CComboUI*>(pm.FindControl("HT_addcard_ssdq_com"));
			CComboUI* pkhjl = static_cast<CComboUI*>(m_pm.FindControl("HT_addcard_jl_com"));
			CComboUI* plltc = static_cast<CComboUI*>(m_pm.FindControl("HT_addcard_lltc_com"));
			CComboUI* pllc = static_cast<CComboUI*>(m_pm.FindControl("HT_addcard_llc_Edit"));
			CComboUI* pzt = static_cast<CComboUI*>(m_pm.FindControl("HT_addcard_zt_com"));
			CEditUI* pKehu = static_cast<CEditUI*>(m_pm.FindControl("HT_addcard_kh_edit"));
			CDateTimeUI* pxsrq = static_cast<CDateTimeUI*>(m_pm.FindControl("HT_addcard_xsrq"));
			CDateTimeUI* pdqrq = static_cast<CDateTimeUI*>(m_pm.FindControl("HT_addcard_dqrq"));
			CDateTimeUI* pxfrq = static_cast<CDateTimeUI*>(m_pm.FindControl("HT_addcard_xfrq"));
			CDateTimeUI* pzxrq = static_cast<CDateTimeUI*>(m_pm.FindControl("HT_addcard_zxrq"));
			CDateTimeUI* pjhrq = static_cast<CDateTimeUI*>(m_pm.FindControl("HT_addcard_jhrq")); 
			CEditUI* pEtbz = static_cast<CEditUI*>(m_pm.FindControl("HT_addcard_bz_et"));
			CEditUI* pEtdj = static_cast<CEditUI*>(m_pm.FindControl("HT_addcard_dj_et"));
			string strSim =pEtsim->GetText();
			string striccid = pEticcid->GetText();
			string strDx = pDXZH->GetText();
			string  strDq = pSSDQ->GetText();
			string strjl = pkhjl->GetText();
			string strtc = plltc->GetText();
			string strllc = pllc->GetText();
			string strxsrq = pxsrq->GetText();
			string strdqrq = pdqrq->GetText();
			string strxfrq = pxfrq->GetText();
			string strzxrq = pzxrq->GetText();
			string strjhrq = pjhrq->GetText();
			string strzt = pzt->GetText();
			string strkehu = pKehu->GetText();
			string strbz = pEtbz->GetText();
			string strdj = pEtdj->GetText();
			CListUI* pLst= static_cast<CListUI*>(m_pm.FindControl("cardaddlist"));
			CListTextElementUI* pele = new CListTextElementUI;
			pLst->Add(pele);
			pele->SetText(0,strSim.c_str());
			pele->SetText(1,striccid.c_str());
			pele->SetText(2,strDx.c_str());
			pele->SetText(3,strkehu.c_str());
			pele->SetText(4,strjl.c_str());
			pele->SetText(5,strllc.c_str());
			pele->SetText(6,strdqrq.c_str());
			pele->SetText(7,strxsrq.c_str());
			pele->SetText(8,strdj.c_str());
			pele->SetText(9,strxfrq.c_str());
			pele->SetText(10,strjhrq.c_str());
			pele->SetText(11,strzxrq.c_str());
			pEtsim->SetText("");
			pEticcid->SetText("");
			pEtdj->SetText("");
			pKehu->SetText("");
			pkhjl->SetText("");
			pllc->SetText("");
			pdqrq->SetText("");
			pxsrq->SetText("");
			pDXZH->SetText("");
			pxfrq->SetText("");
			pjhrq->SetText("");
			pzxrq->SetText("");
			
		}
		else 
		{
			MyMessageBox(NULL,"增添卡号失败");
		}
    }
	else if ( nsubcmd==SIM_TOTAL)
	{
		pObj++;
		int nResult= pObj->as<int>();
		CListUI* plst =static_cast<CListUI*>(m_pm.FindControl("CX_SumCardquerylist"));
		plst->RemoveAll();
		  pObj++;
		if (nResult == 0)
		{
			object* parray = pObj->via.array.ptr;
			int i = pObj->via.array.size;

			for (int t = 0; t< i; t++)
			{//id,Jrhm,Iccid,Dxzh,Khmc,Jlxm,Zt,Llchm,Llclx,Dj,Jhrq,Xfrq,Dqrq,Zxrq,Bz
				object* pItem = (parray++)->via.array.ptr;
				char sz[16];
				string  strsum=itoa(pItem->as<UINT >(),sz,10);
				//总卡数量>,<已出卡数量>,<在用卡数量>,<注销卡数量>,<一个月内到期数量>,<15天内到期数量>,<一到期15天数量>,<已到期一个月数量>]]]

				pItem ++;
				string  strycsl = itoa(pItem->as<UINT >(),sz,10);
				pItem ++;
				string  strzysl = itoa(pItem->as<UINT >(),sz,10);
				pItem ++;
				string  strzxsl =itoa(pItem->as<UINT >(),sz,10);
				pItem ++;
				string  str1ydqsl = itoa(pItem->as<UINT >(),sz,10);
				pItem ++;
				string  str15tdqsl = itoa(pItem->as<UINT >(),sz,10);
				pItem ++;
				string  strydq15tsl = itoa(pItem->as<UINT >(),sz,10);
				pItem ++;
				string  strydq1ysl = itoa(pItem->as<UINT >(),sz,10);
				CListTextElementUI* pEle = new CListTextElementUI;
				plst->Add(pEle);
				pEle->SetText(0,strsum.c_str());
				pEle->SetText(1,strycsl.c_str());
				pEle->SetText(2,strzysl.c_str());
				pEle->SetText(3,strzxsl.c_str());
				pEle->SetText(4,str1ydqsl.c_str());
				pEle->SetText(5,str15tdqsl.c_str());
				pEle->SetText(6,strydq15tsl.c_str());
				pEle->SetText(7,strydq1ysl.c_str());
			}
		}
	}
	return 1;
}