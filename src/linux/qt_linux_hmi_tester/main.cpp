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
#include <QApplication>
#include <QCoreApplication>

#include <hmitestercontrol.h>
#include <linuxpreloadingaction.h>
#include <xmldatamodeladapter.h>
#include <debug.h>

int main(int argc, char *argv[])
{
    // create specific modules for qt-linux-xml adaptation
    PreloadingAction *pa = new LinuxPreloadingAction();
    DataModelAdapter *dma = new XMLDataModelAdapter();

    QCoreApplication::setOrganizationName("pedromateo");
    QCoreApplication::setApplicationName("openhmitester");
    QCoreApplication::setApplicationVersion("git");

    // initialize and launch HMI Tester
    QApplication a(argc, argv);
    HMITesterControl w(pa,dma);

    // get call argument
    QStringList args = a.arguments();

    // CASE: command run application has non-argument
    if (args.size() > 2)
    {
        // TO DO: at user manual
        return 0;
    }
    else
    {
        // run application with GUI
        // CASE: command run application has argument
        if (args.size() == 2)
        {
            // set config path is last element in listArgs
            QString pathConfig(args.at(1));
            w.automationRunTestSuite(pathConfig);

            qDebug() << "Start read file and setup testcase by config file: " << pathConfig;
        }
    }

    w.show();
    return a.exec();
}
