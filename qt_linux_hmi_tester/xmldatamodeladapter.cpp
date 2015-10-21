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

#include "xmldatamodeladapter.h"

#include <QDomDocument>
#include <QFile>
#include <iostream>
#include <cassert>
#include <debug.h>

XMLDataModelAdapter::XMLDataModelAdapter()
{
}



///
std::string XMLDataModelAdapter::id()
{
    return "XMLDA";
}


/// ///
/// xml names
/// ///

const QString DATA_VALUE = "data";
const QString META_VALUE = "meta";
const QString TESTSUITE = "TestSuite";
const QString TESTCASE = "TestCase";
const QString TESTITEM = "TestItem";
const QString TS_ID = "appId";
const QString TSC_NAME = "name";
const QString TI_TYPE = "type";
const QString TI_SUBTYPE = "subtype";
const QString KEY = "key";
const QString VALUE = "value";

/// ///
/// xml tags
/// ///

const QString BEGo = "<";
const QString BEGc = "</";
const QString ENDo = ">";
const QString ENDc = ">\n";
const QString ENDs = "/>\n";

/// ///
///
/// XML to TestSuite
///
/// ///

DataModel::TestSuite*
        XMLDataModelAdapter::file2testSuite(const std::string& filename)
        throw (DataModelAdapter::conversion_error_exception)
{
    DataModel::TestSuite* tsuite = new DataModel::TestSuite();

    ///1. create a DOM document from a file
    QDomDocument doc ( "mydocument" );
    QFile file (filename.c_str());
    //if the file can not be opened...
    if ( !file.open( QIODevice::ReadOnly ) )
    {
        std::cout << "(XMLDataModelAdapter::file2testSuite) ERROR while opening the file " << filename << "." << std::endl;
        throw DataModelAdapter::conversion_error_exception();
    }
    //if the content can not be set...
    if ( !doc.setContent ( &file ) )
    {
        file.close();
        std::cout << "(XMLDataModelAdapter::file2testSuite) ERROR while setting the content from file " << filename << "." << std::endl;
        throw DataModelAdapter::conversion_error_exception();
    }
    file.close();

    ///2. Create a TestSuite
    QDomElement docElem = doc.documentElement();

    //TestSuite content
    for ( QDomNode n1 = docElem.firstChild(); !n1.isNull(); n1 = n1.nextSibling() )
    {
        QDomElement e1 = n1.toElement();
        if ( !e1.isNull() )
        {
            //TestSuite - name
            if ( e1.tagName() == TSC_NAME )
            {
                tsuite->name(e1.text().toStdString());
            }
            //TestSuite - appId
            else if ( e1.tagName() == TS_ID )
            {
                tsuite->appId(e1.text().toStdString());
            }
            //TestSuite - data
            else if ( e1.tagName() == DATA_VALUE )
            {
                QString key = e1.attribute ( KEY );
                QString value = e1.attribute ( VALUE );
                tsuite->addData(key.toStdString(), value.toStdString());
            }
            //TestSuite - metadata
            else if ( e1.tagName() == META_VALUE )
            {
                QString key = e1.attribute ( KEY );
                QString value = e1.attribute ( VALUE );
                tsuite->addMetadata(key.toStdString(), value.toStdString());
            }
            //TestCase content
            else if ( e1.tagName() == TESTCASE )
            {
                DataModel::TestCase* tcase = new DataModel::TestCase();

                for ( QDomNode n2 = n1.firstChild(); !n2.isNull(); n2 = n2.nextSibling() )
                {
                    QDomElement e2 = n2.toElement();
                    if ( !e2.isNull() )
                    {
                        //TestCase - name
                        if ( e2.tagName() == TSC_NAME )
                        {
                            tcase->name(e2.text().toStdString());
                        }
                        //TestCase - data
                        else if ( e2.tagName() == DATA_VALUE )
                        {
                            QString key = e2.attribute ( KEY );
                            QString value = e2.attribute ( VALUE );
                            tcase->addData(key.toStdString(), value.toStdString());
                        }
                        //TestCase - meta
                        else if ( e2.tagName() == META_VALUE )
                        {
                            QString key = e2.attribute ( KEY );
                            QString value = e2.attribute ( VALUE );
                            tcase->addMetadata(key.toStdString(), value.toStdString());
                        }
                        //TestItem content
                        else if ( e2.tagName() == TESTITEM )
                        {
                            DataModel::TestItem* titem = new DataModel::TestItem();

                            for ( QDomNode n3 = n2.firstChild(); !n3.isNull(); n3 = n3.nextSibling() )
                            {
                                QDomElement e3 = n3.toElement();
                                if ( !e3.isNull() )
                                {
                                    //TestItem - type
                                    if ( e3.tagName() == TI_TYPE )
                                    {
                                        QString value = e3.text();
                                        bool ok = false;
                                        titem->type(value.toInt(&ok));
                                        assert(ok);
                                    }
                                    //TestItem - subtype
                                    else if ( e3.tagName() == TI_SUBTYPE )
                                    {
                                        QString value = e3.text();
                                        bool ok = false;
                                        titem->subtype(value.toInt(&ok));
                                        assert(ok);
                                    }
                                    //TestItem - data
                                    else if ( e3.tagName() == DATA_VALUE )
                                    {
                                        QString key = e3.attribute ( KEY );
                                        QString value = e3.attribute ( VALUE );
                                        titem->addData(key.toStdString(), value.toStdString());
                                    }
                                    //TestItem - meta
                                    else if ( e3.tagName() == META_VALUE )
                                    {
                                        QString key = e3.attribute ( KEY );
                                        QString value = e3.attribute ( VALUE );
                                        titem->addMetadata(key.toStdString(), value.toStdString());
                                    }
                                }
                            }

                            //adding testItem to the testCase
                            tcase->addTestItem(titem);

                        }//end testItem content
                    }
                }

                //adding the test case to the test suite
                tsuite->addTestCase(tcase);
            }//end testCase content
        }
    }//end TestSuite content

    //return
    return tsuite;
}

/// ///
///
/// TestSuite to XML File
///
/// ///
void XMLDataModelAdapter::testSuite2file(const DataModel::TestSuite& ts,
                                         const std::string& filename)
throw (DataModelAdapter::conversion_error_exception)
{
    //geting the content of the file
    QString content = pre_TestSuite(ts) + visit_TestSuite(ts) + post_TestSuite(ts);

    //creating the file
    QFile file(filename.c_str());
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw DataModelAdapter::conversion_error_exception();

    file.write(content.toStdString().c_str(), qstrlen(content.toStdString().c_str()));
    file.close();
}


/// ///
///
/// XML Visitors
///
/// ///

///
/// TestBase
///

QString XMLDataModelAdapter::_visit_TestBase (const DataModel::TestBase& tc)
{
    QString xml;

    ///
    /// maps
    ///

    DataModel::KeyValueMap::const_iterator it;
    const DataModel::KeyValueMap& kv1 = tc.dataMap();

    //data
    for(it= kv1.begin(); it != kv1.end(); ++it)
    {
        xml += BEGo + DATA_VALUE + " " +
               KEY + "=\"" + QString(it->first.c_str()) + "\"" + " " +
               VALUE + "=\"" + QString(it->second.c_str()) + "\"" +
               ENDs;
    }

    const DataModel::KeyValueMap& kv2 = tc.metadataMap();

    //meta
    for(it= kv2.begin(); it != kv2.end(); ++it)
    {
        xml += BEGo + META_VALUE + " " +
               KEY + "=\"" + QString(it->first.c_str()) + "\"" +
               VALUE + "=\"" + QString(it->second.c_str()) + "\"" +
               ENDs;
    }

    return xml;
}

///
/// TestSuite
///
QString XMLDataModelAdapter::pre_TestSuite(const DataModel::TestSuite&)
{
    return BEGo + TESTSUITE + ENDc;
}

QString XMLDataModelAdapter::visit_TestSuite(const DataModel::TestSuite& ts)
{
    QString xml;

    //name
    xml += BEGo + TSC_NAME + ENDo +
           ts.name().c_str() +
           BEGc + TSC_NAME + ENDc;

    //binary path
    xml += BEGo + TS_ID + ENDo +
           ts.appId().c_str() +
           BEGc + TS_ID + ENDc;

    //maps
    xml += _visit_TestBase (ts);

    // children
    const DataModel::TestSuite::TestCaseList& tcl = ts.testCases();
    DataModel::TestSuite::TestCaseList::const_iterator it2;
    for(it2= tcl.begin(); it2 != tcl.end(); ++it2)
    {
        xml += pre_TestCase(*it2) + visit_TestCase(*it2) + post_TestCase(*it2);
    }

    //return
    return xml;
}

QString XMLDataModelAdapter::post_TestSuite(const DataModel::TestSuite&)
{
    return BEGc + TESTSUITE + ENDc;
}

///
/// TestCase
///
QString XMLDataModelAdapter::pre_TestCase(const DataModel::TestCase&)
{
    return BEGo + TESTCASE + ENDc;
}

QString XMLDataModelAdapter::visit_TestCase(const DataModel::TestCase& tc)
{
    QString xml;

    //name
    xml += BEGo + TSC_NAME + ENDo +
           tc.name().c_str() +
           BEGc + TSC_NAME + ENDc;

    // maps
    xml += _visit_TestBase (tc);

    // children
    const DataModel::TestCase::TestItemList& il = tc.testItemList();
    DataModel::TestCase::TestItemList::const_iterator it2;
    for(it2= il.begin(); it2 != il.end(); ++it2)
    {
        xml += pre_TestItem(*it2) + visit_TestItem(*it2) + post_TestItem(*it2);
    }

    //return
    return xml;
}

QString XMLDataModelAdapter::post_TestCase(const DataModel::TestCase&)
{
    return BEGc + TESTCASE + ENDc;
}

///
/// TestItem
///
QString XMLDataModelAdapter::pre_TestItem(const DataModel::TestItem&)
{
    return BEGo + TESTITEM + ENDc;
}

QString XMLDataModelAdapter::visit_TestItem(const DataModel::TestItem& ti)
{
    QString xml;

    //type
    xml += BEGo + TI_TYPE + ENDo +
           QString::number(ti.type()) +
           BEGc + TI_TYPE + ENDc;

    //subtype
    xml += BEGo + TI_SUBTYPE + ENDo +
           QString::number(ti.subtype()) +
           BEGc + TI_SUBTYPE + ENDc;


    xml += _visit_TestBase (ti);

    //return
    return xml;
}

QString XMLDataModelAdapter::post_TestItem(const DataModel::TestItem&)
{
    return BEGc + TESTITEM + ENDc;
}

