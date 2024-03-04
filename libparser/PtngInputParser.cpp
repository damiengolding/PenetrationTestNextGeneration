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

PtngInputParser::PtngInputParser(QObject *parent)
    : QObject{parent}
{

}

QMap<QString, QString> PtngInputParser::parseZoneTransfer(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    if( !QFile::exists(inputFile) ){
        qWarning() << "[warning] The supplied file"<<inputFile<< "does not exist:";
        return(addresses);
    }
    QScopedPointer<QFile> file(new QFile(inputFile));

    if( !file->open(QIODevice::ReadOnly) ){
        qWarning() << "[warning] The supplied file"<<inputFile<<"could not be opened for reading.";
        return(addresses);
    }
    PtngEnums::SupportedInputTypes type = PtngIdent::checkFile(inputFile);

    switch(type){
    case PtngEnums::AXFR_DNS_RECON:{
        // qInfo() << "[info] AXFR_DNS_RECON:"<<inputFile;
        addresses = parseAxfrDnsRecon(inputFile);
        break;
    }
    case PtngEnums::AXFR_NS_WIN:{
        // qInfo() << "[info] AXFR_NS_WIN:"<<inputFile;
        addresses = parseAxfrNslookupWin(inputFile);
        break;
    }
    case PtngEnums::AXFR_NS_LIN:{
        // qInfo() << "[info] AXFR_NS_LIN:"<<inputFile;
        addresses = parseAxfrNslookupLin(inputFile);
        break;
    }
    case PtngEnums::ARPSCAN:{
        // qInfo() << "[info] ARPSCAN :"<<inputFile;
        addresses = parseAxfrArpscan(inputFile);
        break;
    }
    case PtngEnums::NBTSCAN:{
        // qInfo() << "[info] NBTSCAN:"<<inputFile;
        addresses = parseAxfrNbtscan(inputFile);
        break;
    }
    case PtngEnums::AXFR_HOST:{
        // qInfo() << "[info] AXFR_HOST:"<<inputFile;
        addresses = parseAxfrHostScan(inputFile);
        break;
    }
    case PtngEnums::AXFR_NMAP:{
        // qInfo() << "[info] AXFR_NMAP:"<<inputFile;
        addresses = parseAxfrNmap(inputFile);
        break;
    }
    case PtngEnums::NMAP:{
        // qInfo() << "[info] The standard nmap scan output is not supported by this function. Call  parseNmap(const QString &inputFile) instead:"<<inputFile;
        break;
    }
    default:{
        // qInfo() << "[info] Supplied AXFR file"<<inputFile<<"is not supported";
        break;
    }
    }
    return(addresses);
}

QList<PtngHostBuilder*> PtngInputParser::parseNmap(const QString &inputFile){
    QList<PtngHostBuilder*> builderList;
    QScopedPointer<QDomDocument> doc(new QDomDocument("mydocument"));
    QScopedPointer<QFile> file(new QFile(inputFile));
    PtngEnums::SupportedInputTypes type = PtngIdent::checkFile(inputFile);
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Failed opening file"<<inputFile;
        return(builderList);
    }

    if( !doc->setContent(file.data()) ){
        qWarning() << "[warning] Failed parsing"<<inputFile;
        file->close();
        return(builderList);
    }
    QDomNodeList nodes = doc->elementsByTagName("host");
    qInfo() << "[info] Number of host nodes:"<<nodes.length();
    for( int i = 0; i != nodes.length(); ++i ){
        // qInfo() << "[info] Node number:"<<i;
        QDomNode node = nodes.at(i);
        PtngHostBuilder *hb = new PtngHostBuilder();
        hb->addNmapScanXmlNode(node);
        addPorts(hb,node);
        builderList.append(hb);
    }
    return(builderList);
}

void PtngInputParser::addPorts(PtngHostBuilder* builder, const QDomNode &node){
    // qInfo() << "[info] addPorts";
    PtngHost *host = builder->getHost();
    QDomElement elem = node.toElement();
    if( elem.isNull() ){
        return;
    }
    QDomNodeList portList = elem.elementsByTagName("ports");
    if( portList.length() == 0 ){
        // qInfo() << "[info] No open ports found on"<<host->getIpAddress();
        return;
    }
    else{
        // qInfo() << "[info] Found"<< portList.length() <<"ports open on"<<host->getIpAddress();
        QDomElement portListElem = portList.at(0).toElement();
        if( portListElem.isNull() ){
            return;
        }

        QDomNodeList ports = portListElem.elementsByTagName("port");
        // qInfo() << "[info] Found"<< ports.length() <<"ports open on"<<host->getIpAddress();

        for( int i = 0;i< ports.length();++i){
            QDomNode node = ports.at(i);
            QDomElement e = node.toElement();
            PtngPort port;
            if( e.isNull() ){
                continue;
            }

            port.portNumber = e.attribute("portid").toInt();
            QString prot = e.attribute("protocol");
            if( prot.toLower() == "tcp" ){
                port.protocol = PtngEnums::TCP;
            }
            else if( prot.toLower() == "udp" ){
                port.protocol = PtngEnums::UDP;
            }
            QDomElement state = e.firstChildElement("state");
            if( !state.isNull()){
                port.state = state.attribute("state");
                port.reason = state.attribute("reason");
            }
            QDomElement service = e.firstChildElement("service");
            if( !service.isNull()){
                port.serviceName = service.attribute("name");
                port.serviceProduct = service.attribute("product");
                port.serviceVersion = service.attribute("version");
                port.idMethod =  service.attribute("method");
                QDomNodeList cpe = service.elementsByTagName("cpe");
                if( cpe.length() > 0 ){
                    for(int i =0;i<cpe.length();++i){
                        QDomElement entry = cpe.at(i).toElement();
                        if( !entry.isNull()){
                            port.serviceCPE.append( entry.text() );
                        }
                    }
                }
                QDomNodeList scripts = e.elementsByTagName("script");
                for( int i =0;i<scripts.length();++i ){
                    QDomElement e = scripts.at(i).toElement();
                    if( e.isNull()){
                        continue;
                    }
                    QString id = e.attribute("id");
                    QString output = e.attribute("output");
                    port.portScripts.insert(id,output);
                }
            }
            builder->addPortSpec(port);
        }
    }
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
    // qInfo() << "[info] Number of ReportHosts:"<<hosts.count();
    QString ipAddress;
    for( int i = 0;i<hosts.count();++i ){
        QDomNode host = hosts.item(i);
        QDomElement elem = host.toElement();
        if( elem.isNull()){
            continue;
        }
        ipAddress = elem.attribute("name");
        ipAddresses.append(ipAddress);
        qInfo() << "[info] IP address:"<<ipAddress;
        QDomNodeList items = elem.elementsByTagName("ReportItem");
        // qInfo() << "[info] Number of ReportItems:"<<items.count();

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
            QString prot =  elem.attribute("protocol");
            if( prot.toLower() == "tcp"  ){
                issue.protocol = PtngEnums::TCP;
            }
            else if( prot.toLower() == "udp" ){
                issue.protocol = PtngEnums::UDP;
            }
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
                issue.severity = PtngEnums::MEDUIM;
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
            // qInfo() << "[info] Risk factor:"<<elem.firstChildElement("risk_factor").text();
            issueList.append(issue);
        }
    }
    return(issueList);
}

QMap<QString, QString> PtngInputParser::parseNesusSeverities(const QString &inputFile)
{
    QList<PtngIssue> issueList = parseNesusIssues(inputFile);
    QMap<QString, QString> severityMap;
    int critical=0;
    int high=0;
    int medium=0;
    int low=0;
    int none=0;
    // QString ipAddress="";
    QStringList ipAddresses;
    qInfo() << "[info] IP Addresses:"<<ipAddresses.count();
    QScopedPointer<QDomDocument> doc(new QDomDocument("input"));
    QScopedPointer<QFile> f(new QFile(inputFile));
    f->open(QIODevice::ReadOnly);
    doc->setContent(f.data());
    QDomElement rootElem = doc->documentElement();
    QDomNodeList hosts = rootElem.elementsByTagName("ReportHost");
    // qInfo() << "[info] Number of ReportHosts:"<<hosts.count();
    QString ipAddress;

    for( auto address : ipAddresses ){
        // QString address = ipAddresses.at(i);
        // qInfo() << "[info] address from ipAddresses list:"<<address;
        QString entry=address;
        // qInfo() << "[info] entry:"<<entry;
        for( auto issue : issueList){
            // PtngIssue issue = issueList.at(i);
            if( address == issue.ipAddress ){
                QString risk_factor = issue.riskFactor.toLower();
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
        //outputString.append(address + "=");

        QString sevStr;
        if(none>0){
            sevStr="NONE\n";
        }
        if(low>0){
            sevStr="LOW\n";
        }
        if(medium>0){
            sevStr="MEDIUM\n";
        }
        if(high>0){
            sevStr="HIGH\n";
        }
        if(critical>0){
            sevStr="CRITICAL\n";
        }
        //outputString.append(sevStr);
        severityMap.insert(address,sevStr);
        critical=0;
        high=0;
        medium=0;
        low=0;
        none=0;
    }
    return(severityMap);
}

QMap<QString, QString> PtngInputParser::parseAxfrNslookupWin(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    QTextStream stream(file.data());
    while( !stream.atEnd() ){
        QString line = stream.readLine().simplified();
        // REFACT the following could be simplified
        if( line.startsWith("[[" ) && line.endsWith("]]") ){
            continue;
        }
        QStringList entries = line.split(" ");
        if( entries.length() != 3 ){
            continue;
        }
        if( entries.at(0).toLower() == "domaindnszones" || entries.at(0).toLower() == "forestdnszones" ){
            continue;
        }
        if( entries.at(1) != "A" ){
            continue;
        }
        QString dnsName =  entries.at(0);
        if(dnsName.isEmpty()){
            dnsName= "Not resolved";
        }
        QString address =  entries.at(2).trimmed();
        // qInfo() << "[info] ns_win Name:"<<dnsName<<"Address:"<<address;
        addresses.insert(address,dnsName);
    }
    // qInfo() << "[info] Number of addresses:"<<addresses.count();
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrNslookupLin(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    QStringList lines;
    QTextStream stream(file.data());
    while( !stream.atEnd() ){
        lines.append(stream.readLine().simplified());
    }
    for( int i = 0;i<lines.length();++i){
        QString line = lines.at(i);
        if( line.toLower().startsWith("name") ){
            QStringList lineSplit = line.split(":");
            QString dnsName = lineSplit.at(1).simplified();
            if( dnsName.toLower().startsWith("forestdnszones") || dnsName.toLower().startsWith("domaindnszones") ){
                continue;
            }
            QString address;
            QString line2 = lines.at(i+1);
            lineSplit = line2.split(":");
            if( lineSplit.at(1).contains(":") || !lineSplit.at(1).contains(".") ){
                continue;
            }
            address = lineSplit.at(1).trimmed();
            // qInfo() << "[info] ns_linux Name:"<<dnsName<<"Address:"<<address;
            if(dnsName.isEmpty()){
                dnsName= "Not resolved";
            }
            addresses.insert(address,dnsName);
        }
    }
    // qInfo() << "[info] Number of addresses:"<<addresses.count();
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrArpscan(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    QStringList lines;
    QTextStream stream(file.data());
    qInfo() << "[info] Reading arp-scan file";
    while( !stream.atEnd() ){
        QString line = stream.readLine().simplified();
        // qInfo() << "[info] arp-scan line:"<<line;
        lines.append(line);
    }
    for(int i = 0;i<lines.length();++i){
        QString line = lines.at(i).simplified();
        // REFACT The following could be simplified
        if( line.toLower().startsWith("interface") || line.toLower().startsWith("starting") || line.isEmpty() ){
            continue;
        }
        if( line.count(".") < 3 || line.count(":") < 5 ){
            continue;
        }
        QStringList lineSplit = line.split(" ");
        QString address = lineSplit.at(0);
        // qInfo() << "[info] address from arpscan:"<<address;
        addresses.insert(address,"Not resolved");
    }
    // qInfo() << "[info] Number of addresses:"<<addresses.count();
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrNbtscan(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    QStringList lines;
    QTextStream stream(file.data());
    while( !stream.atEnd() ){
        lines.append(stream.readLine().simplified());
    }

    for( int i = 0;i<lines.count();++i){
        QString line = lines.at(i).simplified();
        if( line.toLower().startsWith("ip") || line.toLower().startsWith("-") || line.toLower().contains("sendto failed") ){
            continue;
        }
        qInfo() << "[info] Line from nbtscan:"<<line;
        QStringList lineSplit = line.split(" ");
        if( lineSplit.count()<2 ){
            continue;
        }
        QString address = lineSplit.at(0);
        QString dnsName = lineSplit.at(1);
        if(dnsName.isEmpty()){
            dnsName= "Not resolved";
        }
        // qInfo() << "[info] From nbtscan - name:"<<dnsName<<"Address:"<<address;
        addresses.insert(address,dnsName);
    }
    // qInfo() << "[info] Number of addresses:"<<addresses.count();
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrHostScan(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    QStringList lines;
    QTextStream stream(file.data());
    while( !stream.atEnd() ){
        lines.append(stream.readLine().simplified());
    }
    for(int i = 0;i<lines.count();++i){
        QString line = lines.at(i).simplified();
        // REFACT The if statements could be simplified
        if( line.startsWith("_") || line.toLower().startsWith("domaindnszones") || line.toLower().startsWith("forestdnszones") ){
            continue;
        }
        QStringList lineSplit = line.split(" ");
        if( lineSplit.count()<5 ){
            continue;
        }
        if( lineSplit.at(3) != "A" ){
            continue;
        }
        // qInfo() << "[info] Line from host scan:"<<line;
        QString dnsName = lineSplit.at(0);
        if(dnsName.isEmpty()){
            dnsName= "Not resolved";
        }
        QString address = lineSplit.at(4);
        // qInfo() << "[info] Name:"<<dnsName<<"Address:"<<address;
        addresses.insert(address,dnsName);
    }
    // qInfo() << "[info] Number of addresses:"<<addresses.count();
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrDnsRecon(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    QScopedPointer<QDomDocument> doc(new QDomDocument);
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    if( !doc->setContent(file.data())){
        qWarning() << "[warning] Unable to parse"<< inputFile;
        file->close();
        return(addresses);
    }
    QStringList ipAddresses;
    QDomElement root = doc->documentElement();
    QDomNodeList records = root.elementsByTagName("record");
    for( int i = 0;i<records.length();++i ){
        QDomElement record = records.at(i).toElement();
        if(record.isNull()){
            continue;
        }
        if( record.attribute("type") != "A" ){
            continue;
        }
        QString type = record.attribute("type");
        QString address = record.attribute("address");
        QString dnsName = record.attribute("name");
        if(dnsName.toLower().startsWith("domaindnszones")
                || dnsName.toLower().startsWith("forestdnszones")
                || dnsName.toLower().startsWith("@")
                ){
            continue;
        }
        if(dnsName.isEmpty()){
            dnsName= "Not resolved";
        }
        addresses.insert(address,dnsName);
    }
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrNmap(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    QScopedPointer<QDomDocument> doc(new QDomDocument);
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    if( !doc->setContent(file.data())){
        qWarning() << "[warning] Unable to parse"<< inputFile;
        file->close();
        return(addresses);
    }
    QStringList ipAddresses;
    QDomElement root = doc->documentElement();
    QDomNodeList scripts = root.elementsByTagName("script");
    // qInfo() << "[info] Scripts:"<<scripts.count();
    if( scripts.count() != 1 ){
        qWarning() << "[warning] Incorrect number of script nodes in source nmap xml"<<inputFile;
        return(addresses);
    }

    QDomElement elem = scripts.at(0).toElement();
    if( elem.isNull()){
        return(addresses);
    }

    QStringList lines = elem.attribute("output").split("\n", Qt::SkipEmptyParts);
    // qInfo() << "[info] number of lines:"<<lines.count();
    for(QString line : lines){
        if( line.startsWith("_") || line.toLower().startsWith("domaindnszones") || line.toLower().startsWith("forestdnszones") ){
            continue;
        }
        QString entry = line.simplified();
        QStringList lineSplit = entry.split(" ");
        if( lineSplit.count()  < 3 ){
            continue;
        }
        if( lineSplit.at(1) != "A" ){
            continue;
        }
        QString dnsName = lineSplit.at(0);
        if( dnsName.isEmpty() ){
            dnsName = "Not resolved";
        }
        QString address = lineSplit.at(2);
        // qInfo() << "[info] name:"<<dnsName<<"address:"<<address;
        addresses.insert(address,dnsName);
    }
    // qInfo() << "[info] addresses count:"<<addresses.count();
    return(addresses);
}
} // namespace ptng
