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
#ifndef QTOWNEVENTS_H
#define QTOWNEVENTS_H

#include <datamodel.h>
#include <QEvent>
#include <QPoint>
#include <QWidget>

namespace QOE
{

    ///
    /// constants
    ///

    ///
    ///EVENT TYPES
    ///
    const int QOE_DEFAULT = 0;
    //window events
    const int QOE_WINDOW_CLOSE = 1;
    //mouse events
    const int QOE_MOUSE_PRESS = 11;
    const int QOE_MOUSE_RELEASE = 12;
    const int QOE_MOUSE_DOUBLE = 13;
    const int QOE_MOUSE_WHEEL = 14;
    //keyboard events
    const int QOE_KEY_PRESS = 21;

    ///
    /// QOEvent test item base
    ///
    //constants
    const std::string QOE_Base_Widget = "widget";
    const std::string QOE_Base_WidgetValue = "wvalue";
    const std::string QOE_Base_WidgetWidth = "wwidth";
    const std::string QOE_Base_WidgetHeight = "wheight";
    const std::string QOE_Base_X = "x";
    const std::string QOE_Base_Y = "y";
    const std::string QOE_Base_GlobalX = "gx";
    const std::string QOE_Base_GlobalY = "gy";
    const std::string QOE_Base_Ms = "ms";
    const std::string QOE_Base_IsSensitive = "isSens";
    const std::string QOE_Base_SensitiveValue = "svalue";
    //class
    class QOE_Base : public DataModel::TestItem
    {
    public:
        //accesor
        std::string widget();
        void widget(const std::string&);

        std::string widgetValue();
        void widgetValue(const std::string&);

        int widgetWidth();
        void widgetWidth(const int);

        int widgetHeight();
        void widgetHeight(const int);

        int x();
        void x(int);

        int y();
        void y(int);

        QPoint position();

        int globalX();
        void globalX(int);

        int globalY();
        void globalY(int);

        QPoint globalPosition();

        QPoint adaptedPosition(QWidget* w);

        bool isSensitive();
        void isSensitive(bool);

        std::string sensitiveValue();
        void sensitiveValue(const std::string&);

        //command pattern
        virtual void execute(QWidget*) = 0;
    };

    ///
    /// QOEvent Window
    ///
    //constants
    //class
    class QOE_Window : public QOE_Base
    {
    public:

        //accesor

        //command pattern
        virtual void execute(QWidget*) = 0;
    };

    ///
    /// QOEvent Close
    ///
    //constants
    //class
    class QOE_WindowClose : public QOE_Window
    {
    public:
        //constructor
        QOE_WindowClose();

        //accesor

        //command
        virtual void execute(QWidget*);
    };

    ///
    /// QOEvent Mouse
    ///
    //constants
    const std::string QOE_Mouse_Button = "button";
    const std::string QOE_Mouse_Modifiers = "modifiers";
    const std::string QOE_Mouse_Buttons = "buttons";
    //class
    class QOE_Mouse : public QOE_Base
    {
    public:
        //accesor
        Qt::MouseButton button();
        void button(Qt::MouseButton);

        Qt::MouseButtons buttons();
        void buttons(Qt::MouseButtons);

        Qt::KeyboardModifiers modifiers();
        void modifiers(Qt::KeyboardModifiers);

        //command pattern
        virtual void execute(QWidget*) = 0;
    };

    ///
    /// QOEvent MousePress
    ///
    //constants
    //class
    class QOE_MousePress : public QOE_Mouse
    {
    public:
        //constructor
        QOE_MousePress();

        //command
        virtual void execute(QWidget*);
    };

    ///
    /// QOEvent MouseRelease
    ///
    //constants
    //class
    class QOE_MouseRelease : public QOE_Mouse
    {
    public:
        //constructor
        QOE_MouseRelease();

        //command
        virtual void execute(QWidget*);
    };

    ///
    /// QOEvent MouseDouble
    ///
    //constants
    //class
    class QOE_MouseDouble : public QOE_Mouse
    {
    public:
        //constructor
        QOE_MouseDouble();

        //command
        virtual void execute(QWidget*);
    };

    ///
    /// QOEvent MouseWheel
    ///
    //constants
    const std::string QOE_Mouse_Wheel_Delta = "delta";
    const std::string QOE_Mouse_Wheel_Orientation = "orientation";
    //class
    class QOE_MouseWheel : public QOE_Mouse
    {
    public:
        //constructor
        QOE_MouseWheel();

        //accesor
        int delta();
        void delta(int);

        Qt::Orientation orientation();
        void orientation(Qt::Orientation);

        //command
        virtual void execute(QWidget*);
    };

    ///
    /// QOEvent Key
    ///
    //constants
    const std::string QOE_Key_Key = "key";
    const std::string QOE_Key_Modifiers = "modifiers";
    const std::string QOE_Key_Text = "text";
    //class
    class QOE_Key : public QOE_Base
    {
    public:
        //accesor
        int key();
        void key(int);

        QString text();
        void text(const QString&);

        Qt::KeyboardModifiers modifiers();
        void modifiers(Qt::KeyboardModifiers);

        //command pattern
        virtual void execute(QWidget*) = 0;
    };

    ///
    /// QOEvent KeyPress
    ///
    //constants
    //class
    class QOE_KeyPress : public QOE_Key
    {
    public:
        //accesor
        QOE_KeyPress();

        //command
        virtual void execute(QWidget*);
    };


}


#endif // QTOWNEVENTS_H
