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

NewTSDialog::NewTSDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::NewTSDialog)
{
    m_ui->setupUi(this);
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
    //ask for the binary
    QString aux = QtUtils::openFileDialog("Please, select the AUT (Application Under Test):",
                                         ".",//TODO add dir reminder
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
    }
}

///
/// \brief select path to store testsuite
///
void NewTSDialog::on_pb_tsPath_clicked()
{
    //ask for the location
    QString aux = QtUtils::saveFileDialog("Please, select a path and a name to store the TestSuite:",
                                   ".",//TODO add dir reminder
                                   "*."OHT_FILE_EXTENSION);

    if (aux != NULL && aux != ""){
        _tsPath = aux;
        m_ui->le_tsPath->setText(_autPath);
    }
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
