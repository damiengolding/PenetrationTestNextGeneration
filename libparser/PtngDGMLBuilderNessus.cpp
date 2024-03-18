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

#include "inc/PtngDGMLBuilder.hpp"
namespace ptng{

PtngDGMLBuilder& PtngDGMLBuilder::createFromNessus(const QString &nessusFile, const QString &severityFilters,const QString &subnetFilters){
    dgmlObject->root.setAttribute("Title","NessusDGML");
    QStringList severities = severityFilters.split(",", Qt::SkipEmptyParts);
    QStringList subnets = subnetFilters.split(",", Qt::SkipEmptyParts);
    QMap<QString,QString> emptyMap;
    // Root node
    addNode("0","Nessus scan",emptyMap);

    return(*this);
}


} // namespace ptng


// int id = 1;
// for( auto issue : issues ){
//     QString address = issue.ipAddress;
//     QString riskFactor = issue.riskFactor.toUpper();
//     if( isInSubnetFilter(subnets,address) && isInSeverityFilter(severities, riskFactor) ){
//         QString label=address % "\n" % riskFactor;
//         QMap<QString,QString> attributes;
//         attributes.insert("PluginId",issue.pluginId);
//         attributes.insert("PluginName",issue.pluginName);
//         attributes.insert("PluginFamily",issue.pluginFamily);
//         attributes.insert("PluginType",issue.pluginType);
//         attributes.insert("Protocol", issue.protocol);
//         attributes.insert("PortNumber",QString::number(issue.portNumber));
//         attributes.insert("ServiceName",issue.serviceName);
//         attributes.insert("RiskFactor",issue.riskFactor);
//         attributes.insert("Display",issue.display);
//         attributes.insert("Synopsis",issue.synopsis);
//         attributes.insert("Solution",issue.solution);
//         addNode(QString::number(id), label,attributes);
//         addLink("0",QString::number(id));
//         ++id;
//     }
// }
// qInfo() << "[info] Nessus issue count (filtered):"<<id - 1;
