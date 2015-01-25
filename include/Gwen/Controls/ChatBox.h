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

#include "Gwen/Controls/Layout/Table.h"
#include "Gwen/Controls/ScrollControl.h"

namespace Gwen
{
namespace Controls
{

/// \brief This class represents a chat box.
class ChatBox : public ScrollControl
{
public:
    /// \brief Constructor.
    GWEN_CONTROL(ChatBox, ScrollControl);

    /// \brief Lays out the UI element.
    virtual void Layout(Skin::Base* skin) override;

    /// \brief Adds an item to the chat box.
    Layout::TableRow* AddItem(const std::string& label, const std::string& name = "");

    /// \brief Removes an item from the chat box.
    void RemoveItem(Layout::TableRow* row);

    /// \brief Clears the chat box.
    virtual void Clear() override;

protected:
    /// \brief Updates the scroll bar.
    virtual void _UpdateScrollBar() override;

    /// \brief Draws the UI element.
    virtual void Render(Skin::Base* skin) override;

    /// \brief The table which contains the chat UI elements.
    Controls::Layout::Table* _table;
};

}; // namespace Controls

}; // namespace Gwen
