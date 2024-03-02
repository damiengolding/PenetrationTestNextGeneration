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
#include <QColor>
#include <QFont>
#include "libparser_global.hpp"

namespace ptng {

/*!
   \brief The PtngStyleSheet class

   Since most of the PTNG tools produce graphical or other output, it makes sense to have a common, configurable object with sensible defaults
 */
class LIBPARSER_EXPORT PtngStyleSheet : public QObject
{
    Q_OBJECT
public:
    explicit PtngStyleSheet(QObject *parent = nullptr);

public:
    // Colors - in QColor
    QColor colorCritical = QColorConstants::Red;
    QColor colorHigh = QColorConstants::Magenta;
    QColor colorMedium = QColorConstants::Yellow;
    QColor colorLow = QColorConstants::Green;
    QColor colorInfo = QColorConstants::Blue;

    // Fonts and text
    QFont mainFont = QFont("Open Sans",10);
    QFont titleFont = QFont("Open Sans",15);
    QColor fontColor = QColorConstants::Black;
    QColor highlightColor = QColorConstants::Gray;
    QColor borderColor = QColorConstants::Black;
    QColor backgroundColor = QColorConstants::White;

public:
    bool loadStyleSheet(const QString &file);

signals:
    void styleSheetLoadSuccess();
    void styleSheetLoadFail();
};

} // namespace ptng

