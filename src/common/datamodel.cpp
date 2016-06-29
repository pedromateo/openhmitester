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

#include "datamodel.h"

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <algorithm>
#include <functional>
#include <cassert>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <debug.h>

using namespace DataModel;

///
/// test suite
///

TestSuite::TestSuite()
{
}

TestSuite::~TestSuite()
{
}

const TestSuite::TestCaseList&
TestSuite::testCases() const
{
    return testCases_;
}

void
TestSuite::addTestCase(DataModel::TestCase* ti)
{
    testCases_.push_back(ti);
    tcMap_.insert (std::make_pair (ti->name(), ti));
}

void
TestSuite::addTestCase(TestCaseList::iterator pos,
                       DataModel::TestCase* ti)
{
    testCases_.insert(pos, ti);
    tcMap_.insert (std::make_pair (ti->name(), ti));
}

void
TestSuite::deleteTestCase(TestCaseList::iterator ti) throw (not_found)
{
    try {
        testCases_.erase(ti);
        tcMap_.erase (ti->name());
    } catch (...) {
        throw not_found();
    }
}

// void TestSuite::deleteTestCase(uuid_t id) throw (not_found)
// {
//     TestCaseMap::iterator it = tcMap_.find (id);
//     if (it == tcMap_.end())
//         throw not_found();

//     tcMap_.erase (it);

//     // Remove it also from the list of pointers too
//     using namespace boost::lambda;
//     TestCaseList::iterator it2 =
//             std::find_if (testCases_.begin(), testCases_.end(),
//                           bind<bool> (std::equal_to<uuid_t>(),
//                                       bind<uuid_t> (&TestCase::uuid, _1),
//                                       id));
//     testCases_.erase (it2); // will always succeed.
// }

void TestSuite::deleteTestCase(const std::string& name) throw (not_found)
{
    TestCaseMap::iterator it = tcMap_.find (name);
    if (it == tcMap_.end())
        throw not_found();

    uuid_t id = it->second->uuid();

    //delete from the map
    tcMap_.erase (it);

    //delete from the list
    using namespace boost::lambda;
    TestCaseList::iterator it2 =
            std::find_if (testCases_.begin(),
                          testCases_.end(),
                          bind<bool> (std::equal_to<uuid_t>(),
                                           bind<uuid_t>(&TestCase::uuid, _1),
                                           id));
                          /*[&] (const TestCase& tc) {return tc.uuid() == id;});*/
    // TODO check if this code from above works as expected
    testCases_.erase (it2);
}

// DataModel::TestCase*
// TestSuite::getTestCase(uuid_t id) throw (not_found)
// {
//     TestCaseMap::const_iterator it = tcMap_.find (id);
//     if (it == tcMap_.end())
//         throw not_found();

//     return it->second;
// }

DataModel::TestCase*
TestSuite::getTestCase(const std::string& name) throw (not_found)
{
    // using namespace boost::lambda;
    // TestCaseList::iterator it =
    //         std::find_if (testCases_.begin(), testCases_.end(),
    //                       bind<bool>(std::equal_to<std::string>(),
    //                                  bind(static_cast<std::string (TestCase::*)() const>(&TestCase::name), _1),
    //                                  name)); // Tricky, but we have to disambiguate "TestCase::name"

    TestCaseMap::iterator it = tcMap_.find (name);

    if (it == tcMap_.end())
        throw not_found();

    return it->second;
}

// DataModel::TestCase*
// TestSuite::existsTestCase(uuid_t id)
// {
//     DataModel::TestCase* tc;
//     try {
//         tc = getTestCase(id);
//         return tc;
//     } catch (...)
//     {}
//     return 0;
// }

DataModel::TestCase*
TestSuite::existsTestCase(const std::string& name)
{
    DataModel::TestCase* tc;
    try {
        tc = getTestCase(name);
        return tc;
    } catch (...)
    {}
    return 0;
}


size_t TestSuite::count() const
{
    return testCases_.size();
}

std::string TestSuite::appId() const
{
    return appId_;
}

void TestSuite::appId(const std::string& s)
{
    appId_ = s;
}

std::string TestSuite::name() const
{
    return name_;
}

void TestSuite::name(const std::string& s)
{
    name_ = s;
}


///
/// test case
///

TestCase::TestCase()
    : uuid_ (U.uuid_new())
{
    // Initial name
    name_ = std::string ("TestCase (uuid ") +
            boost::lexical_cast<std::string>(uuid_) + ")";
}

TestCase::~TestCase()
{
}

uuid_t
TestCase::uuid() const
{
    return uuid_;
}

const TestCase::TestItemList&
TestCase::testItemList() const
{
    return testItems_;
}

void
TestCase::addTestItem(DataModel::TestItem* ti)
{
    testItems_.push_back(ti);
    itemMap_.insert (std::make_pair (ti->uuid(), ti));
}

void
TestCase::addTestItem(TestItemList::iterator pos,
                      DataModel::TestItem* ti)
{
    testItems_.insert(pos, ti);
    itemMap_.insert (std::make_pair (ti->uuid(), ti));
}

void
TestCase::deleteTestItem(TestItemList::iterator ti) throw (not_found)
{
    try {
        testItems_.erase(ti);
        itemMap_.erase (ti->uuid());
    } catch (...) {
        throw not_found();
    }
}

size_t
TestCase::count() const
{
    return testItems_.size();
}

std::string
TestCase::name() const
{
    return name_;
}

void
TestCase::name(const std::string& s)
{
    name_ = s;
}

void
TestCase::__syncMap()
{
    itemMap_.clear();

    for (TestItemList::iterator it = testItems_.begin();
         it != testItems_.end();
         ++it)
        itemMap_.insert (std::make_pair (it->uuid(), &*it));
}

///
/// test item
///

TestItem::TestItem()
    : uuid_ (U.uuid_new())
{
}

TestItem::TestItem(int type, int subtype, int timestamp = 0)
    : uuid_ (U.uuid_new()), type_ (type), subtype_ (subtype), timestamp_(timestamp)
{
}

TestItem::TestItem(DataModel::TestItem* ti)
    : uuid_ (U.uuid_new())
{
    ///
    ///type and subtype
    type(ti->type());
    subtype(ti->subtype());
    timestamp(ti->timestamp());

    dataMap_ = ti->dataMap_;
    metadataMap_ = ti->metadataMap_;
}


TestItem::~TestItem()
{
}

uuid_t
TestItem::uuid() const
{
    return uuid_;
}

int
TestItem::type() const
{
    return type_;
}

void
TestItem::type(int i)
{
    type_ = i;
}

int
TestItem::subtype() const
{
    return subtype_;
}

void TestItem::subtype(int i)
{
    subtype_ = i;
}

int
TestItem::timestamp() const
{
    return timestamp_;
}

void TestItem::timestamp(int i)
{
    timestamp_ = i;
}

///
///copy methods
///
void TestItem::copy(DataModel::TestItem* ti)
{

    type(ti->type());
    subtype(ti->subtype());
    timestamp(ti->timestamp());

    dataMap_ = ti->dataMap_;
    metadataMap_ = ti->metadataMap_;
}

void TestItem::deepCopy(DataModel::TestItem* ti)
{
    ///
    ///type and subtype
    type(ti->type());
    subtype(ti->subtype());
    timestamp(ti->timestamp());

    ///
    ///data and metadata maps

    DataModel::KeyValueMap::const_iterator it;
    const DataModel::KeyValueMap& kv1 = ti->dataMap();
    //data map
    for(it= kv1.begin(); it != kv1.end(); ++it)
    {
        addData(it->first, it->second);
    }

    const DataModel::KeyValueMap& kv2 = ti->metadataMap();
    //metadata map
    for(it= kv2.begin(); it != kv2.end(); ++it)
    {
        addMetadata(it->first, it->second);
    }
}


///
/// test base
///

TestBase::TestBase()
{
}

TestBase::~TestBase()
{
}


// Access to the data map
const TestBase::DataMap&
TestBase::dataMap() const
{
    return dataMap_;
}

TestBase::DataMap&
TestBase::dataMap()
{
    return dataMap_;
}

const TestBase::MetadataMap&
TestBase::metadataMap() const
{
    return metadataMap_;
}

TestBase::MetadataMap&
TestBase::metadataMap()
{
    return metadataMap_;
}

bool TestBase::addData(const std::string& key, const std::string& value)
{
    return addPair(dataMap_, key, value);
}

std::string
TestBase::getData(const std::string& key) const throw (not_found)
{
    return getValue(dataMap_, key);
}

bool
TestBase::addMetadata(const std::string& key, const std::string& value)
{
    return addPair(metadataMap_, key, value);
}

std::string
TestBase::getMetadata(const std::string& key) const throw (not_found)
{
    return getValue(metadataMap_, key);
}


//private
bool
TestBase::addPair(KeyValueMap& map,
                  const std::string& key,
                  const std::string& value)
{
    map[key] = value;
    return true;
}

const std::string&
TestBase::getValue(const KeyValueMap& map, const std::string& key) const throw (not_found)
{
    KeyValueMap::const_iterator it = map.find(key);

    if (it == map.end())
        throw not_found();

    return it->second;
}
