#pragma once
#include "stdafx.h"
#include "socketworker.h"
#include "msgpack.hpp"
#include "Excel.h"
class CLoginDlg;
using namespace  std;
using namespace msgpack;
#define USER_DATA	0X01
typedef enum
{
	USER_ADD = 0X01,
	USER_LOGIN,
	USER_LIST,
}SUBCMD_USER;
#define SIM_DATA	0X02
typedef enum
{
	SIM_ADD = 0X01,
	SIM_LIST,
	SIM_JRHM,
	SIM_TOTAL,
}SUBCMD_SIM;
typedef enum
{
	SUBCMD_IMPORT_NEWCARD = 0X01,	// 新卡导入
	SUBCMD_IMPORT_SALENOTE,			// 销售清单导入
	SUBCMD_IMPORT_KHSTATE,			// 客户状态导入
	SUBCMD_IMPORT_PAYLIST,			// 续费清单导入
	SUBCMD_IMPORT_CARDRETURNED,		// 退卡信息导入
	SUBCMD_IMPORT_CARDCANCEL,		// 注销卡信息导入
}SUBCMD_IMPORT;
#define KH_DATA	0X03
typedef enum
{
	KH_ADD = 0X01,
	KH_COUNT,
	KH_QUERY,
	KH_LIST,
	KH_SIM_LIST,
	KH_SIM_XSRQ,
	KH_SIM_USING,
	KH_SIM_U15D,
	KH_SIM_U1M,
	KH_SIM_D1M,
	KH_SIM_D15D,
	
}SUBCMD_KH;
#define KHJL_DATA	0X04
typedef enum
{
	KHJL_ADD = 0X01,
	KHJL_KH,
	KHJL_QUERY,
	KHJL_LIST,
	KHJL_SIM_XSRQ,
}SUBCMD_KHJL;
#define LLC_DATA	0X05
typedef enum
{
	LLC_ADD = 0X01,
	LLC_LIST,
}SUBCMD_LLC;
#define LLTC_DATA	0X06
typedef enum
{
	LLTC_ADD = 0X01,
	LLTC_LIST,
}SUBCMD_LLTC;
#define DXZH_DATA	0X07
typedef enum
{
	DXZH_ADD = 0X01,
	DXZH_LIST,
}SUBCMD_DXZH;
#define EXCEL_LOAD	0X08
typedef enum
{
	EXCEL_SIM = 0X01,
	EXCEL_XSQD,
	EXCEL_ZTXX,
	EXCEL_XFQD,
	EXCEL_TKQD,
	EXCEL_ZXQD,
}SUBCMD_EXCEL;

#define SSDQ_DATA	0X09
typedef enum
{
	SSDQ_ADD = 0X01,
	SSDQ_LIST,
}SUBCMD_SSDQ;

#define DISABLE_NUMBER_DATA 0X10

typedef enum
{
	DN_DISABLE = 0X01,
	DN_ABLE,
}SUBCMD_DISABLE_NUMBER;

#define CARD_STATUS_DATA 0X11

typedef enum
{
	CS_QUERY_JRHM = 0X01,
	CS_QUERY_ICCID,
}SUBCMD_CART_STATUS;

class CSocketBussiness :public CSocketWorker
	
{
	
public:
	CSocketBussiness(void);
	~CSocketBussiness(void);
	
	int Login(void);
	
	int SendActive();
	

	// 设置用用户名跟密码
	int SetUserInfo(string & strName, string & strPwd);
	void SetLoginDlg(CLoginDlg* pDlg);
	int DataProc(void);
	std::string GetLoginUserName(){return m_strName;};
	void DealLast(msgpack::sbuffer& sBuf);
private:
	
	// 是否已经登录成功
	bool m_bIsLogin;
	// 用户名
	string  m_strName;
	// 密码
	string  m_strPwd;

	//状态
	string m_strStatu;
	// 心跳无效计数，大于2视为心跳僵死
	volatile short m_nActive;
	// 主窗口够柄，用于传递消息
	CLoginDlg*  m_pLoginDlg;
private:	
	// 连接成功后的规定动作
	int OnConnected(void);	
	int SetStatus(int nStatus);	
private:
	int OnLogin(msgpack::unpacked &result_);
	int OnUser(msgpack::unpacked &result_);
	int OnSimData(msgpack::unpacked &result_);
	int OnKHData(msgpack::unpacked &result_);
    int OnKHJLData(msgpack::unpacked &result_);
	int OnDXData(msgpack::unpacked &result_);
	int OnLLCData(msgpack::unpacked &result_);
	int OnImportCardMsg(msgpack::unpacked &result_);
	int OnSSDQData(msgpack::unpacked &result_);
	int OnLLTCData(msgpack::unpacked &result_);
	int OnTFJCData(msgpack::unpacked &result_);
	int OnUpdatStateData(msgpack::unpacked &result_);
public:	
	//发送的所有请求///////////////////////////
	int SendImportXiaoCard(vector<ST_XIAOKA>& vec);
	int SendImportTuiCard(vector<ST_TUIKA>& vec);
	int SendImportXuCard(vector<ST_XUFEI>& vec);
	int SendImportChuCard(vector<ST_CHUCARD>& vec);
	int SendImportNewCard(vector<ST_CARD> & vec,string strLiuliangchihao,string strleixin);
	int SendRegInfo(string struser,string strPass);
	//id,jrhm,iccid,jlxm,lltc,dxzh,jhrq,zt,ssdq,khmc,llchm,xsrq,dqrq,xfrq,zxrq,bz,dj
	int SendSaveCard(string jrhm,string iccid,string jlxm,string lltc,string dxzh,string jhrq,string zt,string ssdq,string khmc,string llchm,string xsrq,string dqrq,string xfrq,string zxrq,string bz,string dj);
	int SendGetKehuJingliList(int nReqTag,int nAB=0, int nkeyid=0);
	int SendGetDXList(int nReqTag,int nPage= 1,int nAB= 0, int nkeyid= 0);
	int SendAddJingli(string strJingli,string strLianxifs,string strRemark);
	int SendAddKehu(string strName,string strLxfs,string strJl,double dj, string strssdq,string strRemark);
	int SendAddDianXin(string strName, string strUserid, string strPass,string strKey,string strRemark);
	int SendAddLlc(string strllc,string strllclx,string strdxzh,string strbz);//[LLC_DATA,LLC_ADD,[[llchm,llclx,dxzh,bz]]]
	int SendQueryLlcAll(int nReqTag,int nPage=1,int nAB = 0, int nkeyid = 0);
	int SendAddSsdq(string strArea);
	int SendAddLLTC(string strTCname, string strTClx);
	int SendQueryCard(string strSim);
	int SendQueryAllCard(int nReq,int nPage =1);
	int SendKhCardTj(string strkehu,int nPage, int nPageCount, int nAB, int nKeyid );
	int SendKhQueryAll(string strkhu,int nReq,int nPage=1,int nAB=0,int nKeyid=0);
	int SendKhjlQueryAll(string strname,int nReqTag,int nPage =1,int nAB =0, int nKeyid=0);
	int SendSsdqQueryAll(int nReqTag = 0, int nAB= 0, int nKeyid= 0);
	int SendLltcQueryAll(int nReqTag,int nPage =1,int nAB=0, int nkeyid=0);
	int SendQueryCardByKehu(string strkehu, int nPage = 1);
	int SendQueryCardByKehuID(string strkehuID ,int nPage);
	int SendQueryKehuCardBylx(int nReq,string strkehuid,int nType,int nPage,int nAB = 0,int nKeyid = 0);
	int SendTingfuji(int nReq,string strSim,string strDx,bool bTingji = true);
	int SendUpdateSimState(int nReq,string strSim,string strDx);
	int SendAddUser(string strUser,string strPassword,string strKhmc,int Authority,int NUsertype);
	void AddUseridAndtype(packer<msgpack::sbuffer>& msgPack);
	int SendGetJlKh(int nReq,string strJl,int nPage=1,int nAB=0,int nID=0);
	int SendGetJlXSJL(int nReq,string strJl,int nPage=1,int nAB=0,int nID=0);
	int SendGetJLXSDataCards(int nReq,string strJl,string strdate,int nPage=1,int nAB=0,int nID=0);
	int SendGetUserList();
	int SendQueryCardbyJrhm(string strjrhm);
	int SendGetKhRqTj(int nReq,string strkhid,int nPage=1,int nAB=0,int nID=0);
	int SendGetKhXsrqCard(int nReq,string strkhid,string strDate,int nPage=1,int nAB=0,int nID=0);
	int SendTotalSum();
	//int SendAddArea(s)
		

};

