#include<wx/wx.h>
#include<ID.h>
#include<TextCtrl.h>
#include<wx/filedlg.h>
#include<wx/wfstream.h>
#include<wx/fontdlg.h>
#include<PotterDialog.h>
#include<wx/fontpicker.h>
using namespace ID;

class MyApp:public wxApp{
  public:
  bool OnInit() override;
};
wxIMPLEMENT_APP(MyApp);

class MyFrame:public wxFrame{
    private:
    wxString wxstr;
    //For Text Font Dialog
    int startone;
    int endone;
    wxTextAttr myattr;
    //For Text Alignment Dialog
    int startnew;
    int endnew;
    //For Underlined Font Dialog
    int n0;
    int n1;
    wxTextAttr myattr1;

   void TextChanged(wxCommandEvent& evt){     //For text Entry
   Potter_Ctrl* ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
   wxPoint pt=ptrl->PositionToCoords(ptrl->GetInsertionPoint());
   SetStatusText(wxString::Format(wxT("Line:%d,Col:%d,Pos:%d"),pt.y,pt.x,ptrl->GetInsertionPoint()));
   Refresh();
   return;
}
 void AutoSave(wxCommandEvent& evt){    //For autosave
    Potter_Ctrl*ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
    if(wxstr==wxT("")){
        SetStatusText(wxT("Cannot not save File because no file is opened for writing"));
        Refresh();
        return;
    }
    if((GetMenuBar()->FindItem(ID::t5)->IsChecked()) &&(wxstr!=wxT(""))){
       ptrl->SaveFile(wxstr);
       Refresh();
       return;
    }
 }
 void OpenFile(wxCommandEvent& evt) {     //To open a file 
    wxFileDialog dialog(this,wxT("Open File"),wxT("C:\\"),wxEmptyString,wxT("*.txt"),wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    wxTextCtrl*tcrl=(wxTextCtrl*)GetToolBar()->FindControl(ID::t10);
    Potter_Ctrl* ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
    if(dialog.ShowModal()==wxID_OK){
    wxString str=dialog.GetPath();
     wxstr=str;
    wxFileInputStream file(wxstr);
    if(!file.IsOk()){
     SetStatusText(wxT("An error Occurred in opening stream"));
     Refresh();
     return;
     }
    tcrl->SetValue(str);
    ptrl->Clear();
    int ch=file.GetC();
    while(ch!=-1){
    *ptrl<<(wchar_t)ch;
    ch=file.GetC();
    if(ch== -1){
        SetStatusText(wxT("An Error has occurred in reading from file "));
        return;
      }
     }   
    }
    return;
 }

void ClearEditor(wxCommandEvent& evt){
   Potter_Ctrl* ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
   ptrl->Clear();
}
void ClearDisplayEntry(wxCommandEvent& evt){
    wxTextCtrl* tcrl=(wxTextCtrl*)GetToolBar()->FindControl(ID::t10);
    tcrl->Clear();
}
void ClearAll(wxCommandEvent& evt){
    Potter_Ctrl* ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
    wxTextCtrl* tcrl=(wxTextCtrl*)GetToolBar()->FindControl(ID::t10);
    ptrl->Clear();
    tcrl->Clear();
}
void CloseWindow(wxCommandEvent& evt){
    Close();
}


  void SaveFile(wxCommandEvent& evt){// Save Files
  Potter_Ctrl*ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
  wxTextCtrl* tcrl=(wxTextCtrl*)GetToolBar()->FindControl(ID::t10);
   wxFileDialog filedialog(this,wxT("Save File"),wxT("C:\\"),wxEmptyString,wxT("*.txt"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
   if(filedialog.ShowModal()==wxID_OK){
    wxString wstr=filedialog.GetPath();
    if(wstr==wxT("")){
        SetStatusText(wxT("File Entry is Empty"));
        return;
    }
    wxFileOutputStream outputfile(wstr);
    wxFileInputStream inputfile(wxstr);
    if(!inputfile.IsOk()){
        SetStatusText(wxT("Cannot read from an Open Stream"));
        return;
    }
    if(!outputfile.IsOk()){
        SetStatusText(wxT("Cannot write to an Open Stream"));
        return;
    }
    int ch=inputfile.GetC();
    while(ch!=-1){
        outputfile.PutC((char)ch);
        ch=inputfile.GetC();
    }
    outputfile.Close();
    ptrl->Clear();
    return;
}
}

 void FontAllText(wxCommandEvent& evt){
    Potter_Ctrl* ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
    wxFontData fontdata;
    fontdata.SetInitialFont(ptrl->GetFont());
    wxFontDialog fontdialog(this,fontdata);
    if(fontdialog.ShowModal()==wxID_OK){
        wxFont font=fontdialog.GetFontData().GetChosenFont();
        ptrl->SetFont(font);
        Refresh();
        return;
    }
 }


void Display(wxCommandEvent& evt){
    wxTextCtrl* tcrl=(wxTextCtrl*)GetToolBar()->FindControl(ID::t10);
    Potter_Ctrl*ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
    if(tcrl->GetValue()==wxT("")){
        SetStatusText(wxT("Text entry empty,Please fill the text entry by using user File Path"));
        Refresh();
        return;
    }
    wxString str=tcrl->GetValue();
    wxFileInputStream File(str);
    if(!File.IsOk()){
        SetStatusText(wxT("There is an error in reading the opened file,please reopen file "));
        return;
    }
    ptrl->Clear();
    int ch=File.GetC();
    while(ch!=-1){
      *ptrl<<(wchar_t)ch;
      ch=File.GetC();
    }
    return;
}
    
 void SetFrameFileName(const wxString& str){
        wxstr=str;
    }
 wxString GetFrameFileName() const{
    return wxstr;
 }
 void TextPos(wxCommandEvent& evt){
    Potter_Ctrl* ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
    TextPosDialog dialog(this,myattr,startnew,endnew);
    long start,end;
    wxTextAttr attr;
    if(dialog.ShowModal()==wxID_OK){
    start=(long)dialog.GetSpinCtrlValue();
    end=(long)dialog.GetSpinCtrlValue1();
    attr=dialog.GetTextAttr();
    }
    myattr.SetAlignment(attr.GetAlignment());
    startnew=start;
    endnew=end;
    ptrl->SetStyle(start,end,myattr);
    Refresh();
}
  void TextPosFont(wxCommandEvent& evt){
    Potter_Ctrl* ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
    wxFontPickerCtrl* fontpicker=(wxFontPickerCtrl*)GetToolBar()->FindItem(ID::t13);
    TextPosFontDialog textposfontdialog(this,fontpicker,startone,endone);
    long start,end;
    wxTextAttr attr;
    if(textposfontdialog.ShowModal()==wxID_OK){
        start=textposfontdialog.GetSptrlvalue();
        end=textposfontdialog.GetSptrlvalue1();
        attr=textposfontdialog.GetTextAttr();
    }
   fontpicker->SetSelectedFont(attr.GetFont());
   fontpicker->SetSelectedColour(attr.GetTextColour());
   startone=start;
   endone=end;
   ptrl->SetStyle(start,end,attr);
    Refresh();
  }
  void TextUnderlined(wxCommandEvent& evt){
  Potter_Ctrl * ptrl=(Potter_Ctrl*)FindWindow(ID::t0);
   wxFontPickerCtrl* fontpicker=(wxFontPickerCtrl*)GetToolBar()->FindItem(ID::t13);
  long start,end;
  wxTextAttr attr;
  TextUnderlinedDialog text(this,myattr1,n0,n1);
  if(text.ShowModal()==wxID_OK){
    start=text.GetOne();
    end=text.Gettwo();
    attr.SetFont(fontpicker->GetSelectedFont());
    attr.SetFontUnderlined(text.GetCurrentTextAttr().GetUnderlineType(),text.GetCurrentTextAttr().GetUnderlineColour());
  }
  n0=start;
  n1=end;
   myattr1.SetFont(fontpicker->GetSelectedFont());
  myattr1.SetFontUnderlined(attr.GetUnderlineType(),attr.GetUnderlineColour());
  ptrl->SetStyle(start,end,attr);
    return;
  }
    public:
    MyFrame():wxFrame(nullptr,wxID_ANY,wxT("Potter_text")){
    startone=0;
    endone=0;
    startnew=0;
    endnew=0;
    wxPanel* panel=new wxPanel(this);
    wxBoxSizer* bsizer=new wxBoxSizer(wxVERTICAL);
    bsizer->Add(panel,2,wxEXPAND|wxALL,0);
    SetSizerAndFit(bsizer);
    Potter_Ctrl* ptrl=new Potter_Ctrl(panel,ID::t0,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_RICH);
    wxBoxSizer* bsizer1=new wxBoxSizer(wxVERTICAL);
    bsizer1->Add(ptrl,2,wxEXPAND|wxALL,0);
    panel->SetSizerAndFit(bsizer1);
    wxMenu*nmenu=new wxMenu;
    nmenu->Append(ID::t1,wxT("Open File"));
    nmenu->Append(ID::t2,wxT("Save File"));
    nmenu->Append(ID::t3,wxT("Clear Editor"));
    nmenu->Append(ID::t13,wxT("Clear All"));
    nmenu->Append(ID::t4,wxT("Close File"));
    nmenu->Append(ID::t12,wxT("Clear Display Entry"));
    nmenu->AppendCheckItem(ID::t5,wxT("Auto Save"));
    nmenu->Append(ID::t6,wxT("Close Window"));
    wxMenu* nmenu1=new wxMenu;
    nmenu1->Append(ID::t7,wxT("Apply Font"),wxT("This menu item allows you to apply font rendering to all text in the Text Editor"));
    nmenu1->Append(ID::t8,wxT("Positional Font Rendering"),wxT("This menu item allow you to font rendering to a specified line's text in a text Editor"));
    nmenu1->Append(ID::t9,wxT("Positional Text Alignment"));
    nmenu1->Append(ID::t14,wxT("Positional Underline Style"));
    wxMenuBar* nmbar=new wxMenuBar;
    nmbar->Append(nmenu,wxT("File")); 
    nmbar->Append(nmenu1,wxT("Edit"));
    SetMenuBar(nmbar);
    CreateStatusBar();
    CreateToolBar();
    wxToolBar* tbar=GetToolBar();
    wxTextCtrl* tcrl=new wxTextCtrl(tbar,ID::t10,wxEmptyString,wxDefaultPosition,wxSize(300,25));
    GetToolBar()->AddControl(tcrl);
    tbar->Realize();
    wxButton* tbutton=new wxButton(tbar,ID::t11,wxT("Display"));
    GetToolBar()->AddControl(tbutton);
    tbar->Realize();
    tbar->AddSeparator();
    wxFontPickerCtrl* fontpicker=new wxFontPickerCtrl(tbar,ID::t13);
    tbar->AddControl(fontpicker);
    tbar->Realize();
    ptrl->Bind(wxEVT_TEXT,&TextChanged,this);
    Bind(wxEVT_MENU,&OpenFile,this,ID::t1);
    Bind(wxEVT_MENU,&AutoSave,this,ID::t5);
    Bind(wxEVT_MENU,&SaveFile,this,ID::t2);
    tbar->Bind(wxEVT_BUTTON,&Display,this,ID::t11);
    Bind(wxEVT_MENU,&ClearEditor,this,ID::t3);
    Bind(wxEVT_MENU,&ClearAll,this,ID::t13);
    Bind(wxEVT_MENU,&ClearDisplayEntry,this,t12);  
    Bind(wxEVT_MENU,&CloseWindow,this,t6);
    Bind(wxEVT_MENU,&FontAllText,this,ID::t7);
    Bind(wxEVT_MENU,&TextPos,this,ID::t9);
    Bind(wxEVT_MENU,&TextPosFont,this,ID::t8);
    Bind(wxEVT_MENU,&TextUnderlined,this,ID::t14);
    }
};

bool MyApp::OnInit(){
    MyFrame* myframe=new MyFrame;
    myframe->SetSize(500,500);
    myframe->Show(true);
    return true;
}
