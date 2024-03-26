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
#include "inc\PtngIdent.hpp"
#include "QtTest/qtestcase.h"

namespace ptng {

PtngIdent::PtngIdent(QObject *parent)
    : QObject{parent}
{}

PtngEnums::SupportedInputTypes PtngIdent::checkFile(const QString &file){
    PtngEnums::SupportedInputTypes ret = PtngEnums::NUM_SUPPORTED_INPUT_TYPES;
    QDomDocument doc("input");
    QFile f(file);
    if( !f.open(QIODevice::ReadOnly) ){
        return(ret);
    }
    if(doc.setContent(&f)){
        f.close();
        ret = checkXmlFile(file);
        return(ret);
    }
    else{
        f.close();
        ret = checkTextFile(file);
        return(ret);
    }
    return(ret);
}

PtngEnums::SupportedInputTypes PtngIdent::checkXmlFile(const QString &file){
    PtngEnums::SupportedInputTypes ret = PtngEnums::NUM_SUPPORTED_INPUT_TYPES;
    QScopedPointer<QDomDocument> doc(new QDomDocument("input"));
    QScopedPointer<QFile> f(new QFile(file));
    f->open(QIODevice::ReadOnly);
    doc->setContent(f.data());
    QDomElement rootElem = doc->documentElement();

    if( rootElem.hasAttribute("scanner") ){
        if( rootElem.attribute("scanner").toLower() == "nmap" ){
            if( rootElem.attribute("args").contains("-sS") ||
                    rootElem.attribute("args").contains("-sU") ||
                    rootElem.attribute("args").contains("-sT") ||
                    rootElem.attribute("args").contains("-sA") ||
                    rootElem.attribute("args").contains("-sF") ||
                    rootElem.attribute("args").contains("-sN") ||
                    rootElem.attribute("args").contains("-sX")
                    ){
                ret = PtngEnums::NMAP;
                return(ret);
            }
            else if( rootElem.attribute("args").contains("dns-zone-transfer") ){
                ret = PtngEnums::AXFR_NMAP;
                return(ret);
            }
            else{
                return(ret);
            }
        }
        else if(rootElem.attribute("scanner").toLower() == "masscan"){
            ret = PtngEnums::MASSCAN;
            return(ret);
        }
        else{
            return(ret);
        }
    }
    else if( rootElem.tagName().toLower() == "directedgraph" ){
        ret = PtngEnums::DGML;
        return(ret);
    }
    else if( rootElem.tagName().toLower() == "records" ){
        ret = PtngEnums::AXFR_DNS_RECON;
        return(ret);
    }
    else if( rootElem.tagName().toLower() == "niktoscans" ){
        ret = PtngEnums::NIKTO;
        return(ret);
    }
    else if( rootElem.tagName() == "report" && rootElem.attribute("type").toLower() == "security" ){
        ret = PtngEnums::WAPITI;
        return(ret);
    }
    else if( rootElem.tagName() == "report" &&
             rootElem.hasAttribute("id") &&
             rootElem.hasAttribute("format_id") &&
             rootElem.hasAttribute("content_type")
             ){
        ret = PtngEnums::OPENVAS;
        return(ret);
    }
    else if( rootElem.tagName() == "document" && rootElem.attribute("title").toLower() == "sslscan results" ){
        ret = PtngEnums::SSL_SCAN;
        return(ret);
    }
    else if( rootElem.tagName().toLower().contains(("metasploit"))){
        ret = PtngEnums::METASPLOIT;
        return(ret);
    }
    else if( rootElem.tagName().toLower() == "owaspzapreport" ){
        ret = PtngEnums::OWASP_ZAP;
        return(ret);
    }
    else if( rootElem.tagName().toLower().startsWith("nessusclientdata" )){
        ret = PtngEnums::NESSUS;
        return(ret);
    }
    return(ret);
}

PtngEnums::SupportedInputTypes PtngIdent::checkTextFile(const QString &file){
    PtngEnums::SupportedInputTypes ret = PtngEnums::NUM_SUPPORTED_INPUT_TYPES;
    QScopedPointer<QFile> f(new QFile(file));
    f->open(QIODevice::ReadOnly);
    QString firstLine ,secondLine;
    QStringList lines;
    QTextStream stream(f.data());
    while( !stream.atEnd() ){
        QString line = stream.readLine();
        // TODO are there anymore files that may or may not have additional cruft at the beginning of the file?
        if( line.isEmpty() || line.startsWith("> ls") ){ // for dig and nslookup
            continue;
        }
        lines.append(line);
    }
    if( lines.at(0).startsWith("ns:",Qt::CaseInsensitive) && lines.at(1).toLower().startsWith("soa:") ){
        ret = PtngEnums::AXFR_FIERCE;
        return(ret);
    }
    else if( lines.at(0).startsWith("trying",Qt::CaseInsensitive) && lines.at(1).toLower().startsWith("using domain server") ){
        ret = PtngEnums::AXFR_HOST;
        return(ret);
    }
    else if( lines.at(0).startsWith("server:",Qt::CaseInsensitive) && lines.at(1).toLower().startsWith("address:") ){
        ret =PtngEnums:: PtngEnums::AXFR_NS_LIN;
        return(ret);
    }
    else if( lines.at(0).startsWith("[[",Qt::CaseInsensitive) && lines.at(0).toLower().endsWith("]]") ){
        ret = PtngEnums::AXFR_NS_WIN;
        return(ret);
    }
    else if( lines.at(0).startsWith("; <<>> dig",Qt::CaseInsensitive) && lines.at(0).toLower().endsWith("axfr") ){
        ret = PtngEnums::AXFR_DIG;
        return(ret);
    }
    else if( lines.at(0).startsWith("[nessus_hsh]",Qt::CaseInsensitive)){
        ret = PtngEnums::NESSUS_HSH;
        return(ret);
    }
    else if( lines.at(0).count(QLatin1Char(':')) == 6 ){
        ret = PtngEnums::HASHES;
        return(ret);
    }
    else if( lines.at(0).startsWith("interface",Qt::CaseInsensitive) &&  lines.at(1).toLower().startsWith("starting arp-scan")){
        ret = PtngEnums::ARPSCAN;
        return(ret);
    }
    else if( lines.at(0).count(QLatin1Char(':')) == 9 ){
        QString ipAddress = lines.at(0).split(":").at(0);
        if( ipAddress.count(QLatin1Char('.')) == 3 ){
        ret = PtngEnums::NBTSCAN;
        return(ret);
        }
    }
    return(ret);
}

/*
   Unit tests
  */

#ifdef QT_DEBUG
void PtngIdent::checkFile_data()
{
    // qInfo() << "Loading test data for checkFile";
    QTest::addColumn<QString>("tool");
    QTest::addColumn<QString>("file");
    QTest::addRow("nmap") << "nmap" << "inf/discworld_inf_tcp_fast_01.xml";
    QTest::addRow("Nessus") << "nessus" << "inf/discworld_nessus_01.nessus";
    QTest::addRow("dnsrecon") << "dnsrecon" << "inf/discworld_axfr_dnsrecon_01.xml";
    QTest::addRow("host") << "host" << "inf/discworld_axfr_host_01.txt";
    QTest::addRow("nmap_axfr") << "nmap AXFR" << "inf/discworld_axfr_nmap_01.xml";
    QTest::addRow("dig") << "dig" << "inf/discworld_axfr_dig_01.txt";
    QTest::addRow("nslookup windows") << "nslookup (Windows)" << "inf/discworld_axfr_nswin_01.txt";
    QTest::addRow("nslookup linux") << "nslookup (Linux)" << "inf/sample_strikeforce_nslookup_lin_axfr_01.txt";
    QTest::addRow("fierce") << "fierce" << "inf/discworld_axfr_fierce_01.txt";
    QTest::addRow("arpscan") << "arpscan" << "inf/discworld_arp-scan_01.txt";
    QTest::addRow("nbtscan") << "nbtscan" << "inf/discworld_nbtscan_01.txt";
    QTest::addRow("sslscan") << "sslscan" << "inf/sample_sslscan.xml";
    QTest::addRow("hashes") << "hashes" << "inf/sample_hashes.txt";
    QTest::addRow("DGML") << "dgml" << "inf/discworld_inf_tcp_fast_01.dgml";
    QTest::addRow("Metasploit Framework") << "MSF" << "inf/sample_msf_01.xml";
    QTest::addRow("OWASP ZAP") << "OWASP ZAP" << "web/zap/muillidae_01.xml";
    QTest::addRow("Unsupported") << "unsupported" << "unsupported.file";
}

void PtngIdent::checkFile()
{
    QFETCH(QString,tool);
    QFETCH(QString,file);
    if( !QFile::exists(file) ){
        QString message = QString(file % " does not exist. ");
        QFAIL(qPrintable(message));
    }
    PtngEnums::SupportedInputTypes type = PtngIdent::checkFile(file);
    if( tool == "unsupported" ){
        QVERIFY( type == PtngEnums::NUM_SUPPORTED_INPUT_TYPES );
    }
    else{
        QVERIFY( type != PtngEnums::NUM_SUPPORTED_INPUT_TYPES );
    }

}
#endif
} // namespace ptng
