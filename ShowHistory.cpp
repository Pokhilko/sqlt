//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ShowHistory.h"
#include "sqlite3.h"
#include <cstring>
#include <windows.h>
#include <string>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TForm1 *Form1;

using namespace std;
const char* sql = "SELECT * FROM urls;";
const char* History = "History";
unsigned int selectedID;

typedef struct{
	int id = 0;
	UnicodeString url;
	int visit_count = 0;
	int typed_count = 0;
	int last_visit_time = 0;
}TableUrls;
//---------------------------------------------------------------------------
void TForm1::LoadTable(sqlite3 *db, char *err)
{
	if( sqlite3_open(History, &db) )
		fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
	else if (sqlite3_exec(db, sql, 0, 0, &err))
	{
		fprintf(stderr, "Ошибка SQL: %sn", err);
		sqlite3_free(err);
	}
	const char *errmsg;
	sqlite3_stmt *pStatement;
	int result = sqlite3_prepare_v2(db,sql,-1,&pStatement,NULL);
	if(result != SQLITE_OK)
	{
		errmsg = sqlite3_errmsg(db);
	}

	Form1->VirtualStringTree1->NodeDataSize = sizeof(TableUrls);
	Form1->VirtualStringTree1->Clear();

	while(true)
	{
		result = sqlite3_step(pStatement);
		if(result != SQLITE_ROW) break;
		int id = sqlite3_column_int(pStatement, 0);
		unsigned char *url = (unsigned char *)sqlite3_column_text(pStatement, 1);
		int visit_count = sqlite3_column_int(pStatement, 3);
		int typed_count = sqlite3_column_int(pStatement, 4);
		int last_visit_time = sqlite3_column_int(pStatement, 5);
		PVirtualNode entryNode = Form1->VirtualStringTree1->AddChild(Form1->VirtualStringTree1->RootNode);
		TableUrls *nodeData = (TableUrls*)Form1->VirtualStringTree1->GetNodeData(entryNode);
		nodeData->id = id;
		nodeData->url = (char*)url;
		nodeData->visit_count = visit_count;
		nodeData->typed_count = typed_count;
		nodeData->last_visit_time = last_visit_time;
	}

	sqlite3_finalize(pStatement);
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	sqlite3 *db = 0;
	char *err = 0;
	LoadTable(db,err);
	sqlite3_close(db);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TableGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
          TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)

{
    if(!Node) return;
	TableUrls *nodeData = (TableUrls*)Sender->GetNodeData(Node);
	switch(Column)
	{
		case 0: CellText = nodeData->id; break;
		case 1: CellText = nodeData->url; break;
		case 2: CellText = nodeData->visit_count; break;
		case 3: CellText = nodeData->typed_count; break;
		case 4: CellText = nodeData->last_visit_time; break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Selection(TBaseVirtualTree *Sender, PVirtualNode Node)
{
    if(!Node) return;
	TableUrls *nodeData = (TableUrls*)Sender->GetNodeData(Node);
	selectedID = nodeData->id;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DelLine(TObject *Sender)
{
	string sqlDelLine = "DELETE FROM urls WHERE id = ";
	sqlDelLine += to_string(selectedID);
	sqlite3 *dbDelLine = 0;
	char *errDelLine = 0;
	if( sqlite3_open(History, &dbDelLine) )
		fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(dbDelLine));
	else if (sqlite3_exec(dbDelLine, sqlDelLine.c_str(), 0, 0, &errDelLine))
	{
		fprintf(stderr, "Ошибка SQL: %sn", errDelLine);
		sqlite3_free(errDelLine);
	}
	sqlite3_close(dbDelLine);

	sqlite3 *dbUp = 0;
	char *errUp = 0;
	LoadTable(dbUp,errUp);
	sqlite3_close(dbUp);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DelTable(TObject *Sender)
{
	string sqlDelTable = "DELETE FROM urls";
	sqlite3 *dbDelTable = 0;
	char *errDelTable = 0;
	if( sqlite3_open(History, &dbDelTable) )
		fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(dbDelTable));
	else if (sqlite3_exec(dbDelTable, sqlDelTable.c_str(), 0, 0, &errDelTable))
	{
		fprintf(stderr, "Ошибка SQL: %sn", errDelTable);
		sqlite3_free(errDelTable);
	}
	sqlite3_close(dbDelTable);

	sqlite3 *dbUp = 0;
	char *errUp = 0;
	LoadTable(dbUp,errUp);
	sqlite3_close(dbUp);
}
//---------------------------------------------------------------------------
