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
#include "Gwen/Controls/ScrollBar.h"

#include "Gwen/Controls/ScrollBarBar.h"
#include "Gwen/Controls/ScrollBarButton.h"

#include "Gwen/Utility.h"

namespace Gwen
{
namespace ControlsInternal
{

GWEN_CONTROL_CONSTRUCTOR(ScrollBar, Controls::Base)
{
    _bar = new ScrollBarBar(this);

    for (int i = 0; i < 2; ++i)
    {
        _scroll_button[i] = new ScrollBarButton(this);
    }

    _depressed = false;
    _scrolled_amount = 0.0f;
    _content_size = 0.0f;
    _viewable_content_size = 0.0f;

    SetBounds(0, 0, 15, 15);
    SetClampToNudgeAmount(false);
    SetNudgeAmount(20.0f);
}

void ScrollBar::SetClampToNudgeAmount(bool clamp_to_nudge_amount)
{
    _clamp_to_nudge_amount = clamp_to_nudge_amount;
}

bool ScrollBar::GetClampToNudgeAmount() const
{
    return _clamp_to_nudge_amount;
}

int ScrollBar::GetButtonSize() const
{
    return 0;
}

void ScrollBar::ScrollToLeft()
{
}

void ScrollBar::ScrollToRight()
{
}

void ScrollBar::ScrollToTop()
{
}

void ScrollBar::ScrollToBottom()
{
}

void ScrollBar::SetNudgeAmount(float nudge_amount)
{
    if (nudge_amount < 0.0f)
    {
        nudge_amount = 0.0f;
    }
    _nudge_amount = nudge_amount;
}

float ScrollBar::GetNudgeAmount() const
{
    return _nudge_amount;
}

void ScrollBar::SetNudgePercent(float nudge_percent)
{
    nudge_percent = Utility::Clamp(nudge_percent, 0.0f, 1.0f);
    SetNudgeAmount(nudge_percent * _content_size);
}

float ScrollBar::GetNudgePercent() const
{
    float nudge_percent = GetNudgeAmount() / _content_size;
    nudge_percent = Utility::Clamp(nudge_percent, 0.0f, 1.0f);
    return nudge_percent;
}

float ScrollBar::CalculateScrolledAmount()
{
    return 0;
}

int ScrollBar::CalculateBarSize()
{
    return 0;
}

bool ScrollBar::SetScrolledAmount(float amount, bool do_events)
{
    amount = Utility::Clamp(amount, 0.0f, 1.0f);
    if (_scrolled_amount == amount && !do_events)
    {
        return false;
    }

    _scrolled_amount = amount;

    // Clamp to the nudge amount.
    if (_clamp_to_nudge_amount)
    {
        _scrolled_amount = _ClampToNudgeAmount(_scrolled_amount);
    }

    Invalidate();

    OnBarMoved(this);
    return true;
}

float ScrollBar::GetScrolledAmount() const
{
    return _scrolled_amount;
}

void ScrollBar::SetContentSize(float size)
{
    if (size >= 0.0f)
    {
        if (_content_size != size)
        {
            Invalidate();
        }

        _content_size = size;
    }
}

float ScrollBar::GetContentSize() const
{
    return _content_size;
}

void ScrollBar::SetViewableContentSize(float size)
{
    if (size >= 0.0f)
    {
        if (_viewable_content_size != size)
        {
            Invalidate();
        }

        _viewable_content_size = size;
    }
}

float ScrollBar::GetViewableContentSize() const
{
    return _viewable_content_size;
}

bool ScrollBar::GetHorizontal() const
{
    return false;
}

bool ScrollBar::GetVertical() const
{
    return !GetHorizontal();
}

float ScrollBar::_ClampToNudgeAmount(float value) const
{
    float result = value;

    if (_nudge_amount > 0.0f)
    {
        float number_of_nudges = _content_size / _nudge_amount;
        if (number_of_nudges > 0.0f)
        {
            result = floorf(result * number_of_nudges + 0.5f);
            result /= number_of_nudges;
            result = Utility::Clamp(result, 0.0f, 1.0f);
        }
    }

    return result;
}

void ScrollBar::OnBarMoved(Controls::Base*)
{
    _on_bar_moved.Call(this);
}

void ScrollBar::OnMouseClickLeft(int, int, bool)
{
}

void ScrollBar::Render(Skin::Base* skin)
{
    skin->DrawScrollBar(this, GetHorizontal());
}

}; // namespace ControlsInternal

}; // namespace Gwen
