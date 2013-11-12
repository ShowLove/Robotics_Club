////////////////////////////////////////////////////////////////////////////////////
///
///  \file NodeManagerFrame.cpp
///  \brief Main file for the NodeManagerFrame code.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12/2/2011
///  <br>Copyright (c) 2011
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#include "NodeManagerFrame.h"
#include "DiscoveryPanel.h"
#include <cxutils/networking/socket.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <sstream>
#include <iomanip>

#ifdef WIN32
std::string MAIN_ICON = "icons/jaus++_logo.ico";
wxBitmapType MAIN_ICON_TYPE = wxBITMAP_TYPE_ICO;
#else
std::string MAIN_ICON = "icons/jaus++_logo.png";
wxBitmapType MAIN_ICON_TYPE = wxBITMAP_TYPE_PNG;
#endif


DECLARE_EVENT_TYPE(wxEVT_UPDATE_MAIN_PANEL, wxCommandEvent)
DEFINE_EVENT_TYPE(wxEVT_UPDATE_MAIN_PANEL)

enum Columns
{
    NUMBER = 0,
    TRANSPORT,
    IP,
    PORT,
    ID,
    SENT,
    RECEIVED,
    BYTES_SENT,
    BYTES_RECEIVED,
    TOTAL_BYTES_SENT,
    TOTAL_BYTES_RECEIVED,
    COLUMNS
};


/** Constructor, initializes node manager by default. */
NodeManagerFrame::NodeManagerFrame( wxWindow* parent )
    :
NodeManagerFrameBase( parent )
{
    // Add an icon/logo to the GUI.
    wxIcon icon;
    if(icon.LoadFile(wxString(MAIN_ICON.c_str(), wxConvUTF8), MAIN_ICON_TYPE))
    {
        SetIcon(icon);
    }

    CxUtils::IP4Address::List available;
    this->mNetworkInterface->Clear();
    if(CxUtils::Socket::GetHostAddresses(available) && available.size() > 0)
    {
        for(unsigned int i = 0; i < (unsigned int)available.size(); i++)
        {
            if(available[i] != "127.0.0.1")
            {
                this->mNetworkInterface->Append(wxString(available[i].mString.c_str(), wxConvUTF8));
            }
        }
    }
    this->mNetworkInterface->SetValue(wxT("127.0.0.1"));

    // Custom event to update drawing.
    this->Connect(wxEVT_UPDATE_MAIN_PANEL, wxCommandEventHandler(NodeManagerFrame::OnUpdateMainPanel), NULL, this);

    DiscoveryPanel* discoveryPanel = new DiscoveryPanel(mNotebook);
    mpDiscoveryPanel  = discoveryPanel;
    discoveryPanel->SetNodeManager(&mNodeManager);
    this->mNotebook->AddPage(discoveryPanel, wxT("Monitor"), false);

    mTimer.RegisterTimerEvent(this);
    mTimer.Start(1.0/(this->mUpdateIntervalChoiceBox->GetCurrentSelection() + 1));
    // Default settings.
    mNodeManager.LoadSettings("settings/services.xml");
    mNodeManager.SetConnectionsPerThread(1);
    
    if(mNodeManager.Initialize())
    {
        this->mInitializeButton->Disable();
        this->mShutdownButton->Enable();
        this->SetTitle(wxT("JAUS++ Node Manager - Running"));
    }
}

/** Called on UI Update. */
void NodeManagerFrame::OnUpdateUI( wxUpdateUIEvent& event )
{

}


/** Triggered on Exit event. */
void NodeManagerFrame::OnExit( wxCommandEvent& event )
{
    mTimer.Stop();
    mNodeManager.Shutdown();
    Close(true);
}


/** Called when interval selection is chnaged for updating display. */
void NodeManagerFrame::OnUpdateInterval( wxCommandEvent& event )
{
    this->mTimer.ChangeFrequency(1.0/(this->mUpdateIntervalChoiceBox->GetCurrentSelection() + 1));
}


/** Triggered on pressing initialize button, it starts the NodeManager. */
void NodeManagerFrame::OnInitialize( wxCommandEvent& event )
{
    long disconnectTime = mNodeManager.GetSettings()->GetDropConnectionTimeMs()/1000;

    this->mDropConnectionTime->GetValue().ToLong(&disconnectTime);
    mNodeManager.GetSettings()->SetDropConnectionTimeMs(1000*disconnectTime);

    long defaultPort = mNodeManager.GetSettings()->GetDefaultPortNumber();

    this->mDefaultPort->GetValue().ToLong(&defaultPort);

    if(defaultPort > 0)
    {
        mNodeManager.GetSettings()->SetDefaultPortNumber((unsigned short)defaultPort);
    }
    if(this->mDefaultTransport->GetSelection() == 0)
    {
        mNodeManager.GetSettings()->SetTcpAsDefault(false);
    }
    else
    {
        mNodeManager.GetSettings()->SetTcpAsDefault(true);
    }
    mNodeManager.GetSettings()->SetNetworkInterface( std::string(this->mNetworkInterface->GetValue().ToAscii().data()) );

    long ttl = ttl = mNodeManager.GetSettings()->GetMulticastTLL();
    this->mMulticastTLL->GetValue().ToLong(&ttl);
    if(ttl < 0 || ttl > 255)
    {
        ttl = mNodeManager.GetSettings()->GetMulticastTLL();
    }
    JAUS::IP4Address multicast = mNodeManager.GetSettings()->GetMulticastIP();

    multicast = std::string(this->mMulticastIP->GetValue().ToAscii().data());
    mNodeManager.GetSettings()->SetMulticast(multicast, (unsigned char)ttl);

    if(mNodeManager.Initialize())
    {
        this->mInitializeButton->Disable();
        this->mShutdownButton->Enable();
        this->SetTitle(wxT("JAUS++ Node Manager - Running"));
    }
    else
    {
        wxMessageBox(wxString("Another Node Manager is Running", wxConvUTF8), wxString("Unable to Start", wxConvUTF8));
    }
}


/** Triggered on pressing shutdown button, it stops the NodeManager. */
void NodeManagerFrame::OnShutdown( wxCommandEvent& event )
{
    mNodeManager.Shutdown();
    this->mInitializeButton->Enable();
    this->mShutdownButton->Disable();
    this->SetTitle(wxT("JAUS++ Node Manager - Inactive"));

    ((DiscoveryPanel*)mpDiscoveryPanel)->OnShutdownButton(event);
}

/** Triggered on pressing shutdown button, it stops the NodeManager. */
void NodeManagerFrame::OnLoadSettings( wxCommandEvent& event )
{

    wxFileDialog dialog(this);
    if(dialog.GetPath().empty())
    {
        dialog.SetPath(wxString("settings/", wxConvUTF8));
    }
    if(dialog.ShowModal() == wxID_OK)
    {
        bool wasRunning = this->mShutdownButton->IsEnabled();
        mNodeManager.Shutdown();
        this->mInitializeButton->Enable();
        this->mShutdownButton->Disable();
        this->SetTitle(wxT("JAUS++ Node Manager - Inactive"));
        if(mNodeManager.LoadSettings(dialog.GetPath().ToAscii().data()) == false)
        {
            wxMessageBox(wxString("Failed to load settings", wxConvUTF8), wxString("Error", wxConvUTF8));
        }
        else
        {
            std::stringstream str;

            str.clear();
            str.str(std::string());
            str << mNodeManager.GetSettings()->GetDefaultPortNumber();
            this->mDefaultPort->SetValue(wxString(str.str().c_str(), wxConvUTF8));

            str.clear();
            str.str(std::string());
            str << mNodeManager.GetSettings()->GetDropConnectionTimeMs();
            this->mDropConnectionTime->SetValue(wxString(str.str().c_str(), wxConvUTF8));

            if(mNodeManager.GetSettings()->IsTcpDefault())
            {
                mDefaultTransport->SetSelection(1);
            }
            else
            {
                mDefaultTransport->SetSelection(0);
            }

            str.clear();
            str.str(std::string());
            str << mNodeManager.GetSettings()->GetNetworkInterface().mString;
            this->mNetworkInterface->SetValue(wxString(str.str().c_str(), wxConvUTF8));

            str.clear();
            str.str(std::string());
            str << mNodeManager.GetSettings()->GetMulticastIP().mString;
            this->mMulticastIP->SetValue(wxString(str.str().c_str(), wxConvUTF8));

            str.clear();
            str.str(std::string());
            str << (int)mNodeManager.GetSettings()->GetMulticastTLL();
            this->mMulticastTLL->SetValue(wxString(str.str().c_str(), wxConvUTF8));
        }
        if(wasRunning)
        {
            if(mNodeManager.Initialize())
            {
                this->mInitializeButton->Disable();
                this->mShutdownButton->Enable();
                this->SetTitle(wxT("JAUS++ Node Manager - Running"));
            }
            else
            {
                wxMessageBox(wxString("Failed to Re-initialize Node Manager", wxConvUTF8), wxString("Error", wxConvUTF8));
            }
        }
    }
}

/** Event handler to update the display stats. */
void NodeManagerFrame::OnUpdateMainPanel( wxCommandEvent& event)
{
    std::stringstream str;

    // Get stats
    JAUS::Connection::Statistics::List local, remote;
    mNodeManager.GetStatistics(local, remote);
    
    //this->mLocalConnections->remove
    JAUS::Connection::Statistics::List::iterator stat;
    unsigned int count = 0;

    str.clear();
    str.str(std::string());
    str << local.size();
    this->mLocalConnectionsText->SetLabel(wxString( str.str().c_str(), wxConvUTF8));

    str.clear();
    str.str(std::string());
    str << remote.size();
    this->mRemoteConnectionsText->SetLabel(wxString( str.str().c_str(), wxConvUTF8));

    this->mUpdateDisplayTimeText->SetLabel(wxString( JAUS::Time::GetLocalTime().ToString().c_str(), wxConvUTF8) );


    if(mLocalConnections->GetItemCount() != (int)local.size())
    {
        this->mLocalConnections->ClearAll();
        this->mLocalConnections->InsertColumn(NUMBER, wxT("UID"));
        this->mLocalConnections->InsertColumn(TRANSPORT, wxT("Type"));
        this->mLocalConnections->InsertColumn(IP, wxT("Source IP : Dest IP"));
        this->mLocalConnections->InsertColumn(PORT, wxT("Port/Name"));
        this->mLocalConnections->InsertColumn(ID, wxT("Source ID : Dest ID"));
        this->mLocalConnections->InsertColumn(SENT, wxT("Sent To Dest"));
        this->mLocalConnections->InsertColumn(RECEIVED, wxT("Received From Dest"));
        this->mLocalConnections->InsertColumn(BYTES_SENT, wxT("KB Sent"));
        this->mLocalConnections->InsertColumn(BYTES_RECEIVED, wxT("KB Received"));
        this->mLocalConnections->InsertColumn(TOTAL_BYTES_SENT, wxT("Total KB Sent"));
        this->mLocalConnections->InsertColumn(TOTAL_BYTES_RECEIVED, wxT("Total KB Received"));
    }

    for(stat = local.begin();
        stat != local.end();
        stat++)
    {
        wxListItem item;
        str.clear();
        str.str(std::string());
        str << stat->mConnectionNumber;
        item.SetId(stat->mConnectionNumber);
        item.SetText(wxString( str.str().c_str(), wxConvUTF8));
        
        long index = wxNOT_FOUND;
        if( (index = mLocalConnections->FindItem(NUMBER, item.GetText())) == wxNOT_FOUND )
        {
            index = this->mLocalConnections->InsertItem(item);
        }
        
        str.clear();
        str.str(std::string());
        str << stat->mConnectionNumber;
        this->mLocalConnections->SetItem(index, 
                                         NUMBER, 
                                         wxString( str.str().c_str(), wxConvUTF8));
        this->mLocalConnections->SetColumnWidth(NUMBER, wxLIST_AUTOSIZE_USEHEADER);
        
        this->mLocalConnections->SetItem(index, 
                                         TRANSPORT, 
                                         wxString( JAUS::Connection::Transport::ToString(stat->mTransportType).c_str(), wxConvUTF8));
        this->mLocalConnections->SetColumnWidth(TRANSPORT, wxLIST_AUTOSIZE_USEHEADER);

        std::string srcIP = stat->mSourceIP.mString, destIP = stat->mDestIP.mString;

        if(srcIP == "0.0.0.0")
        {
            srcIP = "localhost";
        }
        if(destIP == "0.0.0.0")
        {
            destIP = "localhost";
        }

        this->mLocalConnections->SetItem(index, 
                                         IP, 
                                         wxString( (srcIP + std::string(" : ") + destIP).c_str(), wxConvUTF8));
        this->mLocalConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE);
        
        if(stat->mPortName.empty() == false)
        {
            this->mLocalConnections->SetItem(index, 
                                             PORT, 
                                             wxString( stat->mPortName.c_str(), wxConvUTF8));
        }
        else
        {
            str.clear();
            str.str(std::string());
            str << stat->mSourcePortNumber << " : " << stat->mDestPortNumber;
            this->mLocalConnections->SetItem(index, 
                                             PORT, 
                                             wxString(str.str().c_str(), wxConvUTF8));
        }
        this->mLocalConnections->SetColumnWidth(PORT, wxLIST_AUTOSIZE);

        std::string srcID, destID;
        srcID = stat->mSourceID.IsValid() ? stat->mSourceID.ToString() : "Node Manager";
        destID = stat->mDestinationID.IsValid() ? stat->mDestinationID.ToString() : "Node Manager";
        
        std::string directionStr = " : ";
        if(srcID == destID && stat->mTransportType != JAUS::Connection::Transport::JSharedMemory)
        {
            destID = "Network";
        }

        this->mLocalConnections->SetItem(index, 
                                         ID, 
                                         wxString( (srcID + directionStr + destID ).c_str(), wxConvUTF8));

        this->mLocalConnections->SetColumnWidth(ID, wxLIST_AUTOSIZE);

        str.clear();
        str.str(std::string());
        str << stat->mMessagesSent;
        this->mLocalConnections->SetItem(index, SENT,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("Sent To Dest").size()) { this->mLocalConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mLocalConnections->SetColumnWidth(SENT, wxLIST_AUTOSIZE_USEHEADER); }

        str.clear();
        str.str(std::string());
        str << stat->mMessagesReceived;
        this->mLocalConnections->SetItem(index, RECEIVED,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("Received From Dest").size()) { this->mLocalConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mLocalConnections->SetColumnWidth(RECEIVED, wxLIST_AUTOSIZE_USEHEADER); }

        str.clear();
        str.str(std::string());
        str << stat->mBytesSent*.00098;
        this->mLocalConnections->SetItem(index, BYTES_SENT,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("KB Sent").size()) { this->mLocalConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mLocalConnections->SetColumnWidth(BYTES_SENT, wxLIST_AUTOSIZE_USEHEADER); }

        str.clear();
        str.str(std::string());
        str << stat->mBytesReceived*.00098;
        this->mLocalConnections->SetItem(index, BYTES_RECEIVED,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("KB Received").size()) { this->mLocalConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mLocalConnections->SetColumnWidth(BYTES_RECEIVED, wxLIST_AUTOSIZE_USEHEADER); }

        str.clear();
        str.str(std::string());
        str << stat->mTotalBytesSent*.00098;
        this->mLocalConnections->SetItem(index, TOTAL_BYTES_SENT,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("Total KB Sent").size()) { this->mLocalConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mLocalConnections->SetColumnWidth(TOTAL_BYTES_SENT, wxLIST_AUTOSIZE_USEHEADER); }

        str.clear();
        str.str(std::string());
        str << stat->mTotalBytesReceived*.00098;;
        this->mLocalConnections->SetItem(index, TOTAL_BYTES_RECEIVED,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("Total KB Received").size()) { this->mLocalConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mLocalConnections->SetColumnWidth(TOTAL_BYTES_RECEIVED, wxLIST_AUTOSIZE_USEHEADER); }
    }


    if(mRemoteConnections->GetItemCount() != (int)remote.size())
    {
        this->mRemoteConnections->ClearAll();
        this->mRemoteConnections->InsertColumn(NUMBER, wxT("UID"));
        this->mRemoteConnections->InsertColumn(TRANSPORT, wxT("Type"));
        this->mRemoteConnections->InsertColumn(IP, wxT("Source IP : Dest IP"));
        this->mRemoteConnections->InsertColumn(PORT, wxT("Port/Name"));
        this->mRemoteConnections->InsertColumn(ID, wxT("Source ID : Dest ID"));
        this->mRemoteConnections->InsertColumn(SENT, wxT("Sent To Dest"));
        this->mRemoteConnections->InsertColumn(RECEIVED, wxT("Received From Dest"));
        this->mRemoteConnections->InsertColumn(BYTES_SENT, wxT("KB Sent"));
        this->mRemoteConnections->InsertColumn(BYTES_RECEIVED, wxT("KB Received"));
        this->mRemoteConnections->InsertColumn(TOTAL_BYTES_SENT, wxT("Total KB Sent"));
        this->mRemoteConnections->InsertColumn(TOTAL_BYTES_RECEIVED, wxT("Total KB Received"));
    }

    for(stat = remote.begin();
        stat != remote.end();
        stat++)
    {
        wxListItem item;
        str.clear();
        str.str(std::string());
        str << stat->mConnectionNumber;
        item.SetId(stat->mConnectionNumber);
        item.SetText(wxString( str.str().c_str(), wxConvUTF8));
        
        long index = wxNOT_FOUND;
        if( (index = mRemoteConnections->FindItem(NUMBER, item.GetText())) == wxNOT_FOUND )
        {
            index = this->mRemoteConnections->InsertItem(item);
        }
        
        str.clear();
        str.str(std::string());
        str << stat->mConnectionNumber;
        this->mRemoteConnections->SetItem(index, 
                                         NUMBER, 
                                         wxString( str.str().c_str(), wxConvUTF8));
        this->mRemoteConnections->SetColumnWidth(NUMBER, wxLIST_AUTOSIZE_USEHEADER);
        
        this->mRemoteConnections->SetItem(index, 
                                         TRANSPORT, 
                                         wxString( JAUS::Connection::Transport::ToString(stat->mTransportType).c_str(), wxConvUTF8));
        this->mRemoteConnections->SetColumnWidth(TRANSPORT, wxLIST_AUTOSIZE_USEHEADER);

        std::string srcIP = stat->mSourceIP.mString, destIP = stat->mDestIP.mString;

        if(srcIP == "0.0.0.0")
        {
            srcIP = "localhost";
        }
        if(destIP == "0.0.0.0")
        {
            destIP = "localhost";
        }

        this->mRemoteConnections->SetItem(index, 
                                          IP, 
                                          wxString( (srcIP + std::string(" : ") + destIP).c_str(), wxConvUTF8));

        this->mRemoteConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE);
        
        if(stat->mPortName.empty() == false)
        {
            this->mRemoteConnections->SetItem(index, 
                                             PORT, 
                                             wxString( stat->mPortName.c_str(), wxConvUTF8));
        }
        else
        {
            str.clear();
            str.str(std::string());
            str << stat->mSourcePortNumber << " : " << stat->mDestPortNumber;
            this->mRemoteConnections->SetItem(index, 
                                             PORT, 
                                             wxString(str.str().c_str(), wxConvUTF8));
        }
        this->mRemoteConnections->SetColumnWidth(PORT, wxLIST_AUTOSIZE);

        std::string srcID, destID;
        srcID = stat->mSourceID.IsValid() ? stat->mSourceID.ToString() : "Node Manager";
        destID = stat->mDestinationID.IsValid() ? stat->mDestinationID.ToString() : "Node Manager";
        std::string directionStr = " : ";
        if(srcID == destID)
        {
            destID = "Network";
        }

        this->mRemoteConnections->SetItem(index, 
                                         ID, 
                                         wxString( (srcID + directionStr + destID ).c_str(), wxConvUTF8));

        this->mRemoteConnections->SetColumnWidth(ID, wxLIST_AUTOSIZE);

        str.clear();
        str.str(std::string());
        str << stat->mMessagesSent;
        this->mRemoteConnections->SetItem(index, SENT,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("Sent To Dest").size()) { this->mRemoteConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mRemoteConnections->SetColumnWidth(SENT, wxLIST_AUTOSIZE_USEHEADER); }

        str.clear();
        str.str(std::string());
        str << stat->mMessagesReceived;
        this->mRemoteConnections->SetItem(index, RECEIVED,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("Received From Dest").size()) { this->mRemoteConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mRemoteConnections->SetColumnWidth(RECEIVED, wxLIST_AUTOSIZE_USEHEADER); }

        str.clear();
        str.str(std::string());
        str << stat->mBytesSent*.00098;
        this->mRemoteConnections->SetItem(index, BYTES_SENT,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("KB Sent").size()) { this->mRemoteConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mRemoteConnections->SetColumnWidth(BYTES_SENT, wxLIST_AUTOSIZE_USEHEADER); }

        str.clear();
        str.str(std::string());
        str << stat->mBytesReceived*.00098;
        this->mRemoteConnections->SetItem(index, BYTES_RECEIVED,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("KB Received").size()) { this->mRemoteConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mRemoteConnections->SetColumnWidth(BYTES_RECEIVED, wxLIST_AUTOSIZE_USEHEADER); }

        str.clear();
        str.str(std::string());
        str << stat->mTotalBytesSent*.00098;
        this->mRemoteConnections->SetItem(index, TOTAL_BYTES_SENT,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("Total KB Sent").size()) { this->mRemoteConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mRemoteConnections->SetColumnWidth(TOTAL_BYTES_SENT, wxLIST_AUTOSIZE_USEHEADER); }

        str.clear();
        str.str(std::string());
        str << stat->mTotalBytesReceived*.00098;
        this->mRemoteConnections->SetItem(index, TOTAL_BYTES_RECEIVED,
                                         wxString( str.str().c_str(), wxConvUTF8));
        if(str.str().size() > std::string("Total KB Received").size()) { this->mRemoteConnections->SetColumnWidth(IP, wxLIST_AUTOSIZE); }
        else { this->mRemoteConnections->SetColumnWidth(TOTAL_BYTES_RECEIVED, wxLIST_AUTOSIZE_USEHEADER); }
    }

    ((DiscoveryPanel*)mpDiscoveryPanel)->OnUpdateMainPanel(event);
}


/** Triggers display of main screen/panels. */
void NodeManagerFrame::ProcessTimerEvent()
{
    wxCommandEvent customEvent(wxEVT_UPDATE_MAIN_PANEL, GetId());
    customEvent.SetEventObject(this);
    GetEventHandler()->AddPendingEvent(customEvent);
}

/*  End of File */
