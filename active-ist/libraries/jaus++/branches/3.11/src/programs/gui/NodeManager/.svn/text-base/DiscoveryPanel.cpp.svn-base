////////////////////////////////////////////////////////////////////////////////////
///
///  \file DiscoveryPanel.cpp
///  \brief Panel for running a subsystem discovery component.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 02/01/2012
///  <br>Copyright (c) 2012
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
#include "DiscoveryPanel.h"
#include <cxutils/networking/socket.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/treectrl.h>
#include <sstream>
#include <iomanip>

/** Constructor. */
DiscoveryPanel::DiscoveryPanel(wxWindow* parent) : DiscoveryPanelBase(parent)
{
    mpNodeManager = NULL;
    unsigned int id = 1;
    CxUtils::IP4Address::List available;
    if(CxUtils::Socket::GetHostAddresses(available) && available.size() > 0)
    {
        for(unsigned int i = 0; i < (unsigned int)available.size(); i++)
        {
            if(available[i] != "127.0.0.1")
            {
                id = available[i].mData[3] + 60000;
                std::stringstream str;
                str << id;
                this->mSubsystemTextCtrl->SetValue(wxString(str.str().c_str(), wxConvUTF8));
                break;
            }
        }
    }

    for(unsigned int i = 1; i < 255; i++)
    {
        std::stringstream str;
        str << i;
        this->mNodeChoiceBox->Append(wxString(str.str().c_str(), wxConvUTF8));
    }
    this->mNodeChoiceBox->SetSelection(0);
    for(unsigned int i = 1; i < 255; i++)
    {
        std::stringstream str;
        str << i;
        this->mComponentChoiceBox->Append(wxString(str.str().c_str(), wxConvUTF8));
    }
    this->mComponentChoiceBox->SetSelection(0);
    for(unsigned int i = 0; i <= 255; i++)
    {
        std::stringstream str;
        str << i;
        this->mAuthorityChoice->Append(wxString(str.str().c_str(), wxConvUTF8));
    }
    this->mAuthorityChoice->SetSelection(0);
}


/** Handles the intialize button event.  Initializes the JAUS component. */
void DiscoveryPanel::OnInitializeButton(wxCommandEvent& event )
{
    long s = 0, n = 0, c = 0;

    mSubsystemTextCtrl->GetValue().ToLong(&s);
    mNodeChoiceBox->GetValue().ToLong(&n);
    mComponentChoiceBox->GetValue().ToLong(&c);
    JAUS::Address id;

    if(s >= 1 && s < JAUS::JAUS_USHORT_MAX &&
       n >= 1 && n < 255 && c >= 1 && c < 255)
    {
        id((JAUS::UShort)s, (JAUS::Byte)n, (JAUS::Byte)c);
    }
    else
    {
        wxMessageBox(wxT("Invalid Component ID"), wxT("Invalid ID"));
        return;
    }
    mComponent.AccessControlService()->SetAuthorityCode((JAUS::Byte)this->mAuthorityChoice->GetSelection() );
    mComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::OCU,
                                                              "Monitor");
    *mComponent.TransportService()->GetNodeManager()->GetSettings() = *mpNodeManager->GetSettings();
    mComponent.TransportService()->SetDisconnectTimeMs(1000*(this->mTTL->GetSelection() + 1));
    if(mComponent.Initialize(id) == false)
    {
        wxMessageBox(wxT("Failed to initialize component, ID may be in use."), wxT("Failed to initialize"));
        return;
    }

#if 0 //_DEBUG
    id.mComponent++;
    mDebugComponent.AccessControlService()->SetAuthorityCode((JAUS::Byte)this->mAuthorityChoice->GetSelection() );
    mDebugComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::OCU,
                                                              "Monitor");
    *mDebugComponent.TransportService()->GetNodeManager()->GetSettings() = *mpNodeManager->GetSettings();
    mDebugComponent.TransportService()->SetDisconnectTimeMs(1000*(this->mTTL->GetSelection() + 1));
    if(mDebugComponent.Initialize(id) == false)
    {
        wxMessageBox(wxT("Failed to initialize component, ID may be in use."), wxT("Failed to initialize"));
        return;
    }
#endif

    this->mShutdownButton->Enable(true);
    this->mInitializeButton->Enable(false);
    this->mSubsystemTextCtrl->Enable(false);
    this->mNodeChoiceBox->Enable(false);
    this->mComponentChoiceBox->Enable(false);
    this->mTTL->Enable(false);
}


/** Handles the shutdown button event.  Shutsdown the JAUS component. */
void DiscoveryPanel::OnShutdownButton(wxCommandEvent& event )
{
    mComponent.Shutdown();
    this->mShutdownButton->Enable(false);
    this->mInitializeButton->Enable(true);
    this->mSubsystemTextCtrl->Enable(true);
    this->mNodeChoiceBox->Enable(true);
    this->mComponentChoiceBox->Enable(true);
    this->mTTL->Enable(true);

    this->mSubsystemList->Clear();
    this->mSubsystemTree->DeleteAllItems();

    JAUS::Subsystem::DeleteSubsystemMap(mSubsystems);
}

/**  Called on GUI update. */
void DiscoveryPanel::OnUpdateUI( wxUpdateUIEvent& event)
{
    wxCommandEvent commandEvent;
    OnUpdateMainPanel(commandEvent);
}

/** Event handler to update the display stats. */
void DiscoveryPanel::OnUpdateMainPanel( wxCommandEvent& event)
{
    JAUS::Subsystem::Map system;

    mComponent.DiscoveryService()->GetSubsystems(system);

    // Break out new and old.
    JAUS::Subsystem::Map::iterator s1, s2;
    std::set<JAUS::Subsystem*> found, lost;
    std::set<JAUS::Subsystem*>::iterator s;

    bool change = false;
    for(s1 = system.begin();
        s1 != system.end();
        s1++)
    {
        s2 = mSubsystems.find(s1->first);
        if(s2 == mSubsystems.end())
        {
            // NEW
            found.insert(s1->second.get()); change = true;
        }
        else if(s2->second->HasChanged(*s1->second.get()))
        {
            change = true;
        }
    }
    for(s1 = mSubsystems.begin();
        s1 != mSubsystems.end();
        s1++)
    {
        s2 = system.find(s1->first);
        if(s2 == system.end())
        {
            // LOST
            lost.insert(s1->second.get()); change = true;
        }
    }

    
    for(s = found.begin();
        s != found.end();
        s++)
    {
        std::stringstream str;
        str << (*s)->mSubsystemID;
        this->mSubsystemList->Append(wxString(str.str().c_str(), wxConvUTF8));
    }

    for(s = lost.begin();
        s != lost.end();
        s++)
    {
        std::stringstream str;
        str << (*s)->mSubsystemID;
        for(unsigned int i = 0; i < this->mSubsystemList->GetCount(); i++)
        {
            if(this->mSubsystemList->GetString(i) == wxString(str.str().c_str(), wxConvUTF8))
            {
                if(mSubsystemList->IsSelected(i))
                {
                    this->mSubsystemList->DeselectAll();
                }
                this->mSubsystemList->Delete(i);
            }
        }
    }

    JAUS::Subsystem::CopySubsystemMap(system, mSubsystems);
    // Delete memory.
    JAUS::Subsystem::DeleteSubsystemMap(system);

    UpdateSubsystemTree(change);
}


/** Event handler to update the display stats. */
void DiscoveryPanel::OnSelectSubsystem( wxCommandEvent& event)
{
    if(mSubsystemList->GetCount() > 0)
    {
        // Get the selected subsystem
        long number = 0;
        wxString name = mSubsystemList->GetStringSelection();
        name.ToLong(&number);
        mCurrentSelection = (JAUS::UShort)number;
    }

    UpdateSubsystemTree(true);
}


/** Event handler to update the display stats. */
void DiscoveryPanel::OnAuthorityLevel( wxCommandEvent& event)
{
    mComponent.AccessControlService()->SetAuthorityCode((JAUS::Byte)this->mAuthorityChoice->GetSelection() );
    
}


/** Event handler for request to take control of component */
void DiscoveryPanel::OnTakeControl( wxCommandEvent& event)
{
    int index = this->mSelectComponentChoice->GetSelection();
    if(index >= 0 && index < (int)mControlList.size())
    {
        JAUS::Address id = mControlList[index];
        if(id == mComponent.GetComponentID())
        {
            return;
        }
        JAUS::AccessControl* control = mComponent.AccessControlService();
        if(control->HaveControl(id))
        {
            if(control->ReleaseComponentControl(id))
            {
                this->mControlButton->SetLabel(wxT("Take Control"));
                this->mSendShutdown->Disable();
                this->mStandbyButton->Disable();
                this->mResumeButton->Disable();
            }
        }
        else
        {
            if(control->RequestComponentControl(id))
            {
                this->mControlButton->SetLabel(wxT("Release Control"));
                this->mSendShutdown->Enable();
                this->mStandbyButton->Enable();
                this->mResumeButton->Enable();
            }
        }
    }
}


/** Event handler for selecting component to take control of component */
void DiscoveryPanel::OnSelectComponentChoice( wxCommandEvent& event)
{
    int index = this->mSelectComponentChoice->GetSelection();
    if(index >= 0 && index < (int)mControlList.size())
    {
        JAUS::Address id = mControlList[index];
        if(id == mComponent.GetComponentID())
        {
            this->mControlButton->SetLabel(wxT("Take Control"));
            this->mControlButton->Disable();
            this->mSendShutdown->Disable();
            this->mStandbyButton->Disable();
            this->mResumeButton->Disable();
            return;
        }
        this->mControlButton->Enable();
        JAUS::AccessControl* control = mComponent.AccessControlService();
        if(control->HaveControl(id) == false)
        {
            this->mControlButton->SetLabel(wxT("Take Control"));
            this->mSendShutdown->Disable();
            this->mStandbyButton->Disable();
            this->mResumeButton->Disable();
        }
        else
        {
            this->mControlButton->SetLabel(wxT("Release Control"));
            this->mSendShutdown->Enable();
            this->mStandbyButton->Enable();
            this->mResumeButton->Enable();
        }
    }
}


/** Event handler for pressing send resume button. */
void DiscoveryPanel::OnSendResume( wxCommandEvent& event)
{
    int index = this->mSelectComponentChoice->GetSelection();
    if(index >= 0 && index < (int)mControlList.size())
    {
        JAUS::Address id = mControlList[index];
        if(id == mComponent.GetComponentID())
        {
            return;
        }
        JAUS::AccessControl* control = mComponent.AccessControlService();
        if(control->HaveControl(id) == true)
        {
            mComponent.ManagementService()->Resume(id);
        }
    }
}


/** Event handler for pressing send standby button. */
void DiscoveryPanel::OnSendStandby( wxCommandEvent& event)
{
    int index = this->mSelectComponentChoice->GetSelection();
    if(index >= 0 && index < (int)mControlList.size())
    {
        JAUS::Address id = mControlList[index];
        if(id == mComponent.GetComponentID())
        {
            return;
        }
        JAUS::AccessControl* control = mComponent.AccessControlService();
        if(control->HaveControl(id) == true)
        {
            mComponent.ManagementService()->Standby(id);
        }
    }
}


/** Event handler for pressing send shutdown button. */
void DiscoveryPanel::OnSendShutdown( wxCommandEvent& event)
{
    int index = this->mSelectComponentChoice->GetSelection();
    if(index >= 0 && index < (int)mControlList.size())
    {
        JAUS::Address id = mControlList[index];
        if(id == mComponent.GetComponentID())
        {
            return;
        }
        JAUS::AccessControl* control = mComponent.AccessControlService();
        if(control->HaveControl(id) == true)
        {
            mComponent.ManagementService()->Shutdown(id);
        }
    }
}

/** Updates the displays of the tree. */
void DiscoveryPanel::UpdateSubsystemTree(const bool force)
{
    JAUS::Subsystem::Map::iterator d = mSubsystems.end();
    if(mCurrentSelection > 0)
    {
        d = mSubsystems.find(mCurrentSelection);
    }

    if(d != mSubsystems.end() && force)
    {
        mControlList = d->second->GetAddressList();
        JAUS::Address::List::iterator c;
        this->mSelectComponentChoice->Clear();
        bool first = true;
        for(c = mControlList.begin();
            c != mControlList.end();
            c++)
        {
            this->mSelectComponentChoice->Append(wxString( c->ToString().c_str(), wxConvUTF8));
            if(first)
            {
                first = false;

                this->mSelectComponentChoice->SetSelection(0);

                if(*c == this->mComponent.GetComponentID())
                {
                    this->mSendShutdown->Disable();
                    this->mStandbyButton->Disable();
                    this->mResumeButton->Disable();
                    this->mControlButton->Disable();
                }
                else
                {
                    this->mControlButton->Enable();
                    if(this->mComponent.AccessControlService()->HaveControl(*c))
                    {
                        this->mControlButton->SetLabel(wxT("Release Control"));
                        this->mSendShutdown->Enable();
                        this->mStandbyButton->Enable();
                        this->mResumeButton->Enable();
                    }
                    else
                    {
                        this->mControlButton->SetLabel(wxT("Take Control"));
                        this->mSendShutdown->Disable();
                        this->mStandbyButton->Disable();
                        this->mResumeButton->Disable();
                    }
                }
            }
            
        }

        mTreeUpdateTime = d->second->mUpdateTime;

        this->mSubsystemTree->DeleteAllItems();

        std::stringstream str;

        str << d->first << " - " << d->second->mIdentification;
        wxTreeItemId root = mSubsystemTree->AddRoot(wxString(str.str().c_str(), wxConvUTF8));
        JAUS::Subsystem::Configuration::const_iterator node;
        for(node = d->second->mConfiguration.begin();
            node != d->second->mConfiguration.end();
            node++)
        {
            JAUS::Subsystem::Component::Set::const_iterator component;
            for(component = node->second.begin();
                component != node->second.end();
                component++)
            {

                str.clear();
                str.str(std::string());
                str << component->mID.ToString() << " - " << component->mIdentification << std::endl;

                wxTreeItemId cNode = mSubsystemTree->AppendItem(root, wxString(str.str().c_str(), wxConvUTF8));

                str.clear();
                str.str(std::string());
                str << "Authority Level - " << component->mAuthorityLevel << std::endl;

                mSubsystemTree->AppendItem(cNode, wxString(str.str().c_str(), wxConvUTF8));

                str.clear();
                str.str(std::string());
                str << "Status - ";

                switch((JAUS::Byte)component->mStatus)
                {
                case JAUS::Management::Status::Ready:
                    str << "Ready\n";
                    break;
                case JAUS::Management::Status::Standby:
                    str << "Standby\n";
                    break;
                case JAUS::Management::Status::Shutdown:
                    str << "Shutdown\n";
                    break;
                case JAUS::Management::Status::Failure:
                    str << "Failure\n";
                    break;
                case JAUS::Management::Status::Emergency:
                    str << "Emergency\n";
                    break;
                default:
                    str << "Intialized\n";
                    break;
                }

                mSubsystemTree->AppendItem(cNode, wxString(str.str().c_str(), wxConvUTF8));

                JAUS::Service::ID::Set::const_iterator service;
                unsigned int count = 0;
                for(service = component->mServices.begin();
                    service != component->mServices.end();
                    service++)
                {
                    mSubsystemTree->AppendItem(cNode, wxString(service->mName.c_str(), wxConvUTF8));
                }
            }
        }
        mSubsystemTree->ExpandAllChildren(root);
    }
    else if(d == mSubsystems.end())
    {
        this->mSubsystemTree->DeleteAllItems();
    }
    else if(d != mSubsystems.end()) // Update state information and authority level only
    {
        std::stringstream str;

        wxTreeItemId root = mSubsystemTree->GetRootItem();
        wxTreeItemIdValue cookie;
        wxTreeItemId child = mSubsystemTree->GetFirstChild(root, cookie);
        JAUS::Subsystem::Configuration::const_iterator node;
        for(node = d->second->mConfiguration.begin();
            node != d->second->mConfiguration.end();
            node++)
        {
            JAUS::Subsystem::Component::Set::const_iterator component;
            for(component = node->second.begin();
                component != node->second.end();
                component++)
            {
                wxString finalString;
                str.clear();
                str.str(std::string());
                str << "Authority Level - " << component->mAuthorityLevel << std::endl;

                wxTreeItemId leaf = mSubsystemTree->GetFirstChild(child, cookie);
                
                finalString = wxString(str.str().c_str(), wxConvUTF8);
                if(finalString != mSubsystemTree->GetItemText(leaf))
                {
                    mSubsystemTree->SetItemText(leaf, finalString);
                }

                str.clear();
                str.str(std::string());
                str << "Status - ";

                switch((JAUS::Byte)component->mStatus)
                {
                case JAUS::Management::Status::Ready:
                    str << "Ready\n";
                    break;
                case JAUS::Management::Status::Standby:
                    str << "Standby\n";
                    break;
                case JAUS::Management::Status::Shutdown:
                    str << "Shutdown\n";
                    break;
                case JAUS::Management::Status::Failure:
                    str << "Failure\n";
                    break;
                case JAUS::Management::Status::Emergency:
                    str << "Emergency\n";
                    break;
                default:
                    str << "Intialized\n";
                    break;
                }

                leaf = mSubsystemTree->GetNextSibling(leaf);

                finalString = wxString(str.str().c_str(), wxConvUTF8);
                if(finalString != mSubsystemTree->GetItemText(leaf))
                {
                    mSubsystemTree->SetItemText(leaf, finalString);
                }

                // Next component
                child = mSubsystemTree->GetNextSibling(child);
            }
        }
    }
}



/*  End of File */
