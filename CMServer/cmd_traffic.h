#pragma once

#define TRAFFIC_DATA 0X12

typedef enum
{
	TD_QUERY_TRAFFIC_TODAY = 0X01,// ������ѯ(����)
	TD_QUERY_TRAFFIC, // ������ѯ(����)
	TD_QUERY_TRAFFIC_DATE, // ������ѯ(ʱ���)
}SUBCMD_TRAFFIC_DATA;

bool cmd_traffic(msgpack::object* pRootArray, BUFFER_OBJ* bobj);