#include "stdafx.h"
#include "cmd_proxy.h"

bool cmd_proxy()
{
	if (1)
	{
		_T("select a.User,(select count(1) from sim_tbl b where b.Proxy01_id=a.id) simcount,(select count(1) from sim_tbl b where b.Proxy01_id=a.id) simusingcount from user_tbl a limit 0,10;");
	}
	else if (2)
	{

	}
	return true;
}