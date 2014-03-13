//
// GWEN
// Copyright (c) 2013-2014 James Lammlein
// Copyright (c) 2010 Facepunch Studios
//
// This file is part of GWEN.
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "Gwen.h"
#include "Gwen/Controls/ChatBox.h"

#include "Gwen/Controls/ChatBoxRow.h"
#include "Gwen/Controls/Layout/Table.h"
#include "Gwen/Controls/ScrollControl.h"
#include "Gwen/input/gwen_input_base.h"

#include "Gwen/Utility.h"

namespace Gwen
{
namespace Controls
{

GWEN_CONTROL_CONSTRUCTOR(ChatBox, ScrollControl)
{
    _inner_panel->SetPadding(Padding(2, 2, 2, 2));

    _scroll_bar->SetAlignment(Position::BOTTOM);

    _table = new Controls::Layout::Table(this);
    _table->SetColumnCount(1);

    SetMargin(Margin(1, 1, 1, 1));
    SetPadding(Padding(0, 1, 0, 1));
}

Layout::TableRow* ChatBox::AddItem(const std::string& label, const std::string& name)
{
    // Allocate a new row.
    ChatBoxRow* row = new ChatBoxRow(this);
    row->SetCellText(0, label);
    row->SetName(name);

    // Store the row.
    _table->AddRow(row, Position::BOTTOM, false);

    return row;
}

void ChatBox::RemoveItem(Layout::TableRow* row)
{
    // Remove the row.
    _table->Remove(row);
}

void ChatBox::Clear()
{
    // Clear the table.
    _table->Clear();
}

void ChatBox::_UpdateScrollBar()
{
    if (!_inner_panel)
    {
        return;
    }

    // Get the control's padding.
    const Gwen::Padding& padding = GetPadding();

    // Calculate the control's visible width.
    unsigned width = Width() - padding._left - padding._right;
    unsigned height = Height() - padding._top - padding._bottom;

    // Calculate the scroll bar's width.
    unsigned scroll_bar_width = 0;
    if (!_scroll_bar->Hidden())
    {
        scroll_bar_width = _scroll_bar->Width() - 1;
    }

    // Calculate the children's height.
    unsigned children_height = _table->GetRowCount() * _table->GetDefaultRowHeight();
    children_height += padding._top + padding._bottom;

    // Update the size of the inner panel.
    _inner_panel->SetSize(width - scroll_bar_width, Utility::Max(height, children_height));

    // Determine whether to enable the scroll bar.
    _SetScroll(height <= children_height);

    // Update the scroll bar's content and viewable size.
    _scroll_bar->SetContentSize(children_height);
    _scroll_bar->SetViewableContentSize(height);

    // Set the position of the inner panel.
    int position_y = 0;
    if (!_scroll_bar->Hidden())
    {
        // If clamp to nudge amount...
        unsigned scrolled_amount = _scroll_bar->GetScrolledAmount();
        unsigned content_size = _scroll_bar->GetContentSize();
        unsigned viewable_content_size = _scroll_bar->GetViewableContentSize();
        unsigned nudge_amount = _scroll_bar->GetNudgeAmount();
        if (GetClampToNudgeAmount())
        {
            // Add an offset equal to the amount of label overflow since chat boxes are docked at the bottom.
            if (scrolled_amount > 0)
            {
                if (scrolled_amount < content_size - viewable_content_size)
                {
                    scrolled_amount -= viewable_content_size / nudge_amount;
                    scrolled_amount = Utility::Clamp<unsigned>(scrolled_amount, 0, content_size - viewable_content_size);
                }
            }
        }
        position_y = -static_cast<int>(scrolled_amount);
    }

    _inner_panel->SetPosition(0, position_y);
}

void ChatBox::Render(Skin::Base* skin)
{
    // Draw the list box.
    skin->DrawListBox(this);

    // Update the clipping region for the children.
    Gwen::Point point = LocalPositionToCanvas(Gwen::Point(0, 0));
    Gwen::Padding padding = GetPadding();

    Gwen::Rectangle bounds = GetBounds();
    bounds._x = point._x + padding._left;
    bounds._y = point._y + padding._top;
    bounds._width -= padding._left + padding._right;
    bounds._height -= padding._top + padding._bottom;

    // Store the new clipping region.
    skin->GetRender()->SetClippingRegion(bounds);
    skin->GetRender()->StartClipping();
}

void ChatBox::Layout(Skin::Base* skin)
{
    // Call the base class function.
    ScrollControl::Layout(skin);

    const Gwen::Padding& padding = GetPadding();
    const Gwen::Rectangle& inner_bounds = _inner_panel->GetInnerBounds();
    unsigned row_height = _table->GetDefaultRowHeight();

    // Manually calculate the Y coordinate of the table.
    Gwen::Point position = _table->GetPosition();
    position._y = inner_bounds._y + inner_bounds._height - row_height + padding._top + padding._bottom;

    // Update the table.
    _table->SetPosition(inner_bounds._x, position._y);
    _table->SetWidth(inner_bounds._width);
    _table->SizeToChildren(false, true);

    // Update the scroll bars.
    _scroll_bar->SetNudgeAmount(row_height);
}

}; // namespace Controls

}; // namespace Gwen
