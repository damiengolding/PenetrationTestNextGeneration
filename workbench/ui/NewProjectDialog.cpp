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
    project = new PtngProject();
    connect(ui->workingDirectoryPushButton,
            &QPushButton::clicked,
                this,
            &NewProjectDialog::setWorkingDirectory
                );
    connect(ui->addFileWatchPushButton,
            &QPushButton::clicked,
                this,
            &NewProjectDialog::addWatchDirectory
                );
    connect(ui->removeFileWatchPushButton,
            &QPushButton::clicked,
                this,
            &NewProjectDialog::removeWatchDirectory
                );
    connect(ui->nameLineEdit,
            &QLineEdit::textChanged,
                this,
            &NewProjectDialog::setProjectName
                );
    connect(ui->fileWatchListWidget,
             &QListWidget::itemSelectionChanged,
                this,
            &NewProjectDialog::enableWatchRemoveButton
                );
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

PtngProject *NewProjectDialog::getProject() const
{
    return project;
}

void NewProjectDialog::setProject(PtngProject *newProject)
{
    if (project == newProject)
        return;
    project = newProject;
    emit projectChanged();
}

void NewProjectDialog::done(int r)
{
    if( r == QDialog::Accepted ){
        QString projectName = ui->nameLineEdit->text();
        QString workingDirectory = ui->workingDirectoryLineEdit->text();
        if( projectName.isEmpty() ){
            QMessageBox::information(this,"PTNG Workbench", "Project name needs to be completed.");
            return;
        }
        if( !QFile::exists(workingDirectory) ){
            QMessageBox::information(this,"PTNG Workbench", "An existing directory needs to be\nsupplied for the working directory.");
            return;
        }
        else{
            project->setProjectName(ui->nameLineEdit->text());
            project->setWorkingDirectory(ui->workingDirectoryLineEdit->text());
            QDialog::done(r);
            return;
        }
    }
    else{
        QDialog::done(r);
        return;
    }
}

void NewProjectDialog::setWorkingDirectory()
{
    QSettings s;
    QString directory = QFileDialog::getExistingDirectory(this,"PTNG Workbench",s.value("defaultDirectory").toString());
    if( directory.isEmpty() ){
        return;
    }
    ui->workingDirectoryLineEdit->setText(directory);
    project->setWorkingDirectory(directory);
}

void NewProjectDialog::setProjectName(const QString &projectName)
{
    project->setProjectName(projectName);
}

void NewProjectDialog::addWatchDirectory()
{
    QSettings s;
    QString directory = QFileDialog::getExistingDirectory(this,"PTNG Workbench",s.value("defaultDirectory").toString());
    if( directory.isEmpty() ){
        return;
    }
    if( watchDirectories.contains(directory) ){
        QMessageBox::information(this,"PTNG Workbench","Duplicate directory");
        return;
    }
    ui->fileWatchListWidget->addItem(directory);
    project->addWatchDirectory(directory);
    watchDirectories.append(directory);
}

void NewProjectDialog::removeWatchDirectory()
{
    QListWidgetItem *item = ui->fileWatchListWidget->currentItem();
    QString directory = item->text();
    project->removeWatchDirectory(directory);
    ui->fileWatchListWidget->takeItem(ui->fileWatchListWidget->currentRow());
    watchDirectories.removeAll(directory);
}

void NewProjectDialog::enableWatchRemoveButton()
{
    ui->removeFileWatchPushButton->setEnabled(true);
}
