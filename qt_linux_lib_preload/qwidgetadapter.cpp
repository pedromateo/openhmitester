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

#include "qwidgetadapter.h"
#include <cassert>
#include <boost/lexical_cast.hpp>
#include <debug.h>


using namespace QWA;
///
/// Adapter base class
///

///
/// Adapter manager
///

//constructor
QWidgetAdapterManager::QWidgetAdapterManager()
{
    addAdapter(new QSpinBoxAdapter());
    addAdapter(new QDoubleSpinBoxAdapter());
    addAdapter(new QTimeEditAdapter());
    addAdapter(new QDateEditAdapter());
    addAdapter(new QDateTimeEditAdapter());
    addAdapter(new QDialAdapter());
    addAdapter(new QComboBoxAdapter());
    addAdapter(new QFontComboBoxAdapter());
}

//add adapter
void QWidgetAdapterManager::addAdapter(QWidgetAdapter* qwa)
{
    assert(qwa);
    adapters_[qwa->className()] = qwa;
}

//check if sensitive and returns an
//adapter if true (null if not)
QWidgetAdapter* QWidgetAdapterManager::isSensitive(QWidget* w)
{
    //get the name of the class
    std::string wclass = w->metaObject()->className();

    //return the adapter (NULL if not exists)
    AdapterSet::iterator it = adapters_.find(wclass);
    //if the adapter exists...
    if (it != adapters_.end())
    {
        //and the widget can be set...
        QWidgetAdapter* adapter = *it->second;
        if (adapter->setWidget(w))
        {
            return adapter;
        }
    }
    //if not... return NULL
    return NULL;
}

///
/// QComboBoxAdapter
///

//constructor
QComboBoxAdapter::QComboBoxAdapter()
{
    className_ = QComboBox().metaObject()->className();
}

//config methods
bool QComboBoxAdapter::setWidget(QWidget* w)
{
    widget_ = dynamic_cast<QComboBox*>(w);
    return widget_;
}

std::string QComboBoxAdapter::className()
{
    return className_;
}

//adapter methods
std::string QComboBoxAdapter::sensitiveValue()
{
    assert(widget_);
    return boost::lexical_cast<std::string>(widget_->currentIndex());
}

void QComboBoxAdapter::applySensitiveValue(const std::string& s)
{
    assert(widget_);
    widget_->setCurrentIndex(boost::lexical_cast<int>(s));
}

///
/// QFontComboBoxAdapter
///

//constructor
QFontComboBoxAdapter::QFontComboBoxAdapter()
{
    className_ = QFontComboBox().metaObject()->className();
}

//config methods
bool QFontComboBoxAdapter::setWidget(QWidget* w)
{
    widget_ = dynamic_cast<QFontComboBox*>(w);
    return widget_;
}

std::string QFontComboBoxAdapter::className()
{
    return className_;
}

///
/// QAbstractSliderAdapter
///

//adapter methods
std::string QAbstractSliderAdapter::sensitiveValue()
{
    assert(widget_);
    return boost::lexical_cast<std::string>(widget_->value());
}

void QAbstractSliderAdapter::applySensitiveValue(const std::string& s)
{
    assert(widget_);
    widget_->setValue(boost::lexical_cast<int>(s));
}

///
/// QDialAdapter
///

//constructor
QDialAdapter::QDialAdapter()
{
    className_ = QDial().metaObject()->className();
}

//config methods
bool QDialAdapter::setWidget(QWidget* w)
{
    widget_ = dynamic_cast<QDial*>(w);
    return widget_;
}

std::string QDialAdapter::className()
{
    return className_;
}

///
/// QSpinBoxAdapter
///

//constructor
QSpinBoxAdapter::QSpinBoxAdapter()
{
    className_ = QSpinBox().metaObject()->className();
}

//config methods
bool QSpinBoxAdapter::setWidget(QWidget* w)
{
    widget_ = dynamic_cast<QSpinBox*>(w);
    return widget_;
}

std::string QSpinBoxAdapter::className()
{
    return className_;
}

//adapter methods
std::string QSpinBoxAdapter::sensitiveValue()
{
    assert(widget_);
    return boost::lexical_cast<std::string>(widget_->value());
}

void QSpinBoxAdapter::applySensitiveValue(const std::string& s)
{
    assert(widget_);
    widget_->setValue(boost::lexical_cast<int>(s));
}

///
/// QDoubleSpinBoxAdapter
///

//constructor
QDoubleSpinBoxAdapter::QDoubleSpinBoxAdapter()
{
    className_ = QDoubleSpinBox().metaObject()->className();
}

//config methods
bool QDoubleSpinBoxAdapter::setWidget(QWidget* w)
{
    widget_ = dynamic_cast<QDoubleSpinBox*>(w);
    return widget_;
}

std::string QDoubleSpinBoxAdapter::className()
{
    return className_;
}

//adapter methods
std::string QDoubleSpinBoxAdapter::sensitiveValue()
{
    assert(widget_);
    return boost::lexical_cast<std::string>(widget_->value());
}

void QDoubleSpinBoxAdapter::applySensitiveValue(const std::string& s)
{
    assert(widget_);
    widget_->setValue(boost::lexical_cast<double>(s));
}

///
/// QDateTimeEditAdapter
///

//constructor
QDateTimeEditAdapter::QDateTimeEditAdapter()
{
    className_ = QDateTimeEdit().metaObject()->className();
}


//config methods
bool QDateTimeEditAdapter::setWidget(QWidget* w)
{
    widget_ = dynamic_cast<QDateTimeEdit*>(w);
    return widget_;
}

std::string QDateTimeEditAdapter::className()
{
    return className_;
}


//adapter methods
std::string QDateTimeEditAdapter::sensitiveValue()
{
    assert(widget_);
    return widget_->dateTime().toString(Qt::ISODate).toStdString();
}

void QDateTimeEditAdapter::applySensitiveValue(const std::string& s)
{
    assert(widget_);
    widget_->dateTime().fromString(QString(s.c_str()),Qt::ISODate);
}

///
/// QDateEditAdapter
///

//constructor
QDateEditAdapter::QDateEditAdapter()
{
    className_ = QDateEdit().metaObject()->className();
}


//config methods
bool QDateEditAdapter::setWidget(QWidget* w)
{
    widget_ = dynamic_cast<QDateEdit*>(w);
    return widget_;
}

std::string QDateEditAdapter::className()
{
    return className_;
}


//adapter methods
std::string QDateEditAdapter::sensitiveValue()
{
    assert(widget_);
    return widget_->date().toString(Qt::ISODate).toStdString();
}

void QDateEditAdapter::applySensitiveValue(const std::string& s)
{
    assert(widget_);
    widget_->date().fromString(QString(s.c_str()),Qt::ISODate);
}

///
/// QTimeEditAdapter
///

//constructor
QTimeEditAdapter::QTimeEditAdapter()
{
    className_ = QTimeEdit().metaObject()->className();
}

//config methods
bool QTimeEditAdapter::setWidget(QWidget* w)
{
    widget_ = dynamic_cast<QTimeEdit*>(w);
    return widget_;
}

std::string QTimeEditAdapter::className()
{
    return className_;
}

//adapter methods
std::string QTimeEditAdapter::sensitiveValue()
{
    assert(widget_);
    return widget_->time().toString(Qt::ISODate).toStdString();
}

void QTimeEditAdapter::applySensitiveValue(const std::string& s)
{
    assert(widget_);
    widget_->time().fromString(QString(s.c_str()),Qt::ISODate);
}


