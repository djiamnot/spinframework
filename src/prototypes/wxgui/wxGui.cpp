#include <iostream>
#include <wx/cmdline.h>
#include <wx/wx.h>
#include <wx/chartype.h>
#include <wx/string.h>
#include <wx/version.h>
//#include "logpanel.h"
#include "ServerFrame.h"

// define a class derived from wxApp
class MyApp: public wxApp
{
public:
  virtual bool OnInit();
  virtual void OnInitCmdLine(wxCmdLineParser& parser);
  virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
  virtual bool OnCmdLineError(wxCmdLineParser & parser);

};




static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
     { wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
     { wxCMD_LINE_SWITCH, "s", "server", "automatically start in server mode" },
     { wxCMD_LINE_SWITCH, "c", "client", "automatically start in client mode" },
     { wxCMD_LINE_NONE }
};

// In order to be able to react to a menu command, it needs to have a unique identifier (const or enum)





bool MyApp::OnInit()
{
	wxCmdLineParser parser (g_cmdLineDesc, argc, argv);
	ServerFrame *frame = new ServerFrame(_("My simple frame,"), wxPoint(50,50), wxSize(300,300));


	/*
	frame->Connect(ID_Quit, wxEVT_COMMAND_MENU_SELECTED,
				 (wxObjectEventFunction) &ServerFrame::OnQuit);
	frame->Connect(ID_About, wxEVT_COMMAND_MENU_SELECTED,
				 (wxObjectEventFunction) &ServerFrame::OnAbout);
*/

	frame->Show(true);
	SetTopWindow(frame);
	return true;
 
}

void MyApp::OnInitCmdLine(wxCmdLineParser& parser){
	parser.SetDesc (g_cmdLineDesc);
	parser.SetSwitchChars(wxT("-"));
}

bool MyApp::OnCmdLineError(wxCmdLineParser & parser)
{
    parser.Usage();
    return false;
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser){
	parser.Parse();
	if(parser.Found(wxT("s"))){
		wxMessageBox(wxT("Server!"));
	}
	return true;
}



void ServerFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

void ServerFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
  wxString version;
  version.Printf("Spin server.\nRunning toolkit: %s",wxVERSION_STRING);
  wxMessageBox(_(version),
			   _("About Spin server"), 
			   wxOK | wxICON_INFORMATION, this);
}

BEGIN_EVENT_TABLE(ServerFrame, wxFrame)
    EVT_MENU(ID_Quit,  ServerFrame::OnQuit)
    EVT_MENU(ID_About, ServerFrame::OnAbout)
END_EVENT_TABLE()

// run main() (from wx)
IMPLEMENT_APP(MyApp)
