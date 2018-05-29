// SimCard.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SimCard.h"
#include "LoginDlg.h"
#include <WinSock2.h>
#include "Excel.h"
#include <shellapi.h>
#include <shlwapi.h>
//struct ST_CARD
//{
//	std::string strCardNum;
//	std::string strJieruNUm;
//	std::string strCompany;
//
//};
//typedef bool (* ReadCardProc)(void*,char*,int);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = _T("update.exe");
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;	
	ShellExecuteEx(&ShExecInfo);
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	/*CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\xml"));*/
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() );
	CPaintManagerUI::SetResourceZip(_T("skin.zip"));
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	CLoginDlg* pFrame = new CLoginDlg();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("Login"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW , 0, 0, 600, 320);
	HICON hIcon = ::LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));  
	::SendMessage(pFrame->GetHWND(),STM_SETICON,IMAGE_ICON,(LPARAM)(UINT)hIcon);
	pFrame->CenterWindow();	
	::ShowWindow(*pFrame, SW_SHOW);

 //HMODULE hDllLib = LoadLibrary(_T("ReadExcel.dll"));
  //if (hDllLib)
 //{
    //获取动态连接库里的函数地址。
 //  ReadCardProc fpFun =0;
 //  fpFun = (ReadCardProc) GetProcAddress(hDllLib,"ReadCard");

 ////调用函数运行。
 //  if (fpFun)
 //  {
	//   vector<ST_CARD> veccard;
	//   string strfile = "c:\\流量池_2100001122097_后向.xlsx";
	//   bool bSucc = (fpFun)((void*)&veccard,(char*)strfile.c_str(),strfile.length());
	//   int b = 0;
	//   FreeLibrary(hDllLib);
	//   if (bSucc&& veccard.size() > 0)
	//   {
	//	   strfile = strfile.substr(0,strfile.find("."));
	//	   vector<string>vec= split(strfile,"_");
	//	   if (vec.size()!= 3)
	//	   {
	//		   MyMessageBox(NULL,"文件名格式不对");
	//	   }
	//   }

	 
	/* ReadAllProc fpReadall = 0;
	 fpReadall = (ReadAllProc) GetProcAddress(hDllLib,"ReadAll");
	 if (fpReadall)
	 {
		 vector<ST_KAICARD> veck;
		 vector<ST_CHUCARD> vecc;
		 vector<ST_XUFEI>vecx;
		 string strFile = "c:\\朱工.xlsx";
		 (fpReadall)((void*)&veck,(void*)&vecc,(void*)&vecx,(char*)strFile.c_str(),strFile.length());
		 int c = 0;
	 }*/
  // }
 
  
  

	CPaintManagerUI::MessageLoop();
	  
	return 0;
}