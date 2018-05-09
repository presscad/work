#pragma once

#define SIM_DATA	0X02
typedef enum
{
	SIM_ADD = 0X01,
	SIM_LIST,
	SIM_JRHM,
	SIM_TOTAL,
}SUBCMD_SIM;

bool cmd_sim(msgpack::object* pRootArray, BUFFER_OBJ* bobj);