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
#include "libparser_local.hpp"

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

    /*!
       \brief toDot
       \param dgml
       \param ok
       \return QString
     */
    QString toDot(const QString &dgml, bool &ok);

private:
    void setCategories(const QString &dgml);
    QStringList getDisplaySettings(const QDomNode &node);

public: // Category settings
    // Critical
    QString criticalColor;
    QString criticalForeground;
    QString criticalBackground;
    QString criticalFont;
    QString criticalFontSize;
    QString criticalStroke;

    // High
    QString highColor;
    QString highForeground;
    QString highBackground;
    QString highFont;
    QString highFontSize;
    QString highStroke;

    // Medium
    QString mediumColor;
    QString mediumForeground;
    QString mediumBackground;
    QString mediumFont;
    QString mediumFontSize;
    QString mediumStroke;

    // Low
    QString lowColor;
    QString lowForeground;
    QString lowBackground;
    QString lowFont;
    QString lowFontSize;
    QString lowStroke;

    // None
    QString noneColor;
    QString noneForeground;
    QString noneBackground;
    QString noneFont;
    QString noneFontSize;
    QString noneStroke;

    // Comment
    QString commentColor;
    QString commentForeground;
    QString commentBackground;
    QString commentFont;
    QString commentFontSize;
    QString commentStroke;

    // Subnet
    QString subnetColor;
    QString subnetForeground;
    QString subnetBackground;
    QString subnetFont;
    QString subnetFontSize;
    QString subnetStroke;

};

} // namespace ptng

