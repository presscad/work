#pragma once

#define SSDQ_DATA	0X09
typedef enum
{
	SSDQ_ADD = 0X01,
	SSDQ_LIST,
}SUBCMD_SSDQ;

bool cmd_ssdq(msgpack::object* pRootArray, BUFFER_OBJ* bobj);