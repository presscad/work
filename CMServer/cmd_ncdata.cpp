#include "stdafx.h"
#include "client_signaldata.h"
#include "cmd_ncdata.h"
//#include "tinyxml2.h"
#include "global_data.h"
#include "cmd_error.h"
#include "nc_requestpost.h"
#include "nc_postcompletion.h"
#include "client_mem.h"

extern TCHAR* Utf8ConvertAnsi(const TCHAR* strIn, int inLen);

bool DoNotifyContractRoot(tinyxml2::XMLElement* root);
bool DoWanrningContractRoot(tinyxml2::XMLElement* RootElment);
void doNcResponse(BUFFER_OBJ* bobj);

bool doNcData(BUFFER_OBJ* bobj)
{
	TCHAR* pResponData = Utf8ConvertAnsi(bobj->data, bobj->dwRecvedCount);
	doNcResponse(bobj); // �������ͷ�������
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(pResponData))
	{
		delete pResponData;
		return false;
	}
	delete pResponData;
	
	return true;
}

bool doNcData(tinyxml2::XMLDocument& doc)
{
	tinyxml2::XMLElement* RootElment = doc.RootElement();
	const TCHAR* rootName = RootElment->GetText();
	if (strcmp(rootName, "NotifyContractRoot") == 0)
	{
		return DoNotifyContractRoot(RootElment);
	}
	else if (strcmp(rootName, "NotifyContractRoot") == 0)
	{
		return DoWanrningContractRoot(RootElment);
	}

	return true;
}

//<?xml version = "1.0" encoding = "utf-8"?>
//<NotifyContractRoot>
//<!--�������� 1������ 2���澯-->
//<TYPE>1< / TYPE>
//<!- ��ˮ��->
//	<GROUP_TRANSACTIONID>1000000252201605131983898593< / GROUP_TRANSACTIONID>
//	<!- ������Ϣ�����磺������ǰ�˴���ȣ��жϱ��β����Ƿ񿢹�Ҳ������ڵ�->
//	<STATUSINFO>����< / STATUSINFO>
//	<!- ����->
//	<ACCNBR>1064915901733< / ACCNBR>
//	<!- ������ʱ��->
//	<SENDDT>20160513153805< / SENDDT>
//	<!--�������ͣ��������������ڵ�6��-->
//	<ACCEPTTYPE>8< / ACCEPTTYPE>
//	<!- ��������->
//	<ACCEPTMSG>����:1064915901733������ͣ������< / ACCEPTMSG>
//	<!- ��Ӧʱ��->
//	<STATUSDT>20160513153905< / STATUSDT>
//	<!- ��Ӧ��Ϣ�����д���Ҳ���������, ���ڵ㲻��Ϊ�жϿ���������->
//	<RESULTMSG>�ɹ�< / RESULTMSG>
//	< / NotifyContractRoot>

bool DoNotifyContractRoot(tinyxml2::XMLElement* root)
{
	tinyxml2::XMLElement* TYPE = root->FirstChildElement(); // <TYPE>1</TYPE> 

	tinyxml2::XMLElement* GROUP_TRANSACTIONID = TYPE->NextSiblingElement(); // <GROUP_TRANSACTIONID>1000000252201605131983898593</GROUP_TRANSACTIONID>
	const TCHAR* pGROUP_TRANSACTIONID = GROUP_TRANSACTIONID->GetText();

	tinyxml2::XMLElement* STATUSINFO = GROUP_TRANSACTIONID->NextSiblingElement(); // <STATUSINFO>����</STATUSINFO>
	const TCHAR* pSTATUSINFO = STATUSINFO->GetText();

	tinyxml2::XMLElement* ACCNBR = STATUSINFO->NextSiblingElement(); // <ACCNBR>1064915901733</ACCNBR>
	const TCHAR* pACCNBR = ACCNBR->GetText();

	tinyxml2::XMLElement* SENDDT = ACCNBR->NextSiblingElement(); // <SENDDT>20160513153805</SENDDT>

	tinyxml2::XMLElement* ACCEPTTYPE = SENDDT->NextSiblingElement(); // <ACCEPTTYPE>8</ACCEPTTYPE>
	const TCHAR* pAccepttype = ACCEPTTYPE->GetText();
	int nType = atoi(pAccepttype);

	tinyxml2::XMLElement* ACCEPTMSG = ACCEPTTYPE->NextSiblingElement(); // <ACCEPTMSG>����:1064915901733������ͣ������</ACCEPTMSG>

	tinyxml2::XMLElement* STATUSDT = ACCEPTMSG->NextSiblingElement(); // <STATUSDT>20160513153905</STATUSDT>

	tinyxml2::XMLElement* RESULTMSG = STATUSDT->NextSiblingElement(); // <RESULTMSG>�ɹ�</RESULTMSG>
	const TCHAR* pRESULTMSG = RESULTMSG->GetText();

	const TCHAR* pSql = NULL;
	TCHAR sql[256];
	memset(sql, 0x00, sizeof(sql));
	// ������ˮ��ƥ��ͣ������Ӧ��Ϣ
	switch (nType)
	{
	case 1:
	{}
	break;
	case 2:
	{}
	break;
	case 3:
	{}
	break;
	case 4:
	{}
	break;
	case 5:
	{}
	break;
	case 7:
	{}
	break;
	case 8:// ͣ��
	{
		if (strcmp(pSTATUSINFO, "����") == 0) // �޸�״̬
		{
			pSql = _T("UPDATE sim_tbl SET zt=2 WHERE Jrhm='%s'");
			_stprintf_s(sql, sizeof(sql), pSql, pACCNBR);
		}
	}
	break;
	case 9:// ����
	{
		if (strcmp(pSTATUSINFO, "����") == 0) // �޸�״̬
		{
			pSql = _T("UPDATE sim_tbl SET zt=1 WHERE Jrhm='%s'");
			_stprintf_s(sql, sizeof(sql), pSql, pACCNBR);
		}
	}
	break;
	case 10:
	{}
	break;
	case 11:
	{}
	break;
	case 12:
	{}
	break;
	case 13:
	{}
	break;
	case 14:
	{}
	break;
	default:
		break;
	}

	// ��¼��־
	pSql = _T("UPDATE log_tbl SET Status='�������',Ncmsg='%s',Nctime=now(),Result='%s' WHERE Transid='%s'");
	_stprintf_s(sql, sizeof(sql), pSql, pSTATUSINFO, pRESULTMSG, pGROUP_TRANSACTIONID);

	return true;
}

bool DoWanrningContractRoot(tinyxml2::XMLElement* RootElment)
{
	tinyxml2::XMLElement* pType = RootElment->FirstChildElement("TYPE");
	if (NULL == pType)
	{
		_tprintf(_T("���յ���xml�ڵ�RspType��ʽ����\n"));
		return false;
	}
	const TCHAR* pcType = pType->GetText();
	_tprintf(_T("%s\n"), pcType);
	if (atoi(pcType) != 2)
	{
		return false;
	}
	tinyxml2::XMLElement* pAccnbr = pType->NextSiblingElement("ACCNBR");
	tinyxml2::XMLElement* pWarningtype = pAccnbr->NextSiblingElement("WARNINGTYPE");
	tinyxml2::XMLElement* pWarningtypemsg = pWarningtype->NextSiblingElement("WARNINGTYPEMSG");
	return true;
}

void doNcResponse(BUFFER_OBJ* bobj)
{
	const TCHAR* pData = _T("<?xml version = \"1.0\" encoding = \"UTF8\"?><ContractRoot><SUCCESS>true</SUCCESS><NOTIFY_CODE>000000</NOTIFY_CODE><RESULTMSG>���óɹ�</RESULTMSG></ContractRoot>");
	bobj->dwRecvedCount = (DWORD)strlen(pData);
	memcpy_s(bobj->data, bobj->datalen, pData, bobj->dwRecvedCount);
	bobj->SetIoRequestFunction(NC_SendCompFailed, NC_SendCompSuccess);
	if (!Nc_PostSend((SOCKET_OBJ*)bobj->pRelateClientSock, bobj))
	{
		CMCloseSocket(bobj->pRelateClientSock);
		freeSObj((SOCKET_OBJ*)bobj->pRelateClientSock);
		freeBObj(bobj);
	}
}