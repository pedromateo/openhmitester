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
#ifndef QWIDGETADAPTER_H
#define QWIDGETADAPTER_H

#include <QWidget>
#include <boost/ptr_container/ptr_map.hpp>

#include <QFontComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDial>
#include <QTimeEdit>
#include <QDateEdit>

namespace QWA
{
    ///
    /// Adapter base class
    ///
    class QWidgetAdapter
    {
    public:

        //link a widget to the adapter
        //returns false if it is a wrong widget
        virtual bool setWidget(QWidget*) = 0;

        //gets the adapter if the widget
        //is sensitive
        //returns NULL if no adapter available (not sensitive)
        virtual std::string className() = 0;

        //adapter methods
        virtual std::string sensitiveValue() = 0;
        virtual void applySensitiveValue(const std::string&) = 0;

    protected:

        //the name of the supported class
        std::string className_;
    };

    ///
    /// Adapter manager
    ///
    class QWidgetAdapterManager
    {
    public:

        //constructor
        QWidgetAdapterManager();

        //add adapter
        void addAdapter(QWidgetAdapter*);

        //check if sensitive and returns an
        //adapter if true (null if not)
        QWidgetAdapter* isSensitive(QWidget*);

    private:

        //adapter set
        typedef boost::ptr_map<std::string, QWidgetAdapter* > AdapterSet;
        AdapterSet adapters_;
    };

    ///
    /// QComboBoxAdapter
    ///
    class QComboBoxAdapter : public QWidgetAdapter
    {
    public:

        //constructor
        QComboBoxAdapter();

        //config methods
        virtual bool setWidget(QWidget*);
        virtual std::string className();

        //adapter methods
        virtual std::string sensitiveValue();
        virtual void applySensitiveValue(const std::string&);

    private:
        //widget reference
        QComboBox* widget_;
    };

    ///
    /// QFontComboBoxAdapter
    ///
    class QFontComboBoxAdapter : public QComboBoxAdapter
    {
    public:

        //constructor
        QFontComboBoxAdapter();

        //config methods
        virtual bool setWidget(QWidget*);
        virtual std::string className();

    private:
        //widget reference
        QFontComboBox* widget_;
    };

    ///
    /// QAbstractSliderAdapter
    ///
    class QAbstractSliderAdapter : public QWidgetAdapter
    {
    public:

        //config methods
        virtual bool setWidget(QWidget*) = 0;
        virtual std::string className() = 0;

        //adapter methods
        virtual std::string sensitiveValue();
        virtual void applySensitiveValue(const std::string&);

    protected:
        //widget reference
        QAbstractSlider* widget_;
    };

    ///
    /// QDialAdapter
    ///
    class QDialAdapter : public QAbstractSliderAdapter
    {
    public:

        //constructor
        QDialAdapter();

        //config methods
        virtual bool setWidget(QWidget*);
        virtual std::string className();
    };

    ///
    /// QSpinBoxAdapter
    ///
    class QSpinBoxAdapter : public QWidgetAdapter
    {
    public:

        //constructor
        QSpinBoxAdapter();

        //config methods
        virtual bool setWidget(QWidget*);
        virtual std::string className();

        //adapter methods
        virtual std::string sensitiveValue();
        virtual void applySensitiveValue(const std::string&);

    private:
        //widget reference
        QSpinBox* widget_;
    };

    ///
    /// QSpinBoxAdapter
    ///
    class QDoubleSpinBoxAdapter : public QWidgetAdapter
    {
    public:

        //constructor
        QDoubleSpinBoxAdapter();

        //config methods
        virtual bool setWidget(QWidget*);
        virtual std::string className();

        //adapter methods
        virtual std::string sensitiveValue();
        virtual void applySensitiveValue(const std::string&);

    private:
        //widget reference
        QDoubleSpinBox* widget_;
    };

    ///
    /// QDateTimeAdapter
    ///
    class QDateTimeEditAdapter : public QWidgetAdapter
    {
    public:

        //constructor
        QDateTimeEditAdapter();

        //config methods
        virtual bool setWidget(QWidget*);
        virtual std::string className();

        //adapter methods
        virtual std::string sensitiveValue();
        virtual void applySensitiveValue(const std::string&);

    private:
        //widget reference
        QDateTimeEdit* widget_;
    };

    ///
    /// QDateAdapter
    ///
    class QDateEditAdapter : public QWidgetAdapter
    {
    public:

        //constructor
        QDateEditAdapter();

        //config methods
        virtual bool setWidget(QWidget*);
        virtual std::string className();

        //adapter methods
        virtual std::string sensitiveValue();
        virtual void applySensitiveValue(const std::string&);

    private:
        //widget reference
        QDateEdit* widget_;
    };

    ///
    /// QDateTimeAdapter
    ///
    class QTimeEditAdapter : public QWidgetAdapter
    {
    public:

        //constructor
        QTimeEditAdapter();

        //config methods
        virtual bool setWidget(QWidget*);
        virtual std::string className();

        //adapter methods
        virtual std::string sensitiveValue();
        virtual void applySensitiveValue(const std::string&);

    private:
        //widget reference
        QTimeEdit* widget_;
    };
}

#endif // QWIDGETADAPTER_H
