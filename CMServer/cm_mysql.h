#pragma once
#include <mysql.h>

MYSQL* Mysql_AllocConnection();

void Mysql_BackToPool(MYSQL* pMysql);