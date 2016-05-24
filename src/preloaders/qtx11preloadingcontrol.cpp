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

#include "qtx11preloadingcontrol.h"
#include <debug.h>
#include <QWidget>

#if LINUX_OHT
   #include <X11/Xlib.h>
#endif



QtX11PreloadingControl::QtX11PreloadingControl(EventConsumer *ec, EventExecutor *ex)
    : PreloadingControl(ec, ex)
{
    _event_consumer = ec;
    _event_executor = ex;
}


QtX11PreloadingControl::~QtX11PreloadingControl()
{
    if (_event_consumer != NULL) delete _event_consumer;
    if (_event_executor != NULL) delete _event_executor;
}

///
/// get up method
///

/*
  This method, which is called at the beginning of
  the execution, is reimplemented in order to
  replace the original implementation and add the
  functionality we need.
  The preload library loading allows the replacement.
*/

PreloadingControl *pc = NULL;

/*
void QObject::timerEvent ( QTimerEvent * )
{
    //only the first time...
    if (!pc)
    {
        DEBUG(D_PRELOAD,"(QObject::timerEvent) Initializing hooking process.");

        //create a control instance
        pc = new QtX11PreloadingControl();
        //call the initialize method
        pc->initPreload();
    }
}
*/

#if QT_VERSION >= 0x050000
bool QWidget::nativeEvent(const QByteArray & eventType, void * message, long * result)
#else
bool QWidget::x11Event ( XEvent * event )
#endif
{
    //only the first time...
    if (!pc)
    {
        DEBUG(D_PRELOAD,"(QWidget::x11Event) Initializing hooking process.");

#if LINUX_OHT
        //sinchronizing X11 threads
        XInitThreads();
#endif

        // create specific consumers and executor
        EventConsumer* ec = new QtEventConsumer();
        EventExecutor* ex = new QtEventExecutor();

        //create a control instance
        pc = new QtX11PreloadingControl(ec,ex);

        //call the initialize method
        pc->initPreload();
        DEBUG(D_PRELOAD,"(QWidget::x11Event) Hooking process finished.");
    }
    return false;
}
