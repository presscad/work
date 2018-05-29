// ReadExcel.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ReadExcel.h"
#include "Excel.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
#include <vector>
#include "shellapi.h"


#import "msado15.dll" \
	no_namespace \
	rename("EOF","adoEOF") rename("DataTypeEnum","adoDataTypeEnum") \
	rename("FieldAttributeEnum", "adoFielAttributeEnum") rename("EditModeEnum", "adoEditModeEnum") \
	rename("LockTypeEnum", "adoLockTypeEnum") rename("RecordStatusEnum", "adoRecordStatusEnum") \
	rename("ParameterDirectionEnum", "adoParameterDirectionEnum")
#pragma warning(default:4146)
//C:\Program Files\Common Files\System\ADO
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
std::string sqlSelectSheet1(_bstr_t connStr, int sheetIndex)
{
	_ConnectionPtr pCon = NULL;
	_RecordsetPtr pSchema = NULL;

	TESTHR(pCon.CreateInstance(__uuidof(Connection)));
	TESTHR(pCon->Open(connStr, "", "", NULL));

	pSchema = pCon->OpenSchema(adSchemaTables);     
	

	for(int i = 0; i < sheetIndex; ++i) pSchema->MoveNext();

	std::string sheetName = (char*)(_bstr_t)pSchema->Fields->GetItem("TABLE_NAME")->Value.bstrVal;

	std::stringstream stream;
	stream << "SELECT * FROM [" << sheetName << "]";
	return stream.str();
}


void read(std::string excelFile, int sheetIndex, bool header, std::vector<ST_CARD>* pVeccard)
{
	clock_t t1 = clock();

	std::cout << "reading " << excelFile;

	if(FAILED(::CoInitialize(NULL))) return;

	_RecordsetPtr pSchema = NULL;
	_RecordsetPtr pRec = NULL;

	int cellCount = 0;

	try
 	{                    
		_bstr_t connStr(makeConnStr(excelFile, header).c_str());

		TESTHR(pRec.CreateInstance(__uuidof(Recordset)));  
		string sql ="select SIM卡号,接入号码,电信账号 from [开卡记录$]";
		TESTHR(pRec->Open(sql.c_str(), connStr, adOpenStatic, adLockOptimistic, adCmdText));

		//std::ofstream stream(csvFile.c_str());
		string lastCompany;
		pVeccard->clear();
		while(!pRec->adoEOF)
		{
			ST_CARD st;
			ST_CARD stPre;
			if (pVeccard->size()> 0)
				stPre = pVeccard->back();
			for(long i = 0; i < pRec->Fields->GetCount(); ++i)
			{
				//if(i > 0) stream << ";";     
				
				_variant_t v = pRec->Fields->GetItem(i)->Value;
			
				if (i== 0)
				{
					if (v.vt == VT_NULL ||v.vt== VT_EMPTY)
						break;
					v.ChangeType(VT_BSTR);
					st.strCardNum = (char*)(_bstr_t)v.bstrVal;
					
				}
				else if (i ==1)
				{
					if (v.vt == VT_NULL ||v.vt== VT_EMPTY)
						break;
					v.ChangeType(VT_BSTR);
					st.strJieruNUm=(char*)(_bstr_t)v.bstrVal;
					
				}
				else if (i == 2)
				{
					if (v.vt == VT_NULL ||v.vt== VT_EMPTY)
					{
						st.strCompany = stPre.strCompany;
						
					}
					else
					{
						v.ChangeType(VT_BSTR);
						st.strCompany = (char*)(_bstr_t)v.bstrVal;
						lastCompany = st.strCompany;
						
					}
					
					
				}
				else
					break;
			/*	if(v.vt == VT_R8)                                           
					stream << v.dblVal;
				if(v.vt == VT_BSTR)
					stream << (char*)(_bstr_t)v.bstrVal;  */              
				//++cellCount;
			}      
			if (!(st.strCardNum.empty()|| st.strJieruNUm.empty() || st.strCompany.empty()))			
			pVeccard->push_back(st);
		/*	stream << std::endl;*/
			pRec->MoveNext();
		}
		pRec->Close();
	}
	catch(_com_error &e)
	{        
   		_bstr_t bstrDescription(e.Description());      
		CharToOem(bstrDescription, bstrDescription);
		std::cout << bstrDescription << std::endl;
	}

	::CoUninitialize();

	//clock_t t2 = clock();
	//double t = (double)(t2 - t1) / CLOCKS_PER_SEC;    
	//std::cout << ": " << t << " sec; " << cellCount / t << " cells/sec" << "; see " << csvFile << std::endl;   
}



bool  readKaika(std::string excelFile,  bool header, vector<ST_KAICARD>* pVeckaicar)
{
	if(FAILED(::CoInitialize(NULL))) return false;
	_RecordsetPtr pSchema = NULL;
	_RecordsetPtr pRec = NULL;

	int cellCount = 0;

	try
 	{                    
		_bstr_t connStr(makeConnStr(excelFile, header).c_str());

		TESTHR(pRec.CreateInstance(__uuidof(Recordset)));  
		
  		TESTHR(pRec->Open(sqlSelectSheet(connStr, 0).c_str(), connStr, adOpenStatic, adLockOptimistic, adCmdText));

		//std::ofstream stream(csvFile.c_str());
		string lastCompany;

		while(!pRec->adoEOF)
		{
			//序号 SIM卡号	接入号码	开卡日期	套餐	备注	池号	客户


			ST_KAICARD st;
			ST_KAICARD stPre;
			if (pVeckaicar->size()> 0)
				stPre = pVeckaicar->back();
			for(long i = 0; i < 8; i++)
			{
				//if(i > 0) stream << ";";     
				
				_variant_t v = pRec->Fields->GetItem(i)->Value;
			
				if (i== 1)
				{
					if (v.vt != VT_BSTR)
					{
					}
					else
					{
						v.ChangeType(VT_BSTR);
						st.strSim = (char*)(_bstr_t)v.bstrVal;
					}
					
					
					
				}
				else if (i ==2)
				{
					if (v.vt != VT_BSTR)
					{
					}
					else
					{
						v.ChangeType(VT_BSTR);
						st.strJieruNum=(char*)(_bstr_t)v.bstrVal;
					}
					
				}
				else if (i == 3)
				{
					if (v.vt != VT_BSTR)
					{
						st.strKaikaData = stPre.strKaikaData;
						
					}
					else
					{
						v.ChangeType(VT_BSTR);
						st.strKaikaData = (char*)(_bstr_t)v.bstrVal;						
					}		
					
				}
				else if (i == 4)
				{
					if (v.vt != VT_BSTR)
					{

						st.strTaocan = stPre.strTaocan;
					}
					else
					{
						v.ChangeType(VT_BSTR);
						st.strTaocan = (char*)(_bstr_t)v.bstrVal;						
					}		

				}
				else if (i == 5)
				{
					if (v.vt != VT_BSTR)
					{


					}
					else
					{
						st.strRemark = (char*)(_bstr_t)v.bstrVal;						
					}		

				}
				else if (i == 6)
				{
					if (v.vt != VT_BSTR)
					{

						st.strChihao = stPre.strChihao;
					}
					else
					{
						st.strChihao = (char*)(_bstr_t)v.bstrVal;						
					}		

				}
				else if (i == 7)
				{
					if (v.vt != VT_BSTR)
					{

						st.strKehu = stPre.strKehu;
					}
					else
					{
						st.strKehu = (char*)(_bstr_t)v.bstrVal;						
					}		

				}
				
			/*	if(v.vt == VT_R8)                                           
					stream << v.dblVal;
				if(v.vt == VT_BSTR)
					stream << (char*)(_bstr_t)v.bstrVal;  */              
				//++cellCount;
			}      
			
			pVeckaicar->push_back(st);
		/*	stream << std::endl;*/
			pRec->MoveNext();
		}
		pRec->Close();
	}
	catch(_com_error &e)
	{        
   		_bstr_t bstrDescription(e.Description());      
		CharToOem(bstrDescription, bstrDescription);
		return false;
	}

end:
	::CoUninitialize();                        
	return true;

}


//bool  readChuka(std::string excelFile,  bool header, vector<ST_CHUCARD>* pVecChucar)
//{
//	if(FAILED(::CoInitialize(NULL))) return false;
//	_RecordsetPtr pSchema = NULL;
//	_RecordsetPtr pRec = NULL;
//
//	int cellCount = 0;
//
//	try
//	{                    
//		_bstr_t connStr(makeConnStr(excelFile, header).c_str());
//
//		TESTHR(pRec.CreateInstance(__uuidof(Recordset)));       
//		TESTHR(pRec->Open(sqlSelectSheet(connStr, 1).c_str(), connStr, adOpenStatic, adLockOptimistic, adCmdText));
//
//		//std::ofstream stream(csvFile.c_str());
//		string lastCompany;
//
//		while(!pRec->adoEOF)
//		{
//			//入库	结余	客户名称	数量	卡号	手机号	拿卡日期	销售员	套餐	归属地	开卡日期	卡备注
//
//
//
//			ST_CHUCARD st;
//
//			ST_CHUCARD stPre;
// 			if (pVecChucar->size() > 0)
//				stPre= pVecChucar->back();
//    		for(long i = 0; i < 12; i++)
//			{
//				//if(i > 0) stream << ";";     
//				
//     			_variant_t v = pRec->Fields->GetItem(i)->Value;				
//			
// 				if (i== 2)
//				{
//					if (v.vt != VT_BSTR)
//					{
//						st.strKehu = stPre.strKehu;
//					}
//					else
//					{
//						st.strKehu = (char*)(_bstr_t)v.bstrVal;
//					}
//					
//					
//				}
//				else if (i ==4)
//				{
//					if (v.vt != VT_BSTR)
//					{
//						if (v.vt == VT_R8)
//						{
//							stringstream  stream;
//							stream << v.dblVal;
//							st.strSim = stream.str();
//						}
//					}
//					else
//					{
//						st.strSim=(char*)(_bstr_t)v.bstrVal;
//					}
//					if (st.strSim.empty())
//					{
//						goto end;
//					}
//					
//				}
//				else if (i == 5)
//				{
//					if (v.vt != VT_BSTR)
//					{
//
//						if (v.vt == VT_R8)
//						{
//							stringstream  stream;
//							stream << v.dblVal;
//							st.strJieruNum = stream.str();
//						}
//					}
//					else
//					{
//						st.strJieruNum = (char*)(_bstr_t)v.bstrVal;						
//					}		
//
//				}
//				else if (i == 6)
//				{
//					if (v.vt != VT_BSTR)
//					{
//						
//						//st.strNakaData = stPre.strNakaData;
//						if (v.vt == VT_R8)
//						{
//							stringstream  stream;
//							stream << v.dblVal;
//							st.strNakaData = stream.str();
//						}
//					}
//					else
//					{
//						st.strNakaData = (char*)(_bstr_t)v.bstrVal;						
//					}		
//					
//				}
// 				else if (i == 7)
//				{
//        		if (v.vt != VT_BSTR)
//					{
//
//						st.strXiaoshouyuan = stPre.strXiaoshouyuan;
//					}
//					else
//					{
//						st.strXiaoshouyuan = (char*)(_bstr_t)v.bstrVal;						
//					}		
//
//				}
//				else if (i == 8)
//				{
//					if (v.vt != VT_BSTR)
//					{
//
//						st.strTaocan = stPre.strTaocan;
//					}
//					else
//					{
//						st.strTaocan = (char*)(_bstr_t)v.bstrVal;						
//					}		
//
//				}
//				else if (i == 9)
//				{
//					if (v.vt != VT_BSTR)
//					{
//						st.strGuishudi = stPre.strGuishudi;
//
//					}
//					else
//					{
//						st.strGuishudi = (char*)(_bstr_t)v.bstrVal;						
//					}		
//
//				}
//				else if (i == 10)
//				{
//					if (v.vt != VT_BSTR)
//					{
//						if (v.vt == VT_R8)
//						{
//							stringstream  stream;
//							stream << v.dblVal;
//							st.strKaikaData = stream.str();
//						}
//
//					}
//					else
//					{
//						st.strKaikaData = (char*)(_bstr_t)v.bstrVal;						
//					}		
//
//				}
//				else if (i == 11)
//				{
//					if (v.vt != VT_BSTR)
//					{
//						if (v.vt == VT_R8)
//						{
//							stringstream  stream;
//							stream << v.dblVal;
//							st.strRemark = stream.str();
//						}
//
//					}
//					else
//					{
//						st.strRemark = (char*)(_bstr_t)v.bstrVal;						
//					}		
//
//				}
//				
//			/*	if(v.vt == VT_R8)                                           
//					stream << v.dblVal;
//				if(v.vt == VT_BSTR)
//					stream << (char*)(_bstr_t)v.bstrVal;  */              
//				//++cellCount;
//			}      
//			
//			pVecChucar->push_back(st);
//		/*	stream << std::endl;*/
//			pRec->MoveNext();
//		}
//		pRec->Close();
//	}
//	catch(_com_error &e)
//	{        
//   		_bstr_t bstrDescription(e.Description());      
//		CharToOem(bstrDescription, bstrDescription);
//		return false;
//	}
//
//end:
//	::CoUninitialize();
//	return true;
//
//}



bool  readChuka(std::string excelFile,  bool header, vector<ST_CHUCARD>* pVecChucard)
{
	if(FAILED(::CoInitialize(NULL))) return false;
	_RecordsetPtr pSchema = NULL;
	_RecordsetPtr pRec = NULL;

	int cellCount = 0;

	try
	{                    
		_bstr_t connStr(makeConnStr(excelFile, header).c_str());

		TESTHR(pRec.CreateInstance(__uuidof(Recordset)));  
		string strSql = "select 卡号,客户名称,拿卡日期,销售员,卡备注 from [出卡记录$]";
		TESTHR(pRec->Open(strSql.c_str(), connStr, adOpenStatic, adLockOptimistic, adCmdText));

		//std::ofstream stream(csvFile.c_str());
		int a = 0;
		pVecChucard->clear();
		while(!pRec->adoEOF)
		{
			ST_CHUCARD st;			
			ST_CHUCARD stPre;
			if (pVecChucard->size() > 0)
				stPre= pVecChucard->back();
			for(long i = 0; i < 5; i++)
			{
				//if(i > 0) stream << ";";    


				FieldPtr  ptr = pRec->Fields->GetItem(i);
				_variant_t v = pRec->Fields->GetItem(i)->Value;


				if (i== 0)
				{
					if (v.vt == VT_EMPTY || v.vt == VT_NULL)
						goto endclose;
					v.ChangeType(VT_BSTR);
					st.strSim = (char*)(_bstr_t)v.bstrVal;					

				}
				else if (i == 1)
				{
					if (v.vt == VT_EMPTY || v.vt == VT_NULL)
						st.strKehu =stPre.strKehu;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strKehu = (char*)(_bstr_t)v.bstrVal;
					}


				}
				else if (i == 2)
				{
					if (v.vt == VT_EMPTY|| v.vt == VT_NULL)
						st.strNakaData =stPre.strNakaData;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strNakaData = (char*)(_bstr_t)v.bstrVal;
					}
				}
				else if (i == 3)
				{
					if (v.vt == VT_EMPTY|| v.vt == VT_NULL)
						st.strXiaoshouyuan =stPre.strXiaoshouyuan;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strXiaoshouyuan = (char*)(_bstr_t)v.bstrVal;
					}
				}
				else if (i == 4)
				{
					if (v.vt == VT_EMPTY|| v.vt == VT_NULL)
						st.strRemark =stPre.strRemark;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strRemark = (char*)(_bstr_t)v.bstrVal;
					}
				}
				
			}
			if (!st.strSim.empty())
			{
				pVecChucard->push_back(st);		
				pRec->MoveNext();
			}
			else break;

			
		}	
endclose:
		pRec->Close();
	}
	catch(_com_error &e)
	{        
		_bstr_t bstrDescription(e.Description());      
		CharToOem(bstrDescription, bstrDescription);
		return false;
	}

end:
	::CoUninitialize();
	return true;

}

//客户名	联系方式	所属地区	客户经理	单价	备注
bool  readKehu(std::string excelFile,  bool header, vector<ST_KEHU>* pVec)
{
	if(FAILED(::CoInitialize(NULL))) return false;
	_RecordsetPtr pSchema = NULL;
	_RecordsetPtr pRec = NULL;

	int cellCount = 0;

	try
	{                    
		_bstr_t connStr(makeConnStr(excelFile, header).c_str());

		TESTHR(pRec.CreateInstance(__uuidof(Recordset)));  
		string strSql = "select 客户名,联系方式,所属地区,客户经理,单价,备注 from [客户$]";
		TESTHR(pRec->Open(strSql.c_str(), connStr, adOpenStatic, adLockOptimistic, adCmdText));

		//std::ofstream stream(csvFile.c_str());
		int a = 0;
		pVec->clear();
		while(!pRec->adoEOF)
		{
			ST_KEHU st;			
			ST_KEHU stPre;
			if (pVec->size() > 0)
				stPre= pVec->back();
			for(long i = 0; i < 6; i++)
			{
				//if(i > 0) stream << ";";    


				FieldPtr  ptr = pRec->Fields->GetItem(i);
				_variant_t v = pRec->Fields->GetItem(i)->Value;


				if (i== 0)
				{
					if (v.vt == VT_EMPTY || v.vt == VT_NULL)
						goto end;
					else
					{
						v.ChangeType(VT_BSTR);
						st.strName = (char*)(_bstr_t)v.bstrVal;	
					}
								

				}
				else if (i == 1)
				{
					if (v.vt == VT_EMPTY || v.vt == VT_NULL)
						st.strllfs =stPre.strllfs;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strllfs = (char*)(_bstr_t)v.bstrVal;
					}


				}
				else if (i == 2)
				{
					if (v.vt == VT_EMPTY|| v.vt == VT_NULL)
						st.strssdq =stPre.strssdq;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strssdq = (char*)(_bstr_t)v.bstrVal;
					}
				}
				else if (i == 3)
				{
					if (v.vt == VT_EMPTY|| v.vt == VT_NULL)
						st.strjl =stPre.strjl;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strjl= (char*)(_bstr_t)v.bstrVal;
					}
				}
				else if (i == 4)
				{
					if (v.vt == VT_EMPTY|| v.vt == VT_NULL)
						st.strdj =stPre.strdj;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strdj = (char*)(_bstr_t)v.bstrVal;
					}
				}
				else if (i == 5)
				{
					if (v.vt == VT_EMPTY|| v.vt == VT_NULL)
						st.strbz =stPre.strbz;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strbz = (char*)(_bstr_t)v.bstrVal;
					}
				}
			}
			pVec->push_back(st);			
			pRec->MoveNext();
		}	
		pRec->Close();
	}
	catch(_com_error &e)
	{        
		_bstr_t bstrDescription(e.Description());      
		CharToOem(bstrDescription, bstrDescription);
		return false;
	}

end:
	::CoUninitialize();
	return true;

}

bool  readXuFei(std::string excelFile,  bool header, vector<ST_XUFEI>* pVecXuFei)
{
	if(FAILED(::CoInitialize(NULL))) return false;
	_RecordsetPtr pSchema = NULL;
	_RecordsetPtr pRec = NULL;

	int cellCount = 0;

	try
	{                    
		_bstr_t connStr(makeConnStr(excelFile, header).c_str());

		TESTHR(pRec.CreateInstance(__uuidof(Recordset)));  
		string strSql = "select 接入号,续费日期,续费月数 from [续费$]";
		TESTHR(pRec->Open(strSql.c_str(), connStr, adOpenStatic, adLockOptimistic, adCmdText));

		//std::ofstream stream(csvFile.c_str());
		int a = 0;
		pVecXuFei->clear();
		while(!pRec->adoEOF)
		{
			ST_XUFEI st;			
			ST_XUFEI stPre;
			if (pVecXuFei->size() > 0)
			stPre= pVecXuFei->back();
			for(long i = 0; i < 3; i++)
			{
				//if(i > 0) stream << ";";    
			 				
							
				FieldPtr  ptr = pRec->Fields->GetItem(i);
				_variant_t v = pRec->Fields->GetItem(i)->Value;
							
						
				if (i== 0)
				{
					v.ChangeType(VT_BSTR);
					st.strSim = (char*)(_bstr_t)v.bstrVal;					
								
				}
				else if (i == 1)
				{
					if (v.vt == VT_EMPTY || v.vt == VT_NULL)
						st.strxfrq =stPre.strxfrq;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strxfrq = (char*)(_bstr_t)v.bstrVal;
					}
				
					
				}
				else if (i == 2)
				{
					if (v.vt == VT_EMPTY|| v.vt == VT_NULL)
						st.strxfys =stPre.strxfys;
					else
					{
						v.ChangeType(VT_BSTR);					
						st.strxfys = (char*)(_bstr_t)v.bstrVal;
					}
				}
			}
			pVecXuFei->push_back(st);			
			pRec->MoveNext();
		}	
		pRec->Close();
	}
	catch(_com_error &e)
	{        
		_bstr_t bstrDescription(e.Description());      
		CharToOem(bstrDescription, bstrDescription);
		return false;
	}
		
end:
	::CoUninitialize();
	return true;
	
}

bool  readTuika(std::string excelFile,  bool header, vector<ST_TUIKA>* pVecChucard)
{
	if(FAILED(::CoInitialize(NULL))) return false;
	_RecordsetPtr pSchema = NULL;
	_RecordsetPtr pRec = NULL;

	int cellCount = 0;

	try
	{                    
		_bstr_t connStr(makeConnStr(excelFile, header).c_str());

		TESTHR(pRec.CreateInstance(__uuidof(Recordset)));  
		string strSql = "select SIM卡号 from [退卡记录$]";
		TESTHR(pRec->Open(strSql.c_str(), connStr, adOpenStatic, adLockOptimistic, adCmdText));

		//std::ofstream stream(csvFile.c_str());
		int a = 0;
		pVecChucard->clear();
		while(!pRec->adoEOF)
		{
			ST_TUIKA st;			
			ST_TUIKA stPre;
			if (pVecChucard->size() > 0)
				stPre= pVecChucard->back();
			for(long i = 0; i < 1; i++)
			{
				//if(i > 0) stream << ";";    


				FieldPtr  ptr = pRec->Fields->GetItem(i);
				_variant_t v = pRec->Fields->GetItem(i)->Value;


				if (i== 0)
				{
					v.ChangeType(VT_BSTR);
					st.strSim = (char*)(_bstr_t)v.bstrVal;					

				}
				
			}
			pVecChucard->push_back(st);			
			pRec->MoveNext();
		}	
		pRec->Close();
	}
	catch(_com_error &e)
	{        
		_bstr_t bstrDescription(e.Description());      
		CharToOem(bstrDescription, bstrDescription);
		return false;
	}

end:
	::CoUninitialize();
	return true;

}
bool  readXiaoka(std::string excelFile,  bool header, vector<ST_XIAOKA>* pVecChucard)
{
	if(FAILED(::CoInitialize(NULL))) return false;
	_RecordsetPtr pSchema = NULL;
	_RecordsetPtr pRec = NULL;

	int cellCount = 0;

	try
	{                    
		_bstr_t connStr(makeConnStr(excelFile, header).c_str());

		TESTHR(pRec.CreateInstance(__uuidof(Recordset)));  
		string strSql = "select SIM卡号,消卡日期,状态 from [消卡记录$]";
		TESTHR(pRec->Open(strSql.c_str(), connStr, adOpenStatic, adLockOptimistic, adCmdText));

		//std::ofstream stream(csvFile.c_str());
		int a = 0;
		pVecChucard->clear();
		while(!pRec->adoEOF)
		{
			ST_XIAOKA st;			
			ST_XIAOKA stPre;
			if (pVecChucard->size() > 0)
				stPre= pVecChucard->back();
			for(long i = 0; i < 3; i++)
			{
				//if(i > 0) stream << ";";    


				FieldPtr  ptr = pRec->Fields->GetItem(i);
				_variant_t v = pRec->Fields->GetItem(i)->Value;


				if (i== 0)
				{
					v.ChangeType(VT_BSTR);
					st.strSim = (char*)(_bstr_t)v.bstrVal;					

				}
				if (i== 1)
				{
					if (v.vt == VT_EMPTY ||v.vt == VT_NULL)
					{
						st.strzxrq = stPre.strzxrq;
					}
					else
					{
						v.ChangeType(VT_BSTR);
						st.strSim = (char*)(_bstr_t)v.bstrVal;	
					}
									

				}
				if (i== 2)
				{
					if (v.vt == VT_EMPTY ||v.vt == VT_NULL)
					{
						st.strzt = stPre.strzt;
					}
					else
					{
						v.ChangeType(VT_BSTR);
						st.strzt = (char*)(_bstr_t)v.bstrVal;	
					}


				}

			}
			pVecChucard->push_back(st);			
			pRec->MoveNext();
		}	
		pRec->Close();
	}
	catch(_com_error &e)
	{        
		_bstr_t bstrDescription(e.Description());      
		CharToOem(bstrDescription, bstrDescription);
		return false;
	}

end:
	::CoUninitialize();
	return true;

}
//bool  readXuFei(std::string excelFile,  bool header, vector<ST_XUFEI>* pVecXuFei)
//{
//	if(FAILED(::CoInitialize(NULL))) return false;
//	_RecordsetPtr pSchema = NULL;
//	_RecordsetPtr pRec = NULL;
//
//	int cellCount = 0;
//
//	try
// 	{                    
//		_bstr_t connStr(makeConnStr(excelFile, header).c_str());
//
//		TESTHR(pRec.CreateInstance(__uuidof(Recordset)));       
//  		TESTHR(pRec->Open(sqlSelectSheet(connStr, 2).c_str(), connStr, adOpenStatic, adLockOptimistic, adCmdText));
//
//		//std::ofstream stream(csvFile.c_str());
//		int a = 0;
//		while(!pRec->adoEOF)
//		{
//			//卡号	缴费日期	到期日期	客户	手机号	缴费金额	备注
//
//			a ++;
//
//			ST_XUFEI st;
//
//			ST_XUFEI stPre;
//			if (pVecXuFei->size() > 0)
//				stPre= pVecXuFei->back();
//			for(long i = 0; i < 7; i++)
//			{
//				//if(i > 0) stream << ";";    
// 				
//				
//				FieldPtr  ptr = pRec->Fields->GetItem(i);
//				_variant_t v = pRec->Fields->GetItem(i)->Value;
//				
//			
//				if (i== 0)
//				{
//					if (v.vt != VT_BSTR)
//					{
//						if (v.vt == VT_R8)
//						{
//							stringstream  stream;
//							stream << v.dblVal;
//							st.strSim = stream.str();
//						}
//					}
//					else
//					{
//						st.strSim = (char*)(_bstr_t)v.bstrVal;
//						if (st.strSim.empty())
//							goto end;
//					}
//					
//					
//				}
//				else if (i ==1)
//				{
//					if (v.vt != VT_BSTR)
//					{
//					}
//					else
//					{
//							st.strJiaofeiData = (char*)(_bstr_t)v.bstrVal;	
//							
//					}
//					
//				}
//				else if (i == 2)
//				{
//					if (v.vt != VT_BSTR)
//					{
//						if (v.vt == VT_R8)
//						{
//							stringstream  stream;
//							stream << v.dblVal;
//							st.strDaoqiData = stream.str();
//						}
//
//					}
//					else
//					{
//						st.strDaoqiData = (char*)(_bstr_t)v.bstrVal;				
//					}		
//
//				}
//				else if (i == 3)
//				{
//					if (v.vt != VT_BSTR)
//					{
//						
//						st.strKehu = stPre.strKehu;
//					}
//					else
//					{
//						st.strKehu = (char*)(_bstr_t)v.bstrVal;						
//					}		
//					
//				}
//				else if (i == 4)
//				{
// 					if (v.vt != VT_BSTR)
//					{
//						if (v.vt == VT_R8)
//						{
//							stringstream  stream;
//							stream << v.dblVal;
//							st.strShoujihao = stream.str();
//						}
//						
//					}
//					else
//					{
//						
//						st.strShoujihao = 	(char*)(_bstr_t)v.bstrVal;				
//					}		
//
//				}
// 				else if (i == 5)
//				{
//					if (v.vt != VT_BSTR)
//					{
//						if (v.vt == VT_R8)
//						{
//							stringstream  stream;
//							stream << v.dblVal;
//							st.strMoney = stream.str();
//						}
//						
//					}
//					else
//					{
//						st.strMoney = 	(char*)(_bstr_t)v.bstrVal;					
//					}		
//
//				}
//				else if (i == 6)
//				{
//					 if (v.vt != VT_BSTR)
//					{
//						if (v.vt == VT_R8)
//						{
//							stringstream  stream;
//							stream << v.dblVal;
//							st.strRemark = stream.str();
//						}
//
//					}
//					else
//					{
//						st.strRemark = 	(char*)(_bstr_t)v.bstrVal;			
//					}		
//
//				}
//				else break;
//				
//			/*	if(v.vt == VT_R8)                                           
//					stream << v.dblVal;
//				if(v.vt == VT_BSTR)
//					stream << (char*)(_bstr_t)v.bstrVal;  */              
//				//++cellCount;
//			}      
//			
//			pVecXuFei->push_back(st);
//		/*	stream << std::endl;*/
//			pRec->MoveNext();
//		}
//		pRec->Close();
//	}
//	catch(_com_error &e)
//	{        
//   		_bstr_t bstrDescription(e.Description());      
//		CharToOem(bstrDescription, bstrDescription);
//		return false;
//	}
//
//end:
//	::CoUninitialize();
//	return true;
//
//}

BEGIN_MESSAGE_MAP(CReadExcelApp, CWinApp)
END_MESSAGE_MAP()


// CReadExcelApp 构造

CReadExcelApp::CReadExcelApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CReadExcelApp 对象

CReadExcelApp theApp;


// CReadExcelApp 初始化

BOOL CReadExcelApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
__declspec(dllexport) bool ReadCard(void*pVec,char* szFile,int iLen)
{
	vector<ST_CARD>* veccard=(std::vector<ST_CARD>*)pVec;
	string  strfile ;
	strfile.append(szFile,iLen);
	read(strfile,0,true,veccard);
	return true;
}

__declspec(dllexport) bool ReadAll(void* pVec1,void* pVec2,void* pVec3,char* szFile,int iLen)
{
	vector<ST_KAICARD>* veckaicard=(std::vector<ST_KAICARD>*)pVec1;
	vector<ST_CHUCARD>* vecchucard=(std::vector<ST_CHUCARD>*)pVec2;
	vector<ST_XUFEI>* vecXufei=(std::vector<ST_XUFEI>*)pVec3;
	string  strfile ;
	strfile.append(szFile,iLen);
	readKaika(strfile,true,veckaicard);
	readChuka(strfile,true,vecchucard);
   	readXuFei(strfile,true,vecXufei);
	return true;
}

__declspec(dllexport) bool ReadChuka(void* pVec1,char* szFile,int iLen)
{
	//vector<ST_KAICARD>* veckaicard=(std::vector<ST_KAICARD>*)pVec1;
	vector<ST_CHUCARD>* vecchucard=(std::vector<ST_CHUCARD>*)pVec1;
	//vector<ST_XUFEI>* vecXufei=(std::vector<ST_XUFEI>*)pVec3;
	string  strfile ;
	strfile.append(szFile,iLen);
	//readKaika(strfile,true,veckaicard);
	readChuka(strfile,true,vecchucard);
	//readXuFei(strfile,true,vecXufei);
	return true;
}

__declspec(dllexport) bool ReadXuka(void* pVec1,char* szFile,int iLen)
{
	//vector<ST_KAICARD>* veckaicard=(std::vector<ST_KAICARD>*)pVec1;
	vector<ST_XUFEI>* vecchucard=(std::vector<ST_XUFEI>*)pVec1;
	//vector<ST_XUFEI>* vecXufei=(std::vector<ST_XUFEI>*)pVec3;
	string  strfile ;
	strfile.append(szFile,iLen);
	//readKaika(strfile,true,veckaicard);
	readXuFei(strfile,true,vecchucard);
	//readXuFei(strfile,true,vecXufei);
	return true;
}

_declspec(dllexport) bool ReadTuika(void* pVec1,char* szFile,int iLen)
{
	vector<ST_TUIKA>* veccard=(std::vector<ST_TUIKA>*)pVec1;
	//vector<ST_XUFEI>* vecXufei=(std::vector<ST_XUFEI>*)pVec3;
	string  strfile ;
	strfile.append(szFile,iLen);
	readTuika(strfile,true,veccard);
	//readKaika(strfile,true,veckaicard);
	
	//readXuFei(strfile,true,vecXufei);
	return true;
}
_declspec(dllexport) bool ReadXiaoka(void* pVec1,char* szFile,int iLen)
{
	vector<ST_XIAOKA>* veccard=(std::vector<ST_XIAOKA>*)pVec1;
	//vector<ST_XUFEI>* vecXufei=(std::vector<ST_XUFEI>*)pVec3;
	string  strfile ;
	strfile.append(szFile,iLen);
	readXiaoka(strfile,true,veccard);
	//readKaika(strfile,true,veckaicard);

	//readXuFei(strfile,true,vecXufei);
	return true;
}

_declspec(dllexport) bool ReadKehu(void* pVec1,char* szFile,int iLen)
{
	vector<ST_KEHU>* veccard=(std::vector<ST_KEHU>*)pVec1;
	//vector<ST_XUFEI>* vecXufei=(std::vector<ST_XUFEI>*)pVec3;
	string  strfile ;
	strfile.append(szFile,iLen);
	readKehu(strfile,true,veccard);
	//readKaika(strfile,true,veckaicard);

	//readXuFei(strfile,true,vecXufei);
	return true;
}

_declspec(dllexport) bool WriteExcel(void* pVec,char* szHearder,int nLen)
{
	vector<vector<string>>* pVecData = (vector<vector<string>>*)pVec;
	CString strHeard =  szHearder;
	strHeard +="\r\n";
	CString filePath="D:\\temp.csv";
		CString strLine;
		CFile fileCsv(filePath,CFile::modeCreate|CFile::modeWrite);
		strLine=strHeard;
	
		fileCsv.Write(strLine.GetBuffer(),strLine.GetLength());
		strLine.Empty();
		CString strTotal;
		vector<vector<std::string>>::iterator item2v = pVecData->begin();
		CString strone = "\"测试\"\t,";
		CString strlast  ="\"测试\"\t\r\n";
		while(item2v !=pVecData->end())
		{
			strTotal.Empty();
			vector<std::string>::iterator item = (*item2v).begin();
			while(item != (*item2v).end())
			  
             {
				   strLine.Empty();
 				if ( item != (*item2v).end() -1)
				{
					strLine = strone;
                  	strLine.Replace("测试",(*item).c_str());
					strTotal +=  strLine;
				}
				else
				{
					strLine = strlast;
					strLine.Replace("测试",(*item).c_str());
					strTotal +=  strLine;
				}
				
				item++;
			}
			fileCsv.Write(strTotal.GetBuffer(),strTotal.GetLength());
			std::cout<<std::endl;
			item2v++;
		}
		fileCsv.Close();	
		::ShellExecute(NULL, _T("open"),filePath , NULL, NULL, SW_SHOW);

	return true;
}