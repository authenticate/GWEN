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
#include "Gwen/Controls/Canvas.h"

#include "Gwen/Animation.h"

#include "Gwen/Controls/Menu.h"
#include "Gwen/Controls/ToolTip.h"

#include "Gwen/skin/gwen_skin_base.h"

namespace Gwen
{
namespace Controls
{

Canvas::Canvas(Gwen::Skin::Base* skin) :
    Base(nullptr),
    _first_tab(nullptr),
    _next_tab(nullptr),
    _needs_redraw(true),
    _any_delete(false),
    _draw_background(false)
{
    SetBackgroundColor(Color(255, 255, 255, 255));
    SetBounds(0, 0, 10000, 10000);
    SetDrawBackground(false);

    if (skin)
    {
        SetSkin(skin);
    }
}

Canvas::~Canvas()
{
    ReleaseChildren();
}

void Canvas::Initialize()
{
}

void Canvas::RenderCanvas()
{
    DoThink();

    Gwen::Renderer::Base* render = _skin->GetRender();
    render->Begin();

    RecurseLayout(_skin);

    render->SetClippingRegion(GetBounds());
    render->SetRenderOffset(Gwen::Point(0, 0));

    if (_draw_background)
    {
        render->SetDrawColor(_background_color);
        render->FillRectangle(GetRenderBounds());
    }

    DoRender(_skin);

    ToolTip::Render(_skin);

    render->End();
}

void Canvas::DoThink()
{
    ProcessDelayedDeletes();

    if (Hidden())
    {
        return;
    }

    Animation::Think();

    // Reset the tabbing.
    _next_tab = nullptr;
    _first_tab = nullptr;

    ProcessDelayedDeletes();

    RecurseLayout(_skin);

    // If there's no next tab, cycle to the start.
    if (_next_tab == nullptr)
    {
        _next_tab = _first_tab;
    }

    Gwen::Input::OnCanvasThink(this);
}

void Canvas::Redraw()
{
    _needs_redraw = true;
}

Canvas* Canvas::GetCanvas()
{
    return this;
}

void Canvas::ReleaseChildren()
{
    auto i = _children.begin();
    while (i != _children.end())
    {
        Base* child = *i;
        i = _children.erase(i);
        delete child;
    }
}

void Canvas::AddDelayedDelete(Base* control)
{
    if (!_any_delete || _delete_set.find(control) == _delete_set.end())
    {
        _any_delete = true;
        _delete_set.insert(control);
        _delete_list.push_back(control);
    }
}

void Canvas::ProcessDelayedDeletes()
{
    while (_any_delete)
    {
        _any_delete = false;

        std::list<Base*> delete_list = _delete_list;
        _delete_list.clear();
        _delete_set.clear();
        for (auto i = delete_list.begin(); i != delete_list.end(); ++i)
        {
            Base* control = *i;
            control->PreDelete(GetSkin());
            delete control;
            Redraw();
        }
    }
}

bool Canvas::InputMouseMoved(int x, int y, int delta_x, int delta_y)
{
    if (Hidden())
    {
        return false;
    }

    if (ToolTip::GetToolTipActive())
    {
        Redraw();
    }

    Gwen::Input::OnMouseMoved(this, x, y, delta_x, delta_y);

    if (!Controls::_hovered_control)
    {
        return false;
    }

    if (Controls::_hovered_control == this)
    {
        return false;
    }

    if (Controls::_hovered_control->GetCanvas() != this)
    {
        return false;
    }

    Controls::_hovered_control->OnMouseMoved(x, y, delta_x, delta_y);
    return true;
}

bool Canvas::InputMouseButton(int button, bool is_down)
{
    if (Hidden())
    {
        return false;
    }

    return Gwen::Input::OnMouseClicked(this, button, is_down);
}

bool Canvas::InputMouseWheel(int wheel)
{
    if (Hidden())
    {
        return false;
    }

    if (!Controls::_hovered_control)
    {
        return false;
    }

    if (Controls::_hovered_control == this)
    {
        return false;
    }

    if (Controls::_hovered_control->GetCanvas() != this)
    {
        return false;
    }

    return Controls::_hovered_control->OnMouseWheeled(wheel);
}

bool Canvas::InputKey(int key, bool is_down)
{
    if (Hidden())
    {
        return false;
    }

    if (key <= Gwen::Key::INVALID)
    {
        return false;
    }

    if (key >= Gwen::Key::COUNT)
    {
        return false;
    }

    return Gwen::Input::OnKeyEvent(this, key, is_down);
}

bool Canvas::InputAccelerator(char character)
{
    if (Hidden())
    {
        return false;
    }

    if (!iswprint(character))
    {
        return false;
    }

    // Handle any accelerators.
    if (Input::HandleAccelerator(this, character))
    {
        return true;
    }

    return false;
}

bool Canvas::InputCharacter(char character)
{
    if (Hidden())
    {
        return false;
    }

    if (!iswprint(character))
    {
        return false;
    }

    // Handle any characters.
    if (!Controls::_keyboard_focus)
    {
        return false;
    }

    if (Controls::_keyboard_focus->GetCanvas() != this)
    {
        return false;
    }

    if (!Controls::_keyboard_focus->Visible())
    {
        return false;
    }

    if (Input::IsControlDown())
    {
        return false;
    }

    return _keyboard_focus->OnCharacter(character);
}

bool Canvas::InputQuit()
{
    return true;
}

void Canvas::SetBackgroundColor(const Gwen::Color& color)
{
    _background_color = color;
}

void Canvas::SetDrawBackground(bool should_draw)
{
    _draw_background = should_draw;
}

void Canvas::PreDeleteCanvas(Base* control)
{
    if (_any_delete)
    {
        if (_delete_set.find(control) != _delete_set.end())
        {
            _delete_list.remove(control);
            _delete_set.erase(control);
            _any_delete = !_delete_set.empty();
        }
    }
}

void Canvas::Render(Gwen::Skin::Base*)
{
    _needs_redraw = false;
}

void Canvas::_OnBoundsChanged(const Gwen::Rectangle& old_bounds)
{
    // Call the base class.
    Base::_OnBoundsChanged(old_bounds);

    InvalidateChildren(true);
}


}; // namespace Controls

}; // namespace Gwen
