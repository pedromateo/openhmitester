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

#include "qwidgetutils.h"

#include <QStringList>
#include <QApplication>
#include <cassert>
#include <iostream>
#include <debug.h>

///
///returns a widget
///it supports repeated widget names
///
QWidget* QWidgetUtils::getAbsoluteWidget(QStringList path)
{
    DEBUGc("(QWidgetUtils::getAbsoluteWidget)");
    ///update the view
    QWidgetUtils::updateAppView();

    ///get all the widgets
    QWidgetList qwl = qApp->allWidgets();

    ///get the widget name
    assert ( path.size() );
    QString name = path.back();
    if ( name == "" )
    {
        std::cout << "(QWidgetUtils::getAbsoluteWidget) ERROR. The name of the widget is empty." << std::endl;
        return NULL;
    }
    path.pop_back();

    //DEBUGc("(QWidgetUtils::getAbsoluteWidget) Name = " << name.toStdString());


    ///get all widgets with this name
    QWidgetList qws;
    foreach ( QWidget *w, qwl )
    {
        if ( getWidgetName ( w ) == name )
        {
            qws.push_back ( w );
        }
    }
    //DEBUGc("(QWidgetUtils::getAbsoluteWidget) OK");
    //if there are no selected widgets...
    if ( qws.size() == 0 )
    {
        DEBUGc("(QWidgetUtils::getAbsoluteWidget) ERROR. There are no selected widgets.");
        return NULL;
    }
    //DEBUGc("(QWidgetUtils::getAbsoluteWidget) OK");

    ///get the concrete widget we are looking for...
    int level = 1;
    bool onTopLevel = false;
    int onTopLevelCount = 0;
    QWidgetList::iterator qit;

    /*for (qit = qws.begin(); qit != qws.end(); qit++)
    {
        QWidget* w = static_cast<QWidget*> ( *qit );
        DEBUGc(getWidgetPath(w).toStdString());
    }*/

    //while more than 1 widget remaining
    //or every widget has been analyzed...
    while ( qws.size() > 1 && onTopLevelCount < qws.size() )
    {

        //DEBUGc("(QWidgetUtils::getAbsoluteWidget) OK -> " << qws.size());
        //for the remaining widgets...
        qit = qws.begin();
        while ( qit != qws.end() )
        {
            //get the parent widget
            QWidget* w = static_cast<QWidget*> ( *qit );
            QWidget* wParent = w;
            assert ( wParent );
            onTopLevel = false;

            for ( int i = 0; i < level; i++ )
            {
                //if we are in a top level widget (with no parent node)
                //we have to avoid this widget
                if ( wParent->parent() == NULL )
                {
                    onTopLevel = true;
                    onTopLevelCount++;
                    break;
                }
                //if not, we go on looking for up
                else
                {
                    wParent = static_cast<QWidget*> ( wParent->parent() );
                }
            }

            //if not at top level...
            if ( !onTopLevel )
            {
                //we chech the parent name and delete the
                //widgets whose parent name does not mach
                if ( getWidgetName ( wParent ) != path.back() )
                {
                    qit = qws.erase ( qit );
                }
                else
                {
                    qit++;
                }
            }
            else
            {
                qit++;
            }
        }

        //update the values
        level++;
        path.pop_back();

        //stop condition (end of the search path)
        if ( path.size() == 0 ) break;
    }

    //DEBUGc("(QWidgetUtils::getAbsoluteWidget) OK");

    //there has to be only one element at the list
    if ( qws.size() != 1 )
    {
        std::cout <<
                "(QWidgetUtils::getAbsoluteWidget) ERROR. None or more than one widget selected. Result = "
                << qws.size() << std::endl;
        return NULL;
    }

    //if not, return the widget
    return qws.first();
}

///
///returns a widget
///
QWidget* QWidgetUtils::getAWidget(QStringList* path)
{
    QWidget* wanted = NULL;

    //if it is not null, get a widget using its path
    if ( path != NULL )
    {
        wanted = getAbsoluteWidget ( *path );
        return wanted;
    }
    //else
    return NULL;
}

///
///updates the GUI
///
void QWidgetUtils::updateAppView()
{
    foreach ( QWidget *widget, QApplication::allWidgets() )
        widget->update();
}

///
///returns an identifying path from a widget
///
QString QWidgetUtils::getWidgetPath(QObject* o)
{
    assert ( o );
    //get the object path
    QString opath = "";

    if ( o->isWidgetType() )
    {
        QWidget *w = dynamic_cast<QWidget*> ( o );

        if ( w->objectName() == "" ) w->setObjectName ( getWidgetName ( w ) );

        opath = w->objectName();

        while ( w->parent() != NULL )
        {
            w = dynamic_cast<QWidget*> ( w->parent() );

            if ( w->objectName() == "" ) w->setObjectName ( getWidgetName ( w ) );

            opath = w->objectName() + "/" + opath;
        }
    }

    return opath;
}

///
///returns an identifying name from a widget
///
QString QWidgetUtils::getWidgetName ( QWidget * w )
{
    ///if the widget has a name, return it
    if ( w->objectName() != "" ) return w->objectName();

    ///else, we return a an alternative name composed of:
    /// - a special character
    /// - the class name
    /// - the position repect to the parent
    /// - the widget global position (not used)

    //add a special character
    QString name = QString ( "!" ) + w->metaObject()->className();

    //child position
    if ( w->parent() != NULL )
    {
        bool found = false;
        int i = 0;
        foreach ( QObject *o, w->parent()->children() )
        {
            QWidget *aux = ( QWidget* ) o;

            if ( aux == w )
            {
                found = true;
                break;
            }
            i++;
        }
        assert ( found );
        name = name + "_" + QString::number ( i );
    }

    //return the name
    return name;
}

///
///change the focus to the widget
///
void QWidgetUtils::setFocusOnWidget (QWidget *w )
{
    //activamos la ventana
    w->activateWindow();
    //pasamos el foco al widget
    w->setFocus ( Qt::MouseFocusReason );
}

