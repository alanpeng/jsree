//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
template <typename T>
struct null_del
{
	static void null_del_(T* value_)
	{
		if (value_ != nullptr)
		{
			delete value_;
			value_ = nullptr;
		}
	}
};
TJsonTextReader* __fastcall TForm1::create_jsonr(const String value_)
{
	null_del<TStringReader>::null_del_(f_streader);
	null_del<TJsonTextReader>::null_del_(f_jsontxt);
	f_streader	= new TStringReader(value_);
	f_jsontxt   = new TJsonTextReader(f_streader);
	return f_jsontxt;

}
void __fastcall TForm1::addChild_(TJsonTextReader* jsr, std::stack<TTreeNode* >& st_, UnicodeString value_, const int imagenum)
{
	UnicodeString path_ = StringReplace(jsr->Path, st_.top()->Text, "", TReplaceFlags() << System::Sysutils::rfReplaceAll);
	if (Pos(".", path_) == 1) path_.Delete(1, 1);
	TThread::Synchronize(TThread::CurrentThread, [this, &st_, jsr, &path_, &value_, &imagenum](){
		TTreeNode* node_ = TreeView1->Items->AddChild(st_.top(),path_ + ":" + value_);
		Memo1->Lines->Append(path_ + " = " + value_);
		node_->ImageIndex 		= imagenum;
		node_->SelectedIndex 	= imagenum;
	});
}

void __fastcall TForm1::json_totree_(TJsonTextReader* jsr)
{
	TThread::CreateAnonymousThread([this, jsr](){
		std::stack<TTreeNode* > st_;
		try
		{
			while (jsr->Read() )
			{
				switch (jsr->TokenType)
				{
				case TJsonToken::StartObject:
					TThread::Synchronize(TThread::CurrentThread, [this, &st_, jsr](){
						(st_.empty())? st_.push(TreeView1->Items->Add(nullptr, "JSON")):
							st_.push(TreeView1->Items->AddChild(st_.top(), jsr->Path));
						if (st_.top() != nullptr) {
							st_.top()->ImageIndex 		= 0;
							st_.top()->SelectedIndex 	= 0;
							if (jsr->Path.Length() > 0) {
								Memo1->Lines->Append("[" + jsr->Path + "]");
							}
						}
					});
					break;
				case TJsonToken::StartArray:
				case TJsonToken::PropertyName:
					break;
				case TJsonToken::String:
					addChild_(jsr, st_, jsr->Value.AsString() + " (string)",1 );
					break;
				case TJsonToken::Integer:
					addChild_(jsr, st_, IntToStr(jsr->Value.AsInteger()) + " (integer)" , 2);
					break;
				case TJsonToken::Float:
					addChild_(jsr, st_, CurrToStr(jsr->Value.AsCurrency()) + " (float)" ,2);
					break;
				case TJsonToken::Boolean:
					addChild_(jsr, st_, BoolToStr(jsr->Value.AsBoolean()) + " (bool)" ,2);
					break;
				case TJsonToken::Null:
					addChild_(jsr, st_, "null" ,3);
					break;
				case TJsonToken::EndArray:
					break;
				case TJsonToken::EndObject:
					st_.pop();
					break;
				}
			}
		}
		__finally
		{
			TThread::Synchronize(TThread::CurrentThread, [this](){
				SpeedButton1->Enabled = true;
				SpeedButton2->Enabled = true;
				Panel4->Visible 	  = false;
			});
		}
	})->Start();
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
	if (OpenDialog1->Execute(this->Handle) )
	{
		if (FileExists(OpenDialog1->FileName) )
		{
			UnicodeString s_ = file_towstring(OpenDialog1->FileName);
			if (s_.Length() > 0)
			{
				start_js_build(s_);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	Memo1->Lines->Clear();
    Panel4->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	null_del<TStringReader>::null_del_(f_streader);
	null_del<TJsonTextReader>::null_del_(f_jsontxt);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormResize(TObject *Sender)
{
	if ((Panel2->Width+20) >= this->Width)
		Panel2->Width = this->Width/3;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::start_js_build(String value_)
{
	if (TreeView1->TopItem != nullptr)
	{
		TreeView1->Items->Clear();
		Memo1->Lines->Clear();
	}
	if (value_.Length() > 0)
	{
		SpeedButton1->Enabled = false;
		SpeedButton2->Enabled = false;
		Panel4->Top 	= static_cast<unsigned int>(this->Height / 2) - 35;
		Panel4->Left 	= static_cast<unsigned int>(this->Width  / 2) - 100;
		Panel4->Visible = true;
		json_totree_(create_jsonr(value_));
	}
}


void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
	start_js_build(Clipboard()->AsText);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Shift.Contains(ssCtrl) && (Key == 86))
	{
		SpeedButton2Click(this);
	}
}
//---------------------------------------------------------------------------
String __fastcall TForm1::file_towstring(UnicodeString filename_)
{
	UnicodeString res_;
	std::unique_ptr<TStreamReader> fread_ = std::make_unique<TStreamReader>(filename_, TEncoding::UTF8);
	while (! fread_->EndOfStream)
	{
		res_ = res_ + fread_->ReadLine();
	}
	return res_;
}
//---------------------------------------------------------------------------

