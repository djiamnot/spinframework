#include <wx/wx.h>
#include <string>

class LogPanel : public wxTextCtrl
{
public:
	//LogPanel();
	LogPanel (wxWindow *parent,wxWindowID id, wxString string="", const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL)
		: wxTextCtrl(parent, id, string, pos, size, style ) {}
	//wxPanel *m_parent;
	void writeLine(wxString line);
};


