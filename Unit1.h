//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Clipbrd.hpp>
#include <System.JSON.Readers.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.WinXCtrls.hpp>
#include <memory>
#include <stack>
#include <vector>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TSpeedButton *SpeedButton1;
	TTreeView *TreeView1;
	TMemo *Memo1;
	TSplitter *Splitter1;
	TOpenDialog *OpenDialog1;
	TImageList *ImageList1;
	TPanel *Panel2;
	TPanel *Panel3;
	TSpeedButton *SpeedButton2;
	TPanel *Panel4;
	TActivityIndicator *ActivityIndicator1;
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
	TStringReader* f_streader;
	TJsonTextReader* f_jsontxt;
	void __fastcall json_totree_(TJsonTextReader* jsr);
	void __fastcall start_js_build(String value_);
	TJsonTextReader* __fastcall create_jsonr(const String value_);
	void __fastcall addChild_(TJsonTextReader* jsr, std::stack<TTreeNode* >& st_, UnicodeString value_, const int imagenum);
	String __fastcall file_towstring(UnicodeString filename);
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
