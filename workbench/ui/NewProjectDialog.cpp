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
#include "NewProjectDialog.hpp"
#include "ui_NewProjectDialog.h"

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
    QSettings s;
    // ui->lineEditFile->setText(s.value("defaultProjectDirectory").toString());
    connect(ui->pushButtonFile,
            SIGNAL(clicked()),
                this,
            SLOT(setProjectFile())
            ,Qt::UniqueConnection
                );
    connect(ui->addFileWatchPushButton,
            SIGNAL(clicked()),
                this,
            SLOT(addWatchDirectory())
            ,Qt::UniqueConnection
                );
    connect(ui->removeFileWatchPushButton,
            SIGNAL(clicked()),
                this,
            SLOT(removeWatchDirectory())
            ,Qt::UniqueConnection
                );
    connect(ui->fileWatchListWidget,
             SIGNAL(itemSelectionChanged()),
                this,
            SLOT(enableWatchRemoveButton())
            ,Qt::UniqueConnection
                );
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

void NewProjectDialog::setProjectFile()
{
    QSettings s;
    QString file = QFileDialog::getSaveFileName(this,
                                                "PTNG Workbench",
                                                s.value("defaultProjectDirectory").toString(),
                                                "SQlLite database files (*.db)"
                                                );
    if( !file.isEmpty() ){
        ui->lineEditFile->setText(file);
    }
}

void NewProjectDialog::setProjectName(const QString &projectName)
{
    project->setProjectName(projectName);
}

void NewProjectDialog::addWatchDirectory()
{
    QSettings s;
    QString directory = QFileDialog::getExistingDirectory(this,"PTNG Workbench",s.value("defaultProjectDirectory").toString());
    if( directory.isEmpty() ){
        return;
    }
    if( watchDirectories.contains(directory) ){
        QMessageBox::information(this,"PTNG Workbench","Duplicate directory");
        return;
    }
    ui->fileWatchListWidget->addItem(directory);
    watchDirectories.append(directory);
}

void NewProjectDialog::removeWatchDirectory()
{
    QListWidgetItem *item = ui->fileWatchListWidget->currentItem();
    QString directory = item->text();
    ui->fileWatchListWidget->takeItem(ui->fileWatchListWidget->currentRow());
    watchDirectories.removeAll(directory);
}

void NewProjectDialog::enableWatchRemoveButton()
{
    ui->removeFileWatchPushButton->setEnabled(true);
}

Project *NewProjectDialog::getProject() const
{
    return project;
}

void NewProjectDialog::setProject(Project *newProject)
{
    if (project == newProject)
        return;
    project = newProject;
    emit projectChanged();
}

void NewProjectDialog::done(int r)
{
    if( r == QDialog::Accepted ){
        projectName = ui->lineEditName->text();
        projectDescription = projectName;
        projectFile = ui->lineEditFile->text();
        projectId = ui->lineEditId->text();
        if( projectName.isEmpty() ){
            QMessageBox::information(this,"PTNG Workbench", "Display name needs to be completed.");
            return;
        }
        if( projectId.isEmpty() ){
            QMessageBox::information(this,"PTNG Workbench", "Project Id needs to be completed.");
            return;
        }
        if( projectFile.isEmpty() ){
            QMessageBox::information(this,"PTNG Workbench", "Project file needs to be completed.");
            return;
        }

        else{
            project = new Project();
            project->setProjectFile(projectFile);
            project->setProjectName(ui->lineEditName->text());
            project->setWatchDirectories(watchDirectories);
            project->setProjectDescription(projectDescription);
            project->setProjectId(ui->lineEditId->text());
            project->create();
            QDialog::done(r);
            return;
        }
    }
    else{
        QDialog::done(r);
        return;
    }
}
