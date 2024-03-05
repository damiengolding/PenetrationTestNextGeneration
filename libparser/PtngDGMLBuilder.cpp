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

    // See here for style attribute options: https://github.com/MicrosoftDocs/visualstudio-docs/blob/main/docs/modeling/customize-code-maps-by-editing-the-dgml-files.md#OrganizeNodes

    // QUERY need some null tests here?
    QDomElement criticalCategory = doc->createElement("Category");
    criticalCategory.setAttribute("Id","critical");
    criticalCategory.setAttribute("Label","Critical");
    criticalCategory.setAttribute("Stroke","Firebrick"); // Border colour
    criticalCategory.setAttribute("Background","White");
    criticalCategory.setAttribute("Foreground","Black");
    criticalCategory.setAttribute("StrokeThickness","1");
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
    highCategory.setAttribute("Stroke","OrangeRed"); // Border colour
    highCategory.setAttribute("Background","White");
    highCategory.setAttribute("Foreground","Black");
    highCategory.setAttribute("StrokeThickness","1");
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
    mediumCategory.setAttribute("Stroke","DarkSalmon"); // Border colour
    mediumCategory.setAttribute("Background","White");
    mediumCategory.setAttribute("Foreground","Black");
    mediumCategory.setAttribute("StrokeThickness","1");
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
    lowCategory.setAttribute("Stroke","DarkGreen"); // Border colour
    lowCategory.setAttribute("Background","White");
    lowCategory.setAttribute("Foreground","Black");
    lowCategory.setAttribute("StrokeThickness","1");
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
    noneCategory.setAttribute("Stroke","Navy"); // Border colour
    noneCategory.setAttribute("Background","White");
    noneCategory.setAttribute("Foreground","Black");
    noneCategory.setAttribute("StrokeThickness","1");
    // noneCategory.setAttribute("Icon","<path>");
    noneCategory.setAttribute("FontFamily","Open Sans");
    noneCategory.setAttribute("FontSize","10");
    // noneCategory.setAttribute("FontWeight","<value>");
    noneCategory.setAttribute("FontStyle","<Style>"); // Italic or Bold
    noneCategory.setAttribute("Style","Plain"); // "Plain" or "Glass"
    categories.appendChild(noneCategory);
    root.appendChild(categories);

    QDomElement properties = doc->createElement("Properties");
    root.appendChild(properties);

    QDomElement paths = doc->createElement("Paths");
    root.appendChild(paths);

    QDomElement styles = doc->createElement("Styles");
    root.appendChild(styles);

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

// TODO test all these 'add' methods
// REFACT is there a way to put all of these into a single function with a switch on teh tpye?
PtngDGMLBuilder &PtngDGMLBuilder::addNode(const QString &id, const QString &label, const QMap<QString, QString> &attributes)
{
    // qInfo() << "[info] Adding node:"<<"Id:"<<id<<"Label:"<<label;
    QDomElement newNode = dgmlObject->doc->createElement("Node");
    newNode.setAttribute("Id",id);
    newNode.setAttribute("Label",label);
    newNode.setAttribute("Category","none");
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

PtngDGMLBuilder& PtngDGMLBuilder::createSimple(const QMap<QString, QString> &anPairs, const QString &subnetFilters, bool addLabels)
{
    // qInfo() << "[info] In fromSimple";
    QStringList aClasses,bClasses,cClasses,leaves;
    QMap<QString,QString> map; // For attributes
    // Create the 'Attack Machine' root node
    addNode("attack_machine","Attack Machine",map);

    // Separate the entries into classes
    QStringList ips;
    QStringList subnets = subnetFilters.split(",",Qt::SkipEmptyParts);

    for(auto [address,name] : anPairs.asKeyValueRange()){
        QStringList tempList;
        QString tempStr;
        QString testStr;
        tempList=address.split(".");
        tempStr = tempList.at(0) + ".*.*.*";
        testStr = tempList.at(0) + ".";
        if( !aClasses.contains(tempStr) ){
            if( subnets.count()>0 ){
                for( auto subnet : subnets ){
                    if( subnet.startsWith(testStr) ){
                        aClasses.append(tempStr);
                        break;
                    }
                }
            }
            else{
                aClasses.append(tempStr);
            }
        }

        tempStr = tempList.at(0) + "." + tempList.at(1) + ".*.*";
        testStr = tempList.at(0) + "."+ tempList.at(1) + ".";
        if( !bClasses.contains(tempStr) ){
            if( subnets.count()>0 ){
                for( auto subnet : subnets ){
                    if( subnet.startsWith(testStr) ){
                        bClasses.append(tempStr);
                        break;
                    }
                }
            }
            else{
                bClasses.append(tempStr);
            }
        }

        tempStr = tempList.at(0)  + "." + tempList.at(1)  + "." + tempList.at(2) + ".*";
        testStr = tempList.at(0) + "." + tempList.at(1) + "." + tempList.at(2) + ".";
        if( !cClasses.contains(tempStr) ){
            if( subnets.count()>0 ){
                for( auto subnet : subnets ){
                    if( subnets.count()>0 ){
                        if( subnet.startsWith(testStr) ){
                            cClasses.append(tempStr);
                            break;
                        }
                    }
                }
            }
            else{
                cClasses.append(tempStr);
            }
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
    qInfo() << "[info] aClasses 2:"<<aClasses.count();

    // Add the B class nodes and link to the A Class nodes
    for( auto aClass : aClasses){
        QStringList tempList = aClass.split(".");
        QString tempStr = tempList.at(0) + ".";
        for( auto bc : bClasses){
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
            for( auto cc : cClasses){
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

PtngDGMLBuilder &PtngDGMLBuilder::createFromNmap(QList<PtngHostBuilder*> builders, const QString &issuesFile, const QString &zoneFile, const QString &subnetFilters, bool addLabels)
{
    qInfo() << "[info] Creating from nmap";
    QStringList subnets = subnetFilters.split(",");
    QMap<QString,QString> zoneAddesses;
    if( !zoneFile.isEmpty()){
        zoneAddesses = PtngInputParser::parseZoneTransfer(zoneFile);
    }
    QMap<QString,QString> severities;
    if( !issuesFile.isEmpty()){
        severities = PtngInputParser::parseNesusSeverities(issuesFile);
    }
    QStringList aClasses,bClasses,cClasses,leaves;
    QMap<QString,QString> map; // For attributes
    QMap<QString,QString> emptyMap; // For empty attributes
    // Create the 'Attack Machine' root node
    addNode("attack_machine","Attack Machine",map);

    QStringList ips;
    PtngHost *host;

    // Separate the entries into classes
    for( auto builder : builders ){
        host=builder->getHost();
        QStringList tempList;
        QString tempStr;
        QString testStr;
        QString address = host->getIpAddress();
        tempList = address.split(".");
        // A Class
        tempStr = tempList.at(0) + ".*.*.*";
        testStr = tempList.at(0) + ".";
        if( !aClasses.contains(tempStr) && isInFilter(subnetFilters.split(",",Qt::SkipEmptyParts), testStr)  ){
            aClasses.append(tempStr);
        }
        // B Class
        tempStr = tempList.at(0) + "." + tempList.at(1) + ".*.*";
        if( !bClasses.contains(tempStr) && isInFilter(subnetFilters.split(",",Qt::SkipEmptyParts), tempStr)  ){

            if( !aClasses.contains(tempStr) ){
                aClasses.append(tempList.at(0) + ".*.*.*");
            }
            // aClasses.append(tempList.at(0) + ".*.*.*");
            bClasses.append(tempStr);
        }

        // C Class
        tempStr = tempList.at(0) + "." + tempList.at(1)  + "." + tempList.at(2) + ".*";
        testStr = tempList.at(0) + "." + tempList.at(1) + "." + tempList.at(2) + ".";
        if( !cClasses.contains(tempStr) && isInFilter(subnetFilters.split(",",Qt::SkipEmptyParts),testStr)  ){
            cClasses.append(tempStr);
        }

        // Leaf
        if( !leaves.contains(address) ){
            leaves.append(address);
        }
    }

    // Add the A class nodes and link to the 'Attack Machine'
    for( auto aClass : aClasses){
        // QString testStr = aClass.split(".").at(0) + ".";
        // if( !isInFilter(subnetFilters.split(","), testStr , 0 )){
        //     continue;
        // }

        addNode(aClass, aClass, map);
        if( addLabels ){
            addLink("attack_machine", aClass, "Attack Machine->" + aClass ,emptyMap);
        }
        else{
            addLink("attack_machine", aClass, "" ,emptyMap);
        }
    }

    // Add the B class nodes and link to the C Class nodes
    for( auto aClass : aClasses){
        QStringList tempList = aClass.split(".");
        QString tempStr = tempList.at(0);
        qInfo() << "[info] tempStr for bClass:"<<tempStr;
        for( auto bc : bClasses){

            // QString testStr = bc.split(".").at(0) + "." + bc.split(".").at(1) + ".";
            // qInfo() << "[info] bClass teststr:"<<testStr;
            // if( !isInFilter(subnetFilters.split(","), testStr , 1 )){
            //     continue;
            // }
            // qInfo() << "[info] bClass:"<<bc;

            if( bc.startsWith(tempStr)){
                addNode(bc, bc, map);
                if( addLabels ){
                    addLink(aClass, bc,aClass + "->" + bc,emptyMap);
                }
                else{
                    addLink(aClass, bc,"",emptyMap);
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
            // QString testStr = cc.split(".").at(0) + "." + cc.split(".").at(1) + "." + cc.split(".").at(2) + ".";
            // qInfo() << "[info] cClass teststr:"<<testStr;
            // if( !isInFilter(subnetFilters.split(","), testStr , 2 )){
            //     continue;
            // }
            // qInfo() << "[info] cClass:"<<cc;
            if( cc.startsWith(tempStr)){
                addNode(cc, cc, map);
                if( addLabels ){
                    addLink(bClass, cc,cc + "->" +bClass,emptyMap);
                }
                else{
                    addLink(bClass, cc,"",emptyMap);
                }
            }
        }
    }

    // Add the leaf nodes and link to the C Class nodes
    for(auto cClass : cClasses){
        QStringList tempList = cClass.split(".");
        QString tempStr = tempList.at(0) + "." + tempList.at(1) + "." + tempList.at(2) + ".";
        for( auto builder : builders ) {
            QString address = builder->getHost()->getIpAddress();
            if( !leaves.contains(address)){
                continue;
            }

            for( auto [ipAddress,sev] : severities.asKeyValueRange()){
                // Add severity category and PtngEnums::IssueSeverities to nodes
                if(address != ipAddress){
                    continue;
                }
                QString severity = sev.toLower();
                map.insert("Category",severity.toLower());
                if( severity == "critical"){
                    builder->setHighestSeverity( PtngEnums::CRITICAL);
                }
                else if( severity == "high"){
                    builder->setHighestSeverity( PtngEnums::HIGH);
                }
                else if( severity == "medium"){
                    builder->setHighestSeverity( PtngEnums::MEDIUM);
                }
                else if( severity == "low"){
                    builder->setHighestSeverity( PtngEnums::LOW);
                }
                else if( severity == "none"){
                    builder->setHighestSeverity( PtngEnums::NONE);
                }
                else{
                    builder->setHighestSeverity(  PtngEnums::NUM_ISSUE_SEVERITIES);
                }
            }

            if( address.startsWith(tempStr)){
                map.insert(getAttributes(builder->getHost()));
                QString zAddress = "!"+address+"\n"+builder->getHost()->getDnsName();
                if( zoneAddesses.keys().contains(address) ){
                    addNode(address, address+"\n"+builder->getHost()->getDnsName(), map);
                }
                else{
                    addNode(address, zAddress, map);
                }
                if( addLabels ){
                    addLink(cClass, address,cClass + "->" +address,emptyMap);
                }
                else{
                    addLink(cClass,address,"",emptyMap);
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

QList<PtngHostBuilder*> PtngDGMLBuilder::setHighestSeverity(QList<PtngHostBuilder*> builders)
{
    QList<PtngHostBuilder*> builderList = builders;


    return(builderList);
}

bool PtngDGMLBuilder::isInFilter(const QStringList &subnetFilters, const QString &testString)
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

QString PtngDGMLBuilder::toString(int indent)
{
    return(dgmlObject->doc->toString(indent));
}

} // namespace ptng
