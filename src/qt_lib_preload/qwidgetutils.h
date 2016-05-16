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
#ifndef QWIDGETUTILS_H
#define QWIDGETUTILS_H

#include <QWidget>
#include <QStringList>

class QWidgetUtils
{
public:

    ///returns a widget
    ///it supports repeated widget names
    static QWidget* getAbsoluteWidget(QStringList);

    ///returns a widget
    static QWidget* getAWidget(QStringList*);

    ///updates the GUIactualiza todas las ventanas de la app
    static void updateAppView();

    ///returns an identifying path from a widget
    static QString getWidgetPath(QObject*);

    ///returns an identifying name from a widget
    static QString getWidgetName (QWidget*);

    ///change the focus to the widget
    static void setFocusOnWidget (QWidget*);
};

#endif // QWIDGETUTILS_H
