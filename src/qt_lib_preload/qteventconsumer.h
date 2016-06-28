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
#ifndef QTEVENTCONSUMER_H
#define QTEVENTCONSUMER_H

#include <eventconsumer.h>
#include <qwidgetadapter.h>
#include <qtownevents.h>
#include <QEvent>
#include <QPoint>
#include <QElapsedTimer>
#include <QInputEvent>

class QtEventConsumer : public EventConsumer
{

public:

    QtEventConsumer();

    ///
    /// this method is called to install the event consumer
    ///
    virtual void install();

    ///
    ///capture process control methods
    ///
    virtual void startCapture();
    virtual void pauseCapture();
    virtual void resumeCapture();
    virtual void stopCapture();

protected:
    ///
    /// event filter method
    ///
    bool eventFilter ( QObject *obj, QEvent *event );

private:
    ///
    /// event handlers
    ///
    void handleMousePressEvent ( QObject *obj, QEvent *event );
    void handleMouseReleaseEvent ( QObject *obj, QEvent *event );
    void handleMouseDoubleEvent ( QObject *obj, QEvent *event );
    void handleKeyPressEvent ( QObject *obj, QEvent *event );
    void handleCloseEvent ( QObject *obj, QEvent *event );
    void handleWheelEvent ( QObject *obj, QEvent *event );
    //void handleKeyReleaseEvent ( QObject *obj, QEvent *event );
    //void handleShowEvent ( QObject *obj, QEvent *event );
    //void handleSpecialShowEvent ( QObject *obj, QEvent *event );
    //void handleFocusInEvent ( QObject *obj, QEvent *event );
    //void handleFocusOutEvent ( QObject *obj, QEvent *event );

    ///
    ///handler supporters
    ///

    void completeBasicData(QOE::QOE_Base&, QWidget *w, QMouseEvent *e);
    void completeBasicData(QOE::QOE_Base&, QWidget *w);
    void completeSensitiveData(QOE::QOE_Base&, QWidget*);

    ///
    /// validation methods
    ///

    QWidget* isValidWidget(QObject *obj);
    bool isValidQOE(QOE::QOE_Base&qoe);

    ///
    ///support methods
    ///


    bool filterKeyEvent(Qt::Key);

    ///
    ///process control
    ///
    bool f_recording_;
    bool f_paused_;

    ///
    /// timing
    ///

    QElapsedTimer _timer;

    ///
    ///widget adapters manager
    ///
    QWA::QWidgetAdapterManager qwaManager_;

};

#endif // QTEVENTCONSUMER_H
