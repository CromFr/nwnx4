/***************************************************************************
    NWNX SQLite - Database plugin for SQLite
    Copyright (C) 2007 Ingmar Stieger (Papillon, papillon@blackdagger.com) 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 ***************************************************************************/
#if !defined(SQLITE_H_INCLUDED)
#define SQLITE_H_INCLUDED

#define DLLEXPORT extern "C" __declspec(dllexport)

#include "../dbplugin.h"
#include "lib/sqlite3.h"

class SQLite : public DBPlugin
{
public:
	SQLite();
	~SQLite() override;

	bool Init(char* nwnxhome);  

private:
	std::string dbfile;
	bool firstfetch;

	sqlite3 *sdb;
	sqlite3_stmt* pStmt;
	sqlite3_stmt* pNewStmt;

	bool Connect();
	void Disconnect();
	bool Execute(char* query);
	int Fetch(char* buffer);
	int GetData(int iCol, char* buffer);
	int GetAffectedRows();
	void GetEscapeString(char* str, char* buffer);
	void SafeFinalize(sqlite3_stmt** pStmt);
	int GetErrno();
	const char *GetErrorMessage();
};

#endif