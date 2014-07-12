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
#include "Gwen/Controls/Text.h"

#include "Gwen/skin/gwen_skin_base.h"
#include "Gwen/Utility.h"

namespace Gwen
{
namespace ControlsInternal
{

GWEN_CONTROL_CONSTRUCTOR(Text, Controls::Base)
{
    _font = nullptr;
    _color = GetSkin()->Colors.Label.Default;
    _color_override = Color(255, 255, 255, 0);
    _changed = false;

    SetMouseInputEnabled(false);
    SetSize(200, 20);
    SetWrap(false);
}

void Text::SetText(const std::string& text)
{
    if (_text == text)
    {
        return;
    }

    _text = text;
    _changed = true;
    Invalidate();
}

std::string Text::GetText() const
{
    return _text;
}

void Text::SetFont(Renderer::Font* font)
{
    if (_font == font)
    {
        return;
    }

    _font = font;
    _changed = true;

    // Change the font of multilines too!
    auto begin = _lines.begin();
    auto end = _lines.end();
    while (begin != end)
    {
        (*begin)->SetFont(_font);
        ++begin;
    }

    Invalidate();
}

Renderer::Font* Text::GetFont()
{
    return _font;
}

void Text::SetTextColor(const Gwen::Color& color)
{
    _color = color;
}

Gwen::Color Text::GetTextColor() const
{
    return _color;
}

void Text::SetTextColorOverride(const Gwen::Color& color)
{
    _color_override = color;
}

Gwen::Color Text::GetTextColorOverride()
{
    return _color_override;
}

void Text::SetTextChanged(bool has_text_changed)
{
    _changed = has_text_changed;
}

void Text::SetWrap(bool do_wrap)
{
    if (_wrap == do_wrap)
    {
        return;
    }

    _wrap = do_wrap;
    _changed = true;
    Invalidate();
}

bool Text::GetWrap() const
{
    return _wrap;
}

Gwen::Rectangle Text::GetCharacterPosition(unsigned character)
{
    if (!_lines.empty())
    {
        int characters = 0;
        Text* line = nullptr;

        auto begin = _lines.begin();
        auto end = _lines.end();
        while (begin != end)
        {
            line = *begin;
            ++begin;
            characters += line->GetLength();

            if (characters <= static_cast<int>(character))
            {
                continue;
            }

            characters -= line->GetLength();
            Gwen::Rectangle position = line->GetCharacterPosition(character - characters);
            position._x += line->X();
            position._y += line->Y();
            return position;
        }

        // Handle the special case of the last character.
        Gwen::Rectangle position = line->GetCharacterPosition(line->GetLength());
        position._x += line->X();
        position._y += line->Y();
        return position;
    }

    if (GetLength() == 0 || character == 0)
    {
        Gwen::Point point = GetSkin()->GetRender()->MeasureText(GetFont(), " ");
        Gwen::Rectangle position = Gwen::Rectangle(0, 0, 0, point._y);
        position._x += GetPadding()._right;
        return position;
    }

    std::string sub_string = _text.substr(0, character);
    Gwen::Point point = GetSkin()->GetRender()->MeasureText(GetFont(), sub_string);

    Gwen::Rectangle position = Gwen::Rectangle(point._x, 0, 0, point._y);
    position._x += GetPadding()._right;
    return position;
}

int Text::GetClosestCharacter(const Gwen::Point& point)
{
    // Convert coordinate systems.
    Gwen::Point local_point = CanvasPositionToLocal(point);

    if (!_lines.empty())
    {
        int characters = 0;
        Text* line = nullptr;

        auto begin = _lines.begin();
        auto end = _lines.end();
        while (begin != end)
        {
            line = *begin;
            ++begin;
            characters += line->GetLength();

            if (local_point._y < line->Y())
            {
                continue;
            }
            if (local_point._y > line->Bottom())
            {
                continue;
            }
            if (local_point._y < line->Bottom())
            {
                break;
            }
        }

        characters -= line->GetLength();
        int position = line->GetClosestCharacter(Gwen::Point(local_point._x - line->X(), local_point._y - line->Y()));
        return characters + position;
    }

    int distance = std::numeric_limits<int>::max();
    int character = 0;

    for (unsigned i = 0; i < _text.length() + 1; i++)
    {
        Gwen::Rectangle new_position = GetCharacterPosition(i);

        // TODO: This distance calculation isn't 100% correct.
        int new_distance = abs(new_position._x - local_point._x) + abs(new_position._y - local_point._y);
        if (new_distance > distance)
        {
            continue;
        }

        distance = new_distance;
        character = i;
    }

    return character;
}

unsigned Text::GetLength() const
{
    return static_cast<unsigned>(_text.length());
}

unsigned Text::GetLineCount()
{
    return static_cast<unsigned>(_lines.size());
}

Text* Text::GetLine(int index)
{
    auto begin = _lines.begin();
    auto end = _lines.end();
    while (begin != end)
    {
        if (index == 0)
        {
            return *begin;
        }

        ++begin;
        --index;
    }

    return nullptr;
}

int Text::GetLineFromCharacter(int index)
{
    int characters = 0;
    int line = 0;

    auto begin = _lines.begin();
    auto end = _lines.end();
    while (begin != end)
    {
        Text* text = *begin;
        ++begin;

        characters += text->GetLength();
        if (characters > index)
        {
            return line;
        }

        ++line;
    }

    if (line > 0)
    {
        return line - 1;
    }

    return line;
}

int Text::GetStartCharacterFromLine(int index)
{
    int characters = 0;

    auto begin = _lines.begin();
    auto end = _lines.end();
    while (begin != end)
    {
        Text* text = *begin;
        ++begin;

        if (index == 0)
        {
            return Utility::Clamp<int>(characters, 0, GetLength());
        }

        characters += text->GetLength();
        --index;
    }

    return Utility::Clamp<int>(characters, 0, GetLength());
}

int Text::GetEndCharacterFromLine(int index)
{
    int start = GetStartCharacterFromLine(index);

    Text* line = GetLine(index);
    if (line)
    {
        start += line->GetLength();
    }

    return Utility::Clamp<int>(start, 0, GetLength());
}

int Text::GetCharacterPositionOnLine(int index)
{
    int line_index = GetLineFromCharacter(index);

    Text* line = GetLine(line_index);
    if (!line)
    {
        return 0;
    }

    int start = GetStartCharacterFromLine(line_index);
    return index - start;
}

void Text::RefreshSize()
{
    if (_wrap)
    {
        return RefreshSizeWrap();
    }

    if (!GetFont())
    {
        return;
    }

    Gwen::Point point = GetSkin()->GetRender()->MeasureText(GetFont(), _text);
    point._x += GetPadding()._left + GetPadding()._right;
    point._y += GetPadding()._top + GetPadding()._bottom;

    if (point._x == Width() && point._y == Height())
    {
        return;
    }

    if (point._y < GetFont()->_size)
    {
        point._y = static_cast<int>(GetFont()->_size);
    }

    SetSize(point._x, point._y);
    InvalidateParent();
    Invalidate();
}

void Text::_OnScaleChanged()
{
    Invalidate();
}

void Text::_SplitWords(const std::string& string, std::vector<std::string>& result)
{
    int width = GetParent()->Width() - GetParent()->GetPadding()._left - GetParent()->GetPadding()._right;

    std::string current_word;
    for (unsigned i = 0; i < string.length(); ++i)
    {
        if (string[i] == '\n')
        {
            if (!current_word.empty())
            {
                result.push_back(current_word);
            }

            result.push_back("\n");
            current_word.clear();
            continue;
        }

        if (string[i] == ' ')
        {
            current_word += string[i];
            result.push_back(current_word);
            current_word.clear();
            continue;
        }

        current_word += string[i];

        // If adding a character makes the word bigger than the textbox size...
        Gwen::Point point = GetSkin()->GetRender()->MeasureText(GetFont(), current_word);
        if (point._x > width)
        {
            // Split the words.
            current_word.pop_back();
            result.push_back(current_word);
            current_word.clear();
            --i;
            continue;
        }
    }

    if (!current_word.empty())
    {
        result.push_back(current_word);
    }
}

void Text::Render(Skin::Base* skin)
{
    if (_wrap)
    {
        return;
    }

    if (GetLength() == 0 || !GetFont())
    {
        return;
    }

    if (_color_override._alpha == 0)
    {
        skin->GetRender()->SetDrawColor(_color);
    }
    else
    {
        skin->GetRender()->SetDrawColor(_color_override);
    }

    skin->GetRender()->DrawText(GetFont(), Gwen::Point(GetPadding()._left, GetPadding()._top), _text);
}

void Text::Layout(Skin::Base* skin)
{
    if (_changed)
    {
        RefreshSize();
        _changed = false;
    }
}

void Text::RefreshSizeWrap()
{
    RemoveChildren();

    for (auto i = _lines.begin(); i != _lines.end(); ++i)
    {
        delete *i;
    }
    _lines.clear();

    std::vector<std::string> words;
    _SplitWords(GetText(), words);

    // Adding a word to the end simplifies the code below
    // which is anything but simple.
    words.push_back("");

    // Sanity.
    if (!GetFont())
    {
        return;
    }

    Point font_size = GetSkin()->GetRender()->MeasureText(GetFont(), " ");
    int width = GetParent()->Width() - GetParent()->GetPadding()._left - GetParent()->GetPadding()._right;

    int x = 0;
    int y = 0;
    std::string line;
    for (auto i = words.begin(); i != words.end(); ++i)
    {
        bool is_line_finished = false;
        bool is_wrapped = false;

        // If this word is a newline, make a new line.
        if ((*i)[0] == '\n')
        {
            is_line_finished = true;
        }

        line += (*i);

        // Does adding this word drive us over the width?
        Gwen::Point point = GetSkin()->GetRender()->MeasureText(GetFont(), line);
        if (point._x > Width())
        {
            if (point._x > width)
            {
                is_line_finished = true;
                is_wrapped = true;
            }
        }

        // If this is the last word then finish the line
        if (--words.end() == i)
        {
            is_line_finished = true;
        }

        if (is_line_finished)
        {
            Text* text = new Text(this);
            text->SetFont(GetFont());
            text->SetTextColor(_color);
            text->SetTextColorOverride(_color_override);

            if (is_wrapped)
            {
                text->SetText(line.substr(0, line.length() - (*i).length()));
                line = *i;
            }
            else
            {
                text->SetText(line.substr(0, line.length()));
                line.clear();
            }

            text->RefreshSize();
            text->SetPosition(x, y);
            _lines.push_back(text);

            y += font_size._y;
            x = 0;
        }
    }

    // Size the control to the children height and to the parent width.
    Point child_size = ChildrenSize();
    SetSize(width, child_size._y);

    InvalidateParent();
    Invalidate();
}

}; // namespace ControlsInternal

}; // namespace Gwen
