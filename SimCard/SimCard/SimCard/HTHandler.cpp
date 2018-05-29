#include "StdAfx.h"
#include "HTHandler.h"
#include "commdlg.h "
#include "Excel.h"
#include "MainPage.h"
extern CMainPage * g_MainPage;
CHTHandler::CHTHandler(void)
{
}


CHTHandler::~CHTHandler(void)
{
}


void CHTHandler::HandleClick(TNotifyUI& msg, CPaintManagerUI& pm)
{
	if (msg.pSender->GetName() == "HT_openfile")
	{
		OPENFILENAME opfn;     
		CHAR strFilename[MAX_PATH];//存放文件名     
		//初始化     
		ZeroMemory(&opfn, sizeof(OPENFILENAME));     
		opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小     
		//设置过滤     
		opfn.lpstrFilter = "xlsx文件\0*.xlsx\0xls文件\0*.xls\0";     
		//默认过滤器索引设为1     
		opfn.nFilterIndex = 1;     
		//文件名的字段必须先把第一个字符设为\0
		opfn.lpstrFile = strFilename;     
		opfn.lpstrFile[0] = '\0';     
		opfn.nMaxFile = sizeof(strFilename);     
		//设置标志位，检查目录或文件是否存在     
		opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;     
		//opfn.lpstrInitialDir = NULL;     
		// 显示对话框让用户选择文件     
		if(GetOpenFileName(&opfn))     
		{
			string strfile = strFilename;
			CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_file"));
			pEtFile->SetText(strfile.c_str());
		}
	}
	else if (msg.pSender->GetName() == "HT_chucar_openfile")
	{
		OPENFILENAME opfn;     
		CHAR strFilename[MAX_PATH];//存放文件名     
		//初始化     
		ZeroMemory(&opfn, sizeof(OPENFILENAME));     
		opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小     
		//设置过滤     
		opfn.lpstrFilter = "xlsx文件\0*.xlsx\0xls文件\0*.xls\0";     
		//默认过滤器索引设为1     
		opfn.nFilterIndex = 1;     
		//文件名的字段必须先把第一个字符设为\0
		opfn.lpstrFile = strFilename;     
		opfn.lpstrFile[0] = '\0';     
		opfn.nMaxFile = sizeof(strFilename);     
		//设置标志位，检查目录或文件是否存在     
		opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;     
		//opfn.lpstrInitialDir = NULL;     
		// 显示对话框让用户选择文件     
		if(GetOpenFileName(&opfn))     
		{
			string strfile = strFilename;
			CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_CHUCAR_file"));
			pEtFile->SetText(strfile.c_str());
		}
	}
	else if (msg.pSender->GetName() == "HT_xufei_openfile")
	{
		OPENFILENAME opfn;     
		CHAR strFilename[MAX_PATH];//存放文件名     
		//初始化     
		ZeroMemory(&opfn, sizeof(OPENFILENAME));     
		opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小     
		//设置过滤     
		opfn.lpstrFilter = "xlsx文件\0*.xlsx\0xls文件\0*.xls\0";     
		//默认过滤器索引设为1     
		opfn.nFilterIndex = 1;     
		//文件名的字段必须先把第一个字符设为\0
		opfn.lpstrFile = strFilename;     
		opfn.lpstrFile[0] = '\0';     
		opfn.nMaxFile = sizeof(strFilename);     
		//设置标志位，检查目录或文件是否存在     
		opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;     
		//opfn.lpstrInitialDir = NULL;     
		// 显示对话框让用户选择文件     
		if(GetOpenFileName(&opfn))     
		{
			string strfile = strFilename;
			CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_XF_file"));
			pEtFile->SetText(strfile.c_str());
		}
	}
	else if (msg.pSender->GetName() == "HT_tuika_openfile")
	{
		OPENFILENAME opfn;     
		CHAR strFilename[MAX_PATH];//存放文件名     
		//初始化     
		ZeroMemory(&opfn, sizeof(OPENFILENAME));     
		opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小     
		//设置过滤     
		opfn.lpstrFilter = "xlsx文件\0*.xlsx\0xls文件\0*.xls\0";     
		//默认过滤器索引设为1     
		opfn.nFilterIndex = 1;     
		//文件名的字段必须先把第一个字符设为\0
		opfn.lpstrFile = strFilename;     
		opfn.lpstrFile[0] = '\0';     
		opfn.nMaxFile = sizeof(strFilename);     
		//设置标志位，检查目录或文件是否存在     
		opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;     
		//opfn.lpstrInitialDir = NULL;     
		// 显示对话框让用户选择文件     
		if(GetOpenFileName(&opfn))     
		{
			string strfile = strFilename;
			CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_tuika_file"));
			pEtFile->SetText(strfile.c_str());
		}
	}
	else if (msg.pSender->GetName() == "HT_xiaoka_openfile")
	{
		OPENFILENAME opfn;     
		CHAR strFilename[MAX_PATH];//存放文件名     
		//初始化     
		ZeroMemory(&opfn, sizeof(OPENFILENAME));     
		opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小     
		//设置过滤     
		opfn.lpstrFilter = "xlsx文件\0*.xlsx\0xls文件\0*.xls\0";     
		//默认过滤器索引设为1     
		opfn.nFilterIndex = 1;     
		//文件名的字段必须先把第一个字符设为\0
		opfn.lpstrFile = strFilename;     
		opfn.lpstrFile[0] = '\0';     
		opfn.nMaxFile = sizeof(strFilename);     
		//设置标志位，检查目录或文件是否存在     
		opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;     
		//opfn.lpstrInitialDir = NULL;     
		// 显示对话框让用户选择文件     
		if(GetOpenFileName(&opfn))     
		{
			string strfile = strFilename;
			CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_xiaoka_file"));
			pEtFile->SetText(strfile.c_str());
		}
	}
	else if (msg.pSender->GetName() == "HT_kh_openfile")
	{
		OPENFILENAME opfn;     
		CHAR strFilename[MAX_PATH];//存放文件名     
		//初始化     
		ZeroMemory(&opfn, sizeof(OPENFILENAME));     
		opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小     
		//设置过滤     
		opfn.lpstrFilter = "xlsx文件\0*.xlsx\0xls文件\0*.xls\0";     
		//默认过滤器索引设为1     
		opfn.nFilterIndex = 1;     
		//文件名的字段必须先把第一个字符设为\0
		opfn.lpstrFile = strFilename;     
		opfn.lpstrFile[0] = '\0';     
		opfn.nMaxFile = sizeof(strFilename);     
		//设置标志位，检查目录或文件是否存在     
		opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;     
		//opfn.lpstrInitialDir = NULL;     
		// 显示对话框让用户选择文件     
		if(GetOpenFileName(&opfn))     
		{
			string strfile = strFilename;
			CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_kh_file"));
			pEtFile->SetText(strfile.c_str());
		}
	}
	else if (msg.pSender->GetName() == "HT_export")
	{
		CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing1"));
		pGif->SetVisible(true);
		SetTimer(*g_MainPage, TIMER_XINCARD,100,NULL);
		//HandleExportAll(pm);
	}
	else if (msg.pSender->GetName() == "HT_chuar_export")
	{
		CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing2"));
		pGif->SetVisible(true);
		SetTimer(*g_MainPage, TIMMER_CHUCARD,100,NULL);
		
		//HandleExportAll(pm);
	}

	else if (msg.pSender->GetName() == "HT_kh_export")
	{

		CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing6"));
		pGif->SetVisible(true);
		SetTimer(*g_MainPage, TIMER_KEHU,100,NULL);
		//HandleXufeiExport(pm);
		//HandleExportAll(pm);
	}
	else if (msg.pSender->GetName() == "HT_xufei_export")
	{
		
		CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing3"));
		pGif->SetVisible(true);
		SetTimer(*g_MainPage, TIMER_XUFEI,100,NULL);
		//HandleXufeiExport(pm);
		//HandleExportAll(pm);
	}
	else if (msg.pSender->GetName() == "HT_tuika_export")
	{
		CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing4"));
		pGif->SetVisible(true);
		SetTimer(*g_MainPage, TIMER_TUICARD,100,NULL);
		//HandleExportAll(pm);
	}
	else if (msg.pSender->GetName() == "HT_xiaoka_export")
	{
		CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing5"));
		pGif->SetVisible(true);
		SetTimer(*g_MainPage, TIMER_XIAOCARD,100,NULL);
		//HandleExportAll(pm);
	}

	else if (msg.pSender->GetName() == "HT_cardsaveBtn")
	{
		HandleCardSave(pm);
	}

	else if (msg.pSender->GetName()== "HT_kehusaveBtn")
	{
		CEditUI* pName= static_cast<CEditUI*>(pm.FindControl("HT_addkehu_kehunameEdit"));
		CEditUI* pLxfs= static_cast<CEditUI*>(pm.FindControl("HT_addkehu_lxfsEdit"));
		CComboUI* pJl= static_cast<CComboUI*>(pm.FindControl("HT_addkehu_jingliCombo"));
		CEditUI* pRemark= static_cast<CEditUI*>(pm.FindControl("HT_addkehu_remarkEdit"));
		CComboUI* pSSdq =static_cast<CComboUI*>(pm.FindControl("HT_addkh_ssdq_com"));
		CEditUI* pDj= static_cast<CEditUI*>(pm.FindControl("HT_addkh_dj_et"));
		string strSsdq = pSSdq->GetText();
		string strDj = pDj->GetText();
		string strName= pName->GetText();
		string strLxfs = pLxfs->GetText();
		string strJl = pJl->GetText();
		string strRemark = pRemark->GetText();
		if (strName.empty())
		{
			MyMessageBox(NULL,"姓名不能为空");
			return;
		}
		else if (strJl.empty())
		{
			MyMessageBox(NULL,"经理不能为空");
			return;
		}
	/*	if (g_SockBuss.Connect() == -1)
		{
			MyMessageBox(NULL,"连接服务器失败");
			return;
		}*/
		g_SockBuss.SendAddKehu(strName,strLxfs,strJl,atof(strDj.c_str()),strSsdq,strRemark);
	}
	else if (msg.pSender->GetName() == "HT_jinglisaveBtn")
	{
		CEditUI* pName= static_cast<CEditUI*>(pm.FindControl("HT_jl_name_edit"));
		CEditUI* pLxfs= static_cast<CEditUI*>(pm.FindControl("HT_jl_lxfs_edit"));
		CEditUI* pRemark= static_cast<CEditUI*>(pm.FindControl("HT_jl_remark"));
		
		string strName= pName->GetText();
		string strLxfs = pLxfs->GetText();
		string strRemark = pRemark->GetText();
		
		if (strName.empty())
		{
			MyMessageBox(NULL,"经理名字不能为空");
			return;
		}
		//if (g_SockBuss.Connect() == -1)
		//{
		//	MyMessageBox(NULL,"连接服务器失败");
		//}
		//else
		{
			g_SockBuss.SendAddJingli(strName, strLxfs, strRemark);
		}
	}

	else if (msg.pSender->GetName() == "HT_dianxinusrsaveBtn")
	{
		CEditUI* pName= static_cast<CEditUI*>(pm.FindControl("HT_addDX_nameEdit"));
		CEditUI* pID= static_cast<CEditUI*>(pm.FindControl("HT_addDX_idEdit"));
		CEditUI* pPass= static_cast<CEditUI*>(pm.FindControl("HT_addDX_passEdit"));
		CEditUI* pMiyao= static_cast<CEditUI*>(pm.FindControl("HT_addDX_miyaoEdit"));
		CEditUI* pRemark= static_cast<CEditUI*>(pm.FindControl("HT_addDX_remarkEdit"));
		string strName= pName->GetText();
		string strID = pID->GetText();
		string strPass = pPass->GetText();
		string strMiyao = pMiyao->GetText();
		string strRemark = pRemark->GetText();
		if (strName.empty())
		{
			MyMessageBox(NULL,"名字不能为空");
			return;
		}
		if (strID.empty())
		{
			MyMessageBox(NULL,"id不能为空");
			return;
		}
		if (strPass.empty())
		{
			MyMessageBox(NULL,"密码不能为空");
			return;
		}
		if (strMiyao.empty())
		{
			MyMessageBox(NULL,"密钥不能为空");
			return;
		}
		//if (g_SockBuss.Connect() == -1)
		//{
		//	MyMessageBox(NULL,"连接服务器失败");
		//}
		//else
		{
			g_SockBuss.SendAddDianXin(strName, strID, strPass,strMiyao,strRemark);
		}
	}

	else if (msg.pSender->GetName() == "HT_liulingchisaveBtn")
	{
		CEditUI* pLX= static_cast<CEditUI*>(pm.FindControl("HT_addLLC_lxComb"));
		CEditUI* pID= static_cast<CEditUI*>(pm.FindControl("HT_addLLC_nameEdit"));
		CEditUI* pDX= static_cast<CEditUI*>(pm.FindControl("HT_addLLC_dxzhComb"));
		CEditUI* pBZ= static_cast<CEditUI*>(pm.FindControl("HT_addLLC_bzEdit"));
		
		string strLX= pLX->GetText();
		string strID = pID->GetText();
		string strDX = pDX->GetText();
		string strBZ = pBZ->GetText();
	
		if (strLX.empty())
		{
			MyMessageBox(NULL,"流量池类型不能为空");
			return;
		}
		if (strID.empty())
		{
			MyMessageBox(NULL,"流量池编号不能为空");
			return;
		}
		if (strDX.empty())
		{
			MyMessageBox(NULL,"电信账户不能为空");
			return;
		}

	/*	if (g_SockBuss.Connect() == -1)
		{
			MyMessageBox(NULL,"连接服务器失败");
		}
		else*/
		{
			g_SockBuss.SendAddLlc(strID,strLX, strDX,strBZ);
		}
	}
	else if (msg.pSender->GetName() == "HT_areasaveBtn")
	{
		CEditUI* pLX= static_cast<CEditUI*>(pm.FindControl("HT_addareaEdit"));
		string strArea=pLX->GetText();
		if (strArea.empty())
		{
			MyMessageBox(NULL,"地区不能为空");
			return;
		}
		//if (g_SockBuss.Connect() == -1)
		//{
		//	MyMessageBox(NULL,"连接服务器失败");
		//}
		else
		{
			g_SockBuss.SendAddSsdq(strArea);
		}
	}
	else if (msg.pSender->GetName() == "HT_TCsaveBtn")
	{
		CEditUI* pLX= static_cast<CEditUI*>(pm.FindControl("HT_addTC_LXEdit"));
		CEditUI* pName= static_cast<CEditUI*>(pm.FindControl("HT_addTC_NAMEEdit"));
		string strLx=pLX->GetText();
		string strName = pName->GetText();

		if (strLx.empty())
		{
			MyMessageBox(NULL,"套餐类型不能为空");
			return;
		}
		if (strName.empty())
		{
			MyMessageBox(NULL,"套餐名字不能为空");
			return;
		}
		if (g_SockBuss.Connect() == -1)
		{
			MyMessageBox(NULL,"连接服务器失败");
		}
		else
		{
			g_SockBuss.SendAddLLTC(strName, strLx);
		}
	}
}

void CHTHandler::HandleTuiCarExport(CPaintManagerUI& pm)
{
	CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_tuika_file"));
	string strFile = pEtFile->GetText();
	if (strFile.empty())
	{
		MyMessageBox(NULL,"文件不能为空！","导入错误");
		return;
	}
	HMODULE hDllLib = LoadLibrary(_T("ReadExcel.dll"));
	if (hDllLib)
	{
		//获取动态连接库里的函数地址。
		 ReadTuikaProc fpFun =0;
		 CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing4"));
		 pGif->SetVisible(true);
		fpFun = (ReadTuikaProc) GetProcAddress(hDllLib,"ReadTuika");

		//调用函数运行。
		if (fpFun)
		{
			vector<ST_TUIKA> veccard;

			bool bSucc = (fpFun)((void*)&veccard,(char*)strFile.c_str(),strFile.length());
			
			/*if (g_SockBuss.Connect() != -1)*/
			{
				
				int t = veccard.size()/200;
				for(int i = 0; i<=t; i++)
				{
					vector<ST_TUIKA> vec200;
					if (i< t)
					{
						vec200.assign(veccard.begin()+ (i*200),veccard.begin()+((i+1)*200));
					}
					else
					{
						vec200.assign(veccard.begin()+t*200,veccard.end() );
					}

					/*if (g_SockBuss.Connect() != -1)*/
						g_SockBuss.SendImportTuiCard(vec200);
				}
				

			}
		}
		FreeLibrary(hDllLib);
		pGif->SetVisible(false);

	}
}

void CHTHandler::HandleKehuExport(CPaintManagerUI& pm)
{
	CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_kh_file"));
	string strFile = pEtFile->GetText();
	if (strFile.empty())
	{
		MyMessageBox(NULL,"文件不能为空！","导入错误");
		return;
	}
	HMODULE hDllLib = LoadLibrary(_T("ReadExcel.dll"));
	if (hDllLib)
	{
		//获取动态连接库里的函数地址。
		CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing6"));
		pGif->SetVisible(true);
		ReadXiaokaProc fpFun =0;
		fpFun = (ReadCardProc) GetProcAddress(hDllLib,"ReadKehu");

		//调用函数运行。
		if (fpFun)
		{
			vector<ST_KEHU> veckehu;

			bool bSucc = (fpFun)((void*)&veckehu,(char*)strFile.c_str(),strFile.length());

			/*if (g_SockBuss.Connect() != -1)*/
			{

				int t = veckehu.size();
				for(int i = 0; i<t; i++)
				{					
					Sleep(50);
					g_SockBuss.SendAddKehu(veckehu[i].strName,veckehu[i].strllfs,veckehu[i].strjl,atof(veckehu[i].strdj.c_str()),veckehu[i].strssdq,veckehu[i].strbz);
				}


			}

		}
		FreeLibrary(hDllLib);
		pGif->SetVisible(false);

	}
}
void CHTHandler::HandleXiaoCarExport(CPaintManagerUI& pm)
{
	CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_xiaoka_file"));
	string strFile = pEtFile->GetText();
	if (strFile.empty())
	{
		MyMessageBox(NULL,"文件不能为空！","导入错误");
		return;
	}
	HMODULE hDllLib = LoadLibrary(_T("ReadExcel.dll"));
	if (hDllLib)
	{
		//获取动态连接库里的函数地址。
		CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing5"));
		pGif->SetVisible(true);
		ReadXiaokaProc fpFun =0;
		fpFun = (ReadXiaokaProc) GetProcAddress(hDllLib,"ReadXiaoka");

		//调用函数运行。
		if (fpFun)
		{
			vector<ST_XIAOKA> veccard;
			
			bool bSucc = (fpFun)((void*)&veccard,(char*)strFile.c_str(),strFile.length());

			/*if (g_SockBuss.Connect() != -1)*/
			{
				
				int t = veccard.size()/200;
				for(int i = 0; i<=t; i++)
				{
					vector<ST_XIAOKA> vec200;
					if (i< t)
					{
						vec200.assign(veccard.begin()+ (i*200),veccard.begin()+((i+1)*200));
					}
					else
					{
						vec200.assign(veccard.begin()+t*200,veccard.end() );
					}
/*
					if (g_SockBuss.Connect() != -1)*/
						g_SockBuss.SendImportXiaoCard(vec200);
				}
				

			}
		
		}
		FreeLibrary(hDllLib);
	   pGif->SetVisible(false);

	}
}
void CHTHandler::HandleKaiCarExport(CPaintManagerUI& pm)
{
	CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_file"));
	string strFile = pEtFile->GetText();
	if (strFile.empty())
	{
		MyMessageBox(NULL,"文件不能为空！","导入错误");
		return;
	}
	HMODULE hDllLib = LoadLibrary(_T("ReadExcel.dll"));
	if (hDllLib)
	{
		//获取动态连接库里的函数地址。
		/*CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing1"));
		pGif->SetVisible(true);*/
		ReadCardProc fpFun =0;
		fpFun = (ReadCardProc) GetProcAddress(hDllLib,"ReadCard");

		//调用函数运行。
		if (fpFun)
		{
			vector<ST_CARD> veccard;

			bool bSucc = (fpFun)((void*)&veccard,(char*)strFile.c_str(),strFile.length());
			strFile = strFile.substr(0,strFile.find("."));
			vector<string>vec= split(strFile,"_");
			if (vec.size()!= 3)
			{
				MyMessageBox(NULL,"文件名格式不对");
				return;
			}
			/*if (g_SockBuss.Connect() != -1)*/
			{
				
				int t = veccard.size()/200;
				for(int i = 0; i<=t; i++)
				{
					vector<ST_CARD> vec200;
					if (i< t)
					{
						vec200.assign(veccard.begin()+ (i*200),veccard.begin()+((i+1)*200));
					}
					else
					{
						vec200.assign(veccard.begin()+t*200,veccard.end() );
					}
					
					/*if (g_SockBuss.Connect() != -1)*/
						g_SockBuss.SendImportNewCard(vec200,vec[1],vec[2]);
				}
				

			}
			
		}
		FreeLibrary(hDllLib);
		//

	}

}
void CHTHandler::HandleExportAll(CPaintManagerUI& pm)
{
	CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_file"));
	string strFile = pEtFile->GetText();
	if (strFile.empty())
	{
		MyMessageBox(NULL,"文件不能为空！","导入错误");
		return;
	}
	HMODULE hDllLib = LoadLibrary(_T("ReadExcel.dll"));
	if (hDllLib)
	{
		//获取动态连接库里的函数地址。
	 ReadAllProc fpReadall = 0;
	 fpReadall = (ReadAllProc) GetProcAddress(hDllLib,"ReadAll");
	 if (fpReadall)
	 {
		 vector<ST_KAICARD> veck;
		 vector<ST_CHUCARD> vecc;
		 vector<ST_XUFEI>vecx;
		 
		 (fpReadall)((void*)&veck,(void*)&vecc,(void*)&vecx,(char*)strFile.c_str(),strFile.length());
		 int c = 0;
	 }
	 FreeLibrary(hDllLib);
	}
}

void CHTHandler::HandleChuCarExport(CPaintManagerUI& pm)
{

	CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_CHUCAR_file"));
	string strFile = pEtFile->GetText();
	if (strFile.empty())
	{
		MyMessageBox(NULL,"文件不能为空！","导入错误");
		return;
	}
	HMODULE hDllLib = LoadLibrary(_T("ReadExcel.dll"));
	if (hDllLib)
	{
		//获取动态连接库里的函数地址。
		ReadChukaProc fpReadChucar = 0;
		CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing2"));
		pGif->SetVisible(true);
		fpReadChucar = (ReadChukaProc) GetProcAddress(hDllLib,"ReadChuka");
		if (fpReadChucar)
		{
			
			vector<ST_CHUCARD> veccard;			
	
			(fpReadChucar)((void*)&veccard,(char*)strFile.c_str(),strFile.length());
			int t = veccard.size()/200;
			
			for(int i = 0; i<=t; i++)
			{
				vector<ST_CHUCARD> vec200;
				if (i< t)
				{
					vec200.assign(veccard.begin()+ (i*200),veccard.begin()+((i+1)*200));
				}
				else
				{
					vec200.assign(veccard.begin()+t*200,veccard.end() );
				}

				/*if (g_SockBuss.Connect() != -1)*/
					g_SockBuss.SendImportChuCard(vec200);
			}
			
			int c = 0;
		}
		pGif->SetVisible(false);
		FreeLibrary(hDllLib);
	}
}

void CHTHandler::HandleXufeiExport(CPaintManagerUI& pm)
{

 	CEditUI* pEtFile = static_cast<CEditUI*>(pm.FindControl("HT_ET_XF_file"));
	string strFile = pEtFile->GetText();
	if (strFile.empty())
	{
		MyMessageBox(NULL,"文件不能为空！","导入错误");
		return;
	}
	HMODULE hDllLib = LoadLibrary(_T("ReadExcel.dll"));
	if (hDllLib)
	{
		//获取动态连接库里的函数地址。
		CGifAnimUI* pGif =static_cast<CGifAnimUI*>(pm.FindControl("HT_showing3"));
		pGif->SetVisible(true);
		ReadChukaProc fpReadXucar = 0;
		fpReadXucar = (ReadChukaProc) GetProcAddress(hDllLib,"ReadXuka");
		if (fpReadXucar)
		{

			vector<ST_XUFEI> veccard;			
		
			(fpReadXucar)((void*)&veccard,(char*)strFile.c_str(),strFile.length());
			int t = veccard.size()/200;
			for(int i = 0; i<=t; i++)
			{
				vector<ST_XUFEI> vec200;
				if (i< t)
				{
					vec200.assign(veccard.begin()+ (i*200),veccard.begin()+((i+1)*200));
				}
				else
				{
					vec200.assign(veccard.begin()+t*200,veccard.end() );
				}

				/*if (g_SockBuss.Connect() != -1)*/
					g_SockBuss.SendImportXuCard(vec200);
			}
		
			int c = 0;
		}
			pGif->SetVisible(false);
		FreeLibrary(hDllLib);
	}
}


void CHTHandler::HandleCardSave(CPaintManagerUI& pm)
  {
	CEditUI* pEtsim = static_cast<CEditUI*>(pm.FindControl("HT_savecard_qishisim_et"));
	CEditUI* pEticcid = static_cast<CEditUI*>(pm.FindControl("HT_savecard_qishiiccid_et"));
	CComboUI* pDXZH = static_cast<CComboUI*>(pm.FindControl("HT_addcard_dxzh_Edit"));
	CComboUI* pSSDQ = static_cast<CComboUI*>(pm.FindControl("HT_addcard_ssdq_com"));
	//CComboUI* pDXZH = static_cast<CComboUI*>(pm.FindControl("HT_addcard_dxzh_Edit"));
	//CComboUI* pSSDQ = static_cast<CComboUI*>(pm.FindControl("HT_addcard_ssdq_com"));
	CComboUI* pkhjl = static_cast<CComboUI*>(pm.FindControl("HT_addcard_jl_com"));
	CComboUI* plltc = static_cast<CComboUI*>(pm.FindControl("HT_addcard_lltc_com"));
	CComboUI* pllc = static_cast<CComboUI*>(pm.FindControl("HT_addcard_llc_Edit"));
    CComboUI* pzt = static_cast<CComboUI*>(pm.FindControl("HT_addcard_zt_com"));
    CEditUI* pKehu = static_cast<CEditUI*>(pm.FindControl("HT_addcard_kh_edit"));
	CDateTimeUI* pxsrq = static_cast<CDateTimeUI*>(pm.FindControl("HT_addcard_xsrq"));
	CDateTimeUI* pdqrq = static_cast<CDateTimeUI*>(pm.FindControl("HT_addcard_dqrq"));
	CDateTimeUI* pxfrq = static_cast<CDateTimeUI*>(pm.FindControl("HT_addcard_xfrq"));
	CDateTimeUI* pzxrq = static_cast<CDateTimeUI*>(pm.FindControl("HT_addcard_zxrq"));
	CDateTimeUI* pjhrq = static_cast<CDateTimeUI*>(pm.FindControl("HT_addcard_jhrq")); 
	CEditUI* pEtbz = static_cast<CEditUI*>(pm.FindControl("HT_addcard_bz_et"));
	CEditUI* pEtdj = static_cast<CEditUI*>(pm.FindControl("HT_addcard_dj_et"));
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
	
	//string jrhm,string iccid,string jlxm,string lltc,string dxzh,string jhrq,string zt,string ssdq,string khmc,string llchm,string xsrq,string dqrq,string xfrq,string zxrq,string bz,string dj
	/*if (g_SockBuss.Connect() != -1)*/
		g_SockBuss.SendSaveCard(strSim, striccid,strjl, strllc, strDx, strjhrq,strzt,strDq, strkehu,strllc,strxsrq,strdqrq, strxfrq,strzxrq,strbz,strdj);
	/*else
		MyMessageBox(NULL,"连接服务器失败！");*/
}