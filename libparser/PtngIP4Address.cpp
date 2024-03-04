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
#include "inc\PtngIP4Address.hpp"

namespace ptng {

PtngIP4Address::PtngIP4Address(QObject *parent)
    : QObject{parent}
{
}

bool PtngIP4Address::isValidIpV4Address(const QString &address)
{
    QStringList parts = address.split(".");
    for(auto part : parts ){
        bool ok = false;
        int p = part.toInt(&ok);
        if(!ok){
            return(false);
        }
        if( p < 0 || p > 255 ){
            return(false);
        }
    }
    return(true);
}

bool PtngIP4Address::isReservedAddress(const QString &address)
{
    if( !isValidIpV4Address(address) ){
        return(false);
    }
    QStringList strParts = address.split(".");
    QList<int> intParts;

    for( auto part : strParts ){
        bool ok = false;
        int p = part.toInt(&ok);
        if(!ok){
            return(false);
        }
        intParts.append(p);
    }

    if( intParts.at(0) == 10 ){
        return(true);
    }
    else if( intParts.at(0) == 127 ){
        return(true);
    }
    else if( intParts.at(0) == 192 && intParts.at(1) == 168 ){
        return(true);
    }
    else if( intParts.at(0) == 172 && (intParts.at(1) >= 16 && intParts.at(1) <= 31) ){
        return(true);
    }
    return(false);
}

QList<int> PtngIP4Address::parseAddressInt(const QString &address)
{
    QList<int> ret;
    if( !isValidIpV4Address(address) ){
        return(ret);
    }
    QStringList parts = address.split(".");
    for(auto part : parts ){
        bool ok = false;
        int p = part.toInt(&ok);
        if(!ok){
            return(ret);
        }
        ret.append(p);
    }
    return(ret);
}

QStringList PtngIP4Address::parseAddressString(const QString &address)
{
    QStringList ret;
    if( !isValidIpV4Address(address) ){
        return(ret);
    }
    QStringList parts = address.split(".");
    for(auto part : parts ){
        ret.append(part);
    }
    return(ret);
}

} // namespace ptng
