///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "NodeManagerGUI.h"

///////////////////////////////////////////////////////////////////////////

NodeManagerFrameBase::NodeManagerFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	mMenuBar = new wxMenuBar( 0 );
	mMainMenu = new wxMenu();
	wxMenuItem* mLoadItem;
	mLoadItem = new wxMenuItem( mMainMenu, wxID_ANY, wxString( wxT("Load Settings") ) , wxEmptyString, wxITEM_NORMAL );
	mMainMenu->Append( mLoadItem );
	
	wxMenuItem* mExitItem;
	mExitItem = new wxMenuItem( mMainMenu, wxID_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	mMainMenu->Append( mExitItem );
	
	mMenuBar->Append( mMainMenu, wxT("File") ); 
	
	this->SetMenuBar( mMenuBar );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	mNotebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	mMainPanel = new wxPanel( mNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	mCurrentStatusSizer = new wxStaticBoxSizer( new wxStaticBox( mMainPanel, wxID_ANY, wxT("Current Status") ), wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText9 = new wxStaticText( mMainPanel, wxID_ANY, wxT("Local Connections:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	m_staticText9->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer10->Add( m_staticText9, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mLocalConnectionsText = new wxStaticText( mMainPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	mLocalConnectionsText->Wrap( -1 );
	bSizer10->Add( mLocalConnectionsText, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	gSizer1->Add( bSizer10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText10 = new wxStaticText( mMainPanel, wxID_ANY, wxT("Remote Connections:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	m_staticText10->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer11->Add( m_staticText10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mRemoteConnectionsText = new wxStaticText( mMainPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	mRemoteConnectionsText->Wrap( -1 );
	bSizer11->Add( mRemoteConnectionsText, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	gSizer1->Add( bSizer11, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText18 = new wxStaticText( mMainPanel, wxID_ANY, wxT("Display Update Time:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	m_staticText18->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer18->Add( m_staticText18, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mUpdateDisplayTimeText = new wxStaticText( mMainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mUpdateDisplayTimeText->Wrap( -1 );
	bSizer18->Add( mUpdateDisplayTimeText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	gSizer1->Add( bSizer18, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText181 = new wxStaticText( mMainPanel, wxID_ANY, wxT("Update Interval (seconds):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText181->Wrap( -1 );
	m_staticText181->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer181->Add( m_staticText181, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString mUpdateIntervalChoiceBoxChoices[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10") };
	int mUpdateIntervalChoiceBoxNChoices = sizeof( mUpdateIntervalChoiceBoxChoices ) / sizeof( wxString );
	mUpdateIntervalChoiceBox = new wxChoice( mMainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, mUpdateIntervalChoiceBoxNChoices, mUpdateIntervalChoiceBoxChoices, 0 );
	mUpdateIntervalChoiceBox->SetSelection( 1 );
	bSizer181->Add( mUpdateIntervalChoiceBox, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	gSizer1->Add( bSizer181, 1, wxEXPAND, 5 );
	
	mCurrentStatusSizer->Add( gSizer1, 0, wxEXPAND, 5 );
	
	bSizer9->Add( mCurrentStatusSizer, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( mMainPanel, wxID_ANY, wxT("Local Connections") ), wxVERTICAL );
	
	mLocalConnections = new wxListCtrl( mMainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
	sbSizer6->Add( mLocalConnections, 1, wxALL|wxEXPAND, 5 );
	
	bSizer9->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( mMainPanel, wxID_ANY, wxT("Network Connections") ), wxVERTICAL );
	
	mRemoteConnections = new wxListCtrl( mMainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
	sbSizer61->Add( mRemoteConnections, 1, wxALL|wxEXPAND, 5 );
	
	bSizer9->Add( sbSizer61, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	mInitializeButton = new wxButton( mMainPanel, wxID_ANY, wxT("Initialize"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( mInitializeButton, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	mShutdownButton = new wxButton( mMainPanel, wxID_ANY, wxT("Shutdown"), wxDefaultPosition, wxDefaultSize, 0 );
	mShutdownButton->Enable( false );
	
	bSizer17->Add( mShutdownButton, 0, wxALL, 5 );
	
	bSizer9->Add( bSizer17, 0, wxALIGN_RIGHT, 5 );
	
	mMainPanel->SetSizer( bSizer9 );
	mMainPanel->Layout();
	bSizer9->Fit( mMainPanel );
	mNotebook->AddPage( mMainPanel, wxT("Node Manager"), true );
	mSettings = new wxPanel( mNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer163;
	bSizer163 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText133 = new wxStaticText( mSettings, wxID_ANY, wxT("Drop Connection Time (s)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText133->Wrap( -1 );
	bSizer163->Add( m_staticText133, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mDropConnectionTime = new wxComboBox( mSettings, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	mDropConnectionTime->Append( wxT("0") );
	mDropConnectionTime->Append( wxT("1") );
	mDropConnectionTime->Append( wxT("2") );
	mDropConnectionTime->Append( wxT("3") );
	mDropConnectionTime->Append( wxT("4") );
	mDropConnectionTime->Append( wxT("5") );
	mDropConnectionTime->Append( wxT("6") );
	mDropConnectionTime->Append( wxT("7") );
	mDropConnectionTime->Append( wxT("8") );
	mDropConnectionTime->Append( wxT("9") );
	mDropConnectionTime->Append( wxT("10") );
	mDropConnectionTime->SetToolTip( wxT("How long to wait before dropping inactive connection. 0 = never drop.") );
	
	bSizer163->Add( mDropConnectionTime, 0, wxALL, 5 );
	
	bSizer15->Add( bSizer163, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( mSettings, wxID_ANY, wxT("Primary Network Transport"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer16->Add( m_staticText13, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString mDefaultTransportChoices[] = { wxT("JUDP"), wxT("JTCP") };
	int mDefaultTransportNChoices = sizeof( mDefaultTransportChoices ) / sizeof( wxString );
	mDefaultTransport = new wxChoice( mSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, mDefaultTransportNChoices, mDefaultTransportChoices, 0 );
	mDefaultTransport->SetSelection( 0 );
	mDefaultTransport->SetToolTip( wxT("Preferred connection type to other nodes.") );
	
	bSizer16->Add( mDefaultTransport, 0, wxALL, 5 );
	
	bSizer15->Add( bSizer16, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer161;
	bSizer161 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText131 = new wxStaticText( mSettings, wxID_ANY, wxT("Default Network Port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	bSizer161->Add( m_staticText131, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mDefaultPort = new wxTextCtrl( mSettings, wxID_ANY, wxT("3794"), wxDefaultPosition, wxDefaultSize, 0 );
	mDefaultPort->SetToolTip( wxT("Default network port to use.  3794 = jaus and is preferred") );
	
	bSizer161->Add( mDefaultPort, 0, wxALL, 5 );
	
	bSizer15->Add( bSizer161, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1622;
	bSizer1622 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1322 = new wxStaticText( mSettings, wxID_ANY, wxT("Network Interface"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1322->Wrap( -1 );
	bSizer1622->Add( m_staticText1322, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mNetworkInterface = new wxComboBox( mSettings, wxID_ANY, wxT("127.0.0.1"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_SORT ); 
	mNetworkInterface->SetToolTip( wxT("Default network interface to listen for incomming connections. -1 is default for any.") );
	
	bSizer1622->Add( mNetworkInterface, 0, wxALL, 5 );
	
	bSizer15->Add( bSizer1622, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer162;
	bSizer162 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText132 = new wxStaticText( mSettings, wxID_ANY, wxT("Multicast Address"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText132->Wrap( -1 );
	bSizer162->Add( m_staticText132, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mMulticastIP = new wxTextCtrl( mSettings, wxID_ANY, wxT("239.255.0.1"), wxDefaultPosition, wxDefaultSize, 0 );
	mMulticastIP->SetToolTip( wxT("Multicast IP to use for discovery of other JAUS components.") );
	
	bSizer162->Add( mMulticastIP, 0, wxALL, 5 );
	
	bSizer15->Add( bSizer162, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1621;
	bSizer1621 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1321 = new wxStaticText( mSettings, wxID_ANY, wxT("Multicast TTL"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1321->Wrap( -1 );
	bSizer1621->Add( m_staticText1321, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mMulticastTLL = new wxTextCtrl( mSettings, wxID_ANY, wxT("255"), wxDefaultPosition, wxDefaultSize, 0 );
	mMulticastTLL->SetToolTip( wxT("Multicast time to live.") );
	
	bSizer1621->Add( mMulticastTLL, 0, wxALL, 5 );
	
	bSizer15->Add( bSizer1621, 0, wxEXPAND, 5 );
	
	mSettings->SetSizer( bSizer15 );
	mSettings->Layout();
	bSizer15->Fit( mSettings );
	mNotebook->AddPage( mSettings, wxT("Settings"), false );
	
	bSizer5->Add( mNotebook, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer5 );
	this->Layout();
	mStatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( NodeManagerFrameBase::OnUpdateUI ) );
	this->Connect( mLoadItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( NodeManagerFrameBase::OnLoadSettings ) );
	this->Connect( mExitItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( NodeManagerFrameBase::OnExit ) );
	mUpdateIntervalChoiceBox->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( NodeManagerFrameBase::OnUpdateInterval ), NULL, this );
	mInitializeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NodeManagerFrameBase::OnInitialize ), NULL, this );
	mShutdownButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NodeManagerFrameBase::OnShutdown ), NULL, this );
}

NodeManagerFrameBase::~NodeManagerFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( NodeManagerFrameBase::OnUpdateUI ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( NodeManagerFrameBase::OnLoadSettings ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( NodeManagerFrameBase::OnExit ) );
	mUpdateIntervalChoiceBox->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( NodeManagerFrameBase::OnUpdateInterval ), NULL, this );
	mInitializeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NodeManagerFrameBase::OnInitialize ), NULL, this );
	mShutdownButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NodeManagerFrameBase::OnShutdown ), NULL, this );
	
}

DiscoveryPanelBase::DiscoveryPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Component ID (Subystem | Node | Component):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	m_staticText11->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer101->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mSubsystemTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( mSubsystemTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mNodeChoiceBox = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer101->Add( mNodeChoiceBox, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mComponentChoiceBox = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer101->Add( mComponentChoiceBox, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mInitializeButton = new wxButton( this, wxID_ANY, wxT("Initialize"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( mInitializeButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mShutdownButton = new wxButton( this, wxID_ANY, wxT("Shutdown"), wxDefaultPosition, wxDefaultSize, 0 );
	mShutdownButton->Enable( false );
	
	bSizer101->Add( mShutdownButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	fgSizer1->Add( bSizer101, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->AddGrowableRow( 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Subsystems (Select for Details)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer111->Add( m_staticText12, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mSubsystemList = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SORT ); 
	bSizer111->Add( mSubsystemList, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("TTL"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	m_staticText10->SetToolTip( wxT("Time to Live (TLL) for components and subsystems.  If no updates heard for X seconds, component is dropped.") );
	
	bSizer11->Add( m_staticText10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString mTTLChoices[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10") };
	int mTTLNChoices = sizeof( mTTLChoices ) / sizeof( wxString );
	mTTL = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mTTLNChoices, mTTLChoices, 0 );
	mTTL->SetSelection( 4 );
	mTTL->SetToolTip( wxT("Time to Live in seconds") );
	
	bSizer11->Add( mTTL, 1, wxALL, 5 );
	
	bSizer111->Add( bSizer11, 0, wxEXPAND, 5 );
	
	fgSizer2->Add( bSizer111, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	mSubsystemTree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_FULL_ROW_HIGHLIGHT );
	bSizer12->Add( mSubsystemTree, 1, wxALL|wxEXPAND, 5 );
	
	fgSizer2->Add( bSizer12, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText18 = new wxStaticText( this, wxID_ANY, wxT("Select Component"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer19->Add( m_staticText18, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxArrayString mSelectComponentChoiceChoices;
	mSelectComponentChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mSelectComponentChoiceChoices, 0 );
	mSelectComponentChoice->SetSelection( -1 );
	bSizer19->Add( mSelectComponentChoice, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText19 = new wxStaticText( this, wxID_ANY, wxT("Command Options"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer19->Add( m_staticText19, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer19->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText17 = new wxStaticText( this, wxID_ANY, wxT("Authority Level"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer19->Add( m_staticText17, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxArrayString mAuthorityChoiceChoices;
	mAuthorityChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mAuthorityChoiceChoices, 0 );
	mAuthorityChoice->SetSelection( 0 );
	bSizer19->Add( mAuthorityChoice, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mControlButton = new wxButton( this, wxID_ANY, wxT("Take Control"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( mControlButton, 0, wxALL|wxEXPAND, 5 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer19->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	mResumeButton = new wxButton( this, wxID_ANY, wxT("Send Resume"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( mResumeButton, 0, wxALL|wxEXPAND, 5 );
	
	mStandbyButton = new wxButton( this, wxID_ANY, wxT("Send Standby"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( mStandbyButton, 0, wxALL|wxEXPAND, 5 );
	
	mSendShutdown = new wxButton( this, wxID_ANY, wxT("Send Shutdown"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( mSendShutdown, 0, wxALL|wxEXPAND, 5 );
	
	fgSizer2->Add( bSizer19, 1, wxEXPAND, 5 );
	
	fgSizer1->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( DiscoveryPanelBase::OnUpdateUI ) );
	mInitializeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnInitializeButton ), NULL, this );
	mShutdownButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnShutdownButton ), NULL, this );
	mSubsystemList->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DiscoveryPanelBase::OnSelectSubsystem ), NULL, this );
	mTTL->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DiscoveryPanelBase::OnTLL ), NULL, this );
	mSelectComponentChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DiscoveryPanelBase::OnSelectComponentChoice ), NULL, this );
	mAuthorityChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DiscoveryPanelBase::OnAuthorityLevel ), NULL, this );
	mControlButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnTakeControl ), NULL, this );
	mResumeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnSendResume ), NULL, this );
	mStandbyButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnSendStandby ), NULL, this );
	mSendShutdown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnSendShutdown ), NULL, this );
}

DiscoveryPanelBase::~DiscoveryPanelBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( DiscoveryPanelBase::OnUpdateUI ) );
	mInitializeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnInitializeButton ), NULL, this );
	mShutdownButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnShutdownButton ), NULL, this );
	mSubsystemList->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DiscoveryPanelBase::OnSelectSubsystem ), NULL, this );
	mTTL->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DiscoveryPanelBase::OnTLL ), NULL, this );
	mSelectComponentChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DiscoveryPanelBase::OnSelectComponentChoice ), NULL, this );
	mAuthorityChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DiscoveryPanelBase::OnAuthorityLevel ), NULL, this );
	mControlButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnTakeControl ), NULL, this );
	mResumeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnSendResume ), NULL, this );
	mStandbyButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnSendStandby ), NULL, this );
	mSendShutdown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscoveryPanelBase::OnSendShutdown ), NULL, this );
	
}
