#include "logpanel.h"


void LogPanel::writeLine(wxString line){
	int max_length = 100;
	int length;
	while (wxTextCtrl::GetNumberOfLines() - max_length >=1)
	{
		length = wxTextCtrl::GetLineLength(0);
		if (length != -1)
		{
			wxTextCtrl::Remove(0,length++);
		}
	}
	wxTextCtrl::WriteText(line);
	wxTextCtrl::SetInsertionPoint(wxTextCtrl::GetLastPosition());
}
