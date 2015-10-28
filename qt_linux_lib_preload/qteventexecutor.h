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
#ifndef QTEVENTEXECUTOR_H
#define QTEVENTEXECUTOR_H

#include <eventexecutor.h>
#include <qwidgetadapter.h>
#include <qtownevents.h>
#include <QWidget>
#include <QTest>

class QtEventExecutor : public EventExecutor
{
public:
    QtEventExecutor();

    ///
    /// this method is called to install the event executor
    ///
    virtual void install();

    ///
    ///execution process control methods
    ///
    virtual void startExecution();
    virtual void pauseExecution();
    virtual void resumeExecution();
    virtual void stopExecution();

    ///
    /// this method is called when a new testItem arrives
    ///
    virtual void handleNewTestItemReceived(DataModel::TestItem*);

    ///
    /// event executors
    ///
    void executeMousePressEvent(QOE::QOE_MousePress*);
    void executeMouseReleaseEvent(QOE::QOE_MouseRelease*);
    void executeMouseDoubleEvent(QOE::QOE_MouseDouble*);
    void executeKeyPressEvent(QOE::QOE_KeyPress*);
    void executeCloseEvent(QOE::QOE_WindowClose*);
    void executeWheelEvent(QOE::QOE_MouseWheel*);
    //void executeKeyReleaseEvent ();
    //void executeShowEvent ();
    //void executeSpecialShowEvent ();
    //void executeFocusInEvent ();
    //void executeFocusOutEvent ();

private:

    ///
    ///process control
    ///
    bool f_executing_;
    bool f_paused_;

    ///
    /// mouse simulation
    ///
    ///
    const int MOUSE_MOVE_DELAY_MS = 500;
    const int MOUSE_MOVE_STEPS = 18;
    QPoint _last_mouse_pos;
    void _simulateMouseMove(const QPoint&, const QPoint&, QWidget *hoverOnWidget = NULL);
    //void _simulateMouseHover( QWidget*, const QPoint&, const QPoint&);

    ///
    /// execution support
    ///
    QWidget* _getWidget(QOE::QOE_Base*);
    void _preExecution(QOE::QOE_Base*, QWidget*);
    void _preExecutionWithMouseMove(QOE::QOE_Base*, QWidget*);
    void _preExecutionWithMouseHover(QOE::QOE_Base*, QWidget*);
    void _postExecution(QOE::QOE_Base*, QWidget*);

    ///
    ///widget adapters manager
    ///
    QWA::QWidgetAdapterManager qwaManager_;
};

#endif // QTEVENTEXECUTOR_H
