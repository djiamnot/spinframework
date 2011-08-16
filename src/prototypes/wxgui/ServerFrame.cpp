//#include <wx/event.h>
#include "ServerFrame.h"

enum
  {
	ID_Quit = 1,
	ID_About,
  };



ServerFrame::ServerFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
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
 		  wxCommandEventHandler(ServerFrame::OnQuit));
  button->SetFocus();
  CreateStatusBar();
  SetStatusText(_("Running..."));
  logPanel->writeLine(wxT("Hello!"));
}
