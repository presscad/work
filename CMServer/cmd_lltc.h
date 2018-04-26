#pragma once

#define LLTC_DATA	0X06
typedef enum
{
	LLTC_ADD = 0X01,
	LLTC_LIST,
}SUBCMD_LLTC;

bool cmd_lltc(msgpack::object* pRootArray, BUFFER_OBJ* bobj);