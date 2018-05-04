#pragma once

#define CARD_STATUS_DATA 0X11

typedef enum
{
	CS_QUERY_JRHM = 0X01,
	CS_QUERY_ICCID,
}SUBCMD_CART_STATUS;

bool cmd_cardstatus(msgpack::object* pRootArray, BUFFER_OBJ* bobj);