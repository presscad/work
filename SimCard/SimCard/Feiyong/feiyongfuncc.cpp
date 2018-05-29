#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
#include <vector>


#import "msado15.dll" \
	no_namespace \
	rename("EOF","adoEOF") rename("DataTypeEnum","adoDataTypeEnum") \
	rename("FieldAttributeEnum", "adoFielAttributeEnum") rename("EditModeEnum", "adoEditModeEnum") \
	rename("LockTypeEnum", "adoLockTypeEnum") rename("RecordStatusEnum", "adoRecordStatusEnum") \
	rename("ParameterDirectionEnum", "adoParameterDirectionEnum")
#pragma warning(default:4146)

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const int writeRows = 50000;

inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};




std::string makeConnStr(std::string filename, bool header = true)
{
	std::stringstream stream;

	std::string hdr = header ? "YES" : "NO";    

	if(!filename.empty())       
		if(*--filename.end() == 'x')        
			stream << "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" << filename << ";Extended Properties=\"Excel 12.0 Xml;HDR=" << hdr << "\"";                    
		else        
			stream << "Provider='Microsoft.JET.OLEDB.4.0';Data Source=" << filename << ";Extended Properties=\"Excel 8.0;HDR=" << hdr << "\"";

	return stream.str();
}

std::string sqlSelectSheet(_bstr_t connStr, int sheetIndex)
{
	_ConnectionPtr pCon = NULL;
	_RecordsetPtr pSchema = NULL;

	TESTHR(pCon.CreateInstance(__uuidof(Connection)));
	TESTHR(pCon->Open(connStr, "", "", NULL));

	pSchema = pCon->OpenSchema(adSchemaTables);     
	if(sheetIndex==1)pSchema->MoveFirst();
	else if (sheetIndex == 0)pSchema->MoveNext();
	else if (sheetIndex == 2){pSchema->MoveNext(); pSchema->MoveNext();};



	/*for(int i = 0; i < sheetIndex; ++i) pSchema->MoveNext();*/

	std::string sheetName = (char*)(_bstr_t)pSchema->Fields->GetItem("TABLE_NAME")->Value.bstrVal;

	std::stringstream stream;
	stream << "SELECT * FROM [" << sheetName << "]";
	return stream.str();
}
bool bRecOpened = false;
void query(_RecordsetPtr &pRec,string strTable,string connStr,_variant_t& v1 ,bool bClose = true)
{
	
		const char * pSql = "select 缴费金额 from [%s$] where 手机号 ='%s'";
		char sql[256];
		v1.ChangeType(VT_BSTR);
		sprintf_s(sql, 256, pSql, strTable.c_str(),(char*)(_bstr_t)v1.bstrVal);
		//char * psql2= "select 手机号 from [%s$] ";
		//char sql[256];
		//v1.ChangeType(VT_BSTR);
		//sprintf_s(sql, 256, psql2, strTable.c_str());
		/*string sql="select cast(手机号 as varchar(30)) from [2017.3$]"; */
		if (bRecOpened)
		pRec->Close();
		HRESULT hr =pRec->Open(sql, connStr.c_str(), adOpenStatic, adLockOptimistic, adCmdText);
		if (FAILED(hr))
		bRecOpened = false;
		else 
			bRecOpened = true;
	
}
bool start(string strjfFile, string strxhFile)
{
	
	if(FAILED(::CoInitialize(NULL))) return false;

	_RecordsetPtr pSchema = NULL;
	_RecordsetPtr pRec = NULL;
	_RecordsetPtr pRec2 = NULL;
	_ConnectionPtr pCon1 = NULL;
	_ConnectionPtr pCon2 = NULL;
	_RecordsetPtr pSchema1 = NULL;
	string connStr1 = makeConnStr(strxhFile);
	string connStr2 = makeConnStr(strjfFile);
	/*TESTHR(pCon1.CreateInstance(__uuidof(Connection)));
	TESTHR(pCon1->Open(connStr2.c_str(), "", "", NULL));*/

	
	try{
		TESTHR(pRec2.CreateInstance(__uuidof(Recordset)));      
		TESTHR(pRec.CreateInstance(__uuidof(Recordset)));  
		TESTHR(pRec2->Open("select 接入号码,消耗明细 from [宝蓝$]", connStr2.c_str(), adOpenStatic, adLockOptimistic, adCmdText));
		int count = 0;
		pRec2->MoveFirst();
		while(!pRec2->adoEOF)
		{
			
			try{
			for(long i = 0; i < 1; ++i)
			{
				if (i ==0)
				{
					_variant_t v1=pRec2->Fields->GetItem(i)->Value;
					
					v1.ChangeType(VT_BSTR);
					query(pRec,"2018.4",connStr1,v1,false);
					//string strTable = "2018.4";
					//const char * pSql = "select 缴费金额 from [2018.3$] where 手机号 ='%s'";
					//char sql[256];
					//v1.ChangeType(VT_BSTR);
					//sprintf_s(sql, 256, pSql, strTable.c_str(),(char*)(_bstr_t)v1.bstrVal);
					////char * psql2= "select 手机号 from [%s$] ";
					////char sql[256];
					////v1.ChangeType(VT_BSTR);
					////sprintf_s(sql, 256, psql2, strTable.c_str());
					///*string sql="select cast(手机号 as varchar(30)) from [2017.3$]"; */
					//
					//TESTHR(pRec->Open(sql, connStr1.c_str(), adOpenStatic, adLockOptimistic, adCmdText));
				if (pRec->adoEOF)
				{
					query(pRec,"2018.3",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2018.2",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2018.1",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2017.12",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2017.11",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2017.10",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2017.9",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2017.8",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2017.7",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2017.6",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2017.5",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2017.4",connStr1,v1);
				}
				if (pRec->adoEOF)
				{
					query(pRec,"2017.3",connStr1,v1);
				}
				if ((!pRec->adoEOF))
				{
					count ++;
					_variant_t v = pRec->Fields->GetItem(i)->Value;
					v.ChangeType(VT_BSTR);
					//string strValue = (char*)(_bstr_t)v.bstrVal;
					pRec2->put_Collect(_variant_t("消耗明细"),v);
					pRec2->Update();
					if (count == 1000)
					{
						//pRec2->Close();
					}
					//_variant_t vre;
					//const char * pSql = "update [宝蓝$] set 消耗明细 = '%s' where 1";
					//char sql[256];
					//v1.ChangeType(VT_BSTR);
					//sprintf_s(sql, 256, pSql, (char*)(_bstr_t)v.bstrVal/*,(char*)(_bstr_t)v1.bstrVal*/);
					//_variant_t t;
					//pCon1->Execute(sql,&t,adCmdText);
					//
					int b = 0;
				}
				int b = 0;
				//pRec->Close();
				}
			/*	else if ( i ==1)
				{
					_variant_t v1 = pRec2->Fields->GetItem(i)->Value;
					int b = 0;
				}*/

			}
			}
				catch(_com_error &e)
			{        
				//pRec2->Close();
				_bstr_t bstrDescription(e.Description());      
			/*	CharToOem(bstrDescription, bstrDescription);
				std::cout << bstrDescription << std::endl;*/

			}
			pRec2->MoveNext();
			
			
			
		

		}

		pRec2->Close();
	
		

	
			//ST_CARD st;
			//ST_CARD stPre;
			//if (pVeccard->size()> 0)
			//	stPre = pVeccard->back();
			//for(long i = 0; i < pRec->Fields->GetCount(); ++i)
			//{
			//	//if(i > 0) stream << ";";     
			//	
			//	_variant_t v = pRec->Fields->GetItem(i)->Value;
			//
			//	if (i== 0)
			//	{
			//		st.strCardNum = (char*)(_bstr_t)v.bstrVal;
			//		
			//	}
			//	else if (i ==1)
			//	{
			//		st.strJieruNUm=(char*)(_bstr_t)v.bstrVal;
			//		
			//	}
			//	else if (i == 2)
			//	{
			//		if (v.vt != VT_BSTR)
			//		{
			//			st.strCompany = stPre.strCompany;
			//			
			//		}
			//		else
			//		{
			//			st.strCompany = (char*)(_bstr_t)v.bstrVal;
			//			lastCompany = st.strCompany;
			//			
			//		}
			//		
			//		
			//	}
			//	else
			//		break;
			///*	if(v.vt == VT_R8)                                           
			//		stream << v.dblVal;
			//	if(v.vt == VT_BSTR)
			//		stream << (char*)(_bstr_t)v.bstrVal;  */              
			//	//++cellCount;
			//}      
			//
			//pVeccard->push_back(st);
		/*	stream << std::endl;*/
		
	}
	catch(_com_error &e)
	{        
		//pRec2->Close();
		_bstr_t bstrDescription(e.Description());      
		CharToOem(bstrDescription, bstrDescription);
		std::cout << bstrDescription << std::endl;

	}
	pRec2->Close();
	
	/*_ConnectionPtr pCon2 = NULL;
	_RecordsetPtr pSchema2 = NULL;
	string connStr2 = makeConnStr(strxhFile);
	TESTHR(pCon2.CreateInstance(__uuidof(Connection)));
	TESTHR(pCon2->Open(connStr2.c_str(), "", "", NULL));*/

	

	return true;
}