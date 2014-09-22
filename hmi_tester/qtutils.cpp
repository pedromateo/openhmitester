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

// -*- mode: c++; c-basic-offset: 4; c-basic-style: bsd; -*-
#include "qtutils.h"
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

/// ///
///
/// directory and file utils
///
/// ///

/// returns true if it is a valid binary
bool QtUtils::isExecutable ( const QString &bin )
{
    QFileInfo fi ( bin );
    return fi.exists ()  && fi.isExecutable ();
}

///gets the current directory
QString QtUtils::getCurrentDir()
{
    return QtUtils::getDirFromPath ( QCoreApplication::applicationFilePath () );
}

///gets the working directory
QString QtUtils::getWorkingDir()
{
    return QDir::current().absolutePath();
}

///gets the directory from a full path (linux)
QString QtUtils::getDirFromPath ( const QString &s )
{
    /*only on linux*/
    int levels = s.count ( "/" );
    return s.section ( "/",0,levels-1 );
}

/// ///
///
/// messageBox utils
///
/// ///

//shows an OK/Cancel dialog
bool QtUtils::showOkCancelDialog( const QString &s )
{
    QMessageBox msgBox;
    msgBox.setText("Question:");
    msgBox.setInformativeText(s);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    return msgBox.exec();
}

///shows an error dialog
void QtUtils::newErrorDialog ( const QString &s )
{
    QMessageBox::critical ( 0, "Error", s, "&OK");
}

///shows an info dialog
void QtUtils::newInfoDialog ( const QString &s )
{
    QMessageBox::information(  0, "Info", s, "&OK");
}

/// ///
///
/// file dialog utils
///
/// ///

///returns the path of the selected file (open)
QString QtUtils::openFileDialog ( const QString& title,
                                         const QString& dir,
                                         const QString& ext )
{
    return QFileDialog::getOpenFileName ( 0, title,
                                          dir,
                                          ext );
}

///returns the path of the selected file (save)
QString QtUtils::saveFileDialog( const QString& title,
                                        const QString& dir,
                                        const QString& ext )
{
    return QFileDialog::getSaveFileName ( 0, title,
                                          dir,
                                          ext );
}

///returns the selected path
QString QtUtils::selectPathDialog( const QString& title,
                                        const QString& dir)
{
    return QFileDialog::getExistingDirectory (0,
                                              title,
                                              dir);
}
