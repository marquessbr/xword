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


#include "ClueListBox.hpp"
//#include "PuzEvent.hpp"
#include "utils/wrap.hpp"


BEGIN_EVENT_TABLE(ClueListBox, wxOwnerDrawnListBox<XPuzzle::Clue>)
    EVT_LISTBOX            (wxID_ANY, ClueListBox::OnSelect)
END_EVENT_TABLE()



void
ClueListBox::OnSelect(wxCommandEvent & evt)
{
    /*
    XPuzzle::Clue clue = m_clues.at(evt.GetSelection());
    Update();

    wxPuzEvent puzEvt(wxEVT_PUZ_CLUE_FOCUS, GetId());
    puzEvt.SetClueNumber(m_direction, clue.Number());
    puzEvt.SetDirection(m_direction);
    puzEvt.SetClueText(clue.Text());
    GetEventHandler()->ProcessEvent(puzEvt);
    */
}


void
ClueListBox::SetClueNumber(int number)
{
    int index = FindClue(number);
    if (index != -1) {
        SetSelection(index);
        RefreshLine(index);
        //Update(); // We shouldn't really need this
    }
}



int
ClueListBox::FindClue(int number) const
{
    if (GetItemCount() == 0)
        return -1;
    int num = 0;
    container_t::const_iterator it;
    for (it = m_items.begin(); it != m_items.end(); ++it) {
        if (it->Number() == number)
            return num;
        ++num;
    }
    return -1;
}


//----------------------------------------------------
// Drawing functions
//----------------------------------------------------

void
ClueListBox::OnDrawBackground(wxDC & dc, const wxRect & rect, size_t n) const
{
    if (IsSelected(n))
        dc.SetBackground(GetSelectionBackground());
    else
        dc.SetBackground(GetBackgroundColour());

    dc.Clear();
}


void
ClueListBox::OnDrawItem(wxDC & dc, const wxRect & rect, size_t n) const
{
    wxRect numRect(rect);
    numRect.SetWidth(m_numWidth);

    wxRect textRect(rect);
    textRect.SetX(numRect.GetRight() + GetMargins().x);
    textRect.SetWidth(rect.width - GetMargins().x);

    // Get fonts and colors
    dc.SetFont(GetFont());

    if (IsSelected(n))
        dc.SetTextForeground(GetSelectionForeground());
    else
        dc.SetTextForeground(GetForegroundColour());

    XPuzzle::Clue clue = GetItem(n);

    dc.DrawLabel(wxString::Format(_T("%d."), clue.Number()), numRect, wxALIGN_RIGHT|wxALIGN_TOP);

    // If our clue is not yet cached, cache it
    if (m_cachedClues.at(n).empty())
        m_cachedClues.at(n) = ::Wrap(this, clue.Text(), textRect.width);

    dc.DrawLabel(m_cachedClues.at(n), textRect);

    //dc.DrawLine(textRect.x, textRect.y, textRect.x + textRect.width, textRect.y + textRect.height);
}



wxCoord
ClueListBox::OnMeasureItem(wxDC & dc, size_t n) const
{
    XPuzzle::Clue clue = GetItem(n);

    // Cache the wrapped clue's text if it isn't already
    int height = 0;
    if (m_cachedClues.at(n).empty()) {
        int maxWidth;
        GetClientSize(&maxWidth, NULL);
        m_cachedClues.at(n) = Wrap(this, clue.Text(), maxWidth - m_numWidth - GetMargins().x);
    }
    dc.GetMultiLineTextExtent(m_cachedClues.at(n), NULL, &height, NULL, &GetFont());

    return height;
}


void
ClueListBox::CalculateNumberWidth()
{
    // Calculate the max width, filling in missing values
    int max_width = 0;
    container_t::iterator item_it = m_items.begin();
    std::vector<int>::iterator it;
    for (it = m_numWidths.begin(); it != m_numWidths.end(); ++it) {
        // If there is no width for this item, calculate it
        if ((*it) == -1)
            GetTextExtent(wxString::Format(_T("%d."), item_it->Number()), &*it, NULL, NULL, NULL);
        if (*it > max_width)
            max_width = *it;
        ++item_it;
    }

    // Test to see if the cache needs to be invalidated
    if (max_width != m_numWidth) {
        m_numWidth = max_width;
        InvalidateCache();
    }
}