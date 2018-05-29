#include "StdAfx.h"
#include "SocketBussiness.h"
#include "MainPage.h"
#include "LoginDlg.h"
using namespace msgpack;
typedef int(CSocketBussiness::*ProcessFun)(msgpack::unpacked &result_);
ProcessFun Process[256]={0}; 
extern CSocketBussiness g_SockBuss;
extern CMainPage* g_MainPage;
extern CYonghuCardPage * g_YonghuCardPage;
extern CXsrqCardPage* g_XsrqCardPage;
string g_strAuthority;//权限
string g_strUserID;
string g_strUserType;
CSocketBussiness::CSocketBussiness(void)
	: m_bIsLogin(false)
	, m_strName(_T(""))
	, m_strPwd(_T(""))
	, m_nActive(0)
{
	Process[USER_DATA]=&CSocketBussiness::OnUser;
	Process[SIM_DATA]= &CSocketBussiness::OnSimData;
	Process[KH_DATA] = &CSocketBussiness::OnKHData;
	Process[KHJL_DATA] = &CSocketBussiness::OnKHJLData;
	Process[DXZH_DATA]= &CSocketBussiness::OnDXData;
	Process[LLC_DATA]= &CSocketBussiness::OnLLCData;
	Process[SSDQ_DATA]= &CSocketBussiness::OnSSDQData;
	Process[LLTC_DATA]= &CSocketBussiness::OnLLTCData;
	Process[DISABLE_NUMBER_DATA]= &CSocketBussiness::OnTFJCData;
	Process[CARD_STATUS_DATA]= &CSocketBussiness::OnUpdatStateData;
	Process[EXCEL_LOAD]= &CSocketBussiness::OnImportCardMsg;
}


CSocketBussiness::~CSocketBussiness(void)
{
}

// 设置用用户名跟密码
int CSocketBussiness::SetUserInfo(std::string & strName, std::string & strPwd)
{
	m_strName = strName;
	m_strPwd = strPwd;
	return 0;
}
// 连接成功后的规定动作
int CSocketBussiness::OnConnected(void)
{
	//if (m_bIsLogin)
	//	return 0;
	//SetStatus(1);
	////g_SockBuss.SendActive();
	//return Login();
	return 0;
}

byte csum(unsigned char *addr, int count)
{
	byte sum = 0;
	for (int i = 0; i< count; i++)
	{
		sum +=(byte) addr[i];
	}
	return sum;
}
int CSocketBussiness::Login(void)
{
	//[USER_DATA,USER_LOGIN,[[username,password]]]
   	if("" == m_strName || "" == m_strPwd)
		return -1;
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = USER_DATA;
	int nSubCmd = USER_LOGIN;
	string strName=this->m_strName,strPwd=m_strPwd;
	
	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(3);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	msgPack.pack_array(1);
	msgPack.pack_array(2);
	msgPack.pack(strName);
	msgPack.pack(strPwd);
	DealLast(sBuf);

	return 0;
}



int CSocketBussiness::SetStatus(int nStatus)
{
	char szText[5][13]={"连接中…","已连接","已登录","断开！","登录失败"};
	this->m_strStatu=szText[nStatus];
	//SendMessage(m_pLoginDlg->GetHWND(),UM_SOCKETMSG,SOCKETSTATUS,NULL);
	return 0;
}
int CSocketBussiness::DataProc(void)
{
	
	int nRt(0);
	if(m_nDataLen < 8)//长度太小了
		return 0;	
	int nFrameLen;
	if ((UCHAR)m_ucRecv[0]!=0xFB
		|| (UCHAR)m_ucRecv[1]!=0xFC)//没有数据开始标志	 
	{		
		memset(m_ucRecv,0,m_nDataLen);
		m_nDataLen=0;	
#ifdef _DEBUG
		OutputDebugString("没有数据开始标志\n");
#endif
		//ReConnect();
		return -1;
	}
	
	nFrameLen=*(INT*)(m_ucRecv+2);		
	if(m_nDataLen < (nFrameLen+8))			//数据长度不够	
	{
#ifdef _DEBUG
		OutputDebugString("长度不够\n");
#endif
		return 0;
	}
	byte nSum =m_ucRecv[6 + nFrameLen];//检验和
	//unsigned char szCheck[1024*10];
	//memset(szCheck,0, 1024*10);
	//memcpy(szCheck, m_ucRecv+6, nFrameLen);
	if (nSum != csum(m_ucRecv+6, nFrameLen))//检验和失败
	{
		memset(m_ucRecv,0,m_nDataLen);
		m_nDataLen=0;	
#ifdef _DEBUG
		OutputDebugString("检验和失败\n");
#endif
		//ReConnect();
		return -1;
	}
	if(0x0d != m_ucRecv[nFrameLen+7])								//结尾不是0d
	{
		//OutputDebugString("结尾不对,不是0D\n");
		memset(m_ucRecv,0,m_nDataLen);
		m_nDataLen=0;
#ifdef _DEBUG
		OutputDebugString("结尾不对,不是0D\n");
#endif
		//ReConnect();
		return -3;		
	}
	else//开始处理数据
	{
		try
		{

			msgpack::unpacker unpack_;
			msgpack::object_handle result_;
			unpack_.reserve_buffer(m_nDataLen);
			memcpy(unpack_.buffer(), m_ucRecv+6, nFrameLen);
			unpack_.buffer_consumed(nFrameLen);		
			unpack_.next(result_);
			if(msgpack::type::ARRAY != result_.get().type)
			{			
				//TRACE0("DataProc:数据包不完整\n");
				return -1;
			}
			int cmdno=result_.get().via.array.ptr->as<int>();		
   			if( cmdno>-1)
			{
                int nIndex=0;		
				if(Process[cmdno]) 
				{
					if (m_pLoginDlg)
					m_pLoginDlg->setTimeCount0();
					(this->*Process[cmdno])(result_);				
				}
				else
				{
					//TRACE1("未知命令:%d\n",cmdno);
				}
			}

		}
		catch(msgpack::type_error e)
		{
			memset(m_ucRecv,0,m_nDataLen);
			m_nDataLen=0;
#ifdef _DEBUG
			OutputDebugString("DataProc:数据类型不对\n");
#endif
			//TRACE0("DataProc:数据类型不对\n");
			//this->Close();
			return -1;
		}
		catch(msgpack::unpack_error e)
		{
			string strError=e.what();
			if("insufficient bytes"== strError)
			{
				//TRACE0("DataProc:长度不够\n");
				return 0;//长度不够
			}
			else
			{
				memset(m_ucRecv,0,m_nDataLen);
				m_nDataLen=0;
#ifdef _DEBUG
				OutputDebugString("DataProc:解析错误\n");
#endif
				//TRACE0("DataProc:解析错误1\n");
				//this->Close();
				return -1;
			}
		}

		
	}
	nFrameLen+=8;
	m_nDataLen-=nFrameLen;
	if(m_nDataLen < 0)
	{
		m_nDataLen=0;
	}
	if(m_nDataLen > 0)
	{
#ifdef _DEBUG
		OutputDebugString("DataProc:移动数据\n");
#endif
		memcpy(m_ucRecv,m_ucRecv+nFrameLen,m_nDataLen);				//移动数据		
		memset(m_ucRecv+m_nDataLen,0,nFrameLen);
	}	
	if(m_nDataLen>=8)   
		DataProc();	
	return 0;
}
int CSocketBussiness::OnLogin(msgpack::unpacked &result_)
 {
	/*m_pLoginDlg->setTimeCount0();  */
	 m_bIsLogin = true;
     msgpack::object_handle clsHandle;
    result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	int cmd = pObj->as<int>();
	pObj ++;
	int nSubCmd = pObj->as<int>();
	pObj ++;
	int  nResult = pObj->as<int>();
	if (nResult == 0)
	{

		pObj ++;
		
		object* parray = pObj->via.array.ptr;
		int i = parray->via.array.size;
		for (int  n = 0; n< i; i++)

		{
			object* pitem = (parray++)->via.array.ptr;
			g_strUserID = pitem->as<string>();
			pitem++;
			pitem++;
			g_strAuthority = pitem->as<string >();
			pitem ++;
			g_strUserType = pitem->as<string>();
			SendMessage(*m_pLoginDlg, WM_SHOWMAINDLG_MSG,0,0);
			break;

		}
	
	
	}
	else
	{
		/*SendMessage(*m_pLoginDlg, WM_SHOWMAINDLG_MSG,0,0);*/
		MyMessageBox(*m_pLoginDlg,"登录失败！");
		
		//SendMessage(*m_pLoginDlg, WM_SHOWERROR_MSG,0,0);
	}
	
	return nResult;
}

int CSocketBussiness::OnSSDQData(msgpack::unpacked &result_)
{
	msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	SendMessage(*g_MainPage, WM_GET_SSDQ_MSG, 0,(LPARAM)pObj);
	return 0;
}

int CSocketBussiness::OnLLTCData(msgpack::unpacked &result_)
{
	msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	SendMessage(*g_MainPage, WM_GET_LLTC_MSG, 0,(LPARAM)pObj);
	return 0;
}

int CSocketBussiness::OnTFJCData(msgpack::unpacked &result_)
{
	msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	object* pTemp = pObj;
	pTemp++;
	pTemp++;
	int nReq = pTemp->as<int>();
	if (nReq == 1)
	 SendMessage(*g_YonghuCardPage, WM_TINGFUJI_MSG, 0,(LPARAM)pObj);
	else if (nReq == 0)
	 SendMessage(*g_MainPage, WM_TINGFUJI_MSG, 0,(LPARAM)pObj);
	else if (nReq == 2)
	 SendMessage(*g_XsrqCardPage, WM_TINGFUJI_MSG, 0,(LPARAM)pObj);
	return 0;
}

int CSocketBussiness::OnUpdatStateData(msgpack::unpacked &result_)
{
	msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	object* pTemp = pObj;
	pTemp++;
	pTemp++;
	int nReq = pTemp->as<int>();
	if (nReq == 1)
		SendMessage(*g_YonghuCardPage, WM_UPDATECARDSTATE_MSG, 0,(LPARAM)pObj);
	else if (nReq == 0)
		SendMessage(*g_MainPage, WM_UPDATECARDSTATE_MSG, 0,(LPARAM)pObj);
	else if (nReq ==2)
	    SendMessage(*g_XsrqCardPage, WM_UPDATECARDSTATE_MSG, 0,(LPARAM)pObj);
	return 0;
}

int CSocketBussiness::OnImportCardMsg(msgpack::unpacked &result_)
{
	//[3,<错误码>,<错误信息>]
	msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	//SendMessage(*g_MainPage, WM_UPDATECARDSTATE_MSG, 0,(LPARAM)pObj);
	return 0;
	
	//string strErr = pObj->as<string>();
	//SendMessage(*m_pLoginDlg, WM_SHOWERROR_MSG,(WPARAM)nerr, (LPARAM)strErr.c_str());

	return 0;
}

int CSocketBussiness::SendActive()
{	
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(1);
	msgPack.pack(0);

	DealLast(sBuf);
	return 0;
}
int CSocketBussiness::OnUser(msgpack::unpacked &result_)
{
	msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	int cmd = pObj->as<int>();
	pObj ++;
	int nsubcmd = pObj->as<int>();
	if (nsubcmd == USER_ADD || nsubcmd == USER_LIST)
	{
		::SendMessage(*g_MainPage,WM_USER_MSG,0,(LPARAM)result_.get().via.array.ptr);
	}
	else if (nsubcmd == USER_LOGIN)
	{
		OnLogin(result_);
	}
	
	return 0;
}

int CSocketBussiness::OnSimData(msgpack::unpacked &result_)
{
	msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	SendMessage(*g_MainPage, WM_GET_CARD_MSG, 0,(LPARAM)pObj);
	return 0;
}
int CSocketBussiness::OnKHData(msgpack::unpacked &result_)
{
	msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	SendMessage(*g_MainPage, WM_GET_KH_MSG, 0,(LPARAM)pObj);
	return 0;
}
int CSocketBussiness::OnKHJLData(msgpack::unpacked &result_)
{
   msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	SendMessage(*g_MainPage, WM_GET_KHJL_MSG, 0,(LPARAM)pObj);

	return 0;
}
int CSocketBussiness::OnDXData(msgpack::unpacked &result_)
{
	msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	SendMessage(*g_MainPage, WM_GET_DX_MSG, 0,(LPARAM)pObj);
	return 0;
}
int CSocketBussiness::OnLLCData(msgpack::unpacked &result_)
{
	msgpack::object_handle clsHandle;
	result_.get().via.array.ptr;
	object* pObj= result_.get().via.array.ptr;
	SendMessage(*g_MainPage, WM_GET_LLC_MSG, 0,(LPARAM)pObj);
	return 0;
}

void CSocketBussiness::SetLoginDlg(CLoginDlg* pDlg)
{
	m_pLoginDlg = pDlg;
}

void CSocketBussiness::DealLast(msgpack::sbuffer& sBuf)
{
	char* pCh=sBuf.data();
	int nLen=sBuf.size();
	byte* pData =new byte[nLen];
	memset(pData, 0, nLen);
	memcpy(pData, pCh+6, nLen - 6);
	byte nSum = csum(pData, nLen -6);
	sBuf.write("\x00",1);
	memcpy(pCh + nLen , &nSum,1);
	sBuf.write("\x0d",1);
	nLen=sBuf.size();
	nLen-=8;
	memcpy(pCh+2,&nLen,4);
	nLen+=8;
	int n = this->Send(pCh,nLen);	
	delete []pData;
}

int CSocketBussiness::SendImportNewCard(vector<ST_CARD> & vec,string strLiuliangchihao,string strleixin)
{
	//[CMD_INPORT, SUBCMD_IMPORT_NEWCARD, nTag, <流量池号码>, <流量池类型>, [[<接入号码>, <ICCID>, <电信账号>], ...]]
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = EXCEL_LOAD;//CMD_IMPORT;
	int nSubCmd = EXCEL_SIM;
	int nTag = 0;

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(6+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nTag);
	msgPack.pack(strLiuliangchihao);
	msgPack.pack(strleixin);
	msgPack.pack_array(vec.size());
	for (int i = 0; i<vec.size(); i++)
	{
		msgPack.pack_array(3);
		replace_all(vec[i].strJieruNUm," ","");
		msgPack.pack(vec[i].strJieruNUm);
		replace_all(vec[i].strCardNum," ","");
		msgPack.pack(vec[i].strCardNum);
		msgPack.pack(vec[i].strCompany);
	}
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendImportXuCard(vector<ST_XUFEI>& vec)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = EXCEL_LOAD;//CMD_IMPORT;
	int nSubCmd = EXCEL_XFQD;
	int nTag = 0;
	//[EXCEL_LOAD,EXCEL_XSQD,nTag,[[jrhm,iccid,xsrq,xsy,bz],...]]
	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(4+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nTag);

	msgPack.pack_array(vec.size());
	for (int i = 0; i<vec.size(); i++)
	{
		//[jrhm,xfrq,nMonth
		msgPack.pack_array(3);
		msgPack.pack(vec[i].strSim);
		msgPack.pack(vec[i].strxfrq);
		msgPack.pack(atoi(vec[i].strxfys.c_str()));
	
	}
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendImportChuCard(vector<ST_CHUCARD>& vec)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = EXCEL_LOAD;//CMD_IMPORT;
	int nSubCmd = EXCEL_XSQD;
	int nTag = 0;
	//[EXCEL_LOAD,EXCEL_XSQD,nTag,[[jrhm,iccid,xsrq,xsy,bz],...]]
	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(4+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nTag);
	
	msgPack.pack_array(vec.size());
	for (int i = 0; i<vec.size(); i++)
	{
		msgPack.pack_array(5);
		trim(vec[i].strSim);
		replace_all(vec[i].strSim," ","");
		msgPack.pack(vec[i].strSim);
		trim(vec[i].strKehu);
		msgPack.pack(vec[i].strKehu);
		trim(vec[i].strNakaData);
		msgPack.pack(vec[i].strNakaData);
		trim(vec[i].strXiaoshouyuan);
		msgPack.pack(vec[i].strXiaoshouyuan);
		msgPack.pack(vec[i].strRemark);
	}
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendImportTuiCard(vector<ST_TUIKA>& vec)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = EXCEL_LOAD;//CMD_IMPORT;
	int nSubCmd = EXCEL_TKQD;
	int nTag = 0;
	//[EXCEL_LOAD,EXCEL_XSQD,nTag,[[jrhm,iccid,xsrq,xsy,bz],...]]
	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(4+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nTag);

	msgPack.pack_array(vec.size());
	for (int i = 0; i<vec.size(); i++)
	{
		msgPack.pack_array(1);
		replace_all(vec[i].strSim," ","");
		msgPack.pack(vec[i].strSim);
		
	}
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendImportXiaoCard(vector<ST_XIAOKA>& vec)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = EXCEL_LOAD;//CMD_IMPORT;
	int nSubCmd = EXCEL_ZXQD;
	int nTag = 0;
	//[EXCEL_LOAD,EXCEL_XSQD,nTag,[[jrhm,iccid,xsrq,xsy,bz],...]]
	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(4+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nTag);

	msgPack.pack_array(vec.size());
	for (int i = 0; i<vec.size(); i++)
	{
		msgPack.pack_array(3);
		replace_all(vec[i].strSim," ","");
		msgPack.pack(vec[i].strSim);
		msgPack.pack(vec[i].strzxrq);
		msgPack.pack(vec[i].strzt);

	}
	DealLast(sBuf);
	return 0;
}


int CSocketBussiness::SendSaveCard(string jrhm,string iccid,string jlxm,string lltc,string dxzh,string jhrq,string zt,string ssdq,string khmc,string llchm,string xsrq,string dqrq,string xfrq,string zxrq,string bz,string dj)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = SIM_DATA;
	int nSubCmd = SIM_ADD;
	//id,jrhm,iccid,jlxm,lltc,dxzh,jhrq,zt,ssdq,khmc,llchm,xsrq,dqrq,xfrq,zxrq,bz,dj


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(3+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack_array(1);
	msgPack.pack_array(16);
	msgPack.pack(jrhm);
	msgPack.pack(iccid);
	msgPack.pack(jlxm);
	msgPack.pack(lltc);
	msgPack.pack(dxzh);
	msgPack.pack(jhrq);
	msgPack.pack(zt);
	msgPack.pack(ssdq);
	msgPack.pack(khmc);
	msgPack.pack(llchm);
	msgPack.pack(xsrq);
	msgPack.pack(dqrq);
	msgPack.pack(xfrq);
	msgPack.pack(zxrq);
	msgPack.pack(bz);
	double ddj = strtod(dj.c_str(),NULL);
	msgPack.pack(ddj);

	DealLast(sBuf);
	return 0;
}
int CSocketBussiness::SendRegInfo(string struser,string strPass)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = USER_DATA;
	int nSubCmd = USER_ADD;
	

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(3+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack_array(1);
	msgPack.pack_array(4);
	msgPack.pack(struser);
	msgPack.pack(strPass);
	int nau = 0;
	msgPack.pack(nau);
	double d= 0.0;
	msgPack.pack(d);
	//msgPack.pack()
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendGetKehuJingliList(int nReqTag,int nAB, int nkeyid)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = KHJL_DATA;
	int nSubCmd = KHJL_LIST;
	//[KHJL_DATA,KHJL_LIST,nTag,nPage]
	//[KHJL_DATA,KHJL_LIST,nTag,0/1,nCount,[[id,jlxm,lxfs,xgsj,bz],...]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(5+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReqTag);
	msgPack.pack(1);  
	if (nReqTag== 0)
		msgPack.pack(MAX_COUNT);
	else
		msgPack.pack(PAGE_COUNT);
	
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendGetDXList(int nReqTag,int nPage,int nAB, int nkeyid)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = DXZH_DATA;
	int nSubCmd = DXZH_LIST;
	//[KHJL_DATA,KHJL_LIST,nTag,nPage]
	//[KHJL_DATA,KHJL_LIST,nTag,0/1,nCount,[[id,jlxm,lxfs,xgsj,bz],...]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(9);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReqTag);

	msgPack.pack(nPage);
	if (nReqTag== 0)
		msgPack.pack(MAX_COUNT);
	else
		msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nkeyid);
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendAddJingli(string strJingli,string strLianxifs,string strRemark)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = KHJL_DATA;
	int nSubCmd = KHJL_ADD;
	//[KHJL_DATA,KHJL_ADD,[[jlxm.lxfs,bz]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(3+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack_array(1);
	msgPack.pack_array(3);
	msgPack.pack(strJingli);
	msgPack.pack(strLianxifs);
	msgPack.pack(strRemark);
	

	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendAddKehu(string strName,string strLxfs,string strJl,double dj, string strssdq,string strRemark)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = KH_DATA;
	int nSubCmd = KH_ADD;
	//[nCmd,nSubCmd,Userid,Usertype,[[Khmc,Fatherid,Lxfs,Ssdq,Jlxm,Dj,Bz]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(5);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack_array(1);
	msgPack.pack_array(7);
	msgPack.pack(strName);
	msgPack.pack(atoi(g_strUserID.c_str()));
	msgPack.pack(strLxfs);
	msgPack.pack(strssdq);
	msgPack.pack(strJl);
	msgPack.pack(dj);
	msgPack.pack(strRemark);
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendAddDianXin(string strName, string strUserid, string strPass,string strKey,string strRemark)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = DXZH_DATA;
	int nSubCmd = DXZH_ADD;
	//[KHJL_DATA,KHJL_ADD,[[jlxm.lxfs,bz]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(5);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack_array(1);
	msgPack.pack_array(5);
	msgPack.pack(strName);
	msgPack.pack(strUserid);
	msgPack.pack(strPass);
	msgPack.pack(strKey);
	msgPack.pack(strRemark);
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendAddLlc(string strllc,string strllclx,string strdxzh,string strbz)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = LLC_DATA;
	int nSubCmd = LLC_ADD;
	//[KHJL_DATA,KHJL_ADD,[[jlxm.lxfs,bz]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(3+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack_array(1);
	msgPack.pack_array(4);
	msgPack.pack(strllc);
	msgPack.pack(strllclx);
	msgPack.pack(strdxzh);
	msgPack.pack(strbz);

	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendQueryLlcAll(int nReqTag,int nPage,int nAB, int nkeyid)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = LLC_DATA;
	int nSubCmd = LLC_LIST;
	//[KHJL_DATA,KHJL_ADD,[[jlxm.lxfs,bz]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(9);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReqTag);
	msgPack.pack(nPage);
	if (nReqTag== 0)
		msgPack.pack(MAX_COUNT);
	else
		msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nkeyid);

	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendAddSsdq(string strArea)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = SSDQ_DATA;
	int nSubCmd = SSDQ_ADD;
	//[KHJL_DATA,KHJL_ADD,[[jlxm.lxfs,bz]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(3+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack_array(1);
	msgPack.pack_array(1);
	msgPack.pack(strArea);

	DealLast(sBuf);
	return 0;
}

int  CSocketBussiness::SendAddLLTC(string strTCname, string strTClx)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = LLTC_DATA;
	int nSubCmd = LLTC_ADD;
	//[KHJL_DATA,KHJL_ADD,[[jlxm.lxfs,bz]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(5);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack_array(1);
	msgPack.pack_array(2);
	msgPack.pack(strTCname);
	msgPack.pack(strTClx);

	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendQueryCard(string strSim)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = SIM_DATA;
	int nSubCmd = SIM_JRHM;
	//[KHJL_DATA,KHJL_ADD,[[jlxm.lxfs,bz]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(3);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	//AddUseridAndtype(msgPack);
	msgPack.pack_array(1);
	msgPack.pack_array(1);
	msgPack.pack(strSim.c_str());

	DealLast(sBuf);
	return 0;
}
int CSocketBussiness::SendQueryAllCard(int nReq,int nPage)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = SIM_DATA;
	int nSubCmd = SIM_LIST;
	//[KHJL_DATA,KHJL_ADD,[[jlxm.lxfs,bz]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(4);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	//AddUseridAndtype(msgPack);
	/*msgPack.pack(nReq);*/
	msgPack.pack(nPage);
	msgPack.pack(PAGE_COUNT);


	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendKhCardTj(string strkehu,int nPage, int nPageCount, int nAB, int nKeyid )
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = KH_DATA;
	int nSubCmd = KH_COUNT;
	//[nCmd,nSubCmd,Userid,Usertype,nTag,nPagesize,nAB,nKeyid]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(8);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nPage);
	msgPack.pack(nPageCount);
	msgPack.pack(nAB);
	msgPack.pack(nKeyid);
	//msgPack.pack_array(1);
	//msgPack.pack_array(1);
	//msgPack.pack(strkehu.c_str());


	DealLast(sBuf);
	return 0;
}

//[nCmd,nSubCmd,Userid,Usertype,nTag,nPagesize,nAB,nKeyid]
int CSocketBussiness::SendKhQueryAll(string strkhu,int nReq,int nPage,int nAB,int nKeyid)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = KH_DATA;
	int nSubCmd = KH_LIST;
	

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(9);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nPage);
	msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nKeyid);
	msgPack.pack(strkhu);


	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendKhjlQueryAll(string strname,int nReqTag,int nPage,int nAB, int nKeyid)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = KHJL_DATA;
	int nSubCmd = KHJL_LIST;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(10);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReqTag);
	msgPack.pack(nPage);
	if (nReqTag== 0)
		msgPack.pack(MAX_COUNT);
	else
		msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nKeyid);
	msgPack.pack(strname);
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendSsdqQueryAll(int nReqTag, int nAB, int nKeyid )
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = SSDQ_DATA;
	int nSubCmd = SSDQ_LIST;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(9);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReqTag);
	msgPack.pack(1);
	if (nReqTag== 0)
		msgPack.pack(MAX_COUNT);
	else
		msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nKeyid);
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendLltcQueryAll(int nReqTag,int nPage,int nAB, int nkeyid )
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = LLTC_DATA;
	int nSubCmd = LLTC_LIST;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(9);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReqTag);
	msgPack.pack(nPage);
	if (nReqTag== 0)
		msgPack.pack(MAX_COUNT);
	else
		msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nkeyid);
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendQueryCardByKehu(string strkehu ,int nPage)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = KH_DATA;
	int nSubCmd = KH_SIM_LIST;

	int nTag =0;
	int nAB = 0;
	
	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(10);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nTag);
	msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nPage);
	msgPack.pack_array(1);
	msgPack.pack_array(1);
	msgPack.pack(strkehu.c_str());
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendQueryCardByKehuID(string strkehuID ,int nPage)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = KH_DATA;
	int nSubCmd = KH_SIM_LIST;

	int nTag =0;
	int nAB = 0;

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(10);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nTag);
	msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nPage);
	msgPack.pack_array(1);
	msgPack.pack_array(1);
	msgPack.pack(strkehuID.c_str());
	DealLast(sBuf);
	return 0;
}

//[nCmd,nSubCmd,Userid,Usertype,nTag,nPagesize,nAB,nKeyid,[[Khid]]]// 15，1月
int CSocketBussiness::SendQueryKehuCardBylx(int nReq,string strkehuid,int nType,int nPage,int nAB ,int nKeyid )
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd = KH_DATA;
	int nSubCmd = nType;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(10);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReq);
	msgPack.pack(nPage);
	msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nKeyid);
	msgPack.pack_array(1);
	msgPack.pack_array(1);
	msgPack.pack(strkehuid.c_str());
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendTingfuji(int nReq,string strSim,string strDx,bool bTingji )
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  DISABLE_NUMBER_DATA;
	int nSubCmd = bTingji?DN_DISABLE:DN_ABLE;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(4+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReq);
	msgPack.pack_array(1);
	msgPack.pack_array(2);
	msgPack.pack(strSim);
	msgPack.pack(strDx);

	DealLast(sBuf);
	return 0;
}
int CSocketBussiness::SendAddUser(string strUser,string strPassword,string strKhmc,int Authority,int NUsertype)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  USER_DATA;
	int nSubCmd =USER_ADD;
	//[nCmd,nSubCmd,Userid,Usertype,[[User,Password,Khmc,Authority,NUsertype,Fatherid]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(5);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);

	msgPack.pack_array(1);
	msgPack.pack_array(6);
	msgPack.pack(strUser);
	msgPack.pack(strPassword);
	msgPack.pack(strKhmc);
	msgPack.pack(Authority);
	msgPack.pack(NUsertype);
	msgPack.pack(atoi(g_strUserID.c_str()));

	DealLast(sBuf);
	return 0;
}
int CSocketBussiness::SendUpdateSimState(int nReq,string strSim,string strDx)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  CARD_STATUS_DATA;
	int nSubCmd =CS_QUERY_JRHM;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(4+2);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReq);
	msgPack.pack_array(1);
	msgPack.pack_array(2);
	msgPack.pack(strSim);
	msgPack.pack(strDx);

	DealLast(sBuf);
	return 0;
}
int CSocketBussiness::SendGetJlKh(int nReq,string strJl,int nPage,int nAB,int nID)
{
	//[KHJL_DATA,KHJL_KH,nTag,nPage,[[jlxm]]]
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  KHJL_DATA;
	int nSubCmd =KHJL_KH;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(10);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReq);
	msgPack.pack(nPage);
	msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nID);
	msgPack.pack_array(1);
	msgPack.pack_array(1);
	msgPack.pack(strJl);

	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendGetJlXSJL(int nReq,string strJl,int nPage,int nAB,int nID)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  KHJL_DATA;
	int nSubCmd =KHJL_QUERY;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(10);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReq);
	msgPack.pack(nPage);
	msgPack.pack(100000);
	msgPack.pack(nAB);
	msgPack.pack(nID);
	msgPack.pack_array(1);
	msgPack.pack_array(1);
	msgPack.pack(strJl);

	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendGetJLXSDataCards(int nReq,string strJl,string strdate,int nPage,int nAB,int nID)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  KHJL_DATA;
	int nSubCmd =KHJL_SIM_XSRQ;
	//[KHJL_DATA,KHJL_SIM_XSRQ,nTag,nPage,[[jlxm,xsrq]]]

	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(10);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReq);
	msgPack.pack(nPage);
	msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nID);
	msgPack.pack_array(1);
	msgPack.pack_array(2);
	msgPack.pack(strJl);
	msgPack.pack(strdate);

	DealLast(sBuf);
	return 0;
}
int CSocketBussiness::SendGetUserList()
{
	//[nCmd,nSubCmd,Userid,Usertype,nTag,nPagesize,nAB,nKeyid]
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  USER_DATA;
	int nSubCmd =USER_LIST;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(9);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(0);
	msgPack.pack(1);
	msgPack.pack(10000);
	msgPack.pack(0);
	msgPack.pack(0);

	DealLast(sBuf);
	return 0;
}
int  CSocketBussiness::SendQueryCardbyJrhm(string strjrhm)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  SIM_DATA;
	int nSubCmd =SIM_JRHM;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(5);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack_array(1);
	msgPack.pack_array(1);
	msgPack.pack(strjrhm);
	

	DealLast(sBuf);
	return 0;
}

void CSocketBussiness::AddUseridAndtype(packer<msgpack::sbuffer>& msgPack)
{
	msgPack.pack(g_strUserID);
	msgPack.pack(g_strUserType);
}

int CSocketBussiness::SendGetKhRqTj(int nReq,string strkhid,int nPage,int nAB,int nID)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  KH_DATA;
	int nSubCmd =KH_QUERY;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(10);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReq);
	msgPack.pack(nPage);
	msgPack.pack(10000);
	msgPack.pack(nAB);
	msgPack.pack(nID);
	msgPack.pack_array(1);
	msgPack.pack_array(1);
	msgPack.pack(strkhid);

	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendGetKhXsrqCard(int nReq,string strkhid,string strDate,int nPage,int nAB,int nID)
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  KH_DATA;
	int nSubCmd =KH_SIM_XSRQ;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(10);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	msgPack.pack(nReq);
	msgPack.pack(nPage);
	msgPack.pack(PAGE_COUNT);
	msgPack.pack(nAB);
	msgPack.pack(nID);
	msgPack.pack_array(1);
	msgPack.pack_array(2);
	msgPack.pack(strkhid);
	msgPack.pack(strDate);
	DealLast(sBuf);
	return 0;
}

int CSocketBussiness::SendTotalSum()
{
	sbuffer sBuf;
	packer<msgpack::sbuffer> msgPack(&sBuf);
	int nCmd =  SIM_DATA;
	int nSubCmd =SIM_TOTAL;


	sBuf.write("\xfb\xfc",6);
	msgPack.pack_array(4);
	msgPack.pack(nCmd);
	msgPack.pack(nSubCmd);
	AddUseridAndtype(msgPack);
	DealLast(sBuf);
	return 0;
}