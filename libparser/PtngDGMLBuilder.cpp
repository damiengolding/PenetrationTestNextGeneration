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
#include "inc/PtngDGMLBuilder.hpp"

namespace ptng {

/*
    PtngDGML
*/

PtngDGML::PtngDGML(QObject *parent)
    : QObject{parent}
{
    createXml();
}

void PtngDGML::createXml(){
    QMap<QString,QString> map;
    doc = new QDomDocument("");
    root = doc->createElement("DirectedGraph");
    // root.setAttribute("Title","NetworkDGML");
    // root.setAttribute("Background","#DCDCDC");
    root.setAttribute("xmlns","http://schemas.microsoft.com/vs/2009/dgml");
    doc->appendChild(root);

    QDomElement nodes = doc->createElement("Nodes");
    root.appendChild(nodes);

    QDomElement links = doc->createElement("Links");
    root.appendChild(links);

    QDomElement categories = doc->createElement("Categories");
// Default categroies
    {
    // See here for style attribute options: https://github.com/MicrosoftDocs/visualstudio-docs/blob/main/docs/modeling/customize-code-maps-by-editing-the-dgml-files.md#OrganizeNodes

    // QUERY need some null tests here?
    QDomElement criticalCategory = doc->createElement("Category");
    criticalCategory.setAttribute("Id","critical");
    criticalCategory.setAttribute("Label","Critical");
    // criticalCategory.setAttribute("Stroke","Firebrick"); // Border colour
    criticalCategory.setAttribute("Stroke","#B22222"); // Border colour
    criticalCategory.setAttribute("Background","#FFFFFF");
    criticalCategory.setAttribute("Foreground","#000000");
    criticalCategory.setAttribute("StrokeThickness","2");
    // criticalCategory.setAttribute("Icon","<path>");
    criticalCategory.setAttribute("FontFamily","Open Sans");
    criticalCategory.setAttribute("FontSize","10");
    // criticalCategory.setAttribute("FontWeight","<value>");
    criticalCategory.setAttribute("FontStyle","<Style>"); // Italic or Bold
    criticalCategory.setAttribute("Style","Plain"); // "Plain" or "Glass"
    categories.appendChild(criticalCategory);

    QDomElement highCategory = doc->createElement("Category");
    highCategory.setAttribute("Id","high");
    highCategory.setAttribute("Label","High");
    // highCategory.setAttribute("Stroke","OrangeRed"); // Border colour
    highCategory.setAttribute("Stroke","#FF4500"); // Border colour
    highCategory.setAttribute("Background","#FFFFFF");
    highCategory.setAttribute("Foreground","#000000");
    highCategory.setAttribute("StrokeThickness","2");
    // highCategory.setAttribute("Icon","<path>");
    highCategory.setAttribute("FontFamily","Open Sans");
    highCategory.setAttribute("FontSize","10");
    // highCategory.setAttribute("FontWeight","<value>");
    highCategory.setAttribute("FontStyle","<Style>"); // Italic or Bold
    highCategory.setAttribute("Style","Plain"); // "Plain" or "Glass"
    categories.appendChild(highCategory);

    QDomElement mediumCategory = doc->createElement("Category");
    mediumCategory.setAttribute("Id","medium");
    mediumCategory.setAttribute("Label","Medium");
    // mediumCategory.setAttribute("Stroke","DarkSalmon"); // Border colour
    mediumCategory.setAttribute("Stroke","#E9967A"); // Border colour
    mediumCategory.setAttribute("Background","#FFFFFF");
    mediumCategory.setAttribute("Foreground","#000000");
    mediumCategory.setAttribute("StrokeThickness","2");
    // mediumCategory.setAttribute("Icon","<path>");
    mediumCategory.setAttribute("FontFamily","Open Sans");
    mediumCategory.setAttribute("FontSize","10");
    // mediumCategory.setAttribute("FontWeight","<value>");
    mediumCategory.setAttribute("FontStyle","<Style>"); // Italic or Bold
    mediumCategory.setAttribute("Style","Plain"); // "Plain" or "Glass"
    categories.appendChild(mediumCategory);

    QDomElement lowCategory = doc->createElement("Category");
    lowCategory.setAttribute("Id","low");
    lowCategory.setAttribute("Label","Low");
    // lowCategory.setAttribute("Stroke","DarkGreen"); // Border colour
    lowCategory.setAttribute("Stroke","#006400"); // Border colour
    lowCategory.setAttribute("Background","#FFFFFF");
    lowCategory.setAttribute("Foreground","#000000");
    lowCategory.setAttribute("StrokeThickness","2");
    // lowCategory.setAttribute("Icon","<path>");
    lowCategory.setAttribute("FontFamily","Open Sans");
    lowCategory.setAttribute("FontSize","10");
    // lowCategory.setAttribute("FontWeight","<value>");
    lowCategory.setAttribute("FontStyle","<Style>"); // Italic or Bold
    lowCategory.setAttribute("Style","Plain"); // "Plain" or "Glass"
    categories.appendChild(lowCategory);

    QDomElement noneCategory = doc->createElement("Category");
    noneCategory.setAttribute("Id","none");
    noneCategory.setAttribute("Label","None");
    // noneCategory.setAttribute("Stroke","Navy"); // Border colour
    noneCategory.setAttribute("Stroke","#000080"); // Border colour
    noneCategory.setAttribute("Background","#FFFFFF");
    noneCategory.setAttribute("Foreground","#000000");
    noneCategory.setAttribute("StrokeThickness","2");
    // noneCategory.setAttribute("Icon","<path>");
    noneCategory.setAttribute("FontFamily","Open Sans");
    noneCategory.setAttribute("FontSize","10");
    // noneCategory.setAttribute("FontWeight","<value>");
    noneCategory.setAttribute("FontStyle","<Style>"); // Italic or Bold
    noneCategory.setAttribute("Style","Plain"); // "Plain" or "Glass"
    categories.appendChild(noneCategory);
    root.appendChild(categories);

    QDomElement commentCategory = doc->createElement("Category");
    commentCategory.setAttribute("Id","comment");
    commentCategory.setAttribute("Label","Comment");
    commentCategory.setAttribute("Stroke","#000000"); // Border colour
    commentCategory.setAttribute("Background","#FFD700");
    commentCategory.setAttribute("Foreground","#000000");
    commentCategory.setAttribute("StrokeThickness","2");
    // commentCategory.setAttribute("Icon","<path>");
    commentCategory.setAttribute("FontFamily","Open Sans");
    commentCategory.setAttribute("FontSize","10");
    // commentCategory.setAttribute("FontWeight","<value>");
    commentCategory.setAttribute("FontStyle","<Style>"); // Italic or Bold
    commentCategory.setAttribute("Style","Plain"); // "Plain" or "Glass"
    categories.appendChild(commentCategory);
    root.appendChild(categories);

    QDomElement subnetCategory = doc->createElement("Category");
    subnetCategory.setAttribute("Id","subnet");
    subnetCategory.setAttribute("Label","Subnet");
    subnetCategory.setAttribute("Stroke","#000000"); // Border colour
    subnetCategory.setAttribute("Background","#FFFFFF");
    subnetCategory.setAttribute("Foreground","#000000");
    subnetCategory.setAttribute("StrokeThickness","2");
    // subnetCategory.setAttribute("Icon","<path>");
    subnetCategory.setAttribute("FontFamily","Open Sans");
    subnetCategory.setAttribute("FontSize","10");
    // subnetCategory.setAttribute("FontWeight","<value>");
    subnetCategory.setAttribute("FontStyle","<Style>"); // Italic or Bold
    subnetCategory.setAttribute("Style","Plain"); // "Plain" or "Glass"
    categories.appendChild(subnetCategory);
    root.appendChild(categories);
}
    QDomElement properties = doc->createElement("Properties");
    root.appendChild(properties);

    QDomElement paths = doc->createElement("Paths");
    root.appendChild(paths);

    // QUERY I don't seem to ever use this, implement?
    // QDomElement styles = doc->createElement("Styles");
    // root.appendChild(styles);

    // QDomElement elem = doc->elementsByTagName("Paths").at(0).toElement();
}

/*
    PtngDGMLBuilder
*/

PtngDGMLBuilder::PtngDGMLBuilder(QObject *parent)
    : QObject{parent}
{
    dgmlObject = new PtngDGML();
}

// REFACT is there a way to put all of these into a single function with a switch on the type?
PtngDGMLBuilder &PtngDGMLBuilder::addNode(const QString &id, const QString &label, const QMap<QString, QString> &attributes)
{
    // qInfo() << "[info] Adding node:"<<"Id:"<<id<<"Label:"<<label;
    QDomElement newNode = dgmlObject->doc->createElement("Node");
    newNode.setAttribute("Id",id);
    newNode.setAttribute("Label",label);
    // newNode.setAttribute("Category","none");
    QDomNodeList nodeList = dgmlObject->doc->elementsByTagName("Nodes");
    // qInfo() << "[info] Number of Nodes:"<<nodeList.count();
    if( nodeList.count() != 1 ){
        qInfo() << "[info] Wrong number of \'Nodes\':"<<nodeList.count();
        return(*this);
    }
    for( auto [name,value] : attributes.asKeyValueRange()){
        // qInfo() << "[info] Node attribute: "<<name<<"Value:"<<value;
        newNode.setAttribute(name,value);
    }
    QDomElement nodes = nodeList.at(0).toElement();
    nodes.appendChild(newNode);

    nodeList = dgmlObject->doc->elementsByTagName("Node");
    // qInfo() << "[info] Number of Node:"<<nodeList.count();
    return(*this);
}

PtngDGMLBuilder &PtngDGMLBuilder::addLink(const QString &source, const QString &target, const QString &label, const QMap<QString, QString> &attributes)
{
    // qInfo() << "[info] Adding Link:"<<"Source:"<<source<<"Target:"<<target<<"Label"<<label;
    QDomElement newNode = dgmlObject->doc->createElement("Link");
    newNode.setAttribute("Source",source);
    newNode.setAttribute("Target",target);
    newNode.setAttribute("Label",label);

    QDomNodeList nodeList = dgmlObject->doc->elementsByTagName("Links");
    // qInfo() << "[info] Number of Links:"<<nodeList.count();
    if( nodeList.count() != 1 ){
        // qInfo() << "[info] Wrong number of \'Links\'"<<nodeList.count();
        return(*this);
    }
    for( auto [name,value] : attributes.asKeyValueRange()){
        // qInfo() << "[info] Link attributes: "<<name<<"Value:"<<value;
        newNode.setAttribute(name,value);
    }
    QDomElement links = nodeList.at(0).toElement();
    links.appendChild(newNode);

    // nodeList = dgmlObject->doc->elementsByTagName("Link");
    // qInfo() << "[info] Number of Link:"<<nodeList.count();
    return(*this);
}

PtngDGMLBuilder &PtngDGMLBuilder::addCategory(const QString &id, const QString &label, const QMap<QString, QString> &attributes)
{
    QDomElement newNode = dgmlObject->doc->createElement("Category");
    newNode.setAttribute("Id",id);
    newNode.setAttribute("Label",label);
    QDomNodeList nodeList = dgmlObject->doc->elementsByTagName("Categories");
    if( nodeList.count() != 1 ){
        qInfo() << "[info] Wrong number of \'Categories\':"<<nodeList.count();
        return(*this);
    }
    for( auto [name,value] : attributes.asKeyValueRange()){
        qInfo() << "[info] Category attributes: "<<name<<"Value:"<<value;
        newNode.setAttribute(name,value);
    }
    QDomElement categories = nodeList.at(0).toElement();
    categories.appendChild(newNode);
    return(*this);
}

PtngDGMLBuilder &PtngDGMLBuilder::addProperty(const QString &id, const QString &dataType, const QString &label, const QMap<QString, QString> &attributes)
{
    QDomElement newNode = dgmlObject->doc->createElement("Property");
    newNode.setAttribute("Id",id);
    newNode.setAttribute("Label",label);
    newNode.setAttribute("DataType",dataType);
    QDomNodeList nodeList = dgmlObject->doc->elementsByTagName("Properties");
    if( nodeList.count() != 1 ){
        qInfo() << "[info] Wrong number of \'Properties\':"<<nodeList.count();
        return(*this);
    }
    for( auto [name,value] : attributes.asKeyValueRange()){
        qInfo() << "[info] Property attributes: "<<name<<"Value:"<<value;
        newNode.setAttribute(name,value);
    }
    QDomElement properties = nodeList.at(0).toElement();
    properties.appendChild(newNode);
    return(*this);
}

PtngDGMLBuilder& PtngDGMLBuilder::addPath(const QString &id, const QString &label, const QString &dataType, bool isReference, const QMap<QString,QString> &attributes){
    QDomElement newNode = dgmlObject->doc->createElement("Path");
    newNode.setAttribute("Id",id);
    newNode.setAttribute("Label",label);
    newNode.setAttribute("DataType",dataType);
    QString isRef = isReference ? "True" : "False";
    newNode.setAttribute("Label",label);
    newNode.setAttribute("IsReference",isRef);
    QDomNodeList nodeList = dgmlObject->doc->elementsByTagName("Properties");
    if( nodeList.count() != 1 ){
        qInfo() << "[info] Wrong number of \'Properties\':"<<nodeList.count();
        return(*this);
    }
    for( auto [name,value] : attributes.asKeyValueRange()){
        qInfo() << "[info] Property attributes: "<<name<<"Value:"<<value;
        newNode.setAttribute(name,value);
    }
    QDomElement properties = nodeList.at(0).toElement();
    properties.appendChild(newNode);
    return(*this);
}

QString PtngDGMLBuilder::toString(int indent)
{
    return(dgmlObject->doc->toString(indent));
}

bool PtngDGMLBuilder::isInSubnetFilter(const QStringList &subnetFilters, const QString &testString)
{
    if( subnetFilters.isEmpty() ){
        return(true);
    }
    for( auto subnetFilter : subnetFilters){
        if( testString.startsWith(subnetFilter) ){
            return(true);
        }
    }
    return(false);
}

bool PtngDGMLBuilder::isInSeverityFilter(const QStringList &criticalityFilters, const QString &testString){
    // qInfo() << "[info] Test string:"<<testString;
    if( criticalityFilters.isEmpty() || criticalityFilters.contains("all", Qt::CaseInsensitive) ){
        return(true);
    }
    for( auto criticalityFilter : criticalityFilters){
        if( testString.toUpper() == criticalityFilter.toUpper() ){
            return(true);
        }
    }
    return(false);
}

} // namespace ptng
