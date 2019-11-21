//---------------------------------------------------------------------------

#ifndef ShowHistoryH
#define ShowHistoryH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "VirtualTrees.hpp"
#include "sqlite3.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TVirtualStringTree *VirtualStringTree1;
	TButton *ButtonDelLine;
	TButton *ButtonDelTable;
	void __fastcall TableGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
          TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall Selection(TBaseVirtualTree *Sender, PVirtualNode Node);
	void __fastcall DelLine(TObject *Sender);
	void __fastcall DelTable(TObject *Sender);
private:	// User declarations
	void LoadTable(sqlite3 *db, char *err);
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
