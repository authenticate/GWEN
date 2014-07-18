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
#include "Gwen/Structures.h"

namespace Gwen
{

Margin::Margin(int left, int top, int right, int bottom) :
    _top(top),
    _bottom(bottom),
    _left(left),
    _right(right)
{
}

Margin Margin::operator+(const Margin& left_margin) const
{
    Margin margin;

    margin._top = _top + left_margin._top;
    margin._bottom = _bottom + left_margin._bottom;
    margin._left = _left + left_margin._left;
    margin._right = _right + left_margin._right;

    return margin;
}

Rectangle::Rectangle() :
    _x(0),
    _y(0),
    _width(0),
    _height(0)
{
}

Rectangle::Rectangle(int x, int y, int width, int height) :
    _x(x),
    _y(y),
    _width(width),
    _height(height)
{
}

bool Rectangle::operator==(const Rectangle& other) const
{
    return _x == other._x && _y == other._y && _width == other._width && _height == other._height;
}

bool Rectangle::operator!=(const Rectangle& other) const
{
    return !(*this == other);
}

Rectangle Rectangle::operator+(const Rectangle& other) const
{
    Rectangle rectangle;

    rectangle._x = _x + other._x;
    rectangle._y = _y + other._y;
    rectangle._width = _width + other._width;
    rectangle._height = _height + other._height;

    return rectangle;
}

Point::Point() :
    _x(),
    _y()
{
}

Point::Point(int x, int y) :
    _x(x),
    _y(y)
{
}

void Point::operator+=(const Point& point)
{
    _x += point._x;
    _y += point._y;
}

Point Point::operator+(const Point& point) const
{
    return Point(_x + point._x, point._y + _y);
}

void Point::operator-=(const Point& point)
{
    _x -= point._x;
    _y -= point._y;
}

Point Point::operator-(const Point& point) const
{
    return Point(_x - point._x, _y - point._y);
}

Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) :
    _red(red),
    _green(green),
    _blue(blue),
    _alpha(alpha)
{
}

void Color::operator=(const Color& color)
{
    _red = color._red;
    _green = color._green;
    _blue = color._blue;
    _alpha = color._alpha;
}

void Color::operator+=(const Color& color)
{
    _red += color._red;
    _green += color._green;
    _blue += color._blue;
    _alpha += color._alpha;
}

void Color::operator-=(const Color& color)
{
    _red -= color._red;
    _green -= color._green;
    _blue -= color._blue;
    _alpha -= color._alpha;
}

void Color::operator*=(float scale)
{
    *this = *this * scale;
}

Color Color::operator*(float scale) const
{
    return Color(static_cast<unsigned char>(scale * _red),
                 static_cast<unsigned char>(scale * _green),
                 static_cast<unsigned char>(scale * _blue),
                 static_cast<unsigned char>(scale * _alpha));
}

Color Color::operator-(const Color& color) const
{
    return Color(_red - color._red, _green - color._green, _blue - color._blue, _alpha - color._alpha);
}

Color Color::operator+(const Color& color) const
{
    return Color(_red + color._red, _green + color._green, _blue + color._blue, _alpha + color._alpha);
}

bool Color::operator==(const Color& color) const
{
    return color._red == _red && color._green == _green && color._blue == _blue && color._alpha == _alpha;
}

}; // namespace Gwen
