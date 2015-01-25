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
#include "Gwen/Controls/VerticalSlider.h"

#include "Gwen/Controls/Slider.h"

namespace Gwen
{
namespace Controls
{

GWEN_CONTROL_CONSTRUCTOR(VerticalSlider, ControlsInternal::Slider)
{
    _bar->SetHorizontal(false);

    SetBounds(Rectangle(0, 0, 16, 128));
}

void VerticalSlider::OnMouseClickLeft(int x, int y, bool is_down)
{
    // Convert the mosue coordinates.
    Gwen::Point point = Gwen::Point(x, y);
    point = CanvasPositionToLocal(point);

    _bar->MoveTo(_bar->X(), point._y - _bar->Height() / 2);
    _bar->OnMouseClickLeft(x, y, is_down);

    OnMoved(_bar);
}

void VerticalSlider::Layout(Skin::Base*)
{
    _bar->SetSize(Width(), 15);
}

float VerticalSlider::CalculateValue()
{
    return 1.0f - static_cast<float>(_bar->Y()) / static_cast<float>((Height() - _bar->Height()));
}

void VerticalSlider::UpdateBarFromValue()
{
    _bar->MoveTo(_bar->X(), static_cast<int>((Height() - _bar->Height()) * (1.0f - _value)));
}

void VerticalSlider::Render(Skin::Base* skin)
{
    skin->DrawSlider(this, false, _clamp_to_notches ? _number_of_notches : 0, _bar->Height());
}

}; // namespace Controls

}; // namespace Gwen
