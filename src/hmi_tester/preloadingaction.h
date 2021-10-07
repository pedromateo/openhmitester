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
#ifndef PRELOADINGACTION_H
#define PRELOADINGACTION_H

#include <QObject>
#include <string>

#include <exceptions.h>

class PreloadingAction : public QObject
{
    Q_OBJECT

public:


    ///
    /// returns lib preload location
    ///
    virtual std::string libPreloadPath() = 0;


    ///
    /// launches a binary and the preload library
    ///
    virtual bool launchApplication( const std::string &binaryPath,
                                    const std::string &preloadLibraryPath,
                                    const std::string &outputFile,
                                    const std::string &errorFile,
                                    const QStringList &listArgument = {}) throw (bin_error_exception, lib_error_exception) = 0;

    ///
    /// stops the binary
    ///
    virtual bool stopApplication() = 0;

    //output

signals:
    ///
    /// Standar output notification
    ///
    void standardOutput(const std::string&);

    ///
    /// Error output notification
    ///
    void standardError(const std::string&);

    ///
    /// Preloading error notification
    ///
    void preloadingError(const std::string&);

    //application

    ///
    /// Application finished notification
    ///
    void applicationClosed(int);

private slots:



};

#endif // PRELOADINGACTION_H
