#pragma once

#define POOL_LIST_DATA 0X12

typedef enum
{
	PL_GET_POOL_LIST = 0X01,// �������б��ѯ
	PL_GET_POOL_MEM_LIST,// �����س�Ա�б��ѯ
	PL_GET_POOL_MEM,// ����������Ա��ѯ�ӿ�
	PL_POOL_QRY,// ��������ʹ������ѯ
	PL_POOL_MEM_QRY,// �����ص�����Աʹ������ѯ
	LLC_QRY,
}SUBCMD_POOL_LIST;

bool cmd_poolinfo(msgpack::object* pRootArray, BUFFER_OBJ* bobj);