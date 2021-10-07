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
 *
 */

#ifndef LINUXPRELOADINGACTION_H
#define LINUXPRELOADINGACTION_H

#include <preloadingaction.h>
#include <QObject>
#include <QProcess>
#include <memory>

class LinuxPreloadingAction : public PreloadingAction
{
    Q_OBJECT

public:
    LinuxPreloadingAction();


    ///
    /// returns lib preload location
    ///
    virtual std::string libPreloadPath();

    ///
    /// interface methods
    ///

    virtual bool launchApplication ( const std::string &binaryPath,
                                     const std::string &preloadLibraryPath,
                                     const std::string &outputFile,
                                     const std::string &errorFile,
                                     const QStringList &listArgument = {}) throw (bin_error_exception, lib_error_exception);

    virtual bool stopApplication ();

private slots:

    ///
    ///output signals management
    ///
    //TODO

private:
    ///
    ///output file management
    ///
    bool redirectStandarOutputToFile(const std::string &outputFile);
    bool redirectStandarErrorToFile(const std::string &errorFile);

private:
    std::auto_ptr<QProcess> process_;
};

#endif // LINUXPRELOADINGACTION_H
