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

PtngDGMLBuilder& PtngDGMLBuilder::createFromNessus(QList<PtngHostBuilder*> builders, const QString &nessusFile, const QString &severityFilters,const QString &subnetFilters){
    dgmlObject->root.setAttribute("Title","NessusDGML");
    QStringList sevFilters = severityFilters.split(",", Qt::SkipEmptyParts);
    QStringList subnets = subnetFilters.split(",", Qt::SkipEmptyParts);
    QMap<QString,QString> severities;
    QList<PtngIssue> issues;
    if( !nessusFile.isEmpty()){
        severities = PtngInputParser::parseNesusSeverities(nessusFile);
        issues = PtngInputParser::parseNesusIssues(nessusFile);
    }
    QStringList aClasses,bClasses,cClasses,leaves;
    QMap<QString,QString> map; // For attributes
    map.insert("Category","subnet");
    QMap<QString,QString> emptyMap; // For empty attributes

    // Root node
    addNode("attack_machine","Attack Machine",map);

    QStringList ips;
    PtngHost *host;

    // Separate the entries into classes
    // qInfo() << "[info] Builders:"<<builders.count();
    for( auto builder : builders ){
        // qInfo() << "[info] in builders loop";
        host=builder->getHost();
        // qInfo() << "[info] IP address:"<<host->getIpAddress();
        QStringList tempList;
        QString tempStr;
        QString testStr;
        QString address = host->getIpAddress();
        tempList = address.split(".");
        // A Class
        tempStr = tempList.at(0) + ".*.*.*";
        testStr = tempList.at(0) + ".";
        if( !aClasses.contains(tempStr) && isInSubnetFilter(subnetFilters.split(",",Qt::SkipEmptyParts), testStr)  ){
            aClasses.append(tempStr);
        }
        // B Class
        tempStr = tempList.at(0) + "." + tempList.at(1) + ".*.*";
        if( !bClasses.contains(tempStr) && isInSubnetFilter(subnetFilters.split(",",Qt::SkipEmptyParts), tempStr)  ){

            if( !aClasses.contains(tempStr) ){
                aClasses.append(tempList.at(0) + ".*.*.*");
            }
            // aClasses.append(tempList.at(0) + ".*.*.*");
            bClasses.append(tempStr);
        }

        // C Class
        tempStr = tempList.at(0) + "." + tempList.at(1)  + "." + tempList.at(2) + ".*";
        testStr = tempList.at(0) + "." + tempList.at(1) + "." + tempList.at(2) + ".";
        if( !cClasses.contains(tempStr) && isInSubnetFilter(subnetFilters.split(",",Qt::SkipEmptyParts),testStr)  ){
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
        addLink("attack_machine", aClass, "" ,emptyMap);
    }

    // Add the B class nodes and link to the C Class nodes
    for( auto aClass : aClasses){
        QStringList tempList = aClass.split(".");
        QString tempStr = tempList.at(0);
        for( auto bc : bClasses){
            if( bc.startsWith(tempStr)){
                addNode(bc, bc, map);
                addLink(aClass, bc,"",emptyMap);
            }
        }
    }

    // Add the C class nodes and link to the B Class nodes
    for( auto bClass : bClasses){
        QStringList tempList = bClass.split(".");
        QString tempStr = tempList.at(0) + "." + tempList.at(1) + ".";
        // qInfo() << "[info] tempStr for cClass:"<<tempStr;
        for( auto cc : cClasses){
            if( cc.startsWith(tempStr)){
                addNode(cc, cc, map);
                addLink(bClass, cc,"",emptyMap);
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
                // map.insert(getAttributes(builder->getHost()));
                // QString zAddress = "!"+address+"\n"+builder->getHost()->getDnsName();
                addNode(address, address, map);
                addLink(cClass,address,"",emptyMap);
            }
        }

        // Add the nessus severity counts
        if( !nessusFile.isEmpty() ){
            qInfo() << "[info] About to add nessus severities after isEmpty() test:"<<issues.count();
            addNessusSeverityCount(builders,issues);
        }
        return(*this);
    }

}
} // namespace ptng


// int id = 1;
// for( auto issue : issues ){
//     QString address = issue.ipAddress;
//     QString riskFactor = issue.riskFactor.toUpper();
//     if( isInSubnetFilter(subnets,address) && isInSeverityFilter(severities, riskFactor) ){
//         QString label=address % "\n" % riskFactor;
//         QMap<QString,QString> attributes;
//         attributes.insert("PluginId",issue.pluginId);
//         attributes.insert("PluginName",issue.pluginName);
//         attributes.insert("PluginFamily",issue.pluginFamily);
//         attributes.insert("PluginType",issue.pluginType);
//         attributes.insert("Protocol", issue.protocol);
//         attributes.insert("PortNumber",QString::number(issue.portNumber));
//         attributes.insert("ServiceName",issue.serviceName);
//         attributes.insert("RiskFactor",issue.riskFactor);
//         attributes.insert("Display",issue.display);
//         attributes.insert("Synopsis",issue.synopsis);
//         attributes.insert("Solution",issue.solution);
//         addNode(QString::number(id), label,attributes);
//         addLink("0",QString::number(id));
//         ++id;
//     }
// }
// qInfo() << "[info] Nessus issue count (filtered):"<<id - 1;
