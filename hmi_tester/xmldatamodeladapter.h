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
#ifndef XMLDATAMODELADAPTER_H
#define XMLDATAMODELADAPTER_H

#include "datamodeladapter.h"
#include <QString>

class XMLDataModelAdapter : public DataModelAdapter
{
public:
    XMLDataModelAdapter();

    virtual DataModel::TestSuite*
    file2testSuite(const std::string& filename) throw (conversion_error);

    virtual void testSuite2file(const DataModel::TestSuite&,
                                const std::string& filename) throw (conversion_error);

    // XML Visitors
    QString pre_TestSuite(const DataModel::TestSuite&);
    QString visit_TestSuite(const DataModel::TestSuite&);
    QString post_TestSuite(const DataModel::TestSuite&);

    QString pre_TestCase(const DataModel::TestCase&);
    QString visit_TestCase(const DataModel::TestCase&);
    QString post_TestCase(const DataModel::TestCase&);

    QString pre_TestItem(const DataModel::TestItem&);
    QString visit_TestItem(const DataModel::TestItem&);
    QString post_TestItem(const DataModel::TestItem&);



protected:
    QString _visit_TestBase (const DataModel::TestBase& tc);

};

#endif // XMLDATAMODELADAPTER_H
