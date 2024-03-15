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

QString PtngDGMLConv::toDot(const QString &dgml, bool &ok)
{
    setCategories(dgml);
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));
    if( !doc->setContent(dgml) ){
        qWarning() << "[warning] Unable to parse input string";
        return("");
    }
    // Header
    QString dotString = "strict digraph network_dgml{\nrankdir=TB;";
    dotString += "\ngraph [fontname=\"" + subnetFont + "\",fontsize=\"" + subnetFontSize + "\"];";
    dotString += "\ncompound=true;labelloc=\"t\";label=\"Network DGML Model\";\n";

    // Create a map to displayable labels
    QMap<QString,QString> nodeMap;
    QDomNodeList nodes = doc->elementsByTagName("Node");
    for( int i = 0;i<nodes.count();++i){
        QDomNode n = nodes.at(i);
        QDomElement e = n.toElement();
        QString newId = e.attribute("Id").replace("attack_machine","Attacker");
        newId = newId.replace("Attack Machine","Attacker");
        nodeMap.insert(newId, QString::number(i));
    }

    // Nodes
    for( auto [address,display] : nodeMap.asKeyValueRange() ){
        QStringList displaySettings;
        QString inAxfr = "solid";
        for( int i = 0; i<nodes.count();++i){
            QDomNode n = nodes.at(i);
            QDomElement e = n.toElement();
            if( e.hasAttribute("Id") && e.attribute("Id") == address ){
                displaySettings = getDisplaySettings(n);
            }
            else if( e.hasAttribute("Id")  && e.attribute("Id") == "attack_machine" ){
                displaySettings << subnetColor << subnetFont << subnetFontSize;
            }
            if( e.hasAttribute("InAxfr") && e.attribute("InAxfr").toLower() == "true" ){
                // QUERY do I care enough to make this work?
                // inAxfr = "dashed";
            }
        }

        // Create the nodes
        QString dotEntry = display;
        dotEntry = dotEntry.replace("attack_machine","Attacker");
        dotEntry = dotEntry.replace("Attack Machine","Attacker");
        QString newAddress = address;
        dotEntry += " [shape=box,fontcolor=\"Black\",color=\"" %
                displaySettings.at(0) % "\",fontname=\"" %
                displaySettings.at(1) % "\",fontsize=\"" %
                displaySettings.at(2) % "\",label=\""%
                newAddress % "\",style=\"" %
                inAxfr % "\"];\n";
        dotString += dotEntry;
    }

    // Edges
    // Load edges into this for de-duplication, including dealing with the Attacker root node
    QStringList edgeList;
    QDomNodeList edges = doc->elementsByTagName("Link");
    for(int i = 0; i <edges.count();++i){
        QDomNode n = edges.at(i);
        QDomElement e = n.toElement();
        QString link = e.attribute("Label");

        // Deal with emtpy Labels; will be present with -l/--labels off switch on ndgml
        if( link.isEmpty() ){
            QString source = e.attribute("Source");
            QString target = e.attribute("Target");
            QString linkText = source % "->" % target;

            linkText = linkText.replace("attack_machine","0");
            linkText = linkText.replace("Attack Machine","0");

            // QString dotEntry = linkText % " [color=\"Black\"];\n";
            for( auto [address,display] : nodeMap.asKeyValueRange() ){
                if( linkText.contains(address) ){
                    linkText = linkText.replace(address,display);
                    QString dotEntry = linkText % " [color=\"Black\"];\n";
                    if( dotEntry.contains(".") ){
                        continue;
                    }
                    if( !edgeList.contains(dotEntry) ){
                        edgeList.append(dotEntry);
                    }
                }
            }

            continue;
        }

        // Deal with the Attacker node
        if( link.contains("Attacker") ){
            // link = link.replace("attack_machine","Attacker");
            // link = link.replace("Attack Machine","Attacker");
            QString dotEntry = link % " [color=\"Black\"];\n";
            for( auto [address,display] : nodeMap.asKeyValueRange() ){
                if( link.contains(address) ){
                    link = link.replace(address,display);
                    dotEntry = link % " [color=\"Black\"];\n";
                    if( dotEntry.contains(".") ){
                        continue;
                    }
                    if( !edgeList.contains(dotEntry) ){
                        edgeList.append(dotEntry);
                    }
                }
            }
            continue;
        }
        // Main
        for( auto [address,display] : nodeMap.asKeyValueRange() ){
            if( link.contains(address) ){
                QStringList parts = link.split("->");
                QString part1 = parts.at(0);
                QString part2 = parts.at(1);
                if( part1.count("*") == 1 && part2.count("*") == 2 ){
                    link = part2 % "->" % part1;
                }
                link = link.replace(address,display);
                QString dotEntry = link % " [color=\"" % subnetColor % "\"];\n";
                if( dotEntry.contains(".") ){
                    continue;
                }
                edgeList.append(dotEntry);
            }
        }
    }

    for( auto entry : edgeList){
        QString tmpStr = entry;
        tmpStr.replace("Attack Machine","0");
        dotString += tmpStr;
    }

    // Footer
    dotString += "}//End of digraph\n";
    ok = true;
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
            criticalColor = e.attribute("Stroke");
            criticalForeground = e.attribute("Foreground");
            criticalBackground = e.attribute("Background");
            criticalFont = e.attribute("FontFamily");
            criticalFontSize = e.attribute("FontSize");
            criticalStroke = e.attribute("StrokeThickness");
        }
        else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "high" ){
            highColor = e.attribute("Stroke");
            highForeground = e.attribute("Foreground");
            highBackground = e.attribute("Background");
            highFontSize = e.attribute("FontSize");
            highStroke = e.attribute("StrokeThickness");
            highFont = e.attribute("FontFamily");
        }
        else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "medium" ){
            mediumColor = e.attribute("Stroke");
            mediumForeground = e.attribute("Foreground");
            mediumBackground = e.attribute("Background");
            mediumFontSize = e.attribute("FontSize");
            mediumStroke = e.attribute("StrokeThickness");
            mediumFont = e.attribute("FontFamily");
        }
        else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "low" ){
            lowColor = e.attribute("Stroke");
            lowForeground = e.attribute("Foreground");
            lowBackground = e.attribute("Background");
            lowFontSize = e.attribute("FontSize");
            lowStroke = e.attribute("StrokeThickness");
            lowFont = e.attribute("FontFamily");
        }
        else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "none" ){
            noneColor = e.attribute("Stroke");
            noneForeground = e.attribute("Foreground");
            noneBackground = e.attribute("Background");
            noneFontSize = e.attribute("FontSize");
            noneStroke = e.attribute("StrokeThickness");
            noneFont = e.attribute("FontFamily");
        }
        else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "comment" ){
            commentColor = e.attribute("Stroke");
            commentForeground = e.attribute("Foreground");
            commentBackground = e.attribute("Background");
            commentFontSize = e.attribute("FontSize");
            noneStroke = e.attribute("StrokeThickness");
            commentFont = e.attribute("FontFamily");
        }
        else if( e.hasAttribute("Id") && e.attribute("Id").toLower() == "subnet" ){
            subnetColor = e.attribute("Stroke");
            subnetForeground = e.attribute("Foreground");
            subnetBackground = e.attribute("Background");
            subnetFontSize = e.attribute("FontSize");
            subnetStroke = e.attribute("StrokeThickness");
            subnetFont = e.attribute("FontFamily");
        }
    }
}

QStringList PtngDGMLConv::getDisplaySettings(const QDomNode &node)
{
    QStringList displaySettings;
    QDomElement e = node.toElement();
    if( e.hasAttribute("HighestSeverity") && e.attribute("HighestSeverity").toLower() == "critical" ){
        displaySettings << criticalColor << criticalFont << criticalFontSize;
    }
    else if( e.hasAttribute("HighestSeverity") && e.attribute("HighestSeverity").toLower() == "high" ){
        displaySettings << highColor << highFont << highFontSize;
    }
    else if( e.hasAttribute("HighestSeverity") && e.attribute("HighestSeverity").toLower() == "medium" ){
        displaySettings << mediumColor << mediumFont << mediumFontSize;
    }
    else if( e.hasAttribute("HighestSeverity") && e.attribute("HighestSeverity").toLower() == "low" ){
        displaySettings << lowColor << lowFont << lowFontSize;
    }
    else if( e.hasAttribute("HighestSeverity") && e.attribute("HighestSeverity").toLower() == "none" ){
        displaySettings << noneColor << noneFont << noneFontSize;
    }
    else{
        displaySettings << subnetColor << subnetFont << subnetFontSize;
    }
    return(displaySettings);
}

} // namespace ptng
