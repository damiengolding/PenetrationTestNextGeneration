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
#include "PtngAddressParser.hpp"

namespace ptng {

PtngAddressParser::PtngAddressParser(QObject *parent)
    : QObject{parent}
{

}

QMultiMap<QString, QString> PtngAddressParser::parseAddresses(const QString &inputFile)
{
    QMultiMap<QString, QString> addresses;
    if( !QFile::exists(inputFile) ){
        qWarning() << "[warning] The supplied file"<<inputFile<< "does not exist:";
        return(addresses);
    }
    QScopedPointer<QFile> file(new QFile(inputFile));

    if( !file->open(QIODevice::ReadOnly) ){
        qWarning() << "[warning] The supplied file"<<inputFile<<"could not be opened for reading.";
        return(addresses);
    }
    PtngIdent ident;
    PtngEnums::SupportedInputTypes type = ident.checkFile(inputFile);

    switch(type){
    case PtngEnums::AXFR_DNS_RECON:{
        qInfo() << "[info] AXFR_DNS_RECON:"<<inputFile;
        addresses = parseAxfrDnsRecon(inputFile);
        break;
    }
    case PtngEnums::AXFR_NS_WIN:{
        qInfo() << "[info] AXFR_NS_WIN:"<<inputFile;
        addresses = parseAxfrNslookupWin(inputFile);
        break;
    }
    case PtngEnums::AXFR_NS_LIN:{
        qInfo() << "[info] AXFR_NS_LIN:"<<inputFile;
        addresses = parseAxfrNslookupLin(inputFile);
        break;
    }
    case PtngEnums::ARPSCAN:{
        qInfo() << "[info] ARPSCAN :"<<inputFile;
        addresses = parseArpscan(inputFile);
        break;
    }
    case PtngEnums::NBTSCAN:{
        qInfo() << "[info] NBTSCAN:"<<inputFile;
        addresses = parseNbtscan(inputFile);
        break;
    }
    case PtngEnums::AXFR_HOST:{
        qInfo() << "[info] AXFR_HOST:"<<inputFile;
        addresses = parseHostScan(inputFile);
        break;
    }
    default:{
        qInfo() << "[info] Supplied AXFR file"<<inputFile<<"is not supported";
        break;
    }
    }
    return(addresses);
}

QMultiMap<QString, QString> PtngAddressParser::parseAxfrNslookupWin(const QString &inputFile)
{
    QMultiMap<QString, QString> addresses;
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
        QString address =  entries.at(2).trimmed();
        // qInfo() << "[info] ns_win Name:"<<dnsName<<"Address:"<<address;
        addresses.insert(address,dnsName);
    }
    // qInfo() << "[info] Number of addresses:"<<addresses.count();
    return(addresses);
}

QMultiMap<QString, QString> PtngAddressParser::parseAxfrNslookupLin(const QString &inputFile)
{
    QMultiMap<QString, QString> addresses;
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
            addresses.insert(address,dnsName);
        }
    }
    // qInfo() << "[info] Number of addresses:"<<addresses.count();
    return(addresses);
}

QMultiMap<QString, QString> PtngAddressParser::parseArpscan(const QString &inputFile)
{
    QMultiMap<QString, QString> addresses;
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

QMultiMap<QString, QString> PtngAddressParser::parseNbtscan(const QString &inputFile)
{
    QMultiMap<QString, QString> addresses;
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
        // qInfo() << "[info] From nbtscan - name:"<<dnsName<<"Address:"<<address;
        addresses.insert(address,dnsName);
    }
    // qInfo() << "[info] Number of addresses:"<<addresses.count();
    return(addresses);
}

QMultiMap<QString, QString> PtngAddressParser::parseHostScan(const QString &inputFile)
{
    QMultiMap<QString, QString> addresses;
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
        QString address = lineSplit.at(4);
        // qInfo() << "[info] Name:"<<dnsName<<"Address:"<<address;
        addresses.insert(address,dnsName);
    }
    // qInfo() << "[info] Number of addresses:"<<addresses.count();
    return(addresses);
}

QMultiMap<QString, QString> PtngAddressParser::parseAxfrDnsRecon(const QString &inputFile)
{
    QMultiMap<QString, QString> addresses;
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
        if( !addresses.contains("address") ){
            addresses.insert(address,dnsName);
        }
    }
    return(addresses);
}
} // namespace ptng
