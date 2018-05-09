#pragma once
#include <mysql.h>

MYSQL* Mysql_AllocConnection();

void Mysql_BackToPool(MYSQL* pMysql);

bool InsertIntoTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj);

bool Trans_InsertIntoTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj);

bool SelectFromTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj, MYSQL_RES** res);

bool UpdateTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj);

bool Trans_UpdateTbl(const TCHAR* sql, MYSQL* pMysql, BUFFER_OBJ* bobj);

bool CreateDB();

bool CreateUserTbl();

bool CreateKhTbl();

bool CreateSimTbl();

bool CreateKhjlTbl();

bool CreateLlcTbl();

bool CreateLltcTbl();

bool CreateSsdqTbl();

bool CreateDxzhTbl();

bool CreateStatisticsTbl();
