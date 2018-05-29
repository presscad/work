#pragma once
#include "stdafx.h"
using namespace std;
struct ST_CARD
{
	std::string strCardNum;
	std::string strJieruNUm;
	std::string strCompany;

};
//SIM����	�������	��������	�ײ�	��ע	�غ�	�ͻ�
struct ST_KAICARD//������¼
{
	string strSim;
	string strJieruNum;
	string strKaikaData;
	string strTaocan;
	string strRemark;
	string strChihao;
	string strKehu;
};
//�ͻ�����	����	����	�ֻ���	�ÿ�����	����Ա	�ײ�	������	��������	����ע
struct ST_CHUCARD//������¼
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
//����	�ɷ�����	��������	�ͻ�	�ֻ���	�ɷѽ��	��ע
//struct ST_XUFEI//���Ѽ�¼
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