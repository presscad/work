#pragma once

#define POOL_LIST_DATA 0X12

typedef enum
{
	PL_GET_LIST = 0X01,// �����û�����ѯ�������б��������
	PL_LLC_INFO, // ���������غŻ�ȡ���������س�Ա�б��������
	PL_LLC_POOLQRY, // ��ѯָ�����������ص�������ʹ�����
}SUBCMD_POOL_LIST;

bool cmd_poolinfo(msgpack::object* pRootArray, BUFFER_OBJ* bobj);