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
#include "Gwen/Controls/ProgressBar.h"

#include "Gwen/Animation.h"
#include "Gwen/Controls/ScrollControl.h"
#include "Gwen/Controls/ProgressBarThink.h"
#include "Gwen/Utility.h"

namespace Gwen
{
namespace Controls
{

GWEN_CONTROL_CONSTRUCTOR(ProgressBar, Label)
{
    _progress = 0.0f;
    _auto_label = true;
    _cycle_speed = 0.0f;

    SetAlignment(Position::CENTER);
    SetBounds(Rectangle(0, 0, 128, 32));
    SetHorizontal();
    SetMouseInputEnabled(true);
    SetTextPadding(Padding(3, 3, 3, 3));

    Animation::Add(this, new ControlsInternal::ProgressBarThink());
}

void ProgressBar::SetValue(float value)
{
    if (value < 0.0f)
    {
        value = 0.0f;
    }

    if (value > 1.0f)
    {
        value = 1.0f;
    }

    _progress = value;

    if (_auto_label)
    {
        int display_value = static_cast<int>(_progress * 100);
        SetText(Utility::ToString(display_value) + "%");
    }
}

float ProgressBar::GetValue() const
{
    return _progress;
}

void ProgressBar::SetVertical()
{
    _is_horizontal = false;
}

void ProgressBar::SetHorizontal()
{
    _is_horizontal = true;
}

void ProgressBar::SetAutoLabel(bool auto_label)
{
    _auto_label = auto_label;
}

void ProgressBar::SetCycleSpeed(float cycle_speed)
{
    _cycle_speed = cycle_speed;
}

float ProgressBar::GetCycleSpeed()
{
    return _cycle_speed;
}

void ProgressBar::Think(float delta)
{
    if (!Visible())
    {
        return;
    }

    if (_cycle_speed == 0.0f)
    {
        return;
    }

    _progress += _cycle_speed * delta;
    if (_progress < 0.0f)
    {
        _progress = 0.0f;
    }

    if (_progress > 1.0f)
    {
        _progress = 1.0f;
    }

    Redraw();
}

void ProgressBar::Render(Skin::Base* skin)
{
    skin->DrawProgressBar(this, _is_horizontal, _progress);
}

}; // namespace Controls

}; // namespace Gwen
