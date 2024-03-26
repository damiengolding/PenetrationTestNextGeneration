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
#include "../inc/Project.hpp"

// ProjectArtefact
ProjectArtefact::ProjectArtefact(QObject *parent)
    : QObject{parent}
{


}

QString ProjectArtefact::getId() const
{
    return Id;
}

void ProjectArtefact::setId(const QString &newId)
{
    if (Id == newId)
        return;
    Id = newId;
    emit IdChanged();
}

QString ProjectArtefact::getOutputFile() const
{
    return outputFile;
}

void ProjectArtefact::setOutputFile(const QString &newOutputFile)
{
    if (outputFile == newOutputFile)
        return;
    outputFile = newOutputFile;
    emit outputFileChanged();
}

QString ProjectArtefact::getSourceFile() const
{
    return sourceFile;
}

void ProjectArtefact::setSourceFile(const QString &newSourceFile)
{
    if (sourceFile == newSourceFile)
        return;
    sourceFile = newSourceFile;
    emit sourceFileChanged();
}

QString ProjectArtefact::getSourceTool() const
{
    return sourceTool;
}

void ProjectArtefact::setSourceTool(const QString &newSourceTool)
{
    if (sourceTool == newSourceTool)
        return;
    sourceTool = newSourceTool;
    emit sourceToolChanged();
}

// Project
Project::Project(const QString &inputFileName, QObject *parent)
    : projectFile(inputFileName), QObject{parent}
{
}

Project::~Project()
{
    database.commit();
    database.close();
}

bool Project::load()
{
    if( !QFile::exists(projectFile)){
        qCritical() << "Database file"<<projectFile<<"does not exist";
        return(false);
    }
    // Create the database and run the table sql
    initDatabase();
    return(true);
}

bool Project::create()
{
#ifdef QT_DEBUG
    qDebug() << "Creating project at:"<<projectFile;
#endif
    if( QFile::exists(projectFile)){
        qCritical() << "Database file"<<projectFile<<"already exists";
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
    // database = QSqlDatabase::addDatabase("QSQLITE");
    // database.setDatabaseName(projectFile);
    initDatabase();
    if( !database.open() ){
        qCritical() << "Database file"<<projectFile<< "could not be opened.\nError:\n"<<database.lastError().databaseText();
        return(false);
    }

    for( auto command : commands ){
        QSqlQuery query(command);
        if( !query.exec() ){
            qCritical() << "Create table query failed:"<<query.lastError().databaseText();
        }
    }

    QFileInfo fi(projectFile);
    QString watchDirs = watchDirectories.join(";");
    if( watchDirs.isEmpty() ){
        watchDirs = fi.absolutePath();
    }
#ifdef QT_DEBUG
    qDebug() << "DB open:"<<database.isOpen();
    qDebug() << "DB valid:"<<database.isValid();
#endif
    QSqlQuery project;
    project.prepare("INSERT INTO project (display_name,project_id,created_date,project_title,working_directory,watch_directories)"
                    " VALUES(:dname,:id,:created,:title,:wd,:watch)");
    project.bindValue(":dname",projectDisplayName);
    project.bindValue(":id",projectId);
    project.bindValue(":created",QDateTime::currentDateTime());
    project.bindValue(":title",projectDescription);
    project.bindValue(":wd",fi.absolutePath());
    project.bindValue(":watch",watchDirs);

    if( !project.exec() ){
        qCritical() << "Insert project details failed:"<<project.lastError().databaseText();
    }
#ifdef QT_DEBUG
    qDebug() << "Query:"<<project.executedQuery();
#endif
    return(true);
}

QSqlTableModel *Project::getProjectTable()
{
    QSqlTableModel* table = new QSqlTableModel();
    table->setTable("project");
    table->setEditStrategy(QSqlTableModel::OnManualSubmit);
    table->select();
    table->setHeaderData(0, Qt::Horizontal, "Name");
    table->setHeaderData(1, Qt::Horizontal, "Id");
    table->setHeaderData(2, Qt::Horizontal, "Created");
    table->setHeaderData(3, Qt::Horizontal, "Title");
    table->setHeaderData(4, Qt::Horizontal, "Working directory");
    table->setHeaderData(5, Qt::Horizontal, "Watch directories");
    return(table);
}

QSqlTableModel *Project::getArtefactsTable()
{
    QSqlTableModel* table = new QSqlTableModel();
    table->setTable("artefacts");
    table->setEditStrategy(QSqlTableModel::OnManualSubmit);
    table->select();
    table->setHeaderData(0, Qt::Horizontal, "Name");
    table->setHeaderData(1, Qt::Horizontal, "Id");
    table->setHeaderData(2, Qt::Horizontal, "Source tool");
    table->setHeaderData(3, Qt::Horizontal, "Source file");
    table->setHeaderData(4, Qt::Horizontal, "Output file");
    return(table);
}

QString Project::getProjectName() const
{
    return projectDisplayName;
}

void Project::setProjectName(const QString &newProjectName)
{
    if (projectDisplayName == newProjectName)
        return;
    projectDisplayName = newProjectName;
    emit projectNameChanged();
}

QString Project::getProjectFile() const
{
    return projectFile;
}

void Project::setProjectFile(const QString &newProjectFile)
{
    if (projectFile == newProjectFile)
        return;
    projectFile = newProjectFile;
    emit projectFileChanged();
}

QString Project::getProjectId() const
{
    return projectId;
}

void Project::setProjectId(const QString &newProjectId)
{
    if (projectId == newProjectId)
        return;
    projectId = newProjectId;
    emit projectIdChanged();
}

QSqlDatabase Project::getDatabase() const
{
    return database;
}

void Project::setDatabase(const QSqlDatabase &newDatabase)
{
    if (database.databaseName() == newDatabase.databaseName())
        return;
    database = newDatabase;
    emit databaseChanged();
}

QStringList Project::getWatchDirectories() const
{
    return watchDirectories;
}

void Project::setWatchDirectories(const QStringList &newWatchDirectories)
{
    if (watchDirectories == newWatchDirectories)
        return;
    watchDirectories = newWatchDirectories;
    emit watchDirectoriesChanged();
}

QString Project::getProjectDescription() const
{
    return projectDescription;
}

void Project::setProjectDescription(const QString &newProjectDescription)
{
    if (projectDescription == newProjectDescription)
        return;
    projectDescription = newProjectDescription;
    emit projectDescriptionChanged();
}

bool Project::initDatabase()
{
#ifdef QT_DEBUG
    qDebug() << "Initializing database";
#endif
    if( database.isOpen() ){
#ifdef QT_DEBUG
        qDebug() << "Database"<< database.databaseName() <<"is open - closing";;
#endif
        database.close();
        // QUERY - Do I need to remove the database?
    }
    else{
#ifdef QT_DEBUG
        qDebug() << "Database is closed";
#endif
    }
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(projectFile);
    if( !database.open() ){
        qCritical() << "Database file"<<projectFile<< "could not be opened.\nError:\n"<<database.lastError().databaseText();
        return(false);
    }
}


