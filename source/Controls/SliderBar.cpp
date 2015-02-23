//
// GWEN
// Copyright (c) 2013-2015 James Lammlein
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
#include "Gwen/Controls/SliderBar.h"

#include "Gwen/skin/gwen_skin_base.h"

namespace Gwen
{
namespace ControlsInternal
{

GWEN_CONTROL_CONSTRUCTOR(SliderBar, Dragger)
{
    SetHorizontal(true);
    SetRestrictToParent(true);
    SetTarget(this);
}

void SliderBar::SetHorizontal(bool is_horizontal)
{
    _horizontal = is_horizontal;
}

bool SliderBar::GetHorizontal() const
{
    return _horizontal;
}

void SliderBar::Render(Skin::Base* skin)
{
    skin->DrawSliderButton(this, GetDepressed(), GetHorizontal());
}

}; // namespace ControlsInternal

}; // namespace Gwen
