// -*- mode: c++; c-basic-offset: 4; c-basic-style: bsd; -*-
/*
 *   This program is free software; you can redistribute it and/or
 *   modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation; either version 3.0 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *   02111-1307 USA
 *
 *   This file is part of the Open-HMI Tester,
 *   http://openhmitester.sourceforge.net
 *
 */

#include "newtsdialog.h"
#include <ui_newtsdialog.h>
#include <ohtbaseconfig.h>
#include <debug.h>
#include <qtutils.h>

#include <QLabel>
#include <QDir>
#include <QFileInfo>

NewTSDialog::NewTSDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::NewTSDialog)
{
    m_ui->setupUi(this);

    // set cb_CheckARG is uncheck
    m_ui->cb_CheckARG->setChecked(false);
    m_ui->textArgument->setEnabled(false);

    // set _listArgument is emty
    _listArgument = "";
}

NewTSDialog::~NewTSDialog()
{
    delete m_ui;
}

///
/// accessors
///

QString& NewTSDialog::getTestsuiteName()
{
    return _tsName;
}

QString& NewTSDialog::getTestsuitePath()
{
    return _tsPath;
}

QString& NewTSDialog::getAUTPath()
{
    return _autPath;
}

QString NewTSDialog::getListArgument()
{
    return _listArgument;
}

void NewTSDialog::setListArgument(QString txtARG)
{
    // set _listArgument is emty
    _listArgument = "";

    QString thisARGText = txtARG.trimmed();

    if (!thisARGText.isEmpty())
    {
        // set _listArgument is list string split
        _listArgument = thisARGText;
    }
}



///
/// \brief set testsuite name
/// \param arg1
///
void NewTSDialog::on_le_tsName_editingFinished()
{
    QString s = m_ui->le_tsName->text();

    // set name
    if (s != "")
        _tsName = s;
    else
        m_ui->le_tsName->setText(_tsName);
}

///
/// \brief select AUT binary
///
void NewTSDialog::on_pb_aut_clicked()
{
    _settings.beginGroup("NewTSDialog");
    QString lastAutDirectory = _settings.value(SETT_LAST_AUT_DIR, QDir::homePath()).toString();

    //ask for the binary
    QString aux = QtUtils::openFileDialog("Please, select the AUT (Application Under Test):",
                                         lastAutDirectory,
                                         "*");

    if (aux != NULL && aux != ""){
        // check is is a valid binary
        if (!QtUtils::isExecutable(aux))
        {
            QtUtils::newErrorDialog("The file selected is not a valid binary.");
            return;
        }
        // if it is valid...
        _autPath = aux;
        m_ui->le_aut->setText(_autPath);
        QFileInfo fi(aux);
        _settings.setValue(SETT_LAST_AUT_DIR, fi.absolutePath());
    }
    _settings.endGroup();
}

///
/// \brief select path to store testsuite
///
void NewTSDialog::on_pb_tsPath_clicked()
{
    _settings.beginGroup("NewTSDialog");
    QString lastSaveDir = _settings.value(SETT_LAST_SAVE_DIR, QDir::homePath()).toString();
    //ask for the location
    QString aux = QtUtils::saveFileDialog("Please, select a path and a name to store the TestSuite:",
                                   lastSaveDir + QDir::separator() + m_ui->le_tsName->text().toLower() + "." + OHT_FILE_EXTENSION,
                                   "*."OHT_FILE_EXTENSION);

    if (aux != NULL && aux != ""){
        _tsPath = aux;
        m_ui->le_tsPath->setText(_tsPath);
        QFileInfo fi(aux);
        _settings.setValue(SETT_LAST_SAVE_DIR, fi.absolutePath());
    }
    _settings.endGroup();
}

///
/// \brief NewTSDialog accepted
///
void NewTSDialog::on_buttonBox_accepted()
{
    bool error = false;

    // check valid values
    if (_tsName == ""){
        QtUtils::newErrorDialog("Test suite name must be a valid name.");
        error = true;
    }
    if (_autPath == ""){
        QtUtils::newErrorDialog("AUT must be a valid application to test.");
        error = true;
    }
    if (_tsPath == ""){
        QtUtils::newErrorDialog("A valid path must be selected to store the test suite.");
        error = true;
    }
    if (m_ui->cb_CheckARG->checkState())
    {
        // set list argument by textArgument
        setListArgument(m_ui->textArgument->text());

        if (_listArgument.isEmpty())
        {
            QtUtils::newErrorDialog("Textbox argument is emty.\n"
                                    "This application will be run non-argument");
            error = true;
        }
    }

    if (error) return;

    // if everything is valid...
    done(1);
}

///
/// \brief NewTSDialog rejected
///
void NewTSDialog::on_buttonBox_rejected()
{
    done(0);
}

void NewTSDialog::on_cb_CheckARG_clicked()
{
    bool isChecked = m_ui->cb_CheckARG->checkState();

    if(isChecked)
    {
        // set textbox ARG is enable
        m_ui->textArgument->setEnabled(true);
    }
    else
    {
        // set textbox ARG is unenable
        m_ui->textArgument->setEnabled(false);
    }
}
