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
#include "PtngHostBuilder.hpp"
#include "PtngInputParser.hpp"
#include "PtngIP4Address.hpp"

#include <QObject>
#include <QMetaEnum>

namespace ptng {
/*!
   \brief The PtngDGML class
 */
class LIBPARSER_EXPORT PtngDGML : public QObject
{
    Q_OBJECT
    friend class PtngDGMLBuilder;
protected:
    /*!
       \brief PtngDGML
       \param parent

       Stores a DGML object based initially on Microsoft's DGML specification for Visual Studio.

       This class cannot be used directly, use PtngDGMLBuilder to create and modify  the object.

       Builds and manages a network DGML object; the object is initialsed with a skeleton based on Microsoft's DGML specification for Visual Studio.
       See the Microsoft DGML guides for further information:
                <a href="https://learn.microsoft.com/en-us/visualstudio/modeling/directed-graph-markup-language-dgml-reference?view=vs-2022">Directed Graph Markup Language (DGML) reference</a>
                <a href="https://github.com/MicrosoftDocs/visualstudio-docs/blob/main/docs/modeling/directed-graph-markup-language-dgml-reference.md">Git hub version (more detailed)</a>
                <a href="https://github.com/MicrosoftDocs/visualstudio-docs/blob/main/docs/modeling/customize-code-maps-by-editing-the-dgml-files.md#OrganizeNodes">Cusomizations, including .expanded/collapsed groups</a>

       \see PtngDGMLBuilder
     */
    explicit PtngDGML(QObject *parent = nullptr);
signals:

protected: // Members
    QDomDocument *doc;
    QDomElement root;

protected: // Accessors and mutators

private:
    void createXml();
};

/*!
   \brief The PtngDGMLBuilder class
 */
class LIBPARSER_EXPORT PtngDGMLBuilder : public QObject
{
    Q_OBJECT
public:
    /*!
       \brief PtngDGMLBuilder
       \param parent

       Builds and manages a network DGML object; the object is initialsed with a skeleton based on Microsoft's DGML specification for Visual Studio.
       See the Microsoft DGML guides for further informatio:
                <a href="https://learn.microsoft.com/en-us/visualstudio/modeling/directed-graph-markup-language-dgml-reference?view=vs-2022">Directed Graph Markup Language (DGML) reference</a>
                <a href="https://github.com/MicrosoftDocs/visualstudio-docs/blob/main/docs/modeling/directed-graph-markup-language-dgml-reference.md">Git hub version (more detailed)</a>
                <a href="https://github.com/MicrosoftDocs/visualstudio-docs/blob/main/docs/modeling/customize-code-maps-by-editing-the-dgml-files.md#OrganizeNodes">Cusomizations, including .expanded/collapsed groups</a>
     */
    explicit PtngDGMLBuilder(QObject *parent = nullptr);

public: // Creational - the user can create a DGML string from an arbitrary source;
    PtngDGMLBuilder& addNode(const QString &id, const QString &label = "", const QMap<QString,QString> &attributes = QMap<QString,QString>());
    PtngDGMLBuilder& addLink(const QString &source, const QString &target, const QString &label = "", const QMap<QString,QString> &attributes = QMap<QString,QString>());
    PtngDGMLBuilder& addCategory(const QString &id,  const QString &label, const QMap<QString,QString> &attributes);
    PtngDGMLBuilder& addProperty(const QString &id, const QString &dataType, const QString &label, const QMap<QString,QString> &attributes);
    PtngDGMLBuilder& addPath(const QString &id, const QString &label, const QString &dataType, bool isReference, const QMap<QString,QString> &attributes);

public: // Creational - from known source types, e.g. nmap QList<PtngHostBuilder*> and QMap<QString,QString>
    PtngDGMLBuilder& createSimple(const QMap<QString,QString> &hosts, const QString &subnetFilters = "", bool addLabels = true);
    PtngDGMLBuilder& createFromNmap(QList<PtngHostBuilder*> builders, const QString &nessusFile = "", const QString &zoneFile = "", const QString &subnetFilters = "", bool addLabels = true);
    PtngDGMLBuilder& createFromNessus(const QString &nessusFile, const QString &severityFilters = "", const QString &subnetFilters = "");

    PtngDGMLBuilder& addNessusSeverityCount(QList<PtngHostBuilder*> builders, QList<PtngIssue> issues);

private: // Helper functions
    QMap<QString,QString> getAttributes(PtngHost *host);
    QList<PtngHostBuilder*> setHighestSeverity(QList<PtngHostBuilder*> builderList);
    bool isInSubnetFilter(const QStringList &subnetFilters, const QString &testString);
    bool isInSeverityFilter(const QStringList &criticalityFilters, const QString &testString);
    QMultiMap<QString,int> getSeverityCount( const QString &address,  QList<PtngIssue> issues);

public: // Other accessors/mutators
    QString toString(int indent = 1);

signals: // Q_PROPERTY NOTIFY entries

public: //  Q_PROPERTY READ WRITE entries,. Also, class accessors and mutators

public: // Q_PROPERTY declarations

private: // Members
    PtngDGML *dgmlObject;
};

} // namespace ptng

