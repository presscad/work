#pragma once

#define KH_DATA	0X03
typedef enum
{
	KH_ADD = 0X01,
	KH_COUNT,
	KH_QUERY,
	KH_LIST,
	KH_SIM_LIST,
	KH_SIM_XSRQ,
	KH_SIM_USING,
	KH_SIM_ON15D,
	KH_SIM_ON1M,
	KH_SIM_DU1M,
	KH_SIM_DU15D,
	KH_SELECT_BY_NAME,
}SUBCMD_KH;

bool cmd_kh(msgpack::object* pRootArray, BUFFER_OBJ* bobj);