#pragma once
#include "stdafx.h"
using namespace std;
struct ST_CARD
{
	std::string strCardNum;
	std::string strJieruNUm;
	std::string strCompany;

};
//SIM卡号	接入号码	开卡日期	套餐	备注	池号	客户
struct ST_KAICARD//开卡记录
{
	string strSim;
	string strJieruNum;
	string strKaikaData;
	string strTaocan;
	string strRemark;
	string strChihao;
	string strKehu;
};
//客户名称	数量	卡号	手机号	拿卡日期	销售员	套餐	归属地	开卡日期	卡备注
struct ST_CHUCARD//出卡记录
{
	string strKehu;
	string strSim;
	string strJieruNum;
	string strNakaData;
	string strXiaoshouyuan;
	string strTaocan;
	string strGuishudi;
	string strKaikaData;
	string strRemark;
};
//卡号	缴费日期	到期日期	客户	手机号	缴费金额	备注
//struct ST_XUFEI//续费记录
//{
//	string strSim;
//	string strJiaofeiData;
//	string strDaoqiData;
//	string strKehu;
//	string strShoujihao;
//	string strMoney;;
//	string strRemark;
//
//};
struct ST_XUFEI
{
	string strSim;
	string strxfrq;
	string strxfys;
};
struct ST_TUIKA
{
	string strSim;
};
struct ST_XIAOKA
{
	string strSim;
	string strzxrq;
	string strzt;
};
struct ST_KEHU
{
	string strName;
	string strllfs;
	string strssdq;
	string strdj;
	string strjl;
	string strbz;
};
extern "C"
{
	_declspec(dllexport) bool ReadCard(void* a, char* b,int iLen);
	_declspec(dllexport) bool ReadChuka(void* a, char* b,int iLen);
	_declspec(dllexport) bool ReadXuka(void* a, char* b,int iLen);
	_declspec(dllexport) bool ReadTuika(void* a, char* b,int iLen);
	_declspec(dllexport) bool ReadXiaoka(void* a, char* b,int iLen);
	_declspec(dllexport) bool ReadAll(void*,void*,void*,char*,int iLen);
	_declspec(dllexport) bool ReadKehu(void* pVec1,char* szFile,int iLen);
	_declspec(dllexport) bool WriteExcel(void* pVec,char* strHearder,int nLen);
	typedef bool (* ReadCardProc)(void*,char*,int iLen);
	typedef bool (* ReadChukaProc)(void*,char*,int iLen);
	typedef bool (* ReadXukaProc)(void*,char*,int iLen);
	typedef bool (* ReadTuikaProc)(void*,char*,int iLen);
	typedef bool (* ReadXiaokaProc)(void*,char*,int iLen);
	typedef bool (* ReadAllProc)(void*,void*,void*,char*,int iLen);
	
}