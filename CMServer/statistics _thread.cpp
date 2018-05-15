#include "stdafx.h"
#include "statistics _thread.h"
#include "client_signaldata.h"
#include "cm_mysql.h"

void DueTostatistical();
void SimTotal();

unsigned int _stdcall statistics_func(LPVOID pVoid)
{
	HANDLE hWaitEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	while (WaitForSingleObject(hWaitEvent, 1000 * 60 * 60) != WSA_WAIT_FAILED)
	{
		DueTostatistical();
		SimTotal();
		// 根据已到期的天数，自动做注销
	}
	return 0;
}

void DueTostatistical()
{
	const TCHAR* pSql = _T("SELECT id,Fatherid FROM kh_tbl");
	TCHAR sql[1024];
	memset(sql, 0x00, sizeof(sql));
	_stprintf_s(sql, sizeof(sql), pSql);

	MYSQL* pMysql = Mysql_AllocConnection();
	if (NULL == pMysql)
	{
		return;
	}

	MYSQL_RES* res = NULL;
	size_t len = _tcslen(sql);
	if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
	{
		_tprintf_s(_T("%s\n"), mysql_error(pMysql));
		Mysql_BackToPool(pMysql);
		return;
	}

	res = mysql_store_result(pMysql);
	if (NULL == res)
	{
		Mysql_BackToPool(pMysql);
		return;
	}

	my_ulonglong nErr = mysql_affected_rows(pMysql);
	if ((my_ulonglong)0 == nErr)
	{
		mysql_free_result(res);
		res = NULL;
		Mysql_BackToPool(pMysql);
		return;
	}
	Mysql_BackToPool(pMysql);

	MYSQL_ROW row = mysql_fetch_row(res);

	while (row)
	{
		unsigned int nKhid = 0;
		sscanf_s(row[0], "%u", &nKhid);

		// On1m,On15d,Du15d,Du1m
		if (_tstoi(row[1]) == 1)
		{
			pSql = _T("SELECT SUM(CASE WHEN Zt=1 THEN 1 ELSE 0 END) AS 'Onusing',\
SUM(CASE WHEN Zt=150001 THEN 1 ELSE 0 END) AS 'Zx',\
SUM(CASE WHEN dqrq>CURDATE() AND dqrq<DATE_ADD(CURDATE(), INTERVAL 15 DAY) THEN 1 ELSE 0 END) AS 'On15d',\
SUM(CASE WHEN dqrq>CURDATE() AND dqrq<DATE_ADD(CURDATE(), INTERVAL 1 MONTH) THEN 1 ELSE 0 END) AS 'On1m',\
SUM(CASE WHEN dqrq<CURDATE() AND dqrq>DATE_SUB(CURDATE(), INTERVAL 1 MONTH) THEN 1 ELSE 0 END) AS 'Du1m',\
SUM(CASE WHEN dqrq<CURDATE() AND dqrq>DATE_SUB(CURDATE(), INTERVAL 15 DAY) THEN 1 ELSE 0 END) AS 'Du15d' FROM sim_tbl WHERE Khid01=%u");
		}
		else if (_tstoi(row[1]) > 1)
		{
			pSql = _T("SELECT SUM(CASE WHEN Zt=1 THEN 1 ELSE 0 END) AS 'Onusing',\
SUM(CASE WHEN Zt=150001 THEN 1 ELSE 0 END) AS 'Zx',\
SUM(CASE WHEN dqrq>CURDATE() AND dqrq<DATE_ADD(CURDATE(), INTERVAL 15 DAY) THEN 1 ELSE 0 END) AS 'On15d',\
SUM(CASE WHEN dqrq>CURDATE() AND dqrq<DATE_ADD(CURDATE(), INTERVAL 1 MONTH) THEN 1 ELSE 0 END) AS 'On1m',\
SUM(CASE WHEN dqrq<CURDATE() AND dqrq>DATE_SUB(CURDATE(), INTERVAL 1 MONTH) THEN 1 ELSE 0 END) AS 'Du1m',\
SUM(CASE WHEN dqrq<CURDATE() AND dqrq>DATE_SUB(CURDATE(), INTERVAL 15 DAY) THEN 1 ELSE 0 END) AS 'Du15d' FROM sim_tbl WHERE Khid02=%u");
		}

		unsigned int Onusing = 0,
			On15d = 0,
			On1m = 0,
			Du1m = 0,
			Du15d = 0;

		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, nKhid);

		MYSQL* pMysql1 = Mysql_AllocConnection();
		if (NULL == pMysql1)
		{
			row = mysql_fetch_row(res);
			continue;
		}

		MYSQL_RES* res1 = NULL;
		size_t len1 = _tcslen(sql);
		if (0 != mysql_real_query(pMysql1, sql, (ULONG)len1))
		{
			Mysql_BackToPool(pMysql1);
			row = mysql_fetch_row(res);
			continue;
		}

		res1 = mysql_store_result(pMysql1);
		if (NULL == res1)
		{
			Mysql_BackToPool(pMysql1);
			row = mysql_fetch_row(res);
			continue;
		}

		my_ulonglong nErr1 = mysql_affected_rows(pMysql1);
		if ((my_ulonglong)0 == nErr1)
		{
			mysql_free_result(res1);
			res1 = NULL;
			Mysql_BackToPool(pMysql1);
			row = mysql_fetch_row(res);
			continue;
		}

		Mysql_BackToPool(pMysql1);

		MYSQL_ROW row1 = mysql_fetch_row(res1);

		if (NULL != row1[0])
		{
			sscanf_s(row1[0], "%u", &Onusing);
			sscanf_s(row1[1], "%u", &On15d);
			sscanf_s(row1[2], "%u", &On1m);
			sscanf_s(row1[3], "%u", &Du1m);
			sscanf_s(row1[4], "%u", &Du15d);
		}
		mysql_free_result(res1);

		pSql = _T("UPDATE kh_tbl set On15d=%u,On1m=%u,Du1m=%u,Du15d=%u WHERE id=%u");
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, On15d, On1m, Du1m, Du15d, nKhid);

		MYSQL* pMysql2 = Mysql_AllocConnection();
		if (NULL == pMysql2)
		{
			row = mysql_fetch_row(res);
			continue;
		}

		size_t len2 = _tcslen(sql);
		if (0 != mysql_real_query(pMysql2, sql, (ULONG)len2))
		{
			Mysql_BackToPool(pMysql2);
			row = mysql_fetch_row(res);
			continue;
		}
		Mysql_BackToPool(pMysql2);

		row = mysql_fetch_row(res);
	}
	mysql_free_result(res);
	return;
}

void SimTotal()
{
	const TCHAR* pSql = _T("SELECT SUM(1) AS 'SimTotal',\
SUM(CASE WHEN Khid01=0 THEN 1 ELSE 0 END) AS 'Nousing',\
SUM(CASE WHEN Zt=1 THEN 1 ELSE 0 END) AS 'Onusing',\
SUM(CASE WHEN Zt=150001 THEN 1 ELSE 0 END) AS 'Zx',\
SUM(CASE WHEN dqrq>CURDATE() AND dqrq<DATE_ADD(CURDATE(), INTERVAL 15 DAY) THEN 1 ELSE 0 END) AS 'On15d',\
SUM(CASE WHEN dqrq>CURDATE() AND dqrq<DATE_ADD(CURDATE(), INTERVAL 1 MONTH) THEN 1 ELSE 0 END) AS 'On1m',\
SUM(CASE WHEN dqrq<CURDATE() AND dqrq>DATE_SUB(CURDATE(), INTERVAL 1 MONTH) THEN 1 ELSE 0 END) AS 'Du1m',\
SUM(CASE WHEN dqrq<CURDATE() AND dqrq>DATE_SUB(CURDATE(), INTERVAL 15 DAY) THEN 1 ELSE 0 END) AS 'Du15d' FROM sim_tbl");
	TCHAR sql[1024];
	memset(sql, 0x00, sizeof(sql));
	_stprintf_s(sql, sizeof(sql), pSql);

	MYSQL* pMysql = Mysql_AllocConnection();
	if (NULL == pMysql)
	{
		return;
	}

	MYSQL_RES* res = NULL;
	size_t len = _tcslen(sql);
	if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
	{
		_tprintf_s(_T("%s\n"), mysql_error(pMysql));
		Mysql_BackToPool(pMysql);
		return;
	}

	res = mysql_store_result(pMysql);
	if (NULL == res)
	{
		Mysql_BackToPool(pMysql);
		return;
	}

	my_ulonglong nErr = mysql_affected_rows(pMysql);
	if ((my_ulonglong)0 == nErr)
	{
		mysql_free_result(res);
		res = NULL;
		Mysql_BackToPool(pMysql);
		return;
	}

	MYSQL_ROW row = mysql_fetch_row(res);

	unsigned int Simtotal = 0,
		Nousing = 0,
		Onusing = 0,
		Zx = 0,
		On15d = 0,
		On1m = 0,
		Du1m = 0,
		Du15d = 0;

	if (NULL != row[0])
	{
		sscanf_s(row[0], "%u", &Simtotal);
		sscanf_s(row[1], "%u", &Nousing);
		sscanf_s(row[2], "%u", &Onusing);
		sscanf_s(row[3], "%u", &Zx);
		sscanf_s(row[4], "%u", &On15d);
		sscanf_s(row[5], "%u", &On1m);
		sscanf_s(row[6], "%u", &Du1m);
		sscanf_s(row[7], "%u", &Du15d);
	}
	mysql_free_result(res);

	//pSql = _T("UPDATE statistic_tbl SET Total=%d,Onusing=%d,Zx=%d,On1m=%d,On15d=%d,Du15d=%d,Du1m=%d");
	pSql = _T("INSERT INTO statistic_tbl(id, type, ssid, Total, Xssl, Onusing, Zx, On1m, On15d, Du15d, Du1m) VALUES(null, 1, 1, %u, %u, %u, %u, %u, %u, %u, %u)\
  ON DUPLICATE KEY UPDATE Total=%u,Xssl=%u,Onusing=%u,Zx=%u,On1m=%u,On15d=%u,Du15d=%u,Du1m=%u");
	memset(sql, 0x00, sizeof(sql));
	_stprintf_s(sql, sizeof(sql), pSql, Simtotal, Simtotal - Nousing, Onusing, Zx, On1m, On15d, Du15d, Du1m, Simtotal, Simtotal - Nousing, Onusing, Zx, On1m, On15d, Du15d, Du1m);

	len = _tcslen(sql);
	if (0 != mysql_real_query(pMysql, sql, (ULONG)len))
	{
		_tprintf_s(_T("%s\n"), mysql_error(pMysql));
		Mysql_BackToPool(pMysql);
		return;
	}

	Mysql_BackToPool(pMysql);
}