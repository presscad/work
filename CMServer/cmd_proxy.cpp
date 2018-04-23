#include "stdafx.h"
#include "cmd_proxy.h"

bool cmd_proxy()
{
	if (1)
	{
		// select t1.User, count(t2.Proxy01_id) count from(select * from user_tbl where Usertype = 2) t1 left join sim_tbl t2 on t1.id = t2.Proxy01_id group by t1.User;
	}
	else if (2)
	{

	}
	return true;
}