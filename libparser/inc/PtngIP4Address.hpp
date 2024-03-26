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
#pragma once

#include "libparser_global.hpp"
#include "libparser_local.hpp"
#include "PtngEnums.hpp"
#include "PtngHostBuilder.hpp"
#include <QObject>

namespace ptng {

/*!
   \brief The PtngIP4Address class

   Encapsulates the data needed to process an IP v4 address. Built around the data supplied by nmap or a Zone Transfer, including (if available) MAC address,
   geo-location and OS guess.

   It also provides a small number of validation andcomparison functions, as well as conversion to GraphViz dot entry and other formats
 */
class LIBPARSER_EXPORT PtngIP4Address : public QObject
{
    Q_OBJECT
public:
    explicit PtngIP4Address(QObject *parent = nullptr);
    /*!
       \brief isValidIpV4Address
       \param address
       \return bool
     */
    static bool isValidIpV4Address(const QString &address);
    /*!
       \brief isReservedAddress
       \param address
       \return bool
     */
    static bool isReservedAddress(const QString &address);
    /*!
       \brief parseAddressInt
       \param address
       \return QList<int>
     */
    static QList<int> parseAddressInt(const QString &address);
    /*!
       \brief parseAddressString
       \param address
       \return QStringList
     */
    static QStringList parseAddressString(const QString &address);

};

} // namespace ptng

