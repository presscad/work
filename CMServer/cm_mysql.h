#pragma once
#include <mysql.h>

MYSQL* Mysql_AllocConnection();

void Mysql_BackToPool(MYSQL* pMysql);

bool InsertIntoTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj);

bool SelectFromTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj, MYSQL_RES** res);

bool UpdateTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj);

bool CreateUserTbl();

bool CreateKhTbl();