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

#include "qteventconsumer.h"
#include <debug.h>
#include <qwidgetutils.h>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>

QtEventConsumer::QtEventConsumer() : EventConsumer()
{
    //update flags
    f_recording_ = false;
    f_paused_ = false;
}

///
/// this method is called to install the event consumer
///
void QtEventConsumer::install()
{
    //intall the consumer as an event filter
    //in the tested application
    QApplication::instance()->installEventFilter (this);
    DEBUG(D_CONSUMER,"(QtEventConsumer::install) Event Consumer installed.");
}
///
///capture process control methods
///
void QtEventConsumer::startCapture()
{
    //update flags
    f_recording_ = true;
    f_paused_ = false;

    // timing stuff
    _timer.start();
}

void QtEventConsumer::pauseCapture()
{
    //update flags
    f_recording_ = true;
    f_paused_ = true;

    // timing stuff
    _timer.invalidate();
}

void QtEventConsumer::resumeCapture()
{
    //update flags
    f_recording_ = true;
    f_paused_ = true;

    // timing stuff
    _timer.start();
}

void QtEventConsumer::stopCapture()
{
    //update flags
    f_recording_ = false;
    f_paused_ = false;

    // timing stuff
    _timer.invalidate();
}


///
/// event filter method
///
bool QtEventConsumer::eventFilter ( QObject *obj, QEvent *event )
{
    ///
    /// process control
    ///

    //if process is stoped...
    if (f_recording_ == false)
    {
        //no event handling
        //DEBUG(D_CONSUMER,"(QtEventConsumer::eventFilter) Stop state. No event handled.");
        return false;
    }
    //if process is paused...
    else if (f_recording_ && f_paused_)
    {
        //filter all events and
        //block the application
        DEBUG(D_CONSUMER,"(QtEventConsumer::eventFilter) Pause state. Events blocked.");
        return true;
    }
    //if process is recording...
    else
    {
        //apply the filter...
    }

    ///
    ///filter depending on the type..
    ///
    //window events
    if (event->type() == QEvent::KeyPress)
    {
        handleKeyPressEvent(obj, event);
    }
    //mouse events
    else if (event->type() == QEvent::MouseButtonPress)
    {
        handleMousePressEvent(obj, event);
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        handleMouseReleaseEvent(obj, event);
    }
    else if (event->type() == QEvent::MouseButtonDblClick)
    {
        handleMouseDoubleEvent(obj, event);
    }
    else if (event->type() == QEvent::Wheel)
    {
        handleWheelEvent(obj, event);
    }
    //keyboard events
    else if (event->type() == QEvent::Close)
    {
        handleCloseEvent(obj, event);
    }

    ///the event should continue on its edge...
    return false;
}

///
/// event handlers
///
void QtEventConsumer::handleMousePressEvent ( QObject *obj, QEvent *event )
{
    DEBUG(D_CONSUMER,"(QtEventConsumer::handleMousePressEvent)");

    QMouseEvent *me = dynamic_cast< QMouseEvent*> ( event );

    //create the event
    QOE::QOE_MousePress qoe;// = new QOE::QOE_MousePress();
    QWidget *widget = static_cast<QWidget*>(obj);

    qoe.timestamp(_timer.restart());
    qoe.widget(QWidgetUtils::getWidgetPath (widget).toStdString());
    qoe.button(me->button());
    qoe.buttons(me->buttons());
    qoe.modifiers(me->modifiers());
    qoe.x(me->x());
    qoe.y(me->y());
    qoe.globalX(me->globalX());
    qoe.globalY(me->globalY());

    ///sensitive value
    completeSensitiveData(qoe, widget);

    //send event
    sendNewTestItem(qoe);
}

void QtEventConsumer::handleMouseReleaseEvent ( QObject *obj, QEvent *event )
{
    DEBUG(D_CONSUMER,"(QtEventConsumer::handleMouseReleaseEvent)");

    QMouseEvent *me = dynamic_cast< QMouseEvent*> ( event );

    //create the event
    QOE::QOE_MouseRelease qoe;// = new QOE::QOE_MouseRelease();
    QWidget *widget = static_cast<QWidget*>(obj);

    qoe.timestamp(_timer.restart());
    qoe.widget(QWidgetUtils::getWidgetPath (widget).toStdString());
    qoe.button(me->button());
    qoe.buttons(me->buttons());
    qoe.modifiers(me->modifiers());
    qoe.x(me->x());
    qoe.y(me->y());
    qoe.globalX(me->globalX());
    qoe.globalY(me->globalY());

    ///sensitive value
    completeSensitiveData(qoe, widget);

    //send event
    sendNewTestItem(qoe);
}

void QtEventConsumer::handleMouseDoubleEvent ( QObject *obj, QEvent *event )
{
    DEBUG(D_CONSUMER,"(QtEventConsumer::handleMouseDoubleEvent)");

    QMouseEvent *me = dynamic_cast< QMouseEvent*> ( event );

    //create the event
    QOE::QOE_MouseDouble qoe;// = new QOE::QOE_MouseDouble();
    QWidget *widget = static_cast<QWidget*>(obj);

    qoe.timestamp(_timer.restart());
    qoe.widget(QWidgetUtils::getWidgetPath (widget).toStdString());
    qoe.button(me->button());
    qoe.buttons(me->buttons());
    qoe.modifiers(me->modifiers());
    qoe.x(me->x());
    qoe.y(me->y());
    qoe.globalX(me->globalX());
    qoe.globalY(me->globalY());

    ///sensitive value
    completeSensitiveData(qoe, widget);

    //send event
    sendNewTestItem(qoe);
}

void QtEventConsumer::handleKeyPressEvent ( QObject *obj, QEvent *event )
{
    DEBUG(D_CONSUMER,"(QtEventConsumer::handleKeyPressEvent)");

    QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *> ( event );

    //if it is a recognized key
    if ( filterKeyEvent ( static_cast<Qt::Key> ( keyEvent->key() ) ) )
    {
        //create the event
        QOE::QOE_KeyPress qoe;// = new QOE::QOE_KeyPress();
        QWidget *widget = static_cast<QWidget*>(obj);
        QPoint p ( widget->x(), widget->y() );
        QPoint g = widget->mapToGlobal ( p );

        qoe.timestamp(_timer.restart());
        qoe.widget(QWidgetUtils::getWidgetPath (widget).toStdString());
        qoe.key(keyEvent->key());
        qoe.text(keyEvent->text());
        qoe.modifiers(keyEvent->modifiers());
        qoe.x(p.x());
        qoe.y(p.y());
        qoe.globalX(g.x());
        qoe.globalY(g.y());

        ///sensitive value
        completeSensitiveData(qoe, widget);

        //send event
        sendNewTestItem(qoe);
    }
}


void QtEventConsumer::handleCloseEvent ( QObject *obj, QEvent *event )
{
    DEBUG(D_CONSUMER,"(QtEventConsumer::handleCloseEvent)");

    //create the event
    QOE::QOE_WindowClose qoe;// = new QOE::QOE_WindowClose();
    QWidget *widget = static_cast<QWidget*>(obj);
    QPoint p ( widget->x(), widget->y() );
    QPoint g = widget->mapToGlobal ( p );

    qoe.timestamp(_timer.restart());
    qoe.widget(QWidgetUtils::getWidgetPath (widget).toStdString());
    qoe.x(p.x());
    qoe.y(p.y());
    qoe.globalX(g.x());
    qoe.globalY(g.y());

    //send event
    sendNewTestItem(qoe);
}

void QtEventConsumer::handleWheelEvent ( QObject *obj, QEvent *event )
{
    DEBUG(D_CONSUMER,"(QtEventConsumer::handleWheelEvent)");

    QWheelEvent *we = dynamic_cast<QWheelEvent*> ( event );

    //create the event
    QOE::QOE_MouseWheel qoe;// = new QOE::QOE_MouseWheel();
    QWidget *widget = static_cast<QWidget*>(obj);

    qoe.timestamp(_timer.restart());
    qoe.widget(QWidgetUtils::getWidgetPath (widget).toStdString());
    qoe.delta(we->delta());
    qoe.orientation(we->orientation());
    qoe.buttons(we->buttons());
    qoe.modifiers(we->modifiers());
    qoe.x(we->x());
    qoe.y(we->y());
    qoe.globalX(we->globalX());
    qoe.globalY(we->globalY());

    ///sensitive value
    completeSensitiveData(qoe, widget);

    //send event
    sendNewTestItem(qoe);
}

//void QtEventConsumer::handleKeyReleaseEvent ( QObject *obj, QEvent *event );
//void QtEventConsumer::handleShowEvent ( QObject *obj, QEvent *event );
//void QtEventConsumer::handleSpecialShowEvent ( QObject *obj, QEvent *event );
//void QtEventConsumer::handleFocusInEvent ( QObject *obj, QEvent *event );
//void QtEventConsumer::handleFocusOutEvent ( QObject *obj, QEvent *event );

///
///handler supporters
///
void QtEventConsumer::completeSensitiveData(QOE::QOE_Base& qoe, QWidget* widget)
{
    QWA::QWidgetAdapter* qwa = qwaManager_.isSensitive(widget);
    //if the widget is sensitive, set the value in the event
    if (qwa != NULL)
    {
        qoe.isSensitive(true);
        std::string value = qwa->sensitiveValue();
        qoe.sensitiveValue(value);
    }
    //if not...
    else
    {
        qoe.isSensitive(false);
    }
}

///
///support methods
///
bool QtEventConsumer::filterKeyEvent (Qt::Key k)
{
    ///FIXME review the filter
    /*
   ctrl 16777249 - 67108864;
   alt 16777251 - 134217728;
   altgr 16781571 - 0;
   shift 16777248 - 33554432;
   windows 16777299 - 0;
   Mayús 16777252 - 0;
   Fs:
   F1: 16777264 - 0
   F12: 16777275 - 0
   esc: 16777216
   <: 60
   >: 62
   */
    //Filter cases
    if ( k == 16777216 /*esc*/ ||
         k == 60 /*<*/ ||
         k == 62 /*>*/ ||
         k == 16777249 /*ctrl*/ ||
         k == 16777251 /*alt*/ ||
         k == 16781571 /*altGr*/ ||
         k == 16777248 /*shift*/ ||
         k == 16777299 /*windows*/ ||
         k == 16777252 /*mayus*/ ||
         //k == 16777264 /*F1*/ && k == 16777275 /*F12*/||
         k >= 16777276/*lower limit*/ ||
         k == 9472
         || k == 183
         ) return false;
    //if not filtered...
    return true;
}



