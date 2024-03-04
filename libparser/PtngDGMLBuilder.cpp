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
    // doc = new QDomDocument("ptng_dgml");
    doc = new QDomDocument("");
    // doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    root = doc->createElement("DirectedGraph");
    root.setAttribute("Title","Network DGML");
    root.setAttribute("xmlns","http://schemas.microsoft.com/vs/2009/dgml");
    doc->appendChild(root);

    QDomElement nodes = doc->createElement("Nodes");
    root.appendChild(nodes);

    QDomElement links = doc->createElement("Links");
    root.appendChild(links);

    QDomElement categories = doc->createElement("Categories");
    root.appendChild(categories);

    // QUERY need some null tests here?
    QDomElement properties = doc->createElement("Properties");
    QDomElement criticalProperty = doc->createElement("Property");
    criticalProperty.setAttribute("Id","critical");
    criticalProperty.setAttribute("Label","Critical");
    criticalProperty.setAttribute("Background","Red");
    criticalProperty.setAttribute("Foreground","White");
    properties.appendChild(criticalProperty);

    QDomElement highProperty = doc->createElement("Property");
    highProperty.setAttribute("Id","high");
    highProperty.setAttribute("Label","Medium");
    highProperty.setAttribute("Background","Orange");
    highProperty.setAttribute("Foreground","White");
    properties.appendChild(highProperty);

    QDomElement mediumProperty = doc->createElement("Property");
    mediumProperty.setAttribute("Id","medium");
    mediumProperty.setAttribute("Label","Medium");
    mediumProperty.setAttribute("Background","Green");
    mediumProperty.setAttribute("Foreground","White");
    properties.appendChild(mediumProperty);

    QDomElement lowProperty = doc->createElement("Property");
    lowProperty.setAttribute("Id","low");
    lowProperty.setAttribute("Label","Low");
    lowProperty.setAttribute("Background","Blue");
    lowProperty.setAttribute("Foreground","White");
    properties.appendChild(lowProperty);

    QDomElement noneProperty = doc->createElement("Property");
    noneProperty.setAttribute("Id","none");
    noneProperty.setAttribute("Label","None");
    noneProperty.setAttribute("Background","White");
    noneProperty.setAttribute("Foreground","Black");
    properties.appendChild(noneProperty);

    root.appendChild(properties);

    QDomElement paths = doc->createElement("Paths");
    root.appendChild(paths);

    QDomElement elem = doc->elementsByTagName("Paths").at(0).toElement();
}

/*
    PtngDGMLBuilder
*/

PtngDGMLBuilder::PtngDGMLBuilder(QObject *parent)
    : QObject{parent}
{
    dgmlObject = new PtngDGML();
}

// TODO test all these 'add' methods
// REFACT is there a way to put all of these into a single function with a switch on teh tpye?
PtngDGMLBuilder &PtngDGMLBuilder::addNode(const QString &id, const QString &label, const QMap<QString, QString> &attributes)
{
    // qInfo() << "[info] Adding node:"<<"Id:"<<id<<"Label:"<<label;
    QDomElement newNode = dgmlObject->doc->createElement("Node");
    newNode.setAttribute("Id",id);
    newNode.setAttribute("Label",label);
    QDomNodeList nodeList = dgmlObject->doc->elementsByTagName("Nodes");
    // qInfo() << "[info] Number of Nodes:"<<nodeList.count();
    if( nodeList.count() != 1 ){
        qInfo() << "[info] Wrong number of \'Nodes'\:"<<nodeList.count();
        return(*this);
    }
    for( auto [name,value] : attributes.asKeyValueRange()){
        // qInfo() << "[info] Node attributes: "<<name<<"Value:"<<value;
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
        // qInfo() << "[info] Wrong number of \'Links'\:"<<nodeList.count();
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
        qInfo() << "[info] Wrong number of \'Categories'\:"<<nodeList.count();
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
        qInfo() << "[info] Wrong number of \'Properties'\:"<<nodeList.count();
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
        qInfo() << "[info] Wrong number of \'Properties'\:"<<nodeList.count();
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

PtngDGMLBuilder& PtngDGMLBuilder::createSimple(const QMap<QString, QString> &anPairs, bool addLabels)
{
    // qInfo() << "[info] In fromSimple";
    QStringList aClasses,bClasses,cClasses,leaves;
    QMap<QString,QString> map; // For attributes
    // Create the 'Attack Machine' root node
    addNode("attack_machine","Attack Machine",map);

    // Separate the entries into classes
    QStringList ips;
    for(auto [address,name] : anPairs.asKeyValueRange()){
        QStringList tempList;
        QString tempStr;
        tempList=address.split(".");
        tempStr = tempList.at(0) + ".*.*.*";
        if( !aClasses.contains(tempStr) ){
            aClasses.append(tempStr);
        }
        tempStr = tempList.at(0) + "." + tempList.at(1) + ".*.*";
        if( !bClasses.contains(tempStr) ){
            bClasses.append(tempStr);
        }
        tempStr = tempList.at(0)  + "." + tempList.at(1)  + "." + tempList.at(2) + ".*";
        if( !cClasses.contains(tempStr) ){
            cClasses.append(tempStr);
        }
        if( !leaves.contains(address) ){
            leaves.append(address);
        }
    }

    // Add the A class nodes and link to the 'Attack Machine'
    for( auto aClass : aClasses){
        addNode(aClass, aClass, map);
        if( addLabels ){
            addLink("attack_machine", aClass, "Attack Machine->" + aClass ,map);
        }
        else{
            addLink("attack_machine", aClass, "" ,map);
        }
    }

    // Add the B class nodes and link to the A Class nodes
    for( auto aClass : aClasses){
        QStringList tempList = aClass.split(".");
        QString tempStr = tempList.at(0) + ".";
        // qInfo() << "[info] tempStr for bClass:"<<tempStr;
        for( auto bc : bClasses){
            // qInfo() << "[info] bClass:"<<bc;
            if( bc.startsWith(tempStr)){
                addNode(bc, bc, map);
                if( addLabels ){
                    addLink(aClass, bc,aClass + "->" + bc,map);
                }
                else{
                    addLink(aClass, bc,"",map);
                }
            }
        }

        // Add the C class nodes and link to the B Class nodes
        for( auto bClass : bClasses){
            QStringList tempList = bClass.split(".");
            QString tempStr = tempList.at(0) + "." + tempList.at(1) + ".";
            // qInfo() << "[info] tempStr for cClass:"<<tempStr;
            for( auto cc : cClasses){
                // qInfo() << "[info] cClass:"<<cc;
                if( cc.startsWith(tempStr)){
                    addNode(cc, cc, map);
                    if( addLabels ){
                        addLink(bClass, cc,bClass + "->" +cc,map);
                    }
                    else{
                        addLink(bClass, cc,"",map);
                    }
                }
            }
        }

        // Add the leaf nodes and link to the C Class nodes
        for( auto cClass : cClasses){
            QStringList tempList = cClass.split(".");
            QString tempStr = tempList.at(0) + "." + tempList.at(1) + "." + tempList.at(2) + ".";
            // qInfo() << "[info] tempStr for leaf:"<<tempStr;
            for( auto leaf : leaves){
                // qInfo() << "[info] leaf:"<<leaf;
                if( leaf.startsWith(tempStr)){
                    addNode(leaf, leaf, map);
                    if( addLabels ){
                        addLink(cClass, leaf,cClass + "->" +leaf,map);
                    }
                    else{
                        addLink(cClass,leaf,"",map);
                    }
                }
            }
        }

        return(*this);
    }
}

PtngDGMLBuilder &PtngDGMLBuilder::createFromNmap(QList<PtngHostBuilder*> builders, bool addLabels)
{
    qInfo() << "[info] Creating from nmap:";
    QStringList aClasses,bClasses,cClasses,leaves;
    QMap<QString,QString> map; // For attributes
    // Create the 'Attack Machine' root node
    addNode("attack_machine","Attack Machine",map);
    // Separate the entries into classes
    QStringList ips;
    PtngHost *host;
    for( auto builder : builders ){
        host=builder->getHost();
        QStringList tempList;
        QString tempStr;
        QString address = host->getIpAddress();
        tempList = address.split(".");
        // A Class
        tempStr = tempList.at(0) + ".*.*.*";
        // qInfo() << "[info] A Class temp string:"<<tempStr;
        if( !aClasses.contains(tempStr) ){
            aClasses.append(tempStr);
        }
        // B Class
        tempStr = tempList.at(0) + "." + tempList.at(1) + ".*.*";
        // qInfo() << "[info] B Class ttring:"<<tempStr;
        if( !bClasses.contains(tempStr) ){
            bClasses.append(tempStr);
        }
        // C Class
        tempStr = tempList.at(0) + "." + tempList.at(1)  + "." + tempList.at(2) + ".*";
        // qInfo() << "[info] C Class temp string:"<<tempStr;
        if( !cClasses.contains(tempStr) ){
            cClasses.append(tempStr);
        }
        // Leaf
        // qInfo() << "[info] Orginal address:"<<address;
        if( !leaves.contains(address) ){
            leaves.append(address);
        }
    }

        // Add the A class nodes and link to the 'Attack Machine'
        for( auto aClass : aClasses){
            addNode(aClass, aClass, map);
            if( addLabels ){
                addLink("attack_machine", aClass, "Attack Machine->" + aClass ,map);
            }
            else{
                addLink("attack_machine", aClass, "" ,map);
            }
        }

        for( auto aClass : aClasses){
            QStringList tempList = aClass.split(".");
            QString tempStr = tempList.at(0) + ".";
            // qInfo() << "[info] tempStr for bClass:"<<tempStr;
            for( auto bc : bClasses){
                // qInfo() << "[info] bClass:"<<bc;
                if( bc.startsWith(tempStr)){
                    addNode(bc, bc, map);
                    if( addLabels ){
                        addLink(aClass, bc,aClass + "->" + bc,map);
                    }
                    else{
                        addLink(aClass, bc,"",map);
                    }
                }
            }
        }

            // Add the C class nodes and link to the B Class nodes
            for( auto bClass : bClasses){
                QStringList tempList = bClass.split(".");
                QString tempStr = tempList.at(0) + "." + tempList.at(1) + ".";
                // qInfo() << "[info] tempStr for cClass:"<<tempStr;
                for( auto cc : cClasses){
                    // qInfo() << "[info] cClass:"<<cc;
                    if( cc.startsWith(tempStr)){
                        addNode(cc, cc, map);
                        if( addLabels ){
                            addLink(bClass, cc,bClass + "->" +cc,map);
                        }
                        else{
                            addLink(bClass, cc,"",map);
                        }
                    }
                }
            }

            // Add the leaf nodes and link to the C Class nodes
            for( auto cClass : cClasses){
                QStringList tempList = cClass.split(".");
                QString tempStr = tempList.at(0) + "." + tempList.at(1) + "." + tempList.at(2) + ".";
                // qInfo() << "[info] tempStr for leaf:"<<tempStr;
                for( auto leaf : leaves){
                    // qInfo() << "[info] leaf:"<<leaf;
                    PtngHost *lhost;
                    for( auto builder : builders){
                        if( builder->getHost()->getIpAddress() == leaf ){
                            lhost = builder->getHost();
                            break;
                        }
                    }
                    if( leaf.startsWith(tempStr)){
                        addNode(leaf, leaf, getAttributes(lhost));
                        if( addLabels ){
                            addLink(cClass, leaf,cClass + "->" +leaf,map);
                        }
                        else{
                            addLink(cClass,leaf,"",map);
                        }
                    }
                }
            }
        return(*this);
    }

    QMap<QString,QString> PtngDGMLBuilder::getAttributes(PtngHost *host){
        QMap<QString,QString> attrs;
        QMetaEnum metaEnum = QMetaEnum::fromType<PtngEnums::IssueSeverity>();
        QString sev = metaEnum.valueToKey(host->getHighestSeverity());
        QString dnsName = host->getDnsName().isEmpty() ? "Not resolved" :  host->getDnsName();
        attrs.insert("HighestSeverity",sev);
        attrs.insert("DnsName",dnsName);
        attrs.insert("MacAddress",host->getMacAddress());
        attrs.insert("MacVendor",host->getMacVendor());
        attrs.insert("OsName",host->getOsName());
        // qInfo() << "[info] OsName:"<<host->getOsName();
        attrs.insert("Gateway",host->isGateway() ? "True" : "False");
        attrs.insert("InAxfr",host->isInAxfr() ? "True" : "False");
        attrs.insert("GeoLocation",host->hasGeoLocation() ? "True" : "False");
        if( host->hasGeoLocation() ){
            attrs.insert("City",host->getCity());
            attrs.insert("Longitude",host->getLongitude());
            attrs.insert("Latitude",host->getLatitude());
        }
        attrs.insert("HostState",host->getHostState());
        attrs.insert("HostReason",host->getHostStateReason());
        attrs.insert("HostCPE",host->getHostCPE().join(":"));
        attrs.insert("HostHops",QString::number(host->getDistance()));
        attrs.insert("HostRecordType",host->getRecordType());

        return(attrs);
    }

    QString PtngDGMLBuilder::toString(int indent)
    {
        return(dgmlObject->doc->toString(indent));
    }

} // namespace ptng
