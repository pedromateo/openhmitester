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
#ifndef GENERALINPUTDIALOG_H
#define GENERALINPUTDIALOG_H

#include <QDialog>
#include <boost/ptr_container/ptr_map.hpp>
#include <QLineEdit>
#include <QString>

namespace Ui {
    class GeneralInputDialog;
}

class GeneralInputDialog : public QDialog {
    Q_OBJECT
    //Q_DISABLE_COPY(GeneralInputDialog);

public:
    explicit GeneralInputDialog(QWidget *parent = 0);
    virtual ~GeneralInputDialog();

    //add element
    void addElement(const QString&);

    //getData methods
    QString getData(const QString&);

    //static utilities
    static QStringList askForValues(const QStringList&);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::GeneralInputDialog *m_ui;

    //input map
    boost::ptr_map<QString, QLineEdit*> elements_;
};

#endif // GENERALINPUTDIALOG_H
