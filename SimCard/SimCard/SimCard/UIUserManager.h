#pragma once
#include "stdafx.h"



class CUIUserManager : public CContainerUI
{
public:
	CUIUserManager()
	{
		CDialogBuilder builder;
		CContainerUI* pUI = static_cast<CContainerUI*>(builder.Create(_T("用户.xml"), (UINT)0));
		if( pUI ) {
			this->Add(pUI);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};


class CUIQueryManager : public CContainerUI
{
public:
	CUIQueryManager()
	{
		CDialogBuilder builder;
		CContainerUI* pUI = static_cast<CContainerUI*>(builder.Create(_T("UIQueryManager.xml"), (UINT)0));
		if( pUI ) {
			this->Add(pUI);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};


class CUIBackImport : public CContainerUI
{
public:
	CUIBackImport()
	{
		CDialogBuilder builder;
		CContainerUI* pUI = static_cast<CContainerUI*>(builder.Create(_T("后台导入.xml"), (UINT)0));
		if( pUI ) {
			this->Add(pUI);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};
class CUIOrderManager : public CContainerUI
{
public:
	CUIOrderManager()
	{
		CDialogBuilder builder;
		CContainerUI* pUI = static_cast<CContainerUI*>(builder.Create(_T("订单管理.xml"), (UINT)0));
		if( pUI ) {
			this->Add(pUI);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};
class CUIOperateManager : public CContainerUI
{
public:
	CUIOperateManager()
	{
		CDialogBuilder builder;
		CContainerUI* pUI = static_cast<CContainerUI*>(builder.Create(_T("操作管理.xml"), (UINT)0));
		if( pUI ) {
			this->Add(pUI);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};



class CUIBackManager : public CContainerUI
{
public:
	CUIBackManager()
	{
		CDialogBuilder builder;
		
		CContainerUI* pUI = static_cast<CContainerUI*>(builder.Create(_T("后台管理.xml"), (UINT)0));
		if( pUI ) {
			this->Add(pUI);
		}
		else {
			this->RemoveAll();
			return;
		}
	}

};
class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
	CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if( _tcscmp(pstrClass, _T("UIUserManager")) == 0 ) return new CUIUserManager;
		else if( _tcscmp(pstrClass, _T("UIQueryManager")) == 0 ) return new CUIQueryManager;
		else if( _tcscmp(pstrClass, _T("UIBackManager")) == 0 ) return new CUIBackManager;
		else if( _tcscmp(pstrClass, _T("UIOrderManager")) == 0 ) return new CUIOrderManager;
		else if( _tcscmp(pstrClass, _T("UIOperateManager")) == 0 ) return new CUIOperateManager;
		else if( _tcscmp(pstrClass, _T("UIOperateImport")) == 0 ) return new CUIBackImport;
		else if( _tcscmp(pstrClass, _T("NumberEdit")) == 0 ) return new CNumberEditUI;
		return NULL;
	}
};

//class CDialogBuilderCallbackEx : public IDialogBuilderCallback
//{
//public:
//	CControlUI* CreateControl(LPCTSTR pstrClass) 
//	{
//		 if( _tcscmp(pstrClass, _T("UIQueryManager")) == 0 ) return new CUIQueryManager;
//		return NULL;
//	}
//};