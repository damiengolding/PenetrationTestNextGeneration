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
#include "PreferencesDialog.hpp"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    QSettings s;
    QFont displayFont(s.value("displayFont","Open Sans").toString(), s.value("fontSize",12).toInt() );
    ui->fontComboBox->setCurrentFont(displayFont);
    ui->fontSpinBox->setValue( s.value("fontSize",12).toInt());
    ui->wdLineEdit->setText(s.value("defaultDirectory").toString());
    connect(ui->wdPushButton,
            SIGNAL( clicked() ),
            this,
            SLOT( setDefaultDirectory() )
                );
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::done(int r)
{
    if( r == QDialog::Accepted ){
        QString workingDirectory = ui->wdLineEdit->text();
        if( !QFile::exists(workingDirectory) ){
            QMessageBox::information(this,"PTNG Workbench", "An existing directory needs to be\nsupplied for the working directory.");
            return;
        }
        else{
            QSettings s;
            s.setValue("displayFont", ui->fontComboBox->currentFont().family());
            s.setValue("fontSize",ui->fontSpinBox->value());
            s.setValue("defaultDirectory",ui->wdLineEdit->text());
            QFont font( ui->fontComboBox->currentFont().family(), ui->fontSpinBox->value() );
            if( parentWindow != nullptr ){
                parentWindow->setFont(font);
            }
            QDialog::done(r);
            return;
        }
    }
    else{
        QDialog::done(r);
        return;
    }
    QDialog::done(r);
}

void PreferencesDialog::setDefaultDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this,"PTNG Workbench");
    if( directory.isEmpty() ){
        return;
    }
    ui->wdLineEdit->setText(directory);
}

QMainWindow *PreferencesDialog::getParentWindow() const
{
    return parentWindow;
}

void PreferencesDialog::setParentWindow(QMainWindow *newParentWindow)
{
    if (parentWindow == newParentWindow)
        return;
    parentWindow = newParentWindow;
    emit parentWindowChanged();
}
