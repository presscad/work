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

	if (NULL == mysql_real_connect(*pMysql, _T("localhost"), DB_USER, DB_PWD, DB_NAME, 0, NULL, 0))
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