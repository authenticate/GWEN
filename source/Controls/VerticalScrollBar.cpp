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
#include "Gwen/Controls/VerticalScrollBar.h"

#include "Gwen/Controls/ScrollBar.h"

#include "Gwen/Utility.h"

namespace Gwen
{
namespace Controls
{

GWEN_CONTROL_CONSTRUCTOR(VerticalScrollBar, ScrollBar)
{
    _bar->SetVertical();
    _bar->_on_dragged.Add(this, &VerticalScrollBar::OnBarMoved);

    _scroll_button[SCROLL_BUTTON_UP]->SetDirectionUp();
    _scroll_button[SCROLL_BUTTON_UP]->_on_press_left.Add(this, &VerticalScrollBar::NudgeUp);
    _scroll_button[SCROLL_BUTTON_DOWN]->SetDirectionDown();
    _scroll_button[SCROLL_BUTTON_DOWN]->_on_press_left.Add(this, &VerticalScrollBar::NudgeDown);
}

void VerticalScrollBar::NudgeUp(Base*)
{
    if (!IsDisabled())
    {
        SetScrolledAmount(GetScrolledAmount() - GetNudgePercent());
    }
}

void VerticalScrollBar::NudgeDown(Base*)
{
    if (!IsDisabled())
    {
        SetScrolledAmount(GetScrolledAmount() + GetNudgePercent());
    }
}

void VerticalScrollBar::SetBarSize(int size)
{
    _bar->SetHeight(size);
}

int VerticalScrollBar::GetBarSize() const
{
    return _bar->Height();
}

int VerticalScrollBar::GetBarPosition() const
{
    return _bar->Y() - Width();
}

int VerticalScrollBar::GetButtonSize() const
{
    return Width();
}

void VerticalScrollBar::ScrollToTop()
{
    SetScrolledAmount(0.0f);
}

void VerticalScrollBar::ScrollToBottom()
{
    SetScrolledAmount(1.0f);
}

float VerticalScrollBar::CalculateScrolledAmount()
{
    float numerator = static_cast<float>(_bar->Y() - GetButtonSize());
    float denominator = static_cast<float>(Height() - _bar->Height() - (GetButtonSize() * 2.0f));
    float result = numerator / denominator;

    // Clamp to the nudge amount.
    if (_clamp_to_nudge_amount)
    {
        result = _ClampToNudgeAmount(result);
    }

    return result;
}

bool VerticalScrollBar::SetScrolledAmount(float amount, bool do_events)
{
    if (!ControlsInternal::ScrollBar::SetScrolledAmount(amount, do_events))
    {
        return false;
    }

    if (do_events)
    {
        int new_y = static_cast<int>(GetButtonSize() + (amount * ((Height() - _bar->Height()) - (GetButtonSize() * 2))));
        _bar->MoveTo(_bar->X(), new_y);
    }

    return true;
}

void VerticalScrollBar::OnBarMoved(Controls::Base* control)
{
    if (_bar->GetDepressed())
    {
        SetScrolledAmount(CalculateScrolledAmount(), false);
        ControlsInternal::ScrollBar::OnBarMoved(control);
    }
    else
    {
        InvalidateParent();
    }
}

void VerticalScrollBar::OnMouseClickLeft(int x, int y, bool is_down)
{
    if (is_down)
    {
        _depressed = true;
        Gwen::Controls::_mouse_focus = this;
    }
    else
    {
        Gwen::Point position = CanvasPositionToLocal(Gwen::Point(x, y));
        if (position._y < _bar->Y())
        {
            NudgeUp(this);
        }
        else if (position._y > _bar->Y() + _bar->Height())
        {
            NudgeDown(this);
        }

        _depressed = false;
        Gwen::Controls::_mouse_focus = nullptr;
    }
}

void VerticalScrollBar::Layout(Skin::Base* skin)
{
    // Call the base class.
    ScrollBar::Layout(skin);

    _scroll_button[SCROLL_BUTTON_UP]->SetDock(Position::TOP);
    _scroll_button[SCROLL_BUTTON_UP]->SetHeight(Width());
    _scroll_button[SCROLL_BUTTON_DOWN]->SetDock(Position::BOTTOM);
    _scroll_button[SCROLL_BUTTON_DOWN]->SetHeight(Width());

    _bar->SetWidth(GetButtonSize());

    // Add padding.
    _bar->SetPadding(Padding(0, GetButtonSize(), 0, GetButtonSize()));

    // Calculate the bar size.
    int height = Height() - GetButtonSize() * 2;
    float bar_height = _viewable_content_size / (_content_size + _viewable_content_size) * height;

    // Enforce a minimum bar size.
    if (bar_height < GetButtonSize())
    {
        bar_height = static_cast<float>(GetButtonSize());
    }

    _bar->SetHeight(static_cast<int>(bar_height));
    _bar->SetHidden(height <= bar_height);

    if (Hidden())
    {
        SetScrolledAmount(0);
    }

    // Based on our last scroll amount, produce a position for the bar.
    if (!_bar->GetDepressed())
    {
        SetScrolledAmount(GetScrolledAmount());
    }
}

}; // namespace Controls

}; // namespace Gwen
