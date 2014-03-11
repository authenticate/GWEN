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

#pragma once

#include "Gwen/Controls/Base.h"
#include "Gwen/input/gwen_input_base.h"

#include <set>

namespace Gwen
{
namespace Controls
{

/// \brief This class represents a canvas.
class Canvas : public Base
{
    friend class Controls::Base;

public:
    /// \brief Constructor.
    Canvas(Skin::Base* skin);

    /// \brief Destructor.
    virtual ~Canvas();

    /// \brief Call this to fully initialize the canvas.
    virtual void Initialize();

    /// \brief Draws the canvas.
    virtual void RenderCanvas();

    /// \brief Call this to process input.
    virtual void DoThink();

    /// \brief Call this to redraw the canvas.
    virtual void Redraw();

    /// \brief Gets the canvas.
    virtual Canvas* GetCanvas() override;

    /// \brief Cleans up any children.
    virtual void ReleaseChildren();

    /// \brief Adds a delayed delete.
    virtual void AddDelayedDelete(Base* control);

    /// \brief Processes any delayed deletes.
    virtual void ProcessDelayedDeletes();

    /// \brief Call to move the mouse.
    virtual bool InputMouseMoved(int x, int y, int delta_x, int delta_y);

    /// \brief Call to update a mouse button.
    virtual bool InputMouseButton(int button, bool is_down);

    /// \brief Call to update the mouse wheel.
    virtual bool InputMouseWheel(int wheel);

    /// \brief Call to update a key.
    virtual bool InputKey(int key, bool is_down);

    /// \brief Call to update an accelerator.
    virtual bool InputAccelerator(char character);

    /// \brief Call to update a character.
    virtual bool InputCharacter(char character);

    /// \brief Call to quit.
    virtual bool InputQuit();

    /// \brief Sets the background color.
    virtual void SetBackgroundColor(const Gwen::Color& color);

    /// \brief Sets if the background should be drawn.
    virtual void SetDrawBackground(bool should_draw);

    /// \brief The first tab.
    Base* _first_tab;

    /// \brief The next tab.
    Base* _next_tab;

protected:
    /// \brief Predeletes the canvas.
    virtual void PreDeleteCanvas(Base*);

    /// \brief Draws the UI element.
    virtual void Render(Skin::Base* skin) override;

    /// \brief Called when the bounds change.
    virtual void _OnBoundsChanged(const Gwen::Rectangle& old_bounds) override;

    /// \brief Does the canvas need redrawn?
    bool _needs_redraw;

    /// \brief Are there any controls to delete?
    bool _any_delete;

    /// \brief The controls needing deleted.
    std::list<Base*> _delete_list;

    /// \brief The controls needing deleted.
    std::set<Base*> _delete_set;

    /// \brief Should the background be drawn?
    bool _draw_background;

    /// \brief The background color.
    Gwen::Color _background_color;
};

}; // namespace Controls

}; // namespace Gwen
