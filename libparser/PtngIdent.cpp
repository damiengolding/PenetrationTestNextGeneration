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
#include "PtngIdent.hpp"

namespace ptng {

PtngIdent::PtngIdent(QObject *parent)
    : QObject{parent}
{}

PtngEnums::SupportedInputTypes PtngIdent::checkFile(const QString &file){
    PtngEnums::SupportedInputTypes ret = PtngEnums::NUM_SUPPORTED_INPUT_TYPES;
    QDomDocument doc("input");
    QFile f(file);
    if( !f.open(QIODevice::ReadOnly) ){
        emit typeIdentified(ret);
        return(ret);
    }

    if(doc.setContent(&f)){
        f.close();
        ret = checkXmlFile(file);
        emit typeIdentified(ret);
        return(ret);
    }
    else{
        f.close();
        ret = checkTextFile(file);
        emit typeIdentified(ret);
        return(ret);
    }

    emit typeIdentified(ret);
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
    else if( rootElem.tagName().toLower().contains("nessusclientdata" )){
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
        lines.append(stream.readLine());
    }
    if( lines.at(0).toLower().startsWith("ns:") && lines.at(1).toLower().startsWith("soa:") ){
        ret = PtngEnums::AXFR_FIERCE;
        return(ret);
    }
    else if( lines.at(0).toLower().startsWith("trying") && lines.at(1).toLower().startsWith("using domain server") ){
        ret = PtngEnums::AXFR_HOST;
        return(ret);
    }
    else if( lines.at(0).toLower().startsWith("server:") && lines.at(1).toLower().startsWith("address:") ){
        ret =PtngEnums:: PtngEnums::AXFR_NS_LIN;
        return(ret);
    }
    else if( lines.at(0).toLower().startsWith("[[") && lines.at(0).toLower().endsWith("]]") ){
        ret = PtngEnums::AXFR_NS_WIN;
        return(ret);
    }
    else if( lines.at(0).toLower().startsWith("; << dig") && lines.at(0).toLower().endsWith("axfr") ){
        ret = PtngEnums::AXFR_DIG;
        return(ret);
    }
    else if( lines.at(0).toLower().startsWith("[nessus_hsh]")){
        ret = PtngEnums::NESSUS_HSH;
        return(ret);
    }
    else if( lines.at(0).count(QLatin1Char(':')) == 6 ){
        ret = PtngEnums::HASHES;
        return(ret);
    }
    else if( lines.at(0).toLower().startsWith("interface") &&  lines.at(1).toLower().startsWith("starting arp-scan")){
        ret = PtngEnums::ARPSCAN;
        return(ret);
    }
    else if( lines.at(2).count(QLatin1Char(':')) == 5 ){
        ret = PtngEnums::NBTSCAN;
        return(ret);
    }
    return(ret);
}

} // namespace ptng
