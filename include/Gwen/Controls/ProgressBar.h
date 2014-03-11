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

#include "Gwen/Controls/Label.h"
#include "Gwen/skin/gwen_skin_base.h"

namespace Gwen
{
namespace Controls
{

/// \brief This class represents a progress bar.
class ProgressBar : public Label
{
public:
    /// \brief Constructor.
    GWEN_CONTROL(ProgressBar, Label);

    /// \brief Sets the progress bar to value.
    virtual void SetValue(float value);

    /// \brief Gets the progress bar to value.
    virtual float GetValue() const;

    /// \brief Sets the progress bar to vertical.
    virtual void SetVertical();

    /// \brief Sets the progress bar to horizontal.
    virtual void SetHorizontal();

    /// \brief Automatically updates the label.
    virtual void SetAutoLabel(bool auto_label);

    /// \brief Sets the cycle speed.
    virtual void SetCycleSpeed(float cycle_speed);

    /// \brief Gets the cycle speed.
    virtual float GetCycleSpeed();

    /// \brief Animates the control.
    using Label::Think;
    virtual void Think(float delta);

protected:
    /// \brief Draws the UI element.
    virtual void Render(Skin::Base* skin) override;

    /// \brief The progress.
    float _progress;

    /// \brief The cycle speed.
    float _cycle_speed;

    /// \brief Is the progress bar horizontal?
    bool _is_horizontal;

    /// \brief Should the progress bar automatically update its label?
    bool _auto_label;
};

}; // namespace Controls

}; // namespace Gwen
