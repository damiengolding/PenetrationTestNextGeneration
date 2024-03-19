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

#include "inc\PtngInputParser.hpp"
namespace ptng {

QList<PtngHostBuilder*> PtngInputParser::parseNessus(const QString &inputFile){
    qInfo() << "[info] Starting to parse nessus file:"<<inputFile;
    QList<PtngHostBuilder*> builderList;
    PtngEnums::SupportedInputTypes type = PtngIdent::checkFile(inputFile);
    if( type != PtngEnums::NESSUS ){
        qWarning() << "[warning] Input file"<<inputFile<<"is of incorrect type"<<type;
        return(builderList);
    }
    QScopedPointer<QDomDocument> doc(new QDomDocument("mydocument"));
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Failed opening file"<<inputFile;
        return(builderList);
    }

    if( !doc->setContent(file.data()) ){
        qWarning() << "[warning] Failed parsing"<<inputFile;
        file->close();
        return(builderList);
    }
    QDomNodeList nodes = doc->elementsByTagName("ReportHost");
    qInfo() << "[info] Number of ReportHost nodes:"<<nodes.length();
    for( int i = 0; i != nodes.length(); ++i ){
        // qInfo() << "[info] Node number:"<<i;
        QDomNode node = nodes.at(i);
        PtngHostBuilder *hb = new PtngHostBuilder();
        hb->addNessusScanXmlNode(node);
        builderList.append(hb);
    }
    return(builderList);
}



// TODO this in conjunction with parseNesusSeverities REALLY needs at least developer testing
QList<PtngIssue> PtngInputParser::parseNesusIssues(const QString &inputFile)
{
    QList<PtngIssue> issueList;
    qInfo() << "[info] Starting to process nessus file:"<<inputFile;
    QStringList ipAddresses;
    QScopedPointer<QDomDocument> doc(new QDomDocument("input"));
    QScopedPointer<QFile> f(new QFile(inputFile));
    f->open(QIODevice::ReadOnly);
    doc->setContent(f.data());
    QDomElement rootElem = doc->documentElement();
    QDomNodeList hosts = rootElem.elementsByTagName("ReportHost");
    QString ipAddress;
    for( int i = 0;i<hosts.count();++i ){
        QDomNode host = hosts.item(i);
        QDomElement elem = host.toElement();
        if( elem.isNull()){
            continue;
        }
        ipAddress = elem.attribute("name");
        ipAddresses.append(ipAddress);
        QDomNodeList items = elem.elementsByTagName("ReportItem");

        for( int i = 0;i<items.count();++i){
            QDomNode item = items.at(i);
            QDomElement elem = item.toElement();
            if( elem.isNull() ){
                continue;
            }
            PtngIssue issue;
            issue.ipAddress = ipAddress;
            issue.portNumber = elem.attribute("port").toInt();
            issue.serviceName = elem.attribute("svc_name");
            issue.protocol = elem.attribute("protocol").toUpper();
            // QString prot =  elem.attribute("protocol");
            // if( prot.toLower() == "tcp"  ){
            //     issue.protocol = PtngEnums::TCP;
            // }
            // else if( prot.toLower() == "udp" ){
            //     issue.protocol = PtngEnums::UDP;
            // }
            int sev  = elem.attribute("severity").toInt();
            switch(sev){
            case 0:{
                issue.severity = PtngEnums::NONE;
                break;
            }
            case 1:{
                issue.severity = PtngEnums::LOW;
                break;
            }
            case 2:{
                issue.severity = PtngEnums::MEDIUM;
                break;
            }
            case 3:{
                issue.severity = PtngEnums::HIGH;
                break;
            }
            case 4:{
                issue.severity = PtngEnums::CRITICAL;
                break;
            }
            default:{
                issue.severity = PtngEnums::NUM_ISSUE_SEVERITIES;
                break;
            }
            }
            issue.pluginId = elem.attribute("pluginID");
            issue.pluginName = elem.attribute("pluginName");
            issue.pluginFamily = elem.attribute("pluginFamily");
            issue.pluginType = elem.firstChildElement("plugin_type").text();
            issue.display = elem.firstChildElement("plugin_name").text();
            issue.description = elem.firstChildElement("description").text();
            issue.synopsis = elem.firstChildElement("synopsis").text();
            issue.solution = elem.firstChildElement("solution").text();
            issue.seeAlso = elem.firstChildElement("xref").text();
            issue.riskFactor = elem.firstChildElement("risk_factor").text();
            issueList.append(issue);
        }
    }
    qInfo() << "[info] Nessus issue count (full):"<<issueList.count();
    return(issueList);
}

QMap<QString, QString> PtngInputParser::parseNesusSeverities(const QString &inputFile)
{
    QList<PtngIssue> issueList = parseNesusIssues(inputFile);
    // qInfo() << "[info] issueList size:"<<issueList.count();
    QMap<QString, QString> severityMap;
    // QString ipAddress="";
    QStringList ipAddresses;
    QScopedPointer<QDomDocument> doc(new QDomDocument("input"));
    QScopedPointer<QFile> f(new QFile(inputFile));
    f->open(QIODevice::ReadOnly);
    doc->setContent(f.data());
    QDomElement rootElem = doc->documentElement();
    QDomNodeList hosts = rootElem.elementsByTagName("ReportHost");
    // qInfo() << "[info] Number of ReportHosts:"<<hosts.count();
    QString address;

    for( auto issue : issueList ){
        int critical=0;
        int high=0;
        int medium=0;
        int low=0;
        int none=0;
        if( address == issue.ipAddress ){
            continue;
        }
        address = issue.ipAddress;
        // qInfo() << "[info] address from issueList:"<<address;
        QString entry=address;
        for( auto issue : issueList){
            // qInfo() << "[info] Checking severity for:"<<issue.ipAddress;
            // PtngIssue issue = issueList.at(i);
            if( address == issue.ipAddress ){
                QString risk_factor = issue.riskFactor.toLower();
                // qInfo() << "[info] Risk factor: "<<risk_factor;
                if( risk_factor == "critical" )
                    critical++;
                else if( risk_factor == "high" )
                    high++;
                else if( risk_factor == "medium" )
                    medium++;
                else if( risk_factor == "low" )
                    low++;
                else if( risk_factor == "none" )
                    none++;
            }
        }
        QString sevStr;
        if(none>0){
            sevStr="NONE";
        }
        if(low>0){
            sevStr="LOW";
        }
        if(medium>0){
            sevStr="MEDIUM";
        }
        if(high>0){
            sevStr="HIGH";
        }
        if(critical>0){
            sevStr="CRITICAL";
        }
        critical=0;
        high=0;
        medium=0;
        low=0;
        none=0;
        // qInfo() << "[info] Sev for"<<issue.ipAddress<<sevStr;
        severityMap.insert(address,sevStr);
    } // top is for( auto address : ipAddresses ){
    // qInfo() << "[info] IP Addresses/sev count:"<<severityMap.count();
    return(severityMap);
}

} // namespace ptng
