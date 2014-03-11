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
#include "Gwen/Controls/ScrollControl.h"

#include "Gwen/Controls/ScrollBar.h"
#include "Gwen/Controls/VerticalScrollBar.h"
#include "Gwen/Utility.h"

namespace Gwen
{
namespace Controls
{

GWEN_CONTROL_CONSTRUCTOR(ScrollControl, Base)
{
    _scroll_bar = new VerticalScrollBar(this);
    _scroll_bar->SetDock(Position::RIGHT);
    _scroll_bar->_on_bar_moved.Add(this, &ScrollControl::OnScrollBarMoved);
    _scroll_bar->SetNudgeAmount(30);

    _inner_panel = new Base(this);
    _inner_panel->SetPosition(0, 0);
    _inner_panel->SetMargin(Margin(5, 5, 5, 5));
    _inner_panel->SendToBack();
    _inner_panel->SetMouseInputEnabled(true);

    SetMouseInputEnabled(false);
    SetScrollBarHidden(false);
}

void ScrollControl::Render(Skin::Base*)
{
}

void ScrollControl::Layout(Skin::Base* skin)
{
    // Call the base class function.
    Base::Layout(skin);

    UpdateScrollBar();
}

void ScrollControl::UpdateScrollBar()
{
    if (!_inner_panel)
    {
        return;
    }

    // Get the control's padding.
    const Gwen::Padding& padding = GetPadding();

    // Calculate the control's visible width.
    int width = Width() - padding._left - padding._right;
    int height = Height() - padding._top - padding._bottom;

    // Calculate the scroll bar's width.
    int scroll_bar_width = 0;
    if (!_scroll_bar->Hidden())
    {
        scroll_bar_width = _scroll_bar->Width() - 1;
    }

    int children_width = 0;
    int children_height = 0;

    // Get the maximum size of all the children.
    for (auto i = _inner_panel->GetChildren().begin(); i != _inner_panel->GetChildren().end(); ++i)
    {
        Base* child = *i;
        children_width = Utility::Max(children_width, child->Right());
        children_height = Utility::Max(children_height, child->Bottom());
    }

    // Update the size of the inner panel.
    _inner_panel->SetSize(width - scroll_bar_width, Utility::Max(height, children_height));

    // Determine whether to display the scroll bars.
    float height_percent = static_cast<float>(children_height) / static_cast<float>(height);
    bool can_scroll = height_percent >= 1;
    SetScroll(can_scroll);

    // Update the scroll bar's content and viewable size.
    _scroll_bar->SetContentSize(static_cast<float>(children_height - height));
    _scroll_bar->SetViewableContentSize(static_cast<float>(height));

    // Set the position of the inner panel.
    int position_y = 0;
    if (!_scroll_bar->Hidden())
    {
        position_y = static_cast<int>(_scroll_bar->GetScrolledAmount() * -_scroll_bar->GetContentSize());
    }

    _inner_panel->SetPosition(0, position_y);
}

void ScrollControl::ScrollToTop()
{
    UpdateScrollBar();
    _scroll_bar->ScrollToTop();
}

void ScrollControl::ScrollToBottom()
{
    UpdateScrollBar();
    _scroll_bar->ScrollToBottom();
}

void ScrollControl::SetInnerSize(int width, int height)
{
    _inner_panel->SetSize(width, height);
}

void ScrollControl::Clear()
{
    _inner_panel->RemoveChildren();
}

bool ScrollControl::ContentsAreDocked()
{
    if (!_inner_panel)
    {
        return false;
    }

    for (auto i = _inner_panel->GetChildren().begin(); i != _inner_panel->GetChildren().end(); ++i)
    {
        Base* child = *i;
        if (child->GetDock() == Position::NONE)
        {
            return false;
        }
    }

    return true;
}

void ScrollControl::SetScroll(bool can_scroll)
{
    _scroll_bar->SetDisabled(!can_scroll);
}

void ScrollControl::SetScrollBarHidden(bool is_hidden)
{
    _scroll_bar->SetHidden(is_hidden);
}

bool ScrollControl::GetScrollBarHidden() const
{
    return _scroll_bar->Hidden();
}

void ScrollControl::OnScrollBarMoved(Controls::Base*)
{
    Invalidate();
}

void ScrollControl::_OnChildBoundsChanged(const Gwen::Rectangle&, Controls::Base*)
{
    UpdateScrollBar();
    Invalidate();
}

bool ScrollControl::OnMouseWheeled(int delta)
{
    if (_scroll_bar->Visible())
    {
        float nudge_percent = _scroll_bar->GetNudgeAmount() * static_cast<float>(delta) / _scroll_bar->GetContentSize();
        if (_scroll_bar->SetScrolledAmount(_scroll_bar->GetScrolledAmount() - nudge_percent, true))
        {
            return true;
        }
    }

    return false;
}

}; // namespace Controls

}; // namespace Gwen
