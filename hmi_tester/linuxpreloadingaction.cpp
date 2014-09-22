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

#include "linuxpreloadingaction.h"
#include "qtutils.h"
#include "ohtconfig.h"

#include <iostream>

LinuxPreloadingAction::LinuxPreloadingAction()
{
}

bool LinuxPreloadingAction::launchApplication ( const std::string &binaryPath,
                                                const std::string &preloadLibraryPath,
                                                const std::string &outputFile,
                                                const std::string &errorFile) throw (bin_error)
{
    //checking if the binary exists
    if ( !QtUtils::isExecutable ( QString (binaryPath.c_str()) ) )
        throw bin_error();

    //create a new process
    process_.reset (new QProcess (this));

    //redirect output
    if (outputFile != "") redirectStandarOutputToFile(outputFile);
    if (errorFile != "") redirectStandarErrorToFile(errorFile);

    //connecting signals...

    //... output signals  //TODO
    /*connect ( process_.get(), SIGNAL ( readyReadStandardOutput() ),
              this, SIGNAL ( standardOutput(const QString&) ) );
    connect ( process_.get(), SIGNAL ( readyReadStandardError() ),
              this, SIGNAL ( standardError(const QString&) ) );*/

    //... process signals
    connect ( process_.get(), SIGNAL ( finished ( int, QProcess::ExitStatus ) ),
              this, SIGNAL ( applicationFinished(int) ) );

    //setting preloading environment for the process
    QStringList env = QProcess::systemEnvironment();
    QString path = QtUtils::getCurrentDir();
    path = QString ("LD_PRELOAD=")
           + path
           + PATH_SEPARATOR
           + QString (preloadLibraryPath.c_str());
    env << path;
    process_->setEnvironment ( env );

    //process execution
    process_->start ( QString (binaryPath.c_str()) );
    process_->waitForStarted();

    std::cout << "(PreloadingAction::launchApp) Application launched." << std::endl;
    return true;
}

bool LinuxPreloadingAction::stopApplication ()
{
    process_->kill();
    return true;
}


///
/// private methods
///

bool LinuxPreloadingAction::redirectStandarOutputToFile(const std::string &outputFile)
{
    if ( process_.get() && outputFile != "" )
    {
        process_->setStandardOutputFile ( QString ( outputFile.c_str()) );
        return true;
    }
    return false;
}

bool LinuxPreloadingAction::redirectStandarErrorToFile(const std::string &errorFile)
{
    if ( process_.get() && errorFile != "" )
    {
        process_->setStandardErrorFile ( QString (errorFile.c_str()) );
        return true;
    }
    return false;
}

