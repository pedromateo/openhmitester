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
#ifndef NewTSDialog_H
#define NewTSDialog_H

#include <QDialog>
#include <QLineEdit>
#include <QString>
#include <QSettings>

namespace Ui {
    class NewTSDialog;
}

class NewTSDialog : public QDialog {
    Q_OBJECT
    //Q_DISABLE_COPY(NewTSDialog);

public:
    explicit NewTSDialog(QWidget *parent = 0);
    virtual ~NewTSDialog();

public:

    QString& getTestsuiteName();
    QString& getTestsuitePath();
    QString& getAUTPath();
    QString getListArgument();
    void setListArgument(QString txtARG);




private slots:
    void on_pb_aut_clicked();

    void on_pb_tsPath_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_le_tsName_editingFinished();

    void on_cb_CheckARG_clicked();

private:
    Ui::NewTSDialog *m_ui;
    QString _tsPath;
    QString _autPath;
    QString _tsName;
    QSettings _settings;
    QString _listArgument;
};

#endif // NewTSDialog_H
