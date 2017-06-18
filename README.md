# jsree 0.0.1
This is a tool to read and visualize JSON file.  
I use the TTreeView component for visualization.  
Other than loading the file, JSON of the clipboard is also loaded.  

## Screenshot
![Screenshot1](https://community.embarcadero.com/uploads/23657/2017/05/22/Screenshot2017-05-271729.png "Screenshot1")
## binaries
This binary is downloadable.  
Both Windows 32 Bit and 64 Bit exist.  
> **[binaries](https://github.com/mojeld/jsree/tree/master/binaries)**  

> **[Windows Store(Web)](https://www.microsoft.com/store/apps/9plphk05pjf7)**  
> **[Windows Store](ms-windows-store://pdp/?productid=9plphk05pjf7)**

## code
The visualization section uses the TTreeView component.  
It is the code below.  
```cpp
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
```
This was made in [C++Builder](https://www.embarcadero.com/).



