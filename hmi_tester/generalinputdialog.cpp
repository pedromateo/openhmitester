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

#include "generalinputdialog.h"
#include <ui_generalinputdialog.h>
#include <debug.h>
#include <QLabel>

GeneralInputDialog::GeneralInputDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::GeneralInputDialog)
{
    m_ui->setupUi(this);
}

GeneralInputDialog::~GeneralInputDialog()
{
    delete m_ui;
}

void GeneralInputDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

///
/// add element
///
void GeneralInputDialog::addElement(const QString& text)
{
    QLineEdit *field = new QLineEdit();
    m_ui->formLayout->addRow(text, field);
    elements_[text] = field;
}

///
/// get data
///
QString GeneralInputDialog::getData(const QString& text)
{
    return elements_[text]->text();
}

///
/// static utilities
///
QStringList GeneralInputDialog::askForValues(const QStringList& elements)
{
    GeneralInputDialog *gid = new GeneralInputDialog(/**/);

    //add elements
    foreach(QString s, elements)
    {
        gid->addElement(s);
    }

    //show the dialog
    int result = gid->exec();

    //return the results
    QStringList results;
    if (result)//OK
    {
        foreach(QString s, elements)
        {
            results << gid->getData(s);
        }
    }

    delete gid;
    return results;
}

///
///customized constructor
///
/*GeneralInputDialog::GeneralInputDialog(QStringList& elements)
{
    GeneralInputDialog();

    DEBUG("in");

    //elements
    foreach(QString s, elements)
    {
        DEBUG("proc1" << s.toStdString());
        QLineEdit *le = new QLineEdit();
        DEBUG("proc2");

        DEBUG("proc3");
        assert(m_ui->formLayout);
        //m_ui->formLayout->addRow(new QLabel(s),le);
        DEBUG("proc4");
    }
    setLayout(m_ui->formLayout);
    DEBUG("saliendo");
}*/
