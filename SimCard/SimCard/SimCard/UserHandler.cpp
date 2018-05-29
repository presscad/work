#include "StdAfx.h"
#include "UserHandler.h"


CUserHandler::CUserHandler(void)
{
}


CUserHandler::~CUserHandler(void)
{
}

void CUserHandler::HandleClick(TNotifyUI& msg, CPaintManagerUI& pm)
{
	string strName = msg.pSender->GetName();
	if (strName == "YH_checkBtn")
	{
   		CEditUI* pet = static_cast<CEditUI*>(pm.FindControl(_T("YH_createuserKehuEdit")));
		string strKehu = pet->GetText();
		int b = 0;
	}
	else if (strName == "YH_QueryBtn")
	{
	/*	CEditUI* pet = static_cast<CEditUI*>(pm.FindControl(_T("YH_cxUserEdit")));
		string strUser = pet->GetText();
		if (strUser.empty())*/
		{
			g_SockBuss.SendGetUserList();
		}
	}

	else if (strName == "YH_createBtn")
	{
		CEditUI* pet = static_cast<CEditUI*>(pm.FindControl(_T("YH_createuserKehuEdit")));
		string strKehu = pet->GetText();
		CEditUI* pet2 = static_cast<CEditUI*>(pm.FindControl(_T("YH_createUserNameEdit")));
		string struser = pet2->GetText();
		CEditUI* pet3 = static_cast<CEditUI*>(pm.FindControl(_T("YH_createPasswordEdit")));
		string strPass =pet3->GetText();
		CComboUI* pet4 = static_cast<CComboUI*>(pm.FindControl(_T("YH_createuserTypeCom")));
		string strType =pet4->GetText();
		if (strKehu.empty())
		{
			MyMessageBox(0,"客户不能为空！");
			return;
		}
		if (struser.empty())
		{
			MyMessageBox(0,"用户名不能为空！");
			return;
		}
		if (strPass.empty())
		{
			MyMessageBox(0,"密码不能为空！");
			return;
		}
		if (strType.empty())
		{
			MyMessageBox(0,"用户类型不能为空！");
			return;
		}
		int nAuth = 0;
		 COptionUI* pCheck =static_cast<COptionUI*>(pm.FindControl(_T("tingjicheck")));
		 if (pCheck->IsSelected())
		 {
			 nAuth=1;
		 }
		int nUserType=2;
		if (strType=="超级用户")
		{
			nUserType=1;
			nAuth = 1;
		}
	/*	if (g_SockBuss.Connect() == -1)
		{
			MyMessageBox(0,"连接服务器失败！");
		}
	   
		else*/
			g_SockBuss.SendAddUser(struser,strPass,strKehu,nAuth,nUserType);
		
	}
}