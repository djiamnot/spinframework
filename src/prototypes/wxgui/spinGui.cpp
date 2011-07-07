#include <iostream>
#include <wx/wx.h>
#include <wx/version.h>
#include "button.h"

//const String wxVersion=wxVERSION_STRING

// declare function:
void processArgs();

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

// In order to be able to react to a menu command, it needs to have a unique identifier or (const or enum)

enum 
  {
	ID_Quit = 1,
	ID_About,
  };

bool MyApp::OnInit()
{
  //processArgs();
  // wxPrintf("Using: %s",wxVERSION_STRING );
  MyFrame *frame = new MyFrame(_("My simple frame,"), wxPoint(50,50), wxSize(250,150));

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

  menuFile->Append(ID_About, _("&About.."));
  menuFile->AppendSeparator();
  menuFile->Append(ID_Quit, _("E&xit"));

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));
  SetMenuBar(menuBar);
  wxButton *button = new wxButton(panel, wxID_EXIT, wxT("Quit"), 
								  wxPoint(20, 20));
  Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, 
		  wxCommandEventHandler(Button::OnQuit));
  button->SetFocus(); 
  CreateStatusBar();
  SetStatusText(_("Running..."));
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
  wxString version;
  version.Printf("Spin server.\n Running toolkit: %s",wxVERSION_STRING);
  
  wxMessageBox(_(version),
			   _("About Spin server"), 
			   wxOK | wxICON_INFORMATION, this);
}

void Button::OnQuit(wxCommandEvent & WXUNUSED(event))
{
    Close(true);
}
/*
void processArgs()
{
  wxCmdLineParser parser (cmdLineDesc, argc, argv);
  parser.Parse();
  cout << endl << "Number of params: " << parser.GetParamCount() << endl;
  cout << "Param 1: " << parser.GetParam(0).mb_str() << endl;
  cout << "Param 2: " << parser.GetParam(1).mb_str() << endl;
  cout << "Param 3: " << parser.GetParam(2).mb_str() << endl;
  wxString optf;
  parser.Found( wxT("f"), &optf);
  cout << "Option f: " << optf.mb_str() << endl;
  cout << "Option z? " << parser.Found( wxT("z") ) << endl;
}
*/
// this macro in wx implements the main() function.
IMPLEMENT_APP(MyApp)
