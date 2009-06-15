#include "wxVessConfig.h"

//(*InternalHeaders(wxVessConfig)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(wxVessConfig)
const long wxVessConfig::ID_STATICTEXT1 = wxNewId();
const long wxVessConfig::ID_TEXTCTRL1 = wxNewId();
const long wxVessConfig::ID_STATICTEXT2 = wxNewId();
const long wxVessConfig::ID_TEXTCTRL2 = wxNewId();
const long wxVessConfig::ID_STATICTEXT3 = wxNewId();
const long wxVessConfig::ID_TEXTCTRL3 = wxNewId();
const long wxVessConfig::ID_STATICTEXT4 = wxNewId();
const long wxVessConfig::ID_TEXTCTRL4 = wxNewId();
const long wxVessConfig::ID_STATICTEXT5 = wxNewId();
const long wxVessConfig::ID_TEXTCTRL5 = wxNewId();
const long wxVessConfig::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxVessConfig,wxFrame)
	//(*EventTable(wxVessConfig)
	//*)
END_EVENT_TABLE()

wxVessConfig::wxVessConfig(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(wxVessConfig)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	
	Create(parent, id, _("SpIF :: Configuration"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("VESS Session ID:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	vessID = new wxTextCtrl(Panel1, ID_TEXTCTRL1, _("Default"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	FlexGridSizer1->Add(vessID, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Listening Channel:"));
	FlexGridSizer2 = new wxFlexGridSizer(1, 4, 0, 0);
	StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Addr:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	wxFont StaticText2Font = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
	if ( !StaticText2Font.Ok() ) StaticText2Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	StaticText2Font.SetPointSize(8);
	StaticText2->SetFont(StaticText2Font);
	FlexGridSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	rxAddr = new wxTextCtrl(Panel1, ID_TEXTCTRL2, _("Text"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	FlexGridSizer2->Add(rxAddr, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("Port:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	wxFont StaticText3Font = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
	if ( !StaticText3Font.Ok() ) StaticText3Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	StaticText3Font.SetPointSize(8);
	StaticText3->SetFont(StaticText3Font);
	FlexGridSizer2->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	rxPort = new wxTextCtrl(Panel1, ID_TEXTCTRL3, _("Text"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	FlexGridSizer2->Add(rxPort, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(StaticBoxSizer1, 2, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Broadcasting Channel:"));
	FlexGridSizer3 = new wxFlexGridSizer(1, 4, 0, 0);
	StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, _("Addr:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	wxFont StaticText4Font = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
	if ( !StaticText4Font.Ok() ) StaticText4Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	StaticText4Font.SetPointSize(8);
	StaticText4->SetFont(StaticText4Font);
	FlexGridSizer3->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txAddr = new wxTextCtrl(Panel1, ID_TEXTCTRL4, _("Text"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	FlexGridSizer3->Add(txAddr, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, _("Port:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	wxFont StaticText5Font = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
	if ( !StaticText5Font.Ok() ) StaticText5Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	StaticText5Font.SetPointSize(8);
	StaticText5->SetFont(StaticText5Font);
	FlexGridSizer3->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txPort = new wxTextCtrl(Panel1, ID_TEXTCTRL5, _("Text"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	FlexGridSizer3->Add(txPort, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(StaticBoxSizer2, 2, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel1->SetSizer(BoxSizer1);
	BoxSizer1->Fit(Panel1);
	BoxSizer1->SetSizeHints(Panel1);
	
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&wxVessConfig::OnClose);
	//*)
}

wxVessConfig::~wxVessConfig()
{
	//(*Destroy(wxVessConfig)
	//*)
}


void wxVessConfig::OnClose(wxCloseEvent& event)
{
    this->Hide();

    event.Veto();
}
