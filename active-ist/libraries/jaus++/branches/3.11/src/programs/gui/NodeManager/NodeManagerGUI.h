///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __NODEMANAGERGUI_H__
#define __NODEMANAGERGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/statbox.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/treectrl.h>
#include <wx/statline.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class NodeManagerFrameBase
///////////////////////////////////////////////////////////////////////////////
class NodeManagerFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mMenuBar;
		wxMenu* mMainMenu;
		wxNotebook* mNotebook;
		wxPanel* mMainPanel;
		wxStaticBoxSizer* mCurrentStatusSizer;
		wxStaticText* m_staticText9;
		wxStaticText* mLocalConnectionsText;
		wxStaticText* m_staticText10;
		wxStaticText* mRemoteConnectionsText;
		wxStaticText* m_staticText18;
		wxStaticText* mUpdateDisplayTimeText;
		wxStaticText* m_staticText181;
		wxChoice* mUpdateIntervalChoiceBox;
		wxListCtrl* mLocalConnections;
		wxListCtrl* mRemoteConnections;
		wxButton* mInitializeButton;
		wxButton* mShutdownButton;
		wxPanel* mSettings;
		wxStaticText* m_staticText133;
		wxComboBox* mDropConnectionTime;
		wxStaticText* m_staticText13;
		wxChoice* mDefaultTransport;
		wxStaticText* m_staticText131;
		wxTextCtrl* mDefaultPort;
		wxStaticText* m_staticText1322;
		wxComboBox* mNetworkInterface;
		wxStaticText* m_staticText132;
		wxTextCtrl* mMulticastIP;
		wxStaticText* m_staticText1321;
		wxTextCtrl* mMulticastTLL;
		wxStatusBar* mStatusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnLoadSettings( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateInterval( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInitialize( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShutdown( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		NodeManagerFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("JAUS++ Node Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 940,517 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~NodeManagerFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DiscoveryPanelBase
///////////////////////////////////////////////////////////////////////////////
class DiscoveryPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText11;
		wxTextCtrl* mSubsystemTextCtrl;
		wxComboBox* mNodeChoiceBox;
		wxComboBox* mComponentChoiceBox;
		wxButton* mInitializeButton;
		wxButton* mShutdownButton;
		wxStaticText* m_staticText12;
		wxListBox* mSubsystemList;
		wxStaticText* m_staticText10;
		wxChoice* mTTL;
		wxTreeCtrl* mSubsystemTree;
		wxStaticText* m_staticText18;
		wxChoice* mSelectComponentChoice;
		wxStaticText* m_staticText19;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText17;
		wxChoice* mAuthorityChoice;
		wxButton* mControlButton;
		wxStaticLine* m_staticline3;
		wxButton* mResumeButton;
		wxButton* mStandbyButton;
		wxButton* mSendShutdown;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnInitializeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShutdownButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectSubsystem( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTLL( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectComponentChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAuthorityLevel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTakeControl( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSendResume( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSendStandby( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSendShutdown( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DiscoveryPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 909,466 ), long style = wxTAB_TRAVERSAL ); 
		~DiscoveryPanelBase();
	
};

#endif //__NODEMANAGERGUI_H__
