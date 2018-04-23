#include "stdafx.h"
#include "cmd_proxy.h"

bool cmd_proxy()
{
	if (1)
	{
		const TCHAR* pSql = _T("SELECT a.*,b.User FROM (SELECT COUNT(*) AS 'total' FROM sim_tbl WHERE proxy01=b.id) a LEFT JOIN (SELECT * FROM user_tbl) b ON b.usertype=2");
		pSql = _T("SELECT a.User,b.* FROM (SELECT * FROM user_tbl WHERE usertype=2 LIMIT 10) a LEFT JOIN (SELECT COUNT(*) FROM sim_tbl WHERE proxy01=a.id) b ON ");
		pSql = _T("select a.*,b.* from ()");

		_T("select a.*,b.* from (select * from test01_tbl where usertype=1 limit 10) a left join (select proxy01,count(*) num from test02_tbl group by proxy01) b on b.proxy01=a.id");
		_T("SELECT a.*,b.jlxm,b.lxfs FROM (SELECT COUNT(*) AS 'sim_total',\
SUM(CASE WHEN zt='ÔÚÓÃ' THEN 1 ELSE 0 END) AS 'sim_using',\
SUM(CASE WHEN dqrq>CURDATE() AND dqrq<DATE_ADD(CURDATE(),INTERVAL 15 DAY) THEN 1 ELSE 0 END) AS 'use_15d',\
SUM(CASE WHEN dqrq>CURDATE() AND dqrq<DATE_ADD(CURDATE(),INTERVAL 1 MONTH) THEN 1 ELSE 0 END) AS 'use_1m',\
SUM(CASE WHEN dqrq<CURDATE() AND dqrq>DATE_SUB(CURDATE(),INTERVAL 1 MONTH) THEN 1 ELSE 0 END) AS 'due_1m',\
SUM(CASE WHEN dqrq<CURDATE() AND dqrq>DATE_SUB(CURDATE(),INTERVAL 15 DAY) THEN 1 ELSE 0 END) AS 'due_15d' \
FROM sim_tbl WHERE khmc='%s') a LEFT JOIN kh_tbl b ON b.khmc='%s'");
	}
	else if (2)
	{

	}
	return true;
}