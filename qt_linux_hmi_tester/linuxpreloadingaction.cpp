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
#include <qtutils.h>
#include <qtlinux_ohtconfig.h>
#include <debug.h>

#include <iostream>

LinuxPreloadingAction::LinuxPreloadingAction()
{
}



/// returns lib preload location
std::string LinuxPreloadingAction::libPreloadPath()
{
    return std::string(LIBPRELOAD_PATH);
}

///
bool LinuxPreloadingAction::launchApplication ( const std::string &binaryPath,
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

    //redirect process output
    if (outputFile != "")
        assert(redirectStandarOutputToFile(outputFile));
    if (errorFile != "")
        assert(redirectStandarErrorToFile(errorFile));

    /*connect( process_.get(), SIGNAL ( readyReadStandardOutput() ),
              this, SIGNAL ( standardOutput(const QString&) ) );
    connect( process_.get(), SIGNAL ( readyReadStandardError() ),
              this, SIGNAL ( standardError(const QString&) ) );*/

    //connecting process signals
    connect ( process_.get(), SIGNAL ( finished (int, QProcess::ExitStatus) ),
              this, SIGNAL ( applicationClosed(int) ) );

    //setting preloading environment for the process
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(PRELOAD_ENVVAR, QString(preloadLibraryPath.c_str()));
    //env.insert(PRELOAD_ENVVAR, "/home/pedro/svn_catedra/anotaciones/testing/imp_HMITester_github/openhmitester/build/qt_linux_lib_preload/libOHTPreload.so");
    process_->setProcessEnvironment(env);

    /*QStringList environment = process_->processEnvironment().toStringList();
    for(int i=0; i < environment.size(); i++){
        std::cout << environment.at(i).toLocal8Bit().constData() << std::endl;
    }*/

    DEBUG(D_PRELOAD,"==========================================");
    DEBUG(D_PRELOAD,"(LinuxPreloadingAction::launchApplication) Launching application with:");
    DEBUG(D_PRELOAD," - binaryPath = " << binaryPath);
    DEBUG(D_PRELOAD," - preloadLibraryPath = " << preloadLibraryPath);
    //DEBUG(D_PRELOAD," - envvar = " << envvar.toStdString());
    DEBUG(D_PRELOAD," - outputFile = " << outputFile);
    DEBUG(D_PRELOAD," - errorFile = " << errorFile);
    DEBUG(D_PRELOAD,"==========================================");

    //process execution
    process_->start(QString(binaryPath.c_str()));
    process_->waitForStarted();

    std::cout << "(LinuxPreloadingAction::launchApplication) Application launched." << std::endl;
    return true;
}

///
bool LinuxPreloadingAction::stopApplication ()
{
    process_->kill();
    return true;
}



///
/// private methods
///

bool LinuxPreloadingAction::redirectStandarOutputToFile(const std::string& outputFile)
{
    if ( process_.get() && outputFile != "" )
    {
        process_->setStandardOutputFile ( QString ( outputFile.c_str()) );
        DEBUG(D_PRELOAD,
              "(LinuxPreloadingAction::launchApplication) std output redirected to file: " << outputFile);
        return true;
    }
    return false;
}

bool LinuxPreloadingAction::redirectStandarErrorToFile(const std::string& errorFile)
{
    if ( process_.get() && errorFile != "" )
    {
        process_->setStandardErrorFile ( QString (errorFile.c_str()) );
        DEBUG(D_PRELOAD,
              "(LinuxPreloadingAction::launchApplication) std error redirected to file: " << errorFile);
        return true;
    }
    return false;
}

