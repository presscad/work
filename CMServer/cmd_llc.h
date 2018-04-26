#pragma once

#define LLC_DATA	0X05
typedef enum
{
	LLC_ADD = 0X01,
	LLC_LIST,
}SUBCMD_LLC;

bool cmd_llc(msgpack::object* pRootArray, BUFFER_OBJ* bobj);