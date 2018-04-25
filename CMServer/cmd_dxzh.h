#pragma once

#define DXZH_DATA	0X07
typedef enum
{
	DXZH_ADD = 0X01,
	DXZH_LIST,
}SUBCMD_DXZH;

bool cmd_dxzh(msgpack::object* pRootArray, BUFFER_OBJ* bobj);