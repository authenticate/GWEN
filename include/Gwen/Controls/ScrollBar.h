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
#include "Gwen/Controls/ScrollBarBar.h"
#include "Gwen/Controls/ScrollBarButton.h"

#define SCROLL_BUTTON_UP 0
#define SCROLL_BUTTON_LEFT 0
#define SCROLL_BUTTON_DOWN 1
#define SCROLL_BUTTON_RIGHT 1

namespace Gwen
{
namespace ControlsInternal
{

/// \brief This class represents a scroll bar.
class ScrollBar : public Controls::Base
{
public:
    /// \brief Constructor.
    GWEN_CONTROL(ScrollBar, Controls::Base);

    /// \brief Sets the clamp to nudge amount.
    virtual void SetClampToNudgeAmount(bool clamp_to_nudge_amount);

    /// \brief Gets the clamp to nudge amount.
    virtual bool GetClampToNudgeAmount() const;

    /// \brief Sets the size of the bar.
    virtual void SetBarSize(int size) = 0;

    /// \brief Gets the size of the bar.
    virtual int GetBarSize() const = 0;

    /// \brief Gets the position of the bar.
    virtual int GetBarPosition() const = 0;

    /// \brief Gets the size of the buttons.
    virtual int GetButtonSize() const;

    /// \brief Sets the bar to the left.
    virtual void ScrollToLeft();

    /// \brief Sets the bar to the right.
    virtual void ScrollToRight();

    /// \brief Sets the bar to the top.
    virtual void ScrollToTop();

    /// \brief Sets the bar to the bottom.
    virtual void ScrollToBottom();

    /// \brief Sets the nudge amount.
    virtual void SetNudgeAmount(float nudge_amount);

    /// \brief Gets the nudge amount.
    virtual float GetNudgeAmount() const;

    /// \brief Sets the nudge percent.
    virtual void SetNudgePercent(float nudge_percent);

    /// \brief Gets the nudge percent.
    virtual float GetNudgePercent() const;

    /// \brief Calculate the amount scrolled.
    virtual float CalculateScrolledAmount();

    /// \brief Calculate the bar size.
    virtual int CalculateBarSize();

    /// \brief Sets the amount scrolled.
    virtual bool SetScrolledAmount(float amount, bool do_events = true);

    /// \brief Gets the amount scrolled.
    virtual float GetScrolledAmount() const;

    /// \brief Sets the content size.
    virtual void SetContentSize(float size);

    /// \brief Gets the content size.
    virtual float GetContentSize() const;

    /// \brief Sets the viewable content size.
    virtual void SetViewableContentSize(float size);

    /// \brief Gets the viewable content size.
    virtual float GetViewableContentSize() const;

    /// \brief Is the bar horizontal?
    virtual bool GetHorizontal() const;

    /// \brief Is the bar vertical?
    virtual bool GetVertical() const;

    /// \brief A event for when the bar is moved.
    Gwen::Event::Caller _on_bar_moved;

protected:
    /// \brief Clamps a value to a multiple of the nudge amount.
    virtual float _ClampToNudgeAmount(float value) const;

    /// \brief Called when the bar is moved.
    virtual void OnBarMoved(Controls::Base* control);

    /// \brief Called when the left mouse button is pressed.
    virtual void OnMouseClickLeft(int x, int y, bool is_down) override;

    /// \brief Draws the UI element.
    virtual void Render(Skin::Base* skin) override;

    /// \brief The scroll buttons.
    ControlsInternal::ScrollBarButton* _scroll_button[2];

    /// \brief The scroll bar.
    ControlsInternal::ScrollBarBar* _bar;

    /// \brief Is the bar depressed?
    bool _depressed;

    /// \brief Clamps the scroll bar to the nudge amount.
    bool _clamp_to_nudge_amount;

    /// \brief The amount scrolled.
    float _scrolled_amount;

    /// \brief The content size.
    float _content_size;

    /// \brief The viewable content size.
    float _viewable_content_size;

    /// \brief The nudge amount.
    float _nudge_amount;
};

}; // namespace ControlsInternal

}; // namespace Gwen
