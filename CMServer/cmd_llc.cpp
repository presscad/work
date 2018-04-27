#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_llc.h"
#include "cm_mysql.h"
#include "global_data.h"
#include "cmd_error.h"
#include "cmd_data.h"

bool cmd_llc(msgpack::object* pRootArray, BUFFER_OBJ* bobj)
{
	bobj->nSubCmd = (pRootArray++)->as<int>();
	unsigned int nId = (pRootArray++)->as<unsigned int>();
	unsigned int nUsertype = (pRootArray++)->as<unsigned int>();
	switch (bobj->nSubCmd)
	{
	case LLC_ADD:
	{
		msgpack::object* pDataArray = (pRootArray++)->via.array.ptr;
		msgpack::object* pArray = (pDataArray++)->via.array.ptr;
		std::string strLlchm = (pArray++)->as<std::string>();
		std::string strLlclx = (pArray++)->as<std::string>();
		std::string strDxzh = (pArray++)->as<std::string>();
		std::string strBz = (pArray++)->as<std::string>();

		if (nUsertype != 1)
		{
			error_info(bobj, _T("��Ȩ��"));
			return cmd_error(bobj);
		}

		const TCHAR* pSql = _T("INSERT INTO llc_tbl (id,Dxzh,Llchm,Llclx,Xgsj,Bz) VALUES(null,'%s','%s','%s',now(),'%s')");
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, strDxzh.c_str(), strLlchm.c_str(), strLlclx.c_str(), strBz.c_str());

		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("�������ݿ�ʧ��"));
			return cmd_error(bobj);
		}

		if (!InsertIntoTbl(sql, pMysql, bobj))
		{
			UINT uError = mysql_errno(pMysql);
			if (uError == 1062)
			{
				error_info(bobj, _T("�˺��Ѵ���"));
			}
			else
			{
				error_info(bobj, _T("���ݿ��쳣 ErrorCode = %08x, ErrorMsg = %s"), uError, mysql_error(pMysql));
			}
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj);
		}
		my_ulonglong nIndex = mysql_insert_id(pMysql); // �����ӵ��û���id

		pSql = _T("SELECT %s FROM llc_tbl WHERE id=%u");
		memset(sql, 0x00, sizeof(sql));
		_stprintf_s(sql, sizeof(sql), pSql, LLC_SELECT, (unsigned int)nIndex);

		MYSQL_RES* res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj);
		}

		Mysql_BackToPool(pMysql);

		MYSQL_ROW row = mysql_fetch_row(res);
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(4);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(0);
		_msgpack.pack_array(1);
		ParserLlc(_msgpack, row, LLC_SELECT_SIZE);
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	case LLC_LIST:
	{
		int nIndex = (pRootArray++)->as<int>();
		int nPagesize = (pRootArray++)->as<int>();
		int nAB = (pRootArray++)->as<int>();
		int nKeyid = (pRootArray++)->as<int>();

		if (nUsertype != 1)
		{
			error_info(bobj, _T("��Ȩ��"));
			return cmd_error(bobj, nIndex);
		}

		const TCHAR* pSql = NULL;
		TCHAR sql[256];
		memset(sql, 0x00, sizeof(sql));

		pSql = _T("SELECT COUNT(*) AS num FROM llc_tbl");
		_stprintf_s(sql, sizeof(sql), pSql);

		MYSQL* pMysql = Mysql_AllocConnection();
		if (NULL == pMysql)
		{
			error_info(bobj, _T("�������ݿ�ʧ��"));
			return cmd_error(bobj, nIndex);
		}

		MYSQL_RES* res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj, nIndex);
		}
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);

		unsigned int nNum = 0;
		sscanf_s(row[0], "%u", &nNum);

		if (nAB == 0) // ��ҳ
		{
			pSql = _T("SELECT %s FROM llc_tbl WHERE id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, LLC_SELECT, nKeyid, nPagesize);
		}
		else if (nAB == 1)
		{
			pSql = _T("SELECT %s FROM llc_tbl WHERE id<%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, LLC_SELECT, nKeyid, nPagesize);
		}
		else if (nAB == 2)
		{
			pSql = _T("SELECT %s FROM llc_tbl WHERE id>%u LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, LLC_SELECT, nKeyid, nPagesize);
		}
		else
		{
			unsigned int nTemp = nNum % nPagesize;
			pSql = _T("SELECT %s FROM llc_tbl ORDER BY id desc LIMIT %d");
			_stprintf_s(sql, sizeof(sql), pSql, LLC_SELECT, nPagesize);
		}

		res = NULL;
		if (!SelectFromTbl(sql, pMysql, bobj, &res))
		{
			Mysql_BackToPool(pMysql);
			return cmd_error(bobj, nIndex);
		}

		Mysql_BackToPool(pMysql);

		unsigned int nRows = (unsigned int)mysql_num_rows(res);
		row = mysql_fetch_row(res);
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(6);
		_msgpack.pack(bobj->nCmd);
		_msgpack.pack(bobj->nSubCmd);
		_msgpack.pack(nIndex);
		_msgpack.pack(0);
		_msgpack.pack(nNum);
		_msgpack.pack_array(nRows);
		while (row)
		{
			ParserLlc(_msgpack, row, LLC_SELECT_SIZE);
			row = mysql_fetch_row(res);
		}
		mysql_free_result(res);

		DealTail(sbuf, bobj);
	}
	break;
	default:
		break;
	}
	return true;
}