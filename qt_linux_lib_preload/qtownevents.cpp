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

#include "qtownevents.h"

#include <QString>
#include <QApplication>
#include <QCloseEvent>
#include <QTest>
#include <boost/lexical_cast.hpp>

#include <debug.h>




using namespace QOE;

///
/// QOEvent test item base
///

std::string QOE_Base::widget()
{
    return getData(QOE_Base_Widget);
}
void QOE_Base::widget(const std::string& text)
{
    addData(QOE_Base_Widget,text);
}

std::string QOE_Base::widgetValue()
{
    return getData(QOE_Base_WidgetValue);
}
void QOE_Base::widgetValue(const std::string& text)
{
    addData(QOE_Base_WidgetValue,text);
}

int QOE_Base::x()
{
    return boost::lexical_cast<int>(getData(QOE_Base_X));
}
void QOE_Base::x(int n)
{
    addData(QOE_Base_X,boost::lexical_cast<std::string>(n));
}

int QOE_Base::y()
{
    return boost::lexical_cast<int>(getData(QOE_Base_Y));
}
void QOE_Base::y(int n)
{
    addData(QOE_Base_Y,boost::lexical_cast<std::string>(n));
}

QPoint QOE_Base::position()
{
    return QPoint(x(), y());
}

int QOE_Base::globalX()
{
    return boost::lexical_cast<int>(getData(QOE_Base_GlobalX));
}
void QOE_Base::globalX(int n)
{
    addData(QOE_Base_GlobalX,boost::lexical_cast<std::string>(n));
}

int QOE_Base::globalY()
{
    return boost::lexical_cast<int>(getData(QOE_Base_GlobalY));
}

void QOE_Base::globalY(int n)
{
    addData(QOE_Base_GlobalY,boost::lexical_cast<std::string>(n));
}

QPoint QOE_Base::globalPosition()
{
    return QPoint(globalX(), globalY());
}

bool QOE_Base::isSensitive()
{
    return boost::lexical_cast<bool>(getData(QOE_Base_IsSensitive));
}

void QOE_Base::isSensitive(bool b)
{
    addData(QOE_Base_IsSensitive,boost::lexical_cast<std::string>(b));
}

std::string QOE_Base::sensitiveValue()
{
    return getData(QOE_Base_SensitiveValue);
}

void QOE_Base::sensitiveValue(const std::string& text)
{
    addData(QOE_Base_SensitiveValue,text);
}

///
/// QOEvent Window
///

//acessor


///
/// QOEvent Close
///

//constructor
QOE_WindowClose::QOE_WindowClose()
{
    type(QOE_WINDOW_CLOSE);
    subtype(QOE_DEFAULT);
}

void QOE_WindowClose::execute(QWidget* w)
{
    QCloseEvent ce;
    qApp->notify ( dynamic_cast<QObject*> ( w ), dynamic_cast<QEvent*> ( &ce ) );
}

///
/// QOEvent Mouse
///

//accesor
Qt::MouseButton QOE_Mouse::button()
{
    return static_cast<Qt::MouseButton>(boost::lexical_cast<int>(getData(QOE_Mouse_Button)));
}

void QOE_Mouse::button(Qt::MouseButton n)
{
    addData(QOE_Mouse_Button,boost::lexical_cast<std::string>(n));
}

Qt::MouseButtons QOE_Mouse::buttons()
{
    return static_cast<Qt::MouseButtons>(boost::lexical_cast<int>(getData(QOE_Mouse_Buttons)));
}

void QOE_Mouse::buttons(Qt::MouseButtons n)
{
    addData(QOE_Mouse_Buttons,boost::lexical_cast<std::string>(n));
}

Qt::KeyboardModifiers QOE_Mouse::modifiers()
{
    return static_cast<Qt::KeyboardModifiers>(boost::lexical_cast<int>(getData(QOE_Mouse_Modifiers)));
}

void QOE_Mouse::modifiers(Qt::KeyboardModifiers n)
{
    addData(QOE_Mouse_Modifiers,boost::lexical_cast<std::string>(n));
}

///
/// QOEvent MousePress
///

//constructor
QOE_MousePress::QOE_MousePress()
{
    type(QOE_MOUSE_PRESS);
    subtype(QOE_DEFAULT);
}

void QOE_MousePress::execute(QWidget* w)
{
    QTest::mousePress ( w, (Qt::MouseButton) button(),
                        (Qt::KeyboardModifiers) modifiers(),
                        position() );
}

///
/// QOEvent MouseRelease
///

//constructor
QOE_MouseRelease::QOE_MouseRelease()
{
    type(QOE_MOUSE_RELEASE);
    subtype(QOE_DEFAULT);
}

void QOE_MouseRelease::execute(QWidget* w)
{
    QTest::mouseRelease ( w, (Qt::MouseButton) button(),
                        (Qt::KeyboardModifiers) modifiers(),
                        position() );
}

///
/// QOEvent MouseDouble
///

//constructor
QOE_MouseDouble::QOE_MouseDouble()
{
    type(QOE_MOUSE_DOUBLE);
    subtype(QOE_DEFAULT);
}

void QOE_MouseDouble::execute(QWidget* w)
{
    QTest::mouseDClick ( w, (Qt::MouseButton) button(),
                        (Qt::KeyboardModifiers) modifiers(),
                        position() );
}

///
/// QOEvent MouseWheel
///

//constructor
QOE_MouseWheel::QOE_MouseWheel()
{
    type(QOE_MOUSE_WHEEL);
    subtype(QOE_DEFAULT);
}

void QOE_MouseWheel::execute(QWidget* w)
{
    QWheelEvent we ( position(), globalPosition(), delta(), buttons(), modifiers(), orientation() );
    qApp->notify ( dynamic_cast<QObject*> ( w ), dynamic_cast<QEvent*> ( &we ) );
}

//accesor
int QOE_MouseWheel::delta()
{
    return boost::lexical_cast<int>(getData(QOE_Mouse_Wheel_Delta));
}

void QOE_MouseWheel::delta(int n)
{
    addData(QOE_Mouse_Wheel_Delta,boost::lexical_cast<std::string>(n));
}

Qt::Orientation QOE_MouseWheel::orientation()
{
    return static_cast<Qt::Orientation>(boost::lexical_cast<int>(getData(QOE_Mouse_Wheel_Orientation)));
}

void QOE_MouseWheel::orientation(Qt::Orientation n)
{
    addData(QOE_Mouse_Wheel_Orientation,boost::lexical_cast<std::string>(n));
}

///
/// QOEvent Key
///

int QOE_Key::key()
{
    return boost::lexical_cast<int>(getData(QOE_Key_Key));
}

void QOE_Key::key(int n)
{
    addData(QOE_Key_Key,boost::lexical_cast<std::string>(n));
}

QString QOE_Key::text()
{
    return QString(getData(QOE_Key_Text).c_str());
}

void QOE_Key::text(const QString& s)
{
    addData(QOE_Key_Text,s.toStdString());
}

Qt::KeyboardModifiers QOE_Key::modifiers()
{
    return static_cast<Qt::KeyboardModifiers>(boost::lexical_cast<int>(getData(QOE_Key_Modifiers)));
}

void QOE_Key::modifiers(Qt::KeyboardModifiers n)
{
    addData(QOE_Key_Modifiers,boost::lexical_cast<std::string>(n));
}

///
/// QOEvent KeyPress
///

QOE_KeyPress::QOE_KeyPress()
{
    type(QOE_KEY_PRESS);
    subtype(QOE_DEFAULT);
}

void QOE_KeyPress::execute(QWidget* w)
{
    QTest::keyClick ( w, ( Qt::Key ) key(),
                      ( Qt::KeyboardModifiers ) modifiers());
}
