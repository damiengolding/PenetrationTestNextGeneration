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

namespace ptng {

/*!
    \brief The PtngIssue struct

    Data associated with a vulnerability; modelled closely to Nessus reporting.
*/
LIBPARSER_EXPORT struct PtngIssue{
public:
    QString ipAddress="";
    int portNumber = 0;
    int severity = 0;
    QString serviceName = "";
    // PtngEnums::Protocol protocol = PtngEnums::NUM_PROTOCOLS;
    QString protocol = "";
    QString pluginId = ""; // Report -> ReportHost -> ReportItem -> pluginID (attr)
    QString pluginName = ""; // Report -> ReportHost -> ReportItem -> pluginName (attr)
    QString pluginFamily = ""; // Report -> ReportHost -> ReportItem -> pluginFamily (attr)
    QString pluginType = ""; // Report -> ReportHost -> ReportItem -> plugin_type
    QString display = ""; // Report -> ReportHost -> ReportItem -> plugin_name
    QString description = ""; // Report -> ReportHost -> ReportItem -> description
    QString synopsis = ""; // Report -> ReportHost -> ReportItem -> synopsis
    QString solution = ""; // Report -> ReportHost -> ReportItem -> solution
    QString seeAlso = "";  // Report -> ReportHost -> ReportItem -> xref
    QString riskFactor = "";  // Report -> ReportHost -> ReportItem -> risk_factor
};

/*!
    \brief The PtngPort struct

    Data associated with a vulnerability; modelled closely to nmap reporting.
*/
LIBPARSER_EXPORT struct PtngPort{
public:
    PtngEnums::Protocol protocol = PtngEnums::NUM_PROTOCOLS;
    int portNumber = 0;
    QString state = "";
    QString reason = "";
    QString serviceName = "";
    QString serviceProduct = "";
    QString serviceVersion = "";
    QString idMethod = "";
    QStringList serviceCPE;
    QMap<QString,QString> portScripts;
};

} // end of namespace
