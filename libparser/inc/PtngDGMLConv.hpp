/*
MIT License

Copyright (c) 2024 Damien Golding

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Don't use it to find and eat babies ... unless you're really REALLY hungry ;-)
*/
#pragma once

#include <QObject>

#include "libparser_global.hpp"

namespace ptng {

/*!
   \brief The PtngDGMLConv class

   Provides functions for converting DGML produced by other ptng classes into output such as GraphViz dot runcontrol
 */
class LIBPARSER_EXPORT PtngDGMLConv : public QObject
{
    Q_OBJECT
public:
    explicit PtngDGMLConv(QObject *parent = nullptr);

    QString toDot(const QString &dgml);

private:
    void setCategories(const QString &dgml);

public: // Category settings
    // Critical
    QColor criticalColor;
    QColor criticalForeground;
    QColor criticalBackground;
    QFont criticalFont;
    int criticalFontSize;
    int criticalStroke;

    // High
    QColor highColor;
    QColor highForeground;
    QColor highBackground;
    QFont highFont;
    int highFontSize;
    int highStroke;

    // Medium
    QColor mediumColor;
    QColor mediumForeground;
    QColor mediumBackground;
    QFont mediumFont;
    int mediumFontSize;
    int mediumStroke;

    // Low
    QColor lowColor;
    QColor lowForeground;
    QColor lowBackground;
    QFont lowFont;
    int lowFontSize;
    int lowStroke;

    // None
    QColor noneColor;
    QColor noneForeground;
    QColor noneBackground;
    QFont noneFont;
    int noneFontSize;
    int noneStroke;

    // Comment
    QColor commentColor;
    QColor commentForeground;
    QColor commentBackground;
    QFont commentFont;
    int commentFontSize;
    int commentStroke;

    // Subnet
    QColor subnetColor;
    QColor subnetForeground;
    QColor subnetBackground;
    QFont subnetFont;
    int subnetFontSize;
    int subnetStroke;

};

} // namespace ptng

