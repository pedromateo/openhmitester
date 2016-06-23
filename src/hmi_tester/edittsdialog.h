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
#ifndef EditTSDialog_H
#define EditTSDialog_H

#include <QDialog>
#include <QLineEdit>
#include <QString>
#include <QSettings>
#include <datamodel.h>

namespace Ui {
    class EditTSDialog;
}

class EditTSDialog : public QDialog {
    Q_OBJECT
    //Q_DISABLE_COPY(EditTSDialog);

public:
    explicit EditTSDialog(DataModel::TestSuite* ts, QWidget *parent = 0);
    virtual ~EditTSDialog();




private slots:
    void on_pb_aut_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_le_tsName_editingFinished();

private:
    Ui::EditTSDialog *m_ui;
    //QString _tsPath;
    //QString _autPath;
    //QString _tsName;
    QSettings _settings;
    DataModel::TestSuite* _ts;
};

#endif // EditTSDialog_H
