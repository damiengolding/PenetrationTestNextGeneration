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
        qCritical() << "The supplied file"<<inputFile<< "does not exist:";
        return(addresses);
    }
    QScopedPointer<QFile> file(new QFile(inputFile));

    if( !file->open(QIODevice::ReadOnly) ){
        qCritical() << "The supplied file"<<inputFile<<"could not be opened for reading.";
        return(addresses);
    }
    PtngEnums::SupportedInputTypes type = PtngIdent::checkFile(inputFile);

    switch(type){
    case PtngEnums::AXFR_DNS_RECON:{
        addresses = parseAxfrDnsRecon(inputFile);
        break;
    }
    case PtngEnums::AXFR_NS_WIN:{
        addresses = parseAxfrNslookupWin(inputFile);
        break;
    }
    case PtngEnums::AXFR_NS_LIN:{
        addresses = parseAxfrNslookupLin(inputFile);
        break;
    }
    case PtngEnums::ARPSCAN:{
        addresses = parseAxfrArpscan(inputFile);
        break;
    }
    case PtngEnums::NBTSCAN:{
        addresses = parseAxfrNbtscan(inputFile);
        break;
    }
    case PtngEnums::AXFR_HOST:{
        addresses = parseAxfrHostScan(inputFile);
        break;
    }
    case PtngEnums::AXFR_NMAP:{
        addresses = parseAxfrNmap(inputFile);
        break;
    }
    // case PtngEnums::NMAP:{
    //     break;
    // }
    default:{
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
        qCritical() << "Unable to open"<<inputFile<<"for reading";
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
        addresses.insert(address,dnsName);
    }
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrNslookupLin(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qCritical() << "Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    QStringList lines;
    QTextStream stream(file.data());
    while( !stream.atEnd() ){
        lines.append(stream.readLine().simplified());
    }
    for( int i = 0;i<lines.length();++i){
        QString line = lines.at(i);
        if( line.startsWith("name",Qt::CaseInsensitive) ){
            QStringList lineSplit = line.split(":");
            QString dnsName = lineSplit.at(1).simplified();
            if( dnsName.startsWith("forestdnszones",Qt::CaseInsensitive) || dnsName.startsWith("domaindnszones",Qt::CaseInsensitive) ){
                continue;
            }
            QString address;
            QString line2 = lines.at(i+1);
            QStringList lineSplit2 = line2.split(":");
            QString line3 = lineSplit2.at(1);
            if( line3.contains(":") || !line3.contains(".") ){
                continue;
            }
            address = lineSplit.at(1).trimmed();
            if(dnsName.isEmpty()){
                dnsName= "Not resolved";
            }
            addresses.insert(address,dnsName);
        }
    }
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrArpscan(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qCritical() << "Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    QStringList lines;
    QTextStream stream(file.data());
    while( !stream.atEnd() ){
        QString line = stream.readLine().simplified();
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
        addresses.insert(address,"Not resolved");
    }
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrNbtscan(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qCritical() << "Unable to open"<<inputFile<<"for reading";
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
        QStringList lineSplit = line.split(" ");
        if( lineSplit.count()<2 ){
            continue;
        }
        QString address = lineSplit.at(0);
        QString dnsName = lineSplit.at(1);
        if(dnsName.isEmpty()){
            dnsName= "Not resolved";
        }
        addresses.insert(address,dnsName);
    }
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrHostScan(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qCritical() << "Unable to open"<<inputFile<<"for reading";
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
        QString dnsName = lineSplit.at(0);
        if(dnsName.isEmpty()){
            dnsName= "Not resolved";
        }
        QString address = lineSplit.at(4);
        addresses.insert(address,dnsName);
    }
    return(addresses);
}

QMap<QString, QString> PtngInputParser::parseAxfrDnsRecon(const QString &inputFile)
{
    QMap<QString, QString> addresses;
    QScopedPointer<QFile> file(new QFile(inputFile));
    QScopedPointer<QDomDocument> doc(new QDomDocument);
    if( !file->open(QIODevice::ReadOnly)){
        qCritical() << "Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    if( !doc->setContent(file.data())){
        qCritical() << "Unable to parse"<< inputFile;
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
        qCritical() << "Unable to open"<<inputFile<<"for reading";
        return(addresses);
    }
    if( !doc->setContent(file.data())){
        qCritical() << "Unable to parse"<< inputFile;
        file->close();
        return(addresses);
    }
    QStringList ipAddresses;
    QDomElement root = doc->documentElement();
    QDomNodeList scripts = root.elementsByTagName("script");
    if( scripts.count() != 1 ){
        qCritical() << "Incorrect number of script nodes in source nmap xml"<<inputFile;
        return(addresses);
    }

    QDomElement elem = scripts.at(0).toElement();
    if( elem.isNull()){
        return(addresses);
    }

    QStringList lines = elem.attribute("output").split("\n", Qt::SkipEmptyParts);
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
        addresses.insert(address,dnsName);
    }
    return(addresses);
}

} // namespace ptng
