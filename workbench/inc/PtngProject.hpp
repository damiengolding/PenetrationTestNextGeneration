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

#include <QObject>
#include <QtCore/qglobal.h>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomElement>
#include <QDomText>
#include <QDomAttr>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QDateTime>

namespace ptng {

/*!
   \brief The PtngProjectArtefact class
 */
struct PtngProjectArtefact : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString sourceTool READ getSourceTool WRITE setSourceTool NOTIFY sourceToolChanged FINAL)
    Q_PROPERTY(QString sourceFile READ getSourceFile WRITE setSourceFile NOTIFY sourceFileChanged FINAL)
    Q_PROPERTY(QString outputFile READ getOutputFile WRITE setOutputFile NOTIFY outputFileChanged FINAL)
    Q_PROPERTY(QString Id READ getId WRITE setId NOTIFY IdChanged FINAL)

public:
    explicit PtngProjectArtefact(QObject *parent = nullptr);
    QString getSourceTool() const;
    void setSourceTool(const QString &newSourceTool);
    QString getSourceFile() const;
    void setSourceFile(const QString &newSourceFile);
    QString getOutputFile() const;
    void setOutputFile(const QString &newOutputFile);
    QString getId() const;
    void setId(const QString &newId);

signals:
    void sourceToolChanged();
    void sourceFileChanged();
    void outputFileChanged();
    void IdChanged();

private:
    QString sourceTool;
    QString sourceFile;
    QString outputFile;
    QString Id;
};

/*!
   \brief The PtngProject class
 */
class PtngProject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString projectname READ getProjectName WRITE setProjectName NOTIFY projectNameChanged FINAL)
    Q_PROPERTY(QString workingDirectory READ getWorkingDirectory WRITE setWorkingDirectory NOTIFY workingDirectoryChanged FINAL)
    Q_PROPERTY(QDomDocument *domDocument READ getDomDocument WRITE setDomDocument NOTIFY domDocumentChanged FINAL)

public:
    explicit PtngProject(QObject *parent = nullptr);
    void addWatchDirectory(const QString &directory);
    void removeWatchDirectory(const QString &directory);
    void addArtefact(const QString &sourceTool, const QString &sourceFile, const QString &outputFile, const QString &artefactId = "");
    void removeArtefact(const QString &artefactId);

    QString getProjectName() const;
    void setProjectName(const QString &newProjectname);

    QString getWorkingDirectory() const;
    void setWorkingDirectory(const QString &newWorkingDirectory);

    QDomDocument *getDomDocument() const;
    void setDomDocument(QDomDocument *newDomDocument);

private:
    QString projectName;
    QString workingDirectory;
    QDomDocument *domDocument;
    QStringList watchDirectories;
    QList<PtngProjectArtefact*> artefacts;

signals:
    void projectNameChanged();
    void workingDirectoryChanged();
    void domDocumentChanged();
};

} // namespace ptng

