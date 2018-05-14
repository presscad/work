#pragma once

#define TRAFFIC_DATA 0X12

typedef enum
{
	TD_QUERY_TRAFFIC_TODAY = 0X01,// 流量查询(当天)
	TD_QUERY_TRAFFIC, // 流量查询(当月)
	TD_QUERY_TRAFFIC_DATE, // 流量查询(时间段)
}SUBCMD_TRAFFIC_DATA;

bool cmd_traffic(msgpack::object* pRootArray, BUFFER_OBJ* bobj);