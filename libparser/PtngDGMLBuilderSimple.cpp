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

PtngDGMLBuilder& PtngDGMLBuilder::createSimple(const QMap<QString, QString> &anPairs, const QString &subnetFilters, bool addLabels)
{
    QStringList aClasses,bClasses,cClasses,leaves;
    QMap<QString,QString> map; // For attributes
    QMap<QString,QString> emptyMap; // For empty attributes
    map.insert("Category","subnet");
    // Create the 'Attack Machine' root node
    addNode("attack_machine","Attack Machine",map);

    // Separate the entries into classes
    QStringList ips;
    QStringList subnets = subnetFilters.split(",",Qt::SkipEmptyParts);

    for(auto [address,name] : anPairs.asKeyValueRange()){
        QStringList tempList;
        QString tempStr;
        QString testStr;
        tempList=address.split(".");
        tempStr = tempList.at(0) + ".*.*.*";
        testStr = tempList.at(0) + ".";
        if( !aClasses.contains(tempStr) ){
            if( subnets.count()>0 ){
                for( auto subnet : subnets ){
                    if( subnet.startsWith(testStr) ){
                        aClasses.append(tempStr);
                        break;
                    }
                }
            }
            else{
                aClasses.append(tempStr);
            }
        }

        tempStr = tempList.at(0) + "." + tempList.at(1) + ".*.*";
        testStr = tempList.at(0) + "."+ tempList.at(1) + ".";
        if( !bClasses.contains(tempStr) ){
            if( subnets.count()>0 ){
                for( auto subnet : subnets ){
                    if( subnet.startsWith(testStr) ){
                        bClasses.append(tempStr);
                        break;
                    }
                }
            }
            else{
                bClasses.append(tempStr);
            }
        }

        tempStr = tempList.at(0)  + "." + tempList.at(1)  + "." + tempList.at(2) + ".*";
        testStr = tempList.at(0) + "." + tempList.at(1) + "." + tempList.at(2) + ".";
        if( !cClasses.contains(tempStr) ){
            if( subnets.count()>0 ){
                for( auto subnet : subnets ){
                    if( subnets.count()>0 ){
                        if( subnet.startsWith(testStr) ){
                            cClasses.append(tempStr);
                            break;
                        }
                    }
                }
            }
            else{
                cClasses.append(tempStr);
            }
        }
        if( !leaves.contains(address) ){
            leaves.append(address);
        }
    }

    // Add the A class nodes and link to the 'Attack Machine'
    for( auto aClass : aClasses){
        addNode(aClass, aClass, map);
        if( addLabels ){
            addLink("attack_machine", aClass, "Attack Machine->" + aClass ,emptyMap);
        }
        else{
            addLink("attack_machine", aClass, "" ,emptyMap);
        }
    }

    // Add the B class nodes and link to the A Class nodes
    for( auto aClass : aClasses){
        QStringList tempList = aClass.split(".");
        QString tempStr = tempList.at(0) + ".";
        for( auto bc : bClasses){
            if( bc.startsWith(tempStr)){
                addNode(bc, bc, map);
                if( addLabels ){
                    addLink(aClass, bc,aClass + "->" + bc,emptyMap);
                }
                else{
                    addLink(aClass, bc,"",emptyMap);
                }
            }
        }
        // Add the C class nodes and link to the B Class nodes
        for( auto bClass : bClasses){
            QStringList tempList = bClass.split(".");
            QString tempStr = tempList.at(0) + "." + tempList.at(1) + ".";
            for( auto cc : cClasses){
                if( cc.startsWith(tempStr)){
                    addNode(cc, cc, map);
                    if( addLabels ){
                        addLink(bClass, cc,bClass + "->" +cc,emptyMap);
                    }
                    else{
                        addLink(bClass, cc,"",emptyMap);
                    }
                }
            }
        }

        // Add the leaf nodes and link to the C Class nodes
        for( auto cClass : cClasses){
            QStringList tempList = cClass.split(".");
            QString tempStr = tempList.at(0) + "." + tempList.at(1) + "." + tempList.at(2) + ".";
            // qInfo() << "[info] tempStr for leaf:"<<tempStr;
            for( auto leaf : leaves){
                // qInfo() << "[info] leaf:"<<leaf;
                if( leaf.startsWith(tempStr)){
                    addNode(leaf, leaf, map);
                    if( addLabels ){
                        addLink(cClass, leaf,cClass + "->" +leaf,emptyMap);
                    }
                    else{
                        addLink(cClass,leaf,"",emptyMap);
                    }
                }
            }
        }

        return(*this);
    }
}

} // namespace ptng
