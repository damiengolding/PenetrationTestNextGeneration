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
#include "inc/PtngDGMLConv.hpp"

namespace ptng {

PtngDGMLConv::PtngDGMLConv(QObject *parent)
    : QObject{parent}
{

}

QString PtngDGMLConv::toDot(const QString &dgml)
{
    setCategories(dgml);
    // Header
    QString dotString = "strict digraph network_dgml{{\n\nrankdir=TB;";
    // dotString += "";
    dotString += "graph [fontname=\"" + subnetFont.toString() + "\",fontsize=\"" + QString::number(subnetFontSize) + "\"];";



    // Footer
    dotString += "}\n";
    return(dotString );
}

void PtngDGMLConv::setCategories(const QString &dgml)
{
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));
    if( !doc->setContent(dgml) ){
        qWarning() << "[warning] Unable to parse input string";
        return;
    }
    // Properties
        // Get some properties for use later
        QDomNodeList categories = doc->elementsByTagName("Category");
        if( categories.isEmpty() ){
            qInfo() << "[info] There are no categories in the supplied DGML";
        }

        for( int i = 0; i<categories.count();++i){
            QDomNode n = categories.at(i);
            QDomElement e = n.toElement();
            if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "critical" ){
                criticalColor = QColor::fromString( e.attribute("Stroke") );
                criticalForeground = QColor::fromString( e.attribute("Foreground") );
                criticalBackground = QColor::fromString( e.attribute("Background") );
                criticalFontSize = e.attribute("FontSize").toInt();
                criticalFont.setFamily(e.attribute("FontFamily"));
                criticalFont.setPointSize(criticalFontSize);
            }
            else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "high" ){
                highColor = QColor::fromString( e.attribute("Stroke") );
                highForeground = QColor::fromString( e.attribute("Foreground") );
                highBackground = QColor::fromString( e.attribute("Background") );
                highFontSize = e.attribute("FontSize").toInt();
                highFont.setFamily(e.attribute("FontFamily"));
                highFont.setPointSize(highFontSize);
            }
                else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "medium" ){
                mediumColor = QColor::fromString( e.attribute("Stroke") );
                mediumForeground = QColor::fromString( e.attribute("Foreground") );
                mediumBackground = QColor::fromString( e.attribute("Background") );
                mediumFontSize = e.attribute("FontSize").toInt();
                mediumFont.setFamily(e.attribute("FontFamily"));
                mediumFont.setPointSize(mediumFontSize);
            }
                else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "low" ){
                lowColor = QColor::fromString( e.attribute("Stroke") );
                lowForeground = QColor::fromString( e.attribute("Foreground") );
                lowBackground = QColor::fromString( e.attribute("Background") );
                lowFontSize = e.attribute("FontSize").toInt();
                lowFont.setFamily(e.attribute("FontFamily"));
                lowFont.setPointSize(lowFontSize);
            }
                else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "none" ){
                noneColor = QColor::fromString( e.attribute("Stroke") );
                noneForeground = QColor::fromString( e.attribute("Foreground") );
                noneBackground = QColor::fromString( e.attribute("Background") );
                noneFontSize = e.attribute("FontSize").toInt();
                noneFont.setFamily(e.attribute("FontFamily"));
                noneFont.setPointSize(lowFontSize);
            }
                else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "comment" ){
                commentColor = QColor::fromString( e.attribute("Stroke") );
                commentForeground = QColor::fromString( e.attribute("Foreground") );
                commentBackground = QColor::fromString( e.attribute("Background") );
                commentFontSize = e.attribute("FontSize").toInt();
                commentFont.setFamily(e.attribute("FontFamily"));
                commentFont.setPointSize(commentFontSize);
            }
                else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "subnet" ){
                subnetColor = QColor::fromString( e.attribute("Stroke") );
                subnetForeground = QColor::fromString( e.attribute("Foreground") );
                subnetBackground = QColor::fromString( e.attribute("Background") );
                subnetFontSize = e.attribute("FontSize").toInt();
                subnetFont.setFamily(e.attribute("FontFamily"));
                subnetFont.setPointSize(subnetFontSize);
            }
        }
}

} // namespace ptng
