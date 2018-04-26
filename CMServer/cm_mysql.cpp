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

	if (NULL == mysql_real_connect(*pMysql, _T("localhost"), DB_USER, DB_PWD, DB_NAME, 3306, NULL, 0))
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
		return false;
	}

	if (mysql_affected_rows(pMysql) == 0)
	{
		return false;
	}

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
		Mysql_BackToPool(pMysql);
		return false;
	}
	Mysql_BackToPool(pMysql);
	return true;
}

#define CREATE_USER_TBL _T("CREATE TABLE IF NOT EXISTS user_tbl(id int unsigned not null auto_increment,\
User char(32) NOT NULL,\
Password char(41) NOT NULL,\
Authority int NOT NULL,\
Usertype int NOT NULL,\
Fatherid int unsigned DEFAULT 0,\
Dj double(8,2) DEFAULT 0.00,\
Xgsj date,\
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
On1m int unsigned DEFAULT 0,\
On15d int unsigned DEFAULT 0,\
Du15d int unsigned DEFAULT 0,\
Du1m int unsigned DEFAULT 0,\
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
Llchm varchar(16) NOT NULL,\
Llclx varchar(8),\
Khid01 int unsigned default 0,\
Khid02 int unsigned default 0, \
Xsrq date,\
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
Bz varchar(64),\
primary key(id),\
unique key(Llchm))")
bool CreateLlcTbl()
{
	return CreateTbl(CREATE_LLC_TBL);
}

#define CREATE_LLTC_TBL _T("CREATE TABLE IF NOT EXISTS llc_tbl(id int unsigned not null auto_increment,\
Tcmc char(16) NOT NULL,\
Tcfl char(8) NOT NULL,\
Bz varchar(64),\
primary key(id),\
unique key(Tcmc))")
bool CreateLltcTbl()
{
	return CreateTbl(CREATE_LLTC_TBL);
}

#define CREATE_SSDQ_TBL _T("CREATE TABLE IF NOT EXISTS ssdq_tbl(id int unsigned not null auto_increment,\
Ssdq varchar(64) NOT NULL,\
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
Key varchar(64),\
Bz varchar(64),\
primary key(id),\
unique key(Dxzh))")
bool CreateDxzhTbl()
{
	return CreateTbl(CREATE_DXZH_TBL);
}
