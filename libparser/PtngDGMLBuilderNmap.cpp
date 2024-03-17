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
namespace ptng{

PtngDGMLBuilder &PtngDGMLBuilder::createFromNmap(QList<PtngHostBuilder*> builders, const QString &issuesFile, const QString &zoneFile, const QString &subnetFilters, bool addLabels)
{
    qInfo() << "[info] Creating from nmap";
    dgmlObject->root.setAttribute("Title","NmapDGML");
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
    map.insert("Category","subnet");
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
        for( auto bc : bClasses){

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
    map.clear();
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

} // namespace ptng
