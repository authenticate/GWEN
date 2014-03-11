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
#include "Gwen/Controls/Layout/Table.h"

#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/Layout/TableRow.h"
#include "Gwen/Utility.h"

namespace Gwen
{
namespace Controls
{
namespace Layout
{

GWEN_CONTROL_CONSTRUCTOR(Table, Base)
{
    _default_row_height = 22;
    _size_to_contents = false;
    _column_count = 1;

    for (int i = 0; i < MAX_COLUMNS; ++i)
    {
        _column_width[i] = 0;
    }
}

void Table::SetColumnCount(int count)
{
    if (_column_count == count)
    {
        return;
    }

    for (auto i = _children.begin(); i != _children.end(); ++i)
    {
        TableRow* row = dynamic_cast<TableRow*>(*i);
        if (!row)
        {
            continue;
        }
        row->SetColumnCount(count);
    }

    _column_count = count;
}

void Table::SetColumnWidth(int column, int width)
{
    if (_column_width[column] == width)
    {
        return;
    }

    _column_width[column] = width;
    Invalidate();
}

TableRow* Table::AddRow(int dock, bool to_back)
{
    TableRow* row = new TableRow(this);
    AddRow(row, to_back);
    return row;
}

void Table::AddRow(TableRow* row, int dock, bool to_back)
{
    row->SetParent(this, to_back);
    row->SetColumnCount(_column_count);
    row->SetHeight(_default_row_height);
    row->SetDock(dock);
    Invalidate();
}

void Table::Remove(TableRow* row)
{
    row->DelayedDelete();
}

void Table::Clear()
{
    for (auto i = _children.begin(); i != _children.end(); ++i)
    {
        TableRow* row = dynamic_cast<TableRow*>(*i);
        if (!row)
        {
            continue;
        }

        Remove(row);
    }
}

TableRow* Table::GetRow(int row)
{
    return dynamic_cast<TableRow*>(GetChild(row));
}

unsigned Table::GetRowCount() const
{
    return GetChildrenCount();
}

unsigned Table::GetDefaultRowHeight() const
{
    return _default_row_height;
}

bool Table::GetShouldClip() const
{
    return false;
}

void Table::Layout(Skin::Base* skin)
{
    // Call the base class.
    Base::Layout(skin);

    if (_size_to_contents)
    {
        DoSizeToContents();
    }

    int size_remainder = Width();
    int auto_size_columns = 0;

    for (int i = 0; i < MAX_COLUMNS && i < _column_count; ++i)
    {
        size_remainder -= _column_width[i];

        if (_column_width[i] == 0)
        {
            ++auto_size_columns;
        }
    }

    if (auto_size_columns > 1)
    {
        size_remainder /= auto_size_columns;
    }

    bool even = true;
    for (auto i = _children.begin(); i != _children.end(); ++i)
    {
        TableRow* row = dynamic_cast<TableRow*>(*i);
        if (row != nullptr)
        {
            row->SizeToContents();
            row->SetEven(even);
            even = !even;

            for (int j = 0; j < MAX_COLUMNS && j < _column_count; ++j)
            {
                if (_column_width[j] == 0)
                {
                    row->SetColumnWidth(j, size_remainder);
                }
                else
                {
                    row->SetColumnWidth(j, _column_width[j]);
                }
            }
        }
    }

    InvalidateParent();
}

void Table::PostLayout(Skin::Base*)
{
    if (_size_to_contents)
    {
        SizeToChildren();
        _size_to_contents = false;
    }
}

void Table::SizeToContents()
{
    _size_to_contents = true;
    Invalidate();
}

void Table::DoSizeToContents()
{
    for (int i = 0; i < MAX_COLUMNS; ++i)
    {
        _column_width[i] = 10;
    }

    for (auto i = _children.begin(); i != _children.end(); ++i)
    {
        TableRow* row = dynamic_cast<TableRow*>(*i);
        if (!row)
        {
            continue;
        }

        row->SizeToContents();

        for (int j = 0; j < MAX_COLUMNS; ++j)
        {
            if (row->_columns[j])
            {
                _column_width[j] = Utility::Max(_column_width[j], row->_columns[j]->Width());
            }
        }
    }

    InvalidateParent();
}

}; // namespace Layout

}; // namespace Controls

}; // namespace Gwen
