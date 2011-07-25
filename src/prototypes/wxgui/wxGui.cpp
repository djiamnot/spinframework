#include <iostream>
#include <wx/wx.h>
#include <wx/version.h>
#include "logpanel.h"

// define a class derived from wxApp
class MyApp: public wxApp
{
  virtual bool OnInit();
};


// the main window is created by deriving a class from wxFrame
class MyFrame: public wxFrame
{
public:
  MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
};

// In order to be able to react to a menu command, it needs to have a unique identifier (const or enum)

enum 
  {
	ID_Quit = 1,
	ID_About,
  };

bool MyApp::OnInit()
{
  MyFrame *frame = new MyFrame(_("My simple frame,"), wxPoint(50,50), wxSize(300,300));

  frame->Connect(ID_Quit, wxEVT_COMMAND_MENU_SELECTED, 
				 (wxObjectEventFunction) &MyFrame::OnQuit);
  frame->Connect(ID_About, wxEVT_COMMAND_MENU_SELECTED,
				 (wxObjectEventFunction) &MyFrame::OnAbout);

  frame->Show(true);
  SetTopWindow(frame);
  return true;
 
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame(NULL, -1, title, pos, size)
{

  wxMenu *menuFile = new wxMenu;

  wxPanel *panel = new wxPanel(this, wxID_ANY);
  wxStaticText *empty = new wxStaticText(panel, -1, " ");
  LogPanel *logPanel = new LogPanel(panel, wxID_ANY);
  menuFile->Append(ID_About, _("&About.."));
  menuFile->AppendSeparator();
  menuFile->Append(ID_Quit, _("E&xit"));

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));
  SetMenuBar(menuBar);
  wxBoxSizer *hSizer = new wxBoxSizer(wxHORIZONTAL);
  wxButton *button = new wxButton(panel, wxID_EXIT, wxT("Quit"));
  wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);
  hSizer->Add(empty,0, wxEXPAND|wxALL,10);
  hSizer->Add(button, 0, wxALIGN_RIGHT|wxALL, 10);
  vSizer->Add(hSizer, 0, wxALIGN_RIGHT|wxEXPAND|wxALL, 10);
  vSizer->Add(logPanel, 0, wxEXPAND|wxALL, 10);
  panel->SetSizer(vSizer);
  vSizer->SetSizeHints(this);
  Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, 
 		  wxCommandEventHandler(MyFrame::OnQuit));
  button->SetFocus(); 
  CreateStatusBar();
  SetStatusText(_("Running..."));
  //logPanel->writeLine("Hello!");
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
  wxString version;
  version.Printf("Spin server.\nRunning toolkit: %s",wxVERSION_STRING);
  wxMessageBox(_(version),
			   _("About Spin server"), 
			   wxOK | wxICON_INFORMATION, this);
}

// run main() (from wx)
IMPLEMENT_APP(MyApp)
