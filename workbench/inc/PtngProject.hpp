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
#include <QFile>
#include <QTextStream>
#include <QMap>.
#include <QList>
#include <QDebug>
#include <QDateTime>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QDir>

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
    Q_PROPERTY(QString projectName READ getProjectName WRITE setProjectName NOTIFY projectNameChanged FINAL)
    Q_PROPERTY(QString workingDirectory READ getWorkingDirectory WRITE setWorkingDirectory NOTIFY workingDirectoryChanged FINAL)
    Q_PROPERTY(bool isDirty READ getIsDirty WRITE setIsDirty NOTIFY isDirtyChanged FINAL)
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged FINAL)
    Q_PROPERTY(QSqlDatabase database READ getDatabase WRITE setDatabase NOTIFY databaseChanged FINAL)

public:
    explicit PtngProject(const QString &inputFileName, QObject *parent = nullptr);
    void addWatchDirectory(const QString &directory);
    void removeWatchDirectory(const QString &directory);
    void addArtefact(const QString &sourceTool, const QString &sourceFile, const QString &outputFile, const QString &displayName, const QString &artefactId = "");
    void removeArtefact(const QString &artefactId);
    bool load();
    bool create();
    QSqlTableModel* getProjectTable();
    QSqlTableModel* getArtefactsTable();

    QString getProjectName() const;
    void setProjectName(const QString &newProjectname);

    QString getWorkingDirectory() const;
    void setWorkingDirectory(const QString &newWorkingDirectory);

    bool getIsDirty() const;
    void setIsDirty(bool newIsDirty);

    QString getFileName() const;
    void setFileName(const QString &newFileName);

    QSqlDatabase getDatabase() const;
    void setDatabase(const QSqlDatabase &newDatabase);

private:
    QString projectName;
    QString workingDirectory;
    QStringList watchDirectories;
    QSqlDatabase database;
    QList<PtngProjectArtefact*> artefacts;
    QString fileName = "";
    bool isDirty;
    QString projectTableSql = "CREATE TABLE 'project' ('display_name' TEXT NOT NULL,'project_id' TEXT,'created_date' TEXT,'project_title' TEXT NOT NULL,'working_directory' TEXT NOT NULL,'watch_directories' TEXT)";
    QString artefactsTableSql = "CREATE TABLE 'artefacts' ('display_name' TEXT NOT NULL UNIQUE,'artefact_id' INTEGER NOT NULL UNIQUE,'source_tool' TEXT NOT NULL,'source_file' TEXT NOT NULL,'output_file' TEXT,PRIMARY KEY('artefact_id' AUTOINCREMENT))";

signals:
    void projectNameChanged();
    void workingDirectoryChanged();
    void domDocumentChanged();
    void isDirtyChanged();
    void fileNameChanged();
    void projectSaved();
    void projectLoaded(const QString &fileName,const QString &projectName);
    void databaseChanged();
};

} // namespace ptng

