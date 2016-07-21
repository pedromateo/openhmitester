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

#include "edittsdialog.h"
#include <ui_edittsdialog.h>
#include <ohtbaseconfig.h>
#include <datamodel.h>
#include <debug.h>
#include <qtutils.h>


#include <QLabel>
#include <QDir>
#include <QFileInfo>
#include <QStringListModel>

EditTSDialog::EditTSDialog(DataModel::TestSuite *ts, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::EditTSDialog)
{
    m_ui->setupUi(this);

    // add ts original data to the GUI
    _ts = ts;
    QString aux;

    aux = QString(_ts->name().c_str());
    m_ui->le_tsName->setText(aux);

    aux = QString(_ts->appId().c_str());
    m_ui->le_aut->setText(aux);

    QStringList tclist;
    DataModel::TestSuite::TestCaseList::const_iterator it = _ts->testCases().begin();
    while (it != _ts->testCases().end()){
        tclist << QString(it->name().c_str());
        it++;
    }
    QStringListModel* tclist_model = new QStringListModel();
    tclist_model->setStringList(tclist);
    m_ui->lv_testcases->setModel(tclist_model);
}

EditTSDialog::~EditTSDialog()
{
    delete m_ui;
}



///
/// \brief set testsuite name
/// \param arg1
///
void EditTSDialog::on_le_tsName_editingFinished()
{
    QString s = m_ui->le_tsName->text();

    // set name
    if (s != "")
        _ts->name(s.toStdString());
    else
        m_ui->le_tsName->setText(QString(_ts->name().c_str()));
}

///
/// \brief select AUT binary
///
void EditTSDialog::on_pb_aut_clicked()
{
    _settings.beginGroup("EditTSDialog");
    QString lastAutDirectory;
    lastAutDirectory = _settings.value(SETT_LAST_AUT_DIR, QDir::homePath()).toString();
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
        _ts->appId(aux.toStdString());
        m_ui->le_aut->setText(aux);
        QFileInfo fi(aux);
        _settings.setValue(SETT_LAST_AUT_DIR, fi.absolutePath());
    }
    _settings.endGroup();
}

///
/// \brief EditTSDialog accepted
///
void EditTSDialog::on_buttonBox_accepted()
{
    bool error = false;

    // check valid values
    if (_ts->name() == ""){
        QtUtils::newErrorDialog("Test suite name must be a valid name.");
        error = true;
    }
    if (_ts->appId() == ""){
        QtUtils::newErrorDialog("AUT must be a valid application to test.");
        error = true;
    }

    if (error) return;

    // if everything is valid...
    done(1);
}

///
/// \brief EditTSDialog rejected
///
void EditTSDialog::on_buttonBox_rejected()
{
    done(0);
}



///
/// \brief delete testcase from testsuite
///
void EditTSDialog::on_pb_deleteTestcase_clicked()
{
    // get name of testcase to delete
    QString tcname = m_ui->lv_testcases->model()->data(m_ui->lv_testcases->currentIndex()).toString();

    // show warning
    QString msg = "You are going to delete test case '";
    msg += tcname;
    msg += "'";
    bool result = QtUtils::showOkCancelDialog(msg);

    if (result){
        // delete from gui model
        m_ui->lv_testcases->model()->removeRows(m_ui->lv_testcases->currentIndex().row(),1);

        // delete from testsuite
        _ts->deleteTestCase(tcname.toStdString());

        DEBUG(D_GUI,"(EditTSDialog::on_pb_deleteTestcase_clicked) Removed: " << tcname.toStdString());
    }
}
