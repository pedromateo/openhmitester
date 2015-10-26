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

#include "qteventexecutor.h"
#include <debug.h>
#include <qwidgetutils.h>
#include <ohtbaseconfig.h>
#include <QWidget>

QtEventExecutor::QtEventExecutor() : EventExecutor()
{
    //update flags
    f_executing_ = false;
    f_paused_ = false;
}

///
/// this method is called to install the event executor
///
void QtEventExecutor::install()
{
    //it does not an installation process
    DEBUG(D_EXECUTOR,"(QtEventExecutor::install) Event Executor installed.");
}

///
///execution process control methods
///

void QtEventExecutor::startExecution()
{
    //update flags
    f_executing_ = true;
    f_paused_ = false;

    // execution starts with mouse at 0,0
    lastPos_ = QApplication::activeWindow()->mapToGlobal(QPoint(0,0));
    QCursor::setPos(lastPos_);
}

void QtEventExecutor::pauseExecution()
{
    //update flags
    f_executing_ = true;
    f_paused_ = true;
}

void QtEventExecutor::resumeExecution()
{
    //update flags
    f_executing_ = true;
    f_paused_ = true;
}

void QtEventExecutor::stopExecution()
{
    //update flags
    f_executing_ = false;
    f_paused_ = false;
}

///
/// this method is called when a new testItem arrives
///
void QtEventExecutor::handleNewTestItemReceived(DataModel::TestItem* ti)
{
    ///
    /// process control
    ///

    //if process is stoped...
    if (f_executing_ == false)
    {
        //no event executing
        DEBUG(D_EXECUTOR,"(QtEventExecutor::handleNewTestItemReceived) Stop state. No event executing.");
        return ;
    }
    //if process is paused...
    else if (f_executing_ && f_paused_)
    {
        //no event executing
        DEBUG(D_EXECUTOR,"(QtEventExecutor::handleNewTestItemReceived) Pause state. No event executing.");
        return ;
    }
    //if process is executing...
    else
    {
        //execute the event...
    }

    ///
    ///depending on the type..
    ///
    //window events
    if (ti->type() == QOE::QOE_WINDOW_CLOSE)
    {
        //executeCloseEvent(dynamic_cast<QOE::QOE_WindowClose*>(ti));
        QOE::QOE_WindowClose qoe;
        qoe.copy(ti);
        executeCloseEvent(&qoe);
    }
    //mouse events
    else if (ti->type() == QOE::QOE_MOUSE_PRESS)
    {
        //executeMousePressEvent(dynamic_cast<QOE::QOE_MousePress*>(ti));
        QOE::QOE_MousePress qoe;
        qoe.copy(ti);
        executeMousePressEvent(&qoe);
    }
    else if (ti->type() == QOE::QOE_MOUSE_RELEASE)
    {
        //executeMouseReleaseEvent(dynamic_cast<QOE::QOE_MouseRelease*>(ti));
        QOE::QOE_MouseRelease qoe;
        qoe.copy(ti);
        executeMouseReleaseEvent(&qoe);
    }
    else if (ti->type() == QOE::QOE_MOUSE_DOUBLE)
    {
        //executeMouseDoubleEvent(dynamic_cast<QOE::QOE_MouseDouble*>(ti));
        QOE::QOE_MouseDouble qoe;
        qoe.copy(ti);
        executeMouseDoubleEvent(&qoe);
    }
    else if (ti->type() == QOE::QOE_MOUSE_WHEEL)
    {
        //executeWheelEvent(dynamic_cast<QOE::QOE_MouseWheel*>(ti));
        QOE::QOE_MouseWheel qoe;
        qoe.copy(ti);
        executeWheelEvent(&qoe);
    }
    //keyboard events
    else if (ti->type() == QOE::QOE_KEY_PRESS)
    {
        //executeKeyPressEvent(dynamic_cast<QOE::QOE_KeyPress*>(ti));
        QOE::QOE_KeyPress qoe;
        qoe.copy(ti);
        executeKeyPressEvent(&qoe);
    }
}

///
/// event executors
///

void QtEventExecutor::executeMousePressEvent(QOE::QOE_MousePress* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent)");
    assert(qoe);

    QWidget* widget = getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeMousePressEvent) Missing Widget: "
              << qoe->widget());
    }

    ///
    ///pre_execution
    ///
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent) Pre execution");
    pre_execution_mouse(qoe, widget);

    ///
    ///execution
    ///
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent) Mouse move.");
    //simulate mouse move
    simulateMouseHover(widget, lastPos_, widget->mapToGlobal ( qoe->position() ) );
    lastPos_ = widget->mapToGlobal ( qoe->position() );

    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent) Execute.");
    //execute event
    qoe->execute(widget);

    ///
    ///post_execution
    ///
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent) Post execution.");
    post_execution(qoe, widget);

    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMousePressEvent) Exit");
}

void QtEventExecutor::executeMouseReleaseEvent(QOE::QOE_MouseRelease* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMouseReleaseEvent)");

    //get the widget
    QWidget* widget = getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeMouseReleaseEvent) Missing Widget: "
              << qoe->widget());
    }

    ///
    ///pre_execution
    ///
    pre_execution_mouse(qoe, widget);

    ///
    ///execution
    ///
    //simulate mouse move
    simulateMouseHover (widget, lastPos_, widget->mapToGlobal ( qoe->position() ) );
    lastPos_ = widget->mapToGlobal ( qoe->position() );
    //execute event
    qoe->execute(widget);

    ///
    ///post_execution
    ///
    post_execution(qoe, widget);
}

void QtEventExecutor::executeMouseDoubleEvent(QOE::QOE_MouseDouble* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeMouseDoubleEvent)");
    //get the widget
    QWidget* widget = getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeMouseDoubleEvent) Missing Widget: "
              << qoe->widget());
    }

    ///
    ///pre_execution
    ///
    pre_execution_mouse(qoe, widget);

    ///
    ///execution
    ///
    //simulate mouse move
    simulateMouseHover (widget, lastPos_, widget->mapToGlobal ( qoe->position() ) );
    lastPos_ = widget->mapToGlobal ( qoe->position() );
    //execute event
    qoe->execute(widget);

    ///
    ///post_execution
    ///
    post_execution(qoe, widget);
}

void QtEventExecutor::executeKeyPressEvent(QOE::QOE_KeyPress* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeKeyPressEvent)");
    //get the widget
    QWidget* widget = getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeKeyPressEvent) Missing Widget: "
              << qoe->widget());
    }

    ///
    ///pre_execution
    ///
    pre_execution(qoe, widget);

    ///
    ///execution
    ///
    //execute event
    qoe->execute(widget);

    ///
    ///post_execution
    ///
    post_execution(qoe, widget);
}

void QtEventExecutor::executeCloseEvent(QOE::QOE_WindowClose* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeCloseEvent)");
    //get the widget
    QWidget* widget = getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeWindowCloseEvent) Missing Widget: "
              << qoe->widget());
    }

    ///
    ///pre_execution
    ///
    pre_execution(qoe, widget);

    ///
    ///execution
    ///
    //execute event
    qoe->execute(widget);

    ///
    ///post_execution
    ///
    post_execution(qoe, widget);
}

void QtEventExecutor::executeWheelEvent(QOE::QOE_MouseWheel* qoe)
{
    DEBUG(D_EXECUTOR,"(QtEventExecutor::executeWheelEvent)");
    //get the widget
    QWidget* widget = getWidget(qoe);
    if ( widget == NULL )
    {
        DEBUG(D_ERROR,"(QtEventExecutor::executeMouseWheelEvent) Missing Widget: "
              << qoe->widget());
    }

    ///
    ///pre_execution
    ///
    pre_execution_mouse(qoe, widget);

    ///
    ///execution
    ///
    //simulate mouse move
    simulateMouseHover (widget, lastPos_, widget->mapToGlobal ( qoe->position() ) );
    lastPos_ = widget->mapToGlobal ( qoe->position() );
    //execute event
    qoe->execute(widget);

    ///
    ///post_execution
    ///
    post_execution(qoe, widget);
}

//void executeKeyReleaseEvent ();
//void executeShowEvent ();
//void executeSpecialShowEvent ();
//void executeFocusInEvent ();
//void executeFocusOutEvent ();

///
///
///
/// private methods
///
///
///

///
/// mouse simulation
///

void QtEventExecutor::simulateMouseMove ( const QPoint& pBegin, const QPoint& pEnd )
{
    int ix = pBegin.x();
    int iy = pBegin.y();
    int dx = pEnd.x();
    int dy = pEnd.y();

    //if no moving...
    if ( ix == dx && iy == dy ) return;

    /*
     Initial position: QCursor::pos()
     End position: pEnd
     Movements: n
     Wait between movements: w
   */

    const int MOVEMENTS_FACTOR = 10;

    //adapt n and w values
    uint t = qMax ( qAbs ( dx - ix ), qAbs ( dy - iy ) );//longest track
    //uint n = t / MOVEMENTS_FACTOR + t % MOVEMENTS_FACTOR;
    //uint w = ( 2100 - ( MAX_DELAY_MS - t ) ) / n;
    uint n = t / MOVEMENTS_FACTOR + t % MOVEMENTS_FACTOR;
    uint w = MOUSE_MOVE_DELAY_MS / n;
    assert ( w > 0 );

    //go moving
    QPoint old ( ix, iy );
    const int STEP_X = (dx - ix) / n;
    const int STEP_Y = (dy - iy) / n;
    for ( uint i = 1; i < n; i++ )
    {
        QPoint current( STEP_X * i + ix, STEP_Y * i + iy );
        QCursor::setPos(current);
        QTest::qWait(w);
    }

    //end moving
    QCursor::setPos ( pEnd );
}

void QtEventExecutor::simulateMouseHover ( QWidget *widget,
                                           const QPoint &pBegin,
                                           const QPoint &pEnd )
{
    int ix = pBegin.x();
    int iy = pBegin.y();
    int dx = pEnd.x();
    int dy = pEnd.y();

    //if no moving...
    if ( ix == dx && iy == dy ) return;

    /*
     Initial position: QCursor::pos()
     End position: pEnd
     Movements: MOUSE_MOVE_STEPS
     Wait between movements: w
   */

    //adapt n and w values
    uint t = qMax ( qAbs ( dx - ix ), qAbs ( dy - iy ) );//longest movement
    uint w = MOUSE_MOVE_DELAY_MS / MOUSE_MOVE_STEPS;
    assert ( w > 0 );

    //go moving...
    QPoint old ( ix, iy );
    const int STEP_X = int ( double ( dx - ix ) / double ( MOUSE_MOVE_STEPS ) );
    const int STEP_Y = int ( double ( dy - iy ) / double ( MOUSE_MOVE_STEPS ) );
    for ( uint i = 1; i <= MOUSE_MOVE_STEPS; i++ )
    {
        //calculate current position
        int cx = STEP_X * i  + ix;
        int cy = STEP_Y * i  + iy;
        QPoint current ( cx, cy );

        //set the cursor
        QCursor::setPos ( current );
        QMouseEvent me ( QEvent::MouseMove,
                         widget->mapFromGlobal ( current ),
                         Qt::NoButton, Qt::LeftButton,
                         QApplication::keyboardModifiers() );
        qApp->notify ( dynamic_cast<QObject*> ( widget ),
                       dynamic_cast<QEvent*> ( &me ) );

        //wait
        QTest::qWait ( w );
    }

    //end moving...
    QCursor::setPos ( pEnd );
    QMouseEvent me ( QEvent::MouseMove,
                     widget->mapFromGlobal ( pEnd ),
                     Qt::NoButton, Qt::LeftButton,
                     QApplication::keyboardModifiers() );
    qApp->notify ( ( QObject* ) widget, ( QEvent* ) &me );
}

///
/// execution support
///

QWidget* QtEventExecutor::getWidget(QOE::QOE_Base* qoe)
{
    assert(qoe);
    QString wname = QString(qoe->widget().c_str());
    QStringList wpath = wname.split ( PATH_SEPARATOR );
    return QWidgetUtils::getAWidget(&wpath);
}

void QtEventExecutor::pre_execution_mouse(QOE::QOE_Base* qoe, QWidget* widget)
{
    assert(qoe);
    assert(widget);

    //wait elapsed time for this item
    int sleep_ms = qoe->timestamp() - MOUSE_MOVE_DELAY_MS;
    sleep_ms = sleep_ms > 0? sleep_ms : 0;
    QTest::qWait(sleep_ms);

    //start simulation
    widget->setUpdatesEnabled(false);
}

void QtEventExecutor::pre_execution(QOE::QOE_Base* qoe, QWidget* widget)
{
    assert(qoe);
    assert(widget);

    //start simulation
    widget->setUpdatesEnabled(false);
}

void QtEventExecutor::post_execution(QOE::QOE_Base* qoe, QWidget* widget)
{
    assert(qoe);
    assert(widget);

    //if sensitive -> set the sensitive data
    if ( qoe->isSensitive() )
    {
        QWA::QWidgetAdapter* qwa = qwaManager_.isSensitive(widget);
        qwa->applySensitiveValue(qoe->sensitiveValue());
    }

    //set focus
    QWidgetUtils::setFocusOnWidget(widget);
    //end simulation
    widget->setUpdatesEnabled ( true );
    widget->update();
}

