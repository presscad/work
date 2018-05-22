#pragma once

#define POOL_LIST_DATA 0X12

typedef enum
{
	PL_GET_POOL_LIST = 0X01,// 流量池列表查询
	PL_GET_POOL_MEM_LIST,// 流量池成员列表查询
	PL_GET_POOL_MEM,// 流量单个成员查询接口
	PL_POOL_QRY,// 流量池总使用量查询
	PL_POOL_MEM_QRY,// 流量池单个成员使用量查询
	LLC_QRY,
}SUBCMD_POOL_LIST;

bool cmd_poolinfo(msgpack::object* pRootArray, BUFFER_OBJ* bobj);