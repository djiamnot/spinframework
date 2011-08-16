#include <wx/wx.h>
#include "logpanel.h"

// the main window is created by deriving a class from wxFrame
class ServerFrame: public wxFrame
{
	LogPanel *logPanel;
	wxArrayString myargs;
public:
  ServerFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  DECLARE_EVENT_TABLE()

};
