/*==========================================================================

    Filename: imagepanel.h

    Copyright (C) 2010  Daniel Barber
                        Brian C. Becker                        
                        Robotics Lab at UCF
                        http://robotics.ucf.edu

    Description:
    ------------------------------------------------------------------------
    This is a control designed to display an image or flickerless realtime
    video. Once you give it an image to display, it continues
    to display that image until you set another one. The transition 
    between images is realtime when optimized and almost flickerless.

    License:
    ------------------------------------------------------------------------
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, 
    Boston, MA  02110-1301  USA

===========================================================================*/
#ifndef _WX_IMAGEPANEL__H
#define _WX_IMAGEPANEL__H

// Disables the old MS Visual C++ 6 warnings about 
// debugging symbols exceeding 255 chars
#pragma warning(disable:4786)

#include <wx/wx.h>
#include <wx/dcbuffer.h>

////////////////////////////////////////////////////////////////////////////////
///  
///   \class wxImagePanel
///   \brief Displays a series of images or a video to the screen ussing a buffer
///   to prevent flickering.  Also makes display of an image easy.
///
///   wxImagePanel is is similar to wxStaticBitmap as it displays an image on the 
///   screen. However, it is is designed primarily for displaying a series of
///   images, such as a video. It is flickerless, but does tear (i.e. it starts
///   painting before it has finished painting the frame). This is usually not 
///   that big of a deal, but if you want to get rid of it, you will have to
///   use DirectDraw or something.
///
///   <b>Important Sizing Note:</b><br>Images are scaled to fit the size of
///   the control, with aspect ratio being maintained.  Set the 
///   background color you wish to use using the SetBackgroundColour method.
///
////////////////////////////////////////////////////////////////////////////////
class wxImagePanel : public wxPanel
{
public:
    wxImagePanel(wxFrame* parent);
    ~wxImagePanel() {}
    bool SetImage(const wxString& file);
    bool SetImage(const wxImage& img);
    bool SetImage(const unsigned char *data, 
                  const int width,
                  const int height);
protected:
    void PaintEvent(wxPaintEvent & evt);
    void PaintNow();
    void OnSize(wxSizeEvent& event);
    void Render(wxDC& dc);
    void OnBackgroundErase(wxEraseEvent &e);
    wxMutex mMutex;
    wxImage mImage;
    wxBitmap mResizedImage;
    volatile bool mResizeEventFlag;
    DECLARE_EVENT_TABLE()
};

#endif // _WX_IMAGEPANEL_H
/*  End of File */
