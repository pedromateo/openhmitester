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
#ifndef QTUTILS_H
#define QTUTILS_H

#include <QString>

class QtUtils
{
public:

    /// ///
    ///
    /// directory and file utils
    ///
    /// ///

    ///returns true if it is a valid binary
    static bool isExecutable ( const QString&);

    ///returns true if file exists
    static bool fileExists (const QString&file);

    ///gets the current directory
    static QString getCurrentDir();

    ///gets the working directory
    static QString getWorkingDir();

    ///gets the directory from a full path (linux)
    static QString getDirFromPath ( const QString &s );

    /// ///
    ///
    /// messageBox utils
    ///
    /// ///

    //shows an OK/Cancel dialog
    static bool showOkCancelDialog( const QString &s );

    ///shows an error dialog
    static void newErrorDialog ( const QString &s );

    ///shows an info dialog
    static void newInfoDialog ( const QString &s );

    /// ///
    ///
    /// file dialog utils
    ///
    /// ///

    ///returns the path of the selected file (open)
    static QString openFileDialog ( const QString& title,
                                           const QString& dir,
                                           const QString& ext );

    ///returns the path of the selected file (save)
    static QString saveFileDialog( const QString& title,
                                          const QString& dir,
                                          const QString& ext );

    ///returns the selected path
    static QString selectPathDialog( const QString& title,
                                            const QString& dir);

    /// ///
    ///
    /// time utils
    ///
    /// ///

    ///returns the selected path
    static void sleep(int ms);
};

#endif // QTUTILS_H
