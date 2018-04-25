#pragma once

#define EXCEL_LOAD	0X08
typedef enum
{
	EXCEL_SIM = 0X01,
	EXCEL_XSQD,
	EXCEL_ZTXX,
	EXCEL_XFQD,
	EXCEL_TKQD,
	EXCEL_ZXQD,
}SUBCMD_EXCEL;

bool cmd_load(msgpack::object* pRootArray, BUFFER_OBJ* bobj);