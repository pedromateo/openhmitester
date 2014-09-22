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
#include <X11/Xlib.h>


QtX11PreloadingControl::QtX11PreloadingControl()
{
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

bool QWidget::x11Event ( XEvent * event )
{
    //only the first time...
    if (!pc)
    {
        DEBUG(D_PRELOAD,"(QWidget::x11Event) Initializing hooking process.");

        //sinchronizing X11 threads
        XInitThreads();

        //create a control instance
        pc = new QtX11PreloadingControl();
        //call the initialize method
        pc->initPreload();
    }
    return false;
}
