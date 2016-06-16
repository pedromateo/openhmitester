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

#include "winpreloadingaction.h"
#include <qtutils.h>
#include <qtwin_ohtconfig.h>
#include <debug.h>

#include <iostream>

WinPreloadingAction::WinPreloadingAction()
{
}



/// returns lib preload location
std::string WinPreloadingAction::libPreloadPath()
{
    return std::string(LIBPRELOAD_PATH);
}

///
bool WinPreloadingAction::launchApplication ( const std::string &binaryPath,
                                                const std::string &preloadLibraryPath,
                                                const std::string &outputFile,
                                                const std::string &errorFile) throw (bin_error_exception, lib_error_exception)
{
    //checking if the binary exists
    if ( !QtUtils::isExecutable ( QString (binaryPath.c_str()) ) )
        throw bin_error_exception(binaryPath);

    //checking if the library exists
    if ( !QtUtils::fileExists(QString(preloadLibraryPath.c_str()) ) )
        throw lib_error_exception(preloadLibraryPath);

    //delete current and create a new process
    process_.reset (new QProcess (this));

    // create dirs for logs and redirect process output
    if (outputFile != ""){
        assert(redirectStandarOutputToFile(outputFile));
    }
    if (errorFile != ""){
        assert(redirectStandarErrorToFile(errorFile));
    }

    //connecting process signals
    connect ( process_.get(), SIGNAL ( finished (int, QProcess::ExitStatus) ),
              this, SIGNAL ( applicationClosed(int) ) );

    /// TODO

    ///
    /// \brief TODO: Do preload here...
    ///

    /// TODO

    DEBUG(D_PRELOAD,"==========================================");
    DEBUG(D_PRELOAD,"(WinPreloadingAction::launchApplication) Launching application with:");
    DEBUG(D_PRELOAD," - binaryPath = " << binaryPath);
    DEBUG(D_PRELOAD," - preloadLibraryPath = " << preloadLibraryPath);
    //DEBUG(D_PRELOAD," - envvar = " << envvar.toStdString());
    DEBUG(D_PRELOAD," - outputFile = " << outputFile);
    DEBUG(D_PRELOAD," - errorFile = " << errorFile);
    DEBUG(D_PRELOAD,"==========================================");

    //process execution
    process_->start(QString(binaryPath.c_str()));
    process_->waitForStarted();

    std::cout << "(WinPreloadingAction::launchApplication) Application launched." << std::endl;
    return true;
}

///
bool WinPreloadingAction::stopApplication ()
{
    process_->kill();
    return true;
}



///
/// private methods
///

bool WinPreloadingAction::redirectStandarOutputToFile(const std::string& outputFile)
{
    if ( process_.get() && outputFile != "" )
    {
        process_->setStandardOutputFile ( QString ( outputFile.c_str()) );
        DEBUG(D_PRELOAD,
              "(WinPreloadingAction::launchApplication) std output redirected to file: " << outputFile);
        return true;
    }
    return false;
}

bool WinPreloadingAction::redirectStandarErrorToFile(const std::string& errorFile)
{
    if ( process_.get() && errorFile != "" )
    {
        process_->setStandardErrorFile ( QString (errorFile.c_str()) );
        DEBUG(D_PRELOAD,
              "(WinPreloadingAction::launchApplication) std error redirected to file: " << errorFile);
        return true;
    }
    return false;
}

