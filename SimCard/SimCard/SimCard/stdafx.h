// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <objbase.h>
#include "..\DuiLib\UIlib.h"
#include "CommonFun.h"
#include "SocketBussiness.h"
#pragma comment(lib,"ws2_32.lib")
using namespace DuiLib;
using namespace msgpack;
extern CSocketBussiness g_SockBuss;
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib.lib")
#   endif
#endif
#pragma comment(lib, "..\\bin\\ReadExcel.lib")
#define  PAGE_COUNT 25
#define  MAX_COUNT 1000
#define  WM_SHOWMAINDLG_MSG (WM_USER + 101)
#define  WM_SHOWERROR_MSG   (WM_USER + 102)
#define  WM_GET_KHJL_MSG (WM_USER+ 103)
#define  WM_GET_KH_MSG (WM_USER +104)
#define  WM_GET_DX_MSG (WM_USER +105)
#define  WM_GET_LLC_MSG (WM_USER +106)
#define  WM_GET_SSDQ_MSG (WM_USER +107)
#define  WM_GET_LLTC_MSG (WM_USER + 108)
#define  WM_GET_CARD_MSG (WM_USER + 109)
#define  WM_DATECARD_MSG (WM_USER + 110)
#define  WM_TINGFUJI_MSG (WM_USER + 111)
#define  WM_UPDATECARDSTATE_MSG (WM_USER + 112)
#define  WM_IMPORTCARD_MSG (WM_USER + 113)
#define  WM_USER_MSG (WM_USER + 114)
#define WM_JLKH_LIST_MSG (WM_USER+115)
#define WM_JL_QUERY_MSG (WM_USER+116)
#define WM_KH_RQTJ_MSG (WM_USER+117)
#define WM_KH_RQTJCard_MSG (WM_USER+118)
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

struct ST_ID
{
	string strFirst;
	string strLast;
	/*E_FANYE en_fanye;*/
};
