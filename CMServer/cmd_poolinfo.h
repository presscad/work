#pragma once

#define POOL_LIST_DATA 0X12

typedef enum
{
	LLC_QRY = 0X01,
	PL_GET_POOL_LIST,// �������б��ѯ
	PL_GET_POOL_MEM_LIST,// �����س�Ա�б��ѯ
	PL_GET_POOL_MEM,// ����������Ա��ѯ�ӿ�
	PL_POOL_QRY,// ��������ʹ������ѯ
	PL_POOL_MEM_QRY,// �����ص�����Աʹ������ѯ
}SUBCMD_POOL_LIST;

bool cmd_poolinfo(msgpack::object* pRootArray, BUFFER_OBJ* bobj);