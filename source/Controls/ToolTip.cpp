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
#include "Gwen/Controls/ToolTip.h"

#include "Gwen/Controls/Canvas.h"
#include "Gwen/input/gwen_input_base.h"
#include "Gwen/Utility.h"

namespace Gwen
{
namespace Controls
{
namespace ToolTip
{

/// \brief A pointer to the tool tip.
Controls::Base* _tool_tip = nullptr;

void Enable(Controls::Base* control)
{
    if (!control->GetToolTip())
    {
        return;
    }

    _tool_tip = control;
}

void Disable(Controls::Base* control)
{
    if (_tool_tip == control)
    {
        _tool_tip = nullptr;
    }
}

void Render(Skin::Base* skin)
{
    if (!_tool_tip)
    {
        return;
    }

    Renderer::Base* render = skin->GetRender();
    Point old_offset = render->GetRenderOffset();
    Point mouse_position = Input::GetMousePosition();
    Rectangle bounds = _tool_tip->GetToolTip()->GetBounds();

    Rectangle offset = Gwen::Rectangle(mouse_position._x - bounds._width / 2,
                                       mouse_position._y - bounds._height - 10,
                                       bounds._width,
                                       bounds._height);
    offset = Utility::ClampRectToRect(offset, _tool_tip->GetCanvas()->GetBounds());

    // Calculate the offset on screen bounds.
    render->AddRenderOffset(Point(offset._x, offset._y));
    render->EndClipping();

    skin->DrawToolTip(_tool_tip->GetToolTip());

    _tool_tip->GetToolTip()->DoRender(skin);

    render->SetRenderOffset(old_offset);
}

bool GetToolTipActive()
{
    return _tool_tip != nullptr;
}

}; // namespace ToolTip

}; // namespace Controls

}; // namespace Gwen
