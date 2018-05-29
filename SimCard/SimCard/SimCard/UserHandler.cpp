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
			MyMessageBox(0,"�ͻ�����Ϊ�գ�");
			return;
		}
		if (struser.empty())
		{
			MyMessageBox(0,"�û�������Ϊ�գ�");
			return;
		}
		if (strPass.empty())
		{
			MyMessageBox(0,"���벻��Ϊ�գ�");
			return;
		}
		if (strType.empty())
		{
			MyMessageBox(0,"�û����Ͳ���Ϊ�գ�");
			return;
		}
		int nAuth = 0;
		 COptionUI* pCheck =static_cast<COptionUI*>(pm.FindControl(_T("tingjicheck")));
		 if (pCheck->IsSelected())
		 {
			 nAuth=1;
		 }
		int nUserType=2;
		if (strType=="�����û�")
		{
			nUserType=1;
			nAuth = 1;
		}
	/*	if (g_SockBuss.Connect() == -1)
		{
			MyMessageBox(0,"���ӷ�����ʧ�ܣ�");
		}
	   
		else*/
			g_SockBuss.SendAddUser(struser,strPass,strKehu,nAuth,nUserType);
		
	}
}