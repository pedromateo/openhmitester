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

#include "newtcdialog.h"
#include <ui_newtcdialog.h>
#include <ohtbaseconfig.h>
#include <debug.h>
#include <qtutils.h>
#include <QLabel>

NewTCDialog::NewTCDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::NewTCDialog)
{
    m_ui->setupUi(this);
}

NewTCDialog::~NewTCDialog()
{
    delete m_ui;
}

///
/// accessors
///

QString& NewTCDialog::getTestcaseName()
{
    return _tcName;
}



///
/// \brief set testsuite name
/// \param arg1
///
void NewTCDialog::on_le_tcName_editingFinished()
{
    QString s = m_ui->le_tcName->text();

    // set name
    if (s != "")
        _tcName = s;
    else
        m_ui->le_tcName->setText(_tcName);

}

///
/// \brief NewTCDialog accepted
///
void NewTCDialog::on_buttonBox_accepted()
{
    bool error = false;

    // check valid values
    if (_tcName == ""){
        QtUtils::newErrorDialog("Test case name must be a valid name.");
        error = true;
    }

    if (error) return;

    // if everything is valid...
    done(1);
}

///
/// \brief NewTCDialog rejected
///
void NewTCDialog::on_buttonBox_rejected()
{
    done(0);
}

