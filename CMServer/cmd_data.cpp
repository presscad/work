#include "stdafx.h"
#include <mysql.h>
#include "cmd_data.h"

void ParserData(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row, unsigned int nColumn)
{
	_msgpack.pack_array(nColumn);
	for (unsigned int i = 0; i < nColumn; i++)
	{
		if (row[i] == NULL)
			_msgpack.pack("");
		else
			_msgpack.pack(row[i]);
	}
}

//// id,Khmc,Lxfs,Ssdq,Jlxm,Dj,On1m,On15d,Du15d,Du1m,Xgsj
//void ParserKh(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row)
//{
//	_msgpack.pack_array(10);
//	_msgpack.pack(row[0]);
//	_msgpack.pack(row[1]);
//	_msgpack.pack(row[2]);
//	_msgpack.pack(row[3]);
//	_msgpack.pack(row[4]);
//	_msgpack.pack(row[5]);
//	_msgpack.pack(row[6]);
//	_msgpack.pack(row[7]);
//	_msgpack.pack(row[8]);
//	_msgpack.pack(row[9]);
//}
//
//void ParserKhjl(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row)
//{
//
//}
//
//void ParserDxzh(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row)
//{
//
//}
//
//// row[0] = id;
//// row[1] = User;
//// row[2] = Password;
//// row[3] = Authority;
//// row[4] = Usertype;
//// row[5] = Fatherid;
//// row[6] = Xgsj;
//void ParserUser(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row)
//{
//	_msgpack.pack_array(7);
//	_msgpack.pack(row[0]);
//	_msgpack.pack(row[1]);
//	_msgpack.pack(row[2]);
//	_msgpack.pack(row[3]);
//	_msgpack.pack(row[4]);
//	_msgpack.pack(row[5]);
//	_msgpack.pack(row[6]);
//}
//
//// id,Jrhm,Iccid,Llchm,Llclx,Khid,Xsrq
//void ParserSim(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row)
//{
//	_msgpack.pack_array(8);
//	_msgpack.pack(row[0]);
//	_msgpack.pack(row[1]);
//	_msgpack.pack(row[2]);
//	_msgpack.pack(row[3]);
//	_msgpack.pack(row[4]);
//	_msgpack.pack(row[5]);
//	_msgpack.pack(row[6]);
//}

//void ParserSsdq(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row)
//{
//
//}
//
//void ParserLltc(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row)
//{
//
//}
//
//void ParserLlc(msgpack::packer<msgpack::sbuffer>& _msgpack, MYSQL_ROW& row)
//{
//
//}