#pragma once

#define KHJL_DATA	0X04
typedef enum
{
	KHJL_ADD = 0X01,
	KHJL_KH,
	KHJL_QUERY,
	KHJL_LIST,
	KHJL_SIM_XSRQ,
	KHJL_FIND,
}SUBCMD_KHJL;

bool cmd_khjl(msgpack::object* pRootArray, BUFFER_OBJ* bobj);