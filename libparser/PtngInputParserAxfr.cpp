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
