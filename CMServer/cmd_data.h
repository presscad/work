#pragma once

void ParserData(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row, unsigned int nColumn );

#define KH_SELECT_SIZE 12
#define KH_SELECT _T("id,Khmc,Lxfs,Ssdq,Jlxm,Dj,On1m,On15d,Du15d,Du1m,Xgsj,Bz")
//void ParserKh(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);
#define ParserKh ParserData

#define KHJL_SELECT_SIZE 5
#define KHJL_SELECT _T("id,Jlxm,Lxfs,Xgsj,Bz")
//void ParserKhjl(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);
#define ParserKhjl ParserData

#define DXZH_SELECT_SIZE 7
#define DXZH_SELECT _T("id,Dxzh,User,Password,Key,Xgsj,Bz")
//void ParserDxzh(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);
#define ParserDxzh ParserData

#define USER_SELECT_SIZE 6
#define USER_SELECT _T("id,User,Password,Authority,Usertype,Xgsj")
//void ParserUser(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);
#define ParserUser ParserData

#define SIM_SELECT_SIZE 13
#define SIM_SELECT _T("id,Jrhm,Iccid,Zt,Dxzh,Llchm,Llclx,Xsrq,Jhrq,Xfrq,Dqrq,Zxrq,Bz")
//void ParserSim(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);
#define ParserSim ParserData

#define SSDQ_SELECT_SIZE 3
#define SSDQ_SELECT _T("id,Ssdq,Xgsj")
//void ParserSsdq(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);
#define ParserSsdq ParserData

#define LLTC_SELECT_SIZE 4
#define LLTC_SELECT _T("id,Tcmc,Tcfl,Xgsj")
//void ParserLltc(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);
#define ParserLltc ParserData

#define LLC_SELECT_SIZE 6
#define LLC_SELECT _T("id,Dxzh,Llchm,Llclx,Xgsj,Bz")
//void ParserLlc(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);
#define ParserLlc ParserData