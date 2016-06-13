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

#include <qtpreloadingcontrol.h>
#include <debug.h>
#include <QWidget>

PreloadingControl* QtPreloadingControl::pc = NULL;

QtPreloadingControl::QtPreloadingControl(EventConsumer *ec, EventExecutor *ex)
    : PreloadingControl(ec, ex)
{
    _event_consumer = ec;
    _event_executor = ex;
}


QtPreloadingControl::~QtPreloadingControl()
{
    if (_event_consumer != NULL) delete _event_consumer;
    if (_event_executor != NULL) delete _event_executor;
}

bool QtPreloadingControl::Do_preload()
{
    if (!pc)
    {
        DEBUG(D_PRELOAD,"(QtPreloadingControl::do_preload) Initializing hooking process.");

#if LINUX_OHT
        //sinchronizing X11 threads
        XInitThreads();
#endif

        // create specific consumers and executor...
        // ...to create a control instance
        pc = new QtPreloadingControl(new QtEventConsumer(),new QtEventExecutor());

        //call the initialize method
        pc->initPreload();
        DEBUG(D_PRELOAD,"(QtPreloadingControl::do_preload) Hooking process finished.");

        std::cout << "Waking up..." << std::endl;

        return true;
    }

    return false;
}

///
/// get up method
///

/*
  This method, which is called at the beginning of
  the execution, is reimplemented in order to
  replace the original implementation and add the
*/


bool QWidget::nativeEvent(const QByteArray & eventType, void * message, long * result)
{
    QtPreloadingControl::Do_preload();
    return false;
}
