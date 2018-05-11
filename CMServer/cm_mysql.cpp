#include "stdafx.h"
#include "client_signaldata.h"
#include "global_data.h"
#include "cm_mysql.h"
#include <vector>

#pragma comment(lib, "libmysql.lib")

#define DB_NAME _T("cardb")
#define DB_USER _T("baolan123")
#define DB_PWD _T("baolan123")
#define DB_DSN _T("mysql_db_a")

std::vector<MYSQL*> vctMysql;
CRITICAL_SECTION csMysql;
HANDLE hWaitMysql = NULL;
int g_nMinConnection = 0;
int g_nMaxConnection = 0;
int g_nCurrentNumber = 0;

bool Mysql_ConnectDB(MYSQL** pMysql)
{
	*pMysql = mysql_init((MYSQL*)NULL);
	if (NULL == *pMysql)
		return false;

	//if (NULL == mysql_real_connect(*pMysql, _T("123.149.232.30"), DB_USER, DB_PWD, DB_NAME, 3306, NULL, 0))
	if (NULL == mysql_real_connect(*pMysql, _T("localhost"), DB_USER, DB_PWD, DB_NAME, 3306, NULL, 0))
	{
		return false;
	}

	if (0 != mysql_set_character_set(*pMysql, "gbk"))
	{
		return false;
	}

	return true;
}

void Mysql_CloseConnection(MYSQL** pMysql)
{
	if (NULL != *pMysql)
	{
		mysql_close(*pMysql);
		*pMysql = NULL;
	}
}

bool Mysql_InitConnectionPool(int nMin, int nMax)
{
	static bool bInitial = true;
	if (bInitial)
	{
		InitializeCriticalSection(&csMysql);
		vctMysql.clear();
		hWaitMysql = CreateEvent(NULL, TRUE, FALSE, NULL);
		g_nMinConnection = nMin;
		g_nMaxConnection = nMax;
		for (int i = 0; i < nMin; i++)
		{
			MYSQL* pMysql = NULL;
			if (!Mysql_ConnectDB(&pMysql))
			{
				Mysql_CloseConnection(&pMysql);
				continue;
			}
			EnterCriticalSection(&csMysql);
			vctMysql.push_back(pMysql);
			++g_nCurrentNumber;
			LeaveCriticalSection(&csMysql);
		}
		bInitial = false;
	}
	return !bInitial;
}

void Mysql_RemoveFromPool(MYSQL** pMysql)
{
	if (NULL != *pMysql)
	{
		mysql_close(*pMysql);
		*pMysql = NULL;
	}

	EnterCriticalSection(&csMysql);
	--g_nCurrentNumber;
	LeaveCriticalSection(&csMysql);
}

MYSQL* Mysql_AllocConnection()
{
	while (true)
	{
		MYSQL* pMysql = NULL;
		EnterCriticalSection(&csMysql);
		if (vctMysql.empty())
		{
			if (g_nCurrentNumber < g_nMaxConnection)
			{
				if (!Mysql_ConnectDB(&pMysql))
				{
					LeaveCriticalSection(&csMysql);
					Mysql_CloseConnection(&pMysql);
					return pMysql;
				}
				else
				{
					++g_nCurrentNumber;
				}
			}
			else
			{
				ResetEvent(hWaitMysql);
				LeaveCriticalSection(&csMysql);
				WaitForSingleObject(hWaitMysql, INFINITE);
				continue;
			}
		}
		else
		{
			pMysql = vctMysql.back();
			vctMysql.pop_back();
		}
		LeaveCriticalSection(&csMysql);

		if (0 != mysql_ping(pMysql))
		{
			Mysql_CloseConnection(&pMysql);
			if (!Mysql_ConnectDB(&pMysql))
			{
				Mysql_RemoveFromPool(&pMysql);
			}
		}

		return pMysql;
	}
}

void Mysql_BackToPool(MYSQL* pMysql)
{
	EnterCriticalSection(&csMysql);
	vctMysql.push_back(pMysql);
	LeaveCriticalSection(&csMysql);
	SetEvent(hWaitMysql);
}

bool InsertIntoTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj)
{
	size_t len = _tcslen(sql);
	if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
	{
		error_info(bobj, _T("数据库异常 ErrorCode = %08x, ErrorMsg = %s"), mysql_errno(pMysql), mysql_error(pMysql));
		return false;
	}

	if (mysql_affected_rows(pMysql) == 0)
	{
		return false;
	}

	return true;
}

bool Trans_InsertIntoTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj)
{
	size_t len = _tcslen(sql);
	if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
	{
		error_info(bobj, _T("数据库异常 ErrorCode = %08x, ErrorMsg = %s"), mysql_errno(pMysql), mysql_error(pMysql));
		return false;
	}

	//if (mysql_affected_rows(pMysql) == 0)
	//{
	//	return false;
	//}

	return true;
}

bool SelectFromTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj, MYSQL_RES** res)
{
	size_t len = _tcslen(sql);
	if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
	{
		error_info(bobj, _T("数据库异常 ErrorCode = %08x, ErrorMsg = %s"), mysql_errno(pMysql), mysql_error(pMysql));
		return false;
	}

	*res = mysql_store_result(pMysql);
	if (NULL == *res)
	{
		if (mysql_field_count(pMysql) != 0)
		{
			error_info(bobj, _T("数据库异常 ErrorCode = %08x, ErrorMsg = %s"), mysql_errno(pMysql), mysql_error(pMysql));
		}
		else
		{
			error_info(bobj, _T("未查询到匹配数据"));
		}
		return false;
	}

	my_ulonglong nErr = mysql_affected_rows(pMysql);
	if ((my_ulonglong)0 == nErr)
	{
		error_info(bobj, _T("没有查询到数据 sql = %s"), sql);
		mysql_free_result(*res);
		*res = NULL;
		return false;
	}

	return true;
}

bool UpdateTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj)
{
	size_t len = _tcslen(sql);
	if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
	{
		error_info(bobj, _T("数据库异常 ErrorCode = %08x, ErrorMsg = %s"), mysql_errno(pMysql), mysql_error(pMysql));
		return false;
	}

	if (mysql_affected_rows(pMysql) == 0)
	{
		error_info(bobj, _T("数据不存在"));
		return false;
	}

	return true;
}

bool Trans_UpdateTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj)
{
	size_t len = _tcslen(sql);
	if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
	{
		error_info(bobj, _T("数据库异常 ErrorCode = %08x, ErrorMsg = %s"), mysql_errno(pMysql), mysql_error(pMysql));
		return false;
	}

	//if (mysql_affected_rows(pMysql) == 0)
	//{
	//	error_info(bobj, _T("数据不存在"));
	//	return false;
	//}

	return true;
}

bool CreateTbl(const TCHAR* sql)
{
	MYSQL* pMysql = Mysql_AllocConnection();
	if (NULL == pMysql)
	{
		_tprintf(_T("%s:连接服务器失败\n"), __FUNCTION__);
		return false;
	}
	size_t len = _tcslen(sql);
	if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
	{
		_tprintf_s(_T("数据库异常 ErrorCode = %08x, ErrorMsg = %s"), mysql_errno(pMysql), mysql_error(pMysql));
		Mysql_BackToPool(pMysql);
		return false;
	}
	Mysql_BackToPool(pMysql);
	return true;
}

#define CREATE_DATABASE _T("CREATE DATABASE IF NOT EXISTS cardb")
bool CreateDB()
{

	return CreateTbl(CREATE_DATABASE);
}

#define CREATE_USER_TBL _T("CREATE TABLE IF NOT EXISTS user_tbl(id int unsigned not null auto_increment,\
User char(32) NOT NULL,\
Password char(41) NOT NULL,\
Authority int NOT NULL,\
Usertype int NOT NULL,\
Fatherid int unsigned DEFAULT 0,\
Xgsj datetime,\
Bz varchar(64),\
primary key(id),\
unique key(User),\
key(Fatherid))")
bool CreateUserTbl()
{
	return CreateTbl(CREATE_USER_TBL);
}

#define CREATE_KH_TBL _T("CREATE TABLE IF NOT EXISTS kh_tbl(id int unsigned not null auto_increment,\
Khmc char(41) NOT NULL,\
Userid int unsigned DEFAULT 0,\
Fatherid int unsigned DEFAULT 0,\
Jlxm char(64) NOT NULL,\
Dj double(8,2) DEFAULT 0.00,\
Lxfs varchar(64),\
Ssdq varchar(64), \
Kzsl int DEFAULT 0,\
On1m int unsigned DEFAULT 0,\
On15d int unsigned DEFAULT 0,\
Du15d int unsigned DEFAULT 0,\
Du1m int unsigned DEFAULT 0,\
Xgsj datetime,\
Bz varchar(64),\
primary key(id),\
unique key(Khmc),\
key(Fatherid),\
key(Jlxm))")
bool CreateKhTbl()
{
	return CreateTbl(CREATE_KH_TBL);
}

#define CREATE_SIM_TBL _T("CREATE TABLE IF NOT EXISTS sim_tbl(id int unsigned not null auto_increment,\
Jrhm char(16) NOT NULL,\
Iccid char(20) NOT NULL,\
Dxzh varchar(64) NOT NULL,\
Zt int default 19,\
Llchm varchar(32) NOT NULL,\
Llclx varchar(32),\
Jlxm char(32),\
Khid01 int unsigned default 0,\
Khid02 int unsigned default 0, \
Xsrq date,\
Jhrq date,\
Xfrq date,\
Dqrq date,\
Zxrq date,\
Bz varchar(64),\
primary key(id),\
unique key(Jrhm),\
key(Khid01),\
key(Khid02))")
bool CreateSimTbl()
{
	return CreateTbl(CREATE_SIM_TBL);
}

#define CREATE_KHJL_TBL _T("CREATE TABLE IF NOT EXISTS khjl_tbl(id int unsigned not null auto_increment,\
Jlxm char(16) NOT NULL,\
Lxfs char(20) NOT NULL,\
Xgsj datetime,\
Bz varchar(64),\
primary key(id),\
unique key(Jlxm))")
bool CreateKhjlTbl()
{
	return CreateTbl(CREATE_KHJL_TBL);
}

#define CREATE_LLC_TBL _T("CREATE TABLE IF NOT EXISTS llc_tbl(id int unsigned not null auto_increment,\
Llchm char(16) NOT NULL,\
Llclx char(8) NOT NULL,\
Dxzh varchar(64) NOT NULL,\
Kzsl int DEFAULT 0,\
Kysl int DEFAULT 0,\
Xgsj datetime,\
Bz varchar(64),\
primary key(id),\
unique key(Llchm))")
bool CreateLlcTbl()
{
	return CreateTbl(CREATE_LLC_TBL);
}

#define CREATE_LLTC_TBL _T("CREATE TABLE IF NOT EXISTS lltc_tbl(id int unsigned not null auto_increment,\
Tcmc char(16) NOT NULL,\
Tcfl char(8) NOT NULL,\
Xgsj datetime,\
Bz varchar(64),\
primary key(id),\
unique key(Tcmc))")
bool CreateLltcTbl()
{
	return CreateTbl(CREATE_LLTC_TBL);
}

#define CREATE_SSDQ_TBL _T("CREATE TABLE IF NOT EXISTS ssdq_tbl(id int unsigned not null auto_increment,\
Ssdq varchar(64) NOT NULL,\
Xgsj datetime,\
Bz varchar(64),\
primary key(id),\
unique key(Ssdq))")
bool CreateSsdqTbl()
{
	return CreateTbl(CREATE_SSDQ_TBL);
}

#define CREATE_DXZH_TBL _T("CREATE TABLE IF NOT EXISTS dxzh_tbl(id int unsigned not null auto_increment,\
Dxzh varchar(64) NOT NULL,\
User varchar(64),\
Password varchar(64),\
MKey varchar(64),\
Xgsj datetime,\
Bz varchar(64),\
primary key(id),\
unique key(Dxzh))")
bool CreateDxzhTbl()
{
	return CreateTbl(CREATE_DXZH_TBL);
}

#define CREATE_LOG_TBL _T("CREATE TABLE IF NOT EXISTS log_tbl(id int unsigned not null auto_increment,\
Opname varchar(32) not null,\
Userid int unsigend not null,\
Requesttime datatime(),\
Status varchar(16) not null,\
Respondtime datatime(),\
Respondmsg varchar(32),\
Transid varchar(32),\
Nctime dataime(),\
Ncmsg varchar(32),\
Result varchar(64),\
primary key(id),\
unique key(Transid))")
bool CreateLogTbl()
{
	return CreateTbl(CREATE_LOG_TBL);
}

#define CREATE_STATISTICS_TBL _T("CREATE TABLE IF NOT EXISTS statistic_tbl(id int unsigned not null auto_increment,\
type int not null default 0,\
ssid int unsigned not null default 0,\
Total int unsigned default 0,\
Xssl int unsigned default 0,\
Onusing int unsigned default 0,\
Zx int unsigned default 0,\
On1m int unsigned default 0,\
On15d int unsigned default 0,\
Du15d int unsigned default 0,\
Du1m int unsigned default 0,\
primary key(id),\
unique key(ssid))")
bool CreateStatisticsTbl()
{
	return CreateTbl(CREATE_STATISTICS_TBL);
}
