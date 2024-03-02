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
#include "PtngHostBuilder.hpp"

namespace ptng {
/*
    PtngHost
*/

PtngHost::PtngHost(QObject *parent)
    : QObject{parent}
{}

/*
    PtngHostBuilder
*/

PtngHostBuilder::PtngHostBuilder(QObject *parent)
    : QObject{parent}
{

}

PtngHostBuilder& PtngHostBuilder::setSeverity( PtngEnums::IssueSeverity sev )
{
    host.highestSeverity = sev;
    return(*this);
}

PtngHostBuilder& PtngHostBuilder::setIsAXFR(bool isAxfr ){
    host.inAxfr = isAxfr;
    return(*this);
}

PtngHostBuilder& PtngHostBuilder::addScript( const QString &id, const QString &output ){

    return(*this);
}

PtngHostBuilder& PtngHostBuilder::addPortSpec(const PtngPort &portSpec ){
    host.portSpecs.append(portSpec);
    return(*this);
}

PtngHostBuilder &PtngHostBuilder::addNmapAXFRXmlNode(const QDomNode &node)
{

    return(*this);
}

PtngHostBuilder &PtngHostBuilder::addNmapScanXmlNode(const QDomNode &node)
{
    QDomElement e = node.toElement();
    if( e.isNull() ){
        qWarning() << "[warning] Cannot convert the supplied QDomNode to a QDomElement";
        return(*this);
    }
    QDomNodeList nodes = e.childNodes();

    for( int i = 0; i < nodes.length(); ++i){
        QDomNode node = nodes.at(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }

        if( elem.tagName() == "address" ){
            QString t = elem.attribute("addrtype");
            if( t.toLower() == "ipv4" ){
                host.ipAddress = elem.attribute("addr");
                host.addrType = elem.attribute("addrtype");
            }
            else if( t.toLower() == "mac" ){
                host.macAddress = elem.attribute("addr");
                host.macVendor = elem.attribute("vendor");
            }

        }
        else if( elem.tagName() == "status" ){
            host.hostState = elem.attribute("state");
            host.hostStateReason = elem.attribute("reason");
        }
        else if( elem.tagName() == "os" ){
            // qInfo() << "[info] in the os node for:"<<host.getIpAddress();
            host.osName = elem.attribute("name");
            QDomNodeList osmatch = elem.elementsByTagName("osmatch");
            // qInfo() << "[info] osmatch:"<<osmatch.length();
            if( osmatch.length() > 0){
                for( int i = 0;i<osmatch.length();++i ){
                    QDomElement e = osmatch.at(i).toElement();
                    if( !e.isNull()){
                        QDomNodeList osclass = e.elementsByTagName("osclass");
                        // qInfo() << "[info] osclass:"<<osclass.length();
                        if( osclass.isEmpty() ){
                            continue;
                        }
                        for( int j = 0;j<osclass.length();++j ){
                            QDomElement e = osclass.at(j).toElement();
                            if( !e.isNull()){
                                QString cpe = e.firstChildElement().text();
                                // qInfo() << "[info] OS CPE text:"<<cpe;
                            }
                        }
                    }
                }
            }
        }
        else if( elem.tagName() == "distance" ){
            host.distance = elem.attribute("value").toInt();
        }
        else if( elem.tagName() == "hostscript" ){
            QDomNodeList scripts = elem.elementsByTagName("script");
            for( int i =0; i < scripts.length(); ++i ){
                QDomElement elem = scripts.at(i).toElement();
                if( elem.isNull() ){
                    continue;
                }
                QString id = elem.attribute("id");
                QString output = elem.attribute("output");
                host.hostScripts.insert( id,  output);

                if( id == "ip-geolocation-ipinfodb"  ){
                    qInfo() << "[info] ip-geolocation-ipinfodb";
                    host.geoLocation = true;
                    QStringList ol = output.split("\n",Qt::SkipEmptyParts);
                    qInfo() << "[info] ip-geolocation-ipinfodb split:"<<ol.length();
                    for( int i = 0; i < ol.length(); ++i ){
                        QString o = ol.at(i).trimmed();
                      if( o.toLower().startsWith("coordinates") ){
                          QStringList coords = o.split(":");
                          QString ll = coords.at(1).trimmed();
                          QStringList lll = ll.split(",");
                          host.latitude = lll.at(0);
                          host.longitude = lll.at(1);
                          qInfo() << "[info] Latitude:"<<host.latitude<< "Longitude:"<< host.longitude;
                      }
                      else if( o.toLower().startsWith("city") ){
                           QStringList cy = o.split(":");
                           host.city = cy.at(1).trimmed();
                           qInfo() << "[info] City:"<<host.city;
                      }
                    }
                }
                if( id == "ip-forwarding"  ){
                    qInfo() << "[info] ip-forwarding";
                    host.gateway = true;
                }
            }
        }
    }

    return(*this);
}

} // namespace ptng
