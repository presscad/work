#pragma once

#define KH_SELECT _T("id,Khmc,Lxfs,Ssdq,Jlxm,Dj,On1m,On15d,Du15d,Du1m")
void ParserKh(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);

#define KHJL_SELECT _T("id,Jlxm,Lxfs")
void ParserKhjl(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);

#define DXZH_SELECT _T("id,Dxzh,User,Password,Key")
void ParserDxzh(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);

#define USER_SELECT _T("id,User,Password,Authority,Usertype,Dj")
void ParserUser(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);

#define SIM_SELECT _T("id,Jrhm,Iccid,Llchm,Llclx,Khid")
void ParserSim(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);

#define SSDQ_SELECT _T("id,Ssdq")
void ParserSsdq(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);

#define LLTC_SELECT _T("id,Tcmc,Tcfl")
void ParserLltc(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);

#define LLC_SELECT _T("id,Dxzh,Llchm,Llclx")
void ParserLlc(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row);