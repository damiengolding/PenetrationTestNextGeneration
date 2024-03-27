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

#include "inc/PtngInputParser.hpp"

using namespace ptng;

#ifdef QT_DEBUG

#pragma nmap & nessus {

void PtngInputParser::shouldCountNmapHosts_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    // qInfo() << "ptnginputparser nmap elements:"<<testFiles.count();
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "nmap" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}

void PtngInputParser::shouldCountNmapHosts(){
    QFETCH(QString,file);
    QList<PtngHostBuilder*> hosts = PtngInputParser::parseNmap(file);
    QCOMPARE(hosts.count(),7);
}

void PtngInputParser::shouldCountNessusHosts_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    // qInfo() << "ptnginputparser elements:"<<testFiles.count();
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "nessus" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}

void PtngInputParser::shouldCountNessusHosts(){
    QFETCH(QString,file);
    QList<PtngHostBuilder*> hosts = PtngInputParser::parseNessus(file);
    QCOMPARE(hosts.count(),7);
}

void PtngInputParser::shouldCountNessusIssues_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    // qInfo() << "ptnginputparser elements:"<<testFiles.count();
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "nessus" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}

void PtngInputParser::shouldCountNessusIssues(){
    QFETCH(QString,file);
    QList<PtngIssue> issues = PtngInputParser::parseNesusIssues(file);
    QCOMPARE(issues.count(),3126);
}

void PtngInputParser::shouldCountNessusSeverities_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    // qInfo() << "ptnginputparser elements:"<<testFiles.count();
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "nessus" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}

void PtngInputParser::shouldCountNessusSeverities(){
    QFETCH(QString,file);
    QMap<QString,QString> severities = PtngInputParser::parseNesusSeverities(file);
    QCOMPARE(severities.count(),7);

    for( auto [address,severity] : severities.asKeyValueRange()){
        // qInfo() << "Name:"<<name<<"Value"<<value;
        if( address == "192.168.4.29" ){
            QCOMPARE("CRITICAL",severity);
        }
        else if( address == "192.168.4.32" ){
            QCOMPARE("CRITICAL",severity);
        }
        else if( address == "192.168.4.36" ){
            QCOMPARE("CRITICAL",severity);
        }
        else if( address == "192.168.4.42" ){
            QCOMPARE("CRITICAL",severity);
        }
        else if( address == "192.168.4.43" ){
            QCOMPARE("CRITICAL",severity);
        }
        else if( address == "192.168.4.45" ){
            QCOMPARE("CRITICAL",severity);
        }
        else if( address == "192.168.4.46" ){
            QCOMPARE("CRITICAL",severity);
        }
        else{
            QString message = QString( "Unrecognised IP address: %1").arg(address);
            QWARN(qPrintable(message));
        }

    }
}

#pragma nmap & nessus }

#pragma AXFR {

// AXFR files
void PtngInputParser::shouldCountDnsreconAXFRHosts_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    // qInfo() << "ptnginputparser elements:"<<testFiles.count();
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "dnsrecon" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}
void PtngInputParser::shouldCountNmapAXFRHosts_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "nmap_axfr" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}
void PtngInputParser::shouldCountNslookupWindowsAXFRHosts_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    // qInfo() << "ptnginputparser elements:"<<testFiles.count();
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "nslookup_windows" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}
void PtngInputParser::shouldCountNslookupLinuxAXFRHosts_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    // qInfo() << "ptnginputparser elements:"<<testFiles.count();
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "nslookup_linux" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}
void PtngInputParser::shouldCountArpscanAXFRHosts_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    // qInfo() << "ptnginputparser elements:"<<testFiles.count();
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "arpscan" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}
void PtngInputParser::shouldCountNbtscanAXFRHosts_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    // qInfo() << "ptnginputparser elements:"<<testFiles.count();
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "nbtscan" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}
void PtngInputParser::shouldCountHostAXFRHosts_data(){
    QScopedPointer<QFile> file(new QFile(PtngConfig::testConfiguration));
    QScopedPointer<QDomDocument> doc(new QDomDocument(""));

    if( !file->open(QIODevice::ReadOnly) ){
        QString message = QString("Couldn't open the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    if( !doc->setContent(file->readAll()) ){
        QString message = QString("Couldn't parse the XML configuration file: %1").arg( PtngConfig::testConfiguration );
        QFAIL(qPrintable(message));
    }

    QDomNodeList testFiles = doc->elementsByTagName("ptnginputparser");
    // qInfo() << "ptnginputparser elements:"<<testFiles.count();
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");

    for( int i = 0; i< testFiles.count(); ++i ){
        QDomNode node = testFiles.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }
        QString name = elem.attribute("name");
        if( name != "host" ){
            continue;
        }
        QString value = elem.attribute("value");
        QTest::addRow(qPrintable(name)) << name << value;
    }
}

void PtngInputParser::shouldCountDnsreconAXFRHosts(){
    QFETCH(QString,file);
    QMap<QString,QString> nv = parseAxfrDnsRecon(file);
    QCOMPARE(nv.count(),8);
}
void PtngInputParser::shouldCountNmapAXFRHosts(){
    QFETCH(QString,file);
    QMap<QString,QString> nv = parseAxfrNmap(file);
    QCOMPARE(nv.count(),8);
}
void PtngInputParser::shouldCountNslookupWindowsAXFRHosts(){
    QFETCH(QString,file);
    QMap<QString,QString> nv = parseAxfrNslookupWin(file);
    QCOMPARE(nv.count(),8);
}
void PtngInputParser::shouldCountNslookupLinuxAXFRHosts(){
    QFETCH(QString,file);
    QMap<QString,QString> nv = parseAxfrNslookupLin(file);
    QCOMPARE(nv.count(),28);
}
void PtngInputParser::shouldCountArpscanAXFRHosts(){
    QFETCH(QString,file);
    QMap<QString,QString> nv = parseAxfrArpscan(file);
    QCOMPARE(nv.count(),12);
}
void PtngInputParser::shouldCountNbtscanAXFRHosts(){
    QFETCH(QString,file);
    QMap<QString,QString> nv = parseAxfrNbtscan(file);
    QCOMPARE(nv.count(),7);
}
void PtngInputParser::shouldCountHostAXFRHosts(){
    QFETCH(QString,file);
    QMap<QString,QString> nv = parseAxfrHostScan(file);
    QCOMPARE(nv.count(),8);
}

#pragma AXFR }


#endif
