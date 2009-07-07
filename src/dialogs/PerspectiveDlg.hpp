// This file is part of XWord    
// Copyright (C) 2009 Mike Richards ( mrichards42@gmx.com )
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#ifndef PERSPECTIVE_DLG_H
#define PERSPECTIVE_DLG_H

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
#    include <wx/wx.h>
#endif

#include "../MyFrame.hpp"


// Note that this should only be used as a modal dialog because of
// wxSingleChoiceDialog
class PerspectiveDialog
    : public wxSingleChoiceDialog
{
public:
    PerspectiveDialog(MyFrame * frame,
                      const wxString & message,
                      const wxString & caption,
                      const wxArrayString & choices)
        : wxSingleChoiceDialog(frame, message, caption, choices),
          m_frame(frame)
    {}
    ~PerspectiveDialog() {}

protected:
    void OnSelect(wxCommandEvent & WXUNUSED(evt));

    // Keep a pointer to the frame so we can dynamically update it.
    MyFrame * m_frame;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(PerspectiveDialog, wxSingleChoiceDialog)
    EVT_LISTBOX    (wxID_ANY, PerspectiveDialog::OnSelect)
END_EVENT_TABLE()

void
PerspectiveDialog::OnSelect(wxCommandEvent & WXUNUSED(evt))
{
    wxLogDebug(_T("List box selection: '%s'"), m_listbox->GetStringSelection());
    m_frame->LoadPerspective(m_listbox->GetStringSelection(), true);
}

#endif // PERSPECTIVE_DLG_H