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
PtngProject::PtngProject(const QString &inputFileName, QObject *parent)
    : fileName(inputFileName), QObject{parent}
{
    isDirty  = false;
}

void PtngProject::addWatchDirectory(const QString &directory)
{
    if( watchDirectories.contains(directory) ){
        return;
    }
    isDirty = true;
    watchDirectories.append(directory);
}

void PtngProject::removeWatchDirectory(const QString &directory)
{
    if( !watchDirectories.contains(directory) )
        return;
    isDirty = true;
    watchDirectories.removeAll(directory);
}

void PtngProject::addArtefact(const QString &sourceTool, const QString &sourceFile, const QString &outputFile, const QString &displayName, const QString &artefactId)
{
    QString id = artefactId;
    if( id.isEmpty() ){
        id = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
    }
    isDirty = true;
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
    isDirty = true;
}

bool PtngProject::load()
{
    if( !QFile::exists(fileName)){
        qWarning() << "[warning] Database file"<<fileName<<"does not exist";
        return(false);
    }
    return(true);
}

bool PtngProject::create()
{
    qInfo() << "[info] Creating project at:"<<fileName;
    if( QFile::exists(fileName)){
        qWarning() << "[warning] Database file"<<fileName<<"already exists";
        return(false);
    }
    // Get the table create sql
    QSettings s;
    QString createScript = s.value("configDirectory").toString() % QDir::separator() % "new_project.txt";
    QScopedPointer<QFile> file(new QFile(createScript));
    QStringList commands;
    if( file->open(QIODevice::ReadOnly) ){
        while(!file->atEnd())
            commands<<file->readLine();
    }
    else{
            commands<<artefactsTableSql<<projectTableSql;
    }

    // Create the database and run the table sql
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    if( !db.open() ){
        qWarning() << "[warning] Could not open database"<<fileName<< "Error:\n"<<db.lastError().databaseText();
        return(false);
    }

    for( auto command : commands ){
        QSqlQuery query(command);
        if( !query.exec() ){
            qWarning() << "[warning] Create table query failed:"<<query.lastError().databaseText();
        }
    }
    database = db;
    return(true);
}

QSqlTableModel *PtngProject::getProjectTable()
{

    return(nullptr);
}

QSqlTableModel *PtngProject::getArtefactsTable()
{

    return(nullptr);
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
    isDirty = true;
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
    isDirty = true;
    workingDirectory = newWorkingDirectory;
    emit workingDirectoryChanged();
}

bool PtngProject::getIsDirty() const
{
    return isDirty;
}

void PtngProject::setIsDirty(bool newIsDirty)
{
    if (isDirty == newIsDirty)
        return;
    isDirty = newIsDirty;
    emit isDirtyChanged();
}

QString PtngProject::getFileName() const
{
    return fileName;
}

void PtngProject::setFileName(const QString &newFileName)
{
    if (fileName == newFileName)
        return;
    fileName = newFileName;
    emit fileNameChanged();
}

QSqlDatabase PtngProject::getDatabase() const
{
    return database;
}

void PtngProject::setDatabase(const QSqlDatabase &newDatabase)
{
    if (database.databaseName() == newDatabase.databaseName())
        return;
    database = newDatabase;
    emit databaseChanged();
}

} // namespace ptng
