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
#include "../inc/PtngProject.hpp"

namespace ptng {

// PtngProjectArtefact
PtngProjectArtefact::PtngProjectArtefact(QObject *parent)
    : QObject{parent}
{


}

QString PtngProjectArtefact::getId() const
{
    return Id;
}

void PtngProjectArtefact::setId(const QString &newId)
{
    if (Id == newId)
        return;
    Id = newId;
    emit IdChanged();
}

QString PtngProjectArtefact::getOutputFile() const
{
    return outputFile;
}

void PtngProjectArtefact::setOutputFile(const QString &newOutputFile)
{
    if (outputFile == newOutputFile)
        return;
    outputFile = newOutputFile;
    emit outputFileChanged();
}

QString PtngProjectArtefact::getSourceFile() const
{
    return sourceFile;
}

void PtngProjectArtefact::setSourceFile(const QString &newSourceFile)
{
    if (sourceFile == newSourceFile)
        return;
    sourceFile = newSourceFile;
    emit sourceFileChanged();
}

QString PtngProjectArtefact::getSourceTool() const
{
    return sourceTool;
}

void PtngProjectArtefact::setSourceTool(const QString &newSourceTool)
{
    if (sourceTool == newSourceTool)
        return;
    sourceTool = newSourceTool;
    emit sourceToolChanged();
}

// PtngProject
PtngProject::PtngProject(QObject *parent)
    : QObject{parent}
{

    domDocument = new QDomDocument("");
    QDomElement root = domDocument->createElement("PtngProject");
    QDomElement artefacts = domDocument->createElement("Artefacts");
    QDomElement project = domDocument->createElement("Project");
    root.setAttribute("Title","Ptng Workbench Project");
    root.setAttribute("CreatedTime",QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()));
    root.appendChild(artefacts);
    root.appendChild(project);
    domDocument->appendChild(root);
    // qInfo() << "[info] Project XML:"<<domDocument->toString();
}

void PtngProject::addWatchDirectory(const QString &directory)
{
    if( watchDirectories.contains(directory) ){
        return;
    }
    watchDirectories.append(directory);
    QDomElement project = domDocument->documentElement().firstChildElement("Project");
    QDomElement wd = domDocument->createElement("WatchDirectory");
    wd.setAttribute("Value",directory);
    project.appendChild(wd);
}

void PtngProject::removeWatchDirectory(const QString &directory)
{
    if( !watchDirectories.contains(directory) ){
        return;
    }
    watchDirectories.removeAll(directory);
    QDomNode projectNode = domDocument->documentElement().firstChildElement("Project");
    QDomElement projectElem = projectNode.toElement();
    QDomNodeList nl = projectElem.elementsByTagName("WatchDirectory");
    for( int i=0; i<nl.count();++i ){
        QDomNode node = nl.at(i);
        QDomElement elem = node.toElement();
        if( elem.attribute("Value") == directory ){
            projectNode.removeChild(node);
        }
    }
}

void PtngProject::addArtefact(const QString &sourceTool, const QString &sourceFile, const QString &outputFile, const QString &displayName, const QString &artefactId)
{
    QString id = artefactId;
    if( id.isEmpty() ){
        id = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
    }

    QDomNode artefactsNode = domDocument->documentElement().firstChildElement("Artefacts");
    QDomElement artefactsElem = artefactsNode.toElement();
    QDomElement newArtefact = domDocument->createElement("Artefact");
    newArtefact.setAttribute("SourceTool",sourceTool);
    newArtefact.setAttribute("SourceFile",sourceFile);
    newArtefact.setAttribute("OutputFile",outputFile);
    newArtefact.setAttribute("Id",id);
    newArtefact.setAttribute("DisplayName",displayName);
    artefactsElem.appendChild(newArtefact);
}

void PtngProject::removeArtefact(const QString &artefactId)
{
    bool found = false;
    for( auto artefact : artefacts ){
        if( artefact->getId() == artefactId ){
            artefacts.removeAll(artefact);
            found = true;
            break;
        }
    }
    QDomNode artefactsNode = domDocument->documentElement().firstChildElement("Artefacts");
    QDomElement artefactsElem = artefactsNode.toElement();
    QDomNodeList nl = artefactsElem.elementsByTagName("Artefact");
    for( int i = 0;i<nl.count();++i ){
        QDomNode node = nl.at(i);
        QDomElement elem = node.toElement();
        if( elem.attribute("Id") == artefactId ){
            artefactsNode.removeChild(node);
        }
    }
}

QString PtngProject::getProjectName() const
{
    return projectName;
}

void PtngProject::setProjectName(const QString &newProjectName)
{
    if (projectName == newProjectName)
        return;
    projectName = newProjectName;
    QDomElement root = domDocument->documentElement();
    root.setAttribute("Title",projectName);
    emit projectNameChanged();
}

QString PtngProject::getWorkingDirectory() const
{
    return workingDirectory;
}

void PtngProject::setWorkingDirectory(const QString &newWorkingDirectory)
{
    if (workingDirectory == newWorkingDirectory)
        return;
    QDomNodeList nl = domDocument->elementsByTagName("WorkingDirectory");
    if( nl.count() != 1 ){
        qInfo() << "[info] I'm confused - there should only be one WorkingDirectory node:"<<nl.count();
        return;
    }
    QDomElement elem = nl.at(0).toElement();
    if( elem.isNull() ){
        qWarning() << "[warning] Cannot convert WorkingDirectory node to element";
        return;
    }
    elem.setAttribute("Value",newWorkingDirectory);
    workingDirectory = newWorkingDirectory;
    emit workingDirectoryChanged();
}

QDomDocument *PtngProject::getDomDocument() const
{
    return domDocument;
}

void PtngProject::setDomDocument(QDomDocument *newDomDocument)
{
    if (domDocument == newDomDocument)
        return;
    domDocument = newDomDocument;
    emit domDocumentChanged();
}

} // namespace ptng
