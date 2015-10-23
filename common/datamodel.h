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
#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <uuid.h>
#include <map>
#include <boost/ptr_container/ptr_list.hpp>
#include <string>
#include <exception>

#define WANT_SERIALIZE

#ifdef WANT_SERIALIZE
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>
#endif

namespace DataModel
{
    ///
    /// types
    ///
    typedef std::map<std::string, std::string> KeyValueMap;

    struct not_found : public std::exception
    {
    };

    ///
    /// test base
    /// object base which includes data and metadata maps
    ///
    class TestBase
    {
    public:
        TestBase();
        ~TestBase();

        // Inner types to define maps
        typedef KeyValueMap DataMap;
        typedef KeyValueMap MetadataMap;

        bool addData(const std::string&, const std::string&);
        std::string getData(const std::string&) const throw (not_found);

        // Access to the data map
        const DataMap& dataMap() const;

        // TODO: Provide non-readonly operation or just add an operation
        // to add and remove data and metadata pairs?
        DataMap& dataMap();

        bool addMetadata(const std::string&, const std::string&);
        std::string getMetadata(const std::string&) const throw (not_found);

        const MetadataMap& metadataMap() const;
        // TODO: Provide non-readonly operation or just add an operation
        // to add and remove data and metadata pairs?
        MetadataMap& metadataMap();

    protected:
        bool addPair(KeyValueMap&, const std::string&, const std::string&);
        const std::string& getValue(const KeyValueMap&, const std::string& ) const throw (not_found);

        DataMap dataMap_;
        MetadataMap metadataMap_;
    };

    ///
    /// test item
    ///
    class TestItem : virtual public TestBase
    {
        // ID of this testitem
        uuid_t uuid_;

    public:
        TestItem();
        TestItem(int type, int subtype, int timestamp);
        TestItem(DataModel::TestItem*);
        ~TestItem();

        uuid_t uuid() const;

        int type() const;
        void type(int);
        int subtype() const;
        void subtype(int);
        int timestamp() const;
        void timestamp(int);

#ifdef WANT_SERIALIZE
        //serialization
        friend class boost::serialization::access;
        template<class Archive>
                void serialize(Archive & ar, const unsigned int /*version*/)
        {
            ar & dataMap_;
            ar & metadataMap_;

            ar & uuid_;
            ar & type_;
            ar & subtype_;
            ar & timestamp_;
        }
#endif

        //copy methods
        virtual void copy(DataModel::TestItem*);
        virtual void deepCopy(DataModel::TestItem*);

    protected:
        int type_;
        int subtype_;
        int timestamp_;

    private:
        //this class must be virtual
        virtual void none(){}
    };

    ///
    /// test case
    ///
    class TestCase : virtual public TestBase
    {
        uuid_t uuid_;

    public:
        TestCase();
        ~TestCase();

        uuid_t uuid() const;

        typedef boost::ptr_list<TestItem> TestItemList;

        const TestItemList& testItemList() const;

        void addTestItem(DataModel::TestItem*);
        void addTestItem(TestItemList::iterator,
                         DataModel::TestItem*);

        void deleteTestItem(TestItemList::iterator) throw (not_found);

        size_t count() const;

        //own properties
        std::string name() const;
        void name(const std::string&);

#ifdef WANT_SERIALIZE
        //serialization
        friend class boost::serialization::access;
        template<class Archive>
                void serialize(Archive & ar, const unsigned int /*version*/)
        {
            ar & dataMap_;
            ar & metadataMap_;

            ar & uuid_;
            ar & name_;
            ar & testItems_;

            // This is a little bit tricky.
            // When we serialize we have to make sure
            // that the map is synced with the read data
            // Allows us not to serialize the map we use
            // for optimization
            __syncMap();
        }
#endif

    protected:
        typedef std::map<uuid_t, TestItem*> ItemMap;

        ItemMap itemMap_;
        TestItemList testItems_;
        std::string name_;

        void __syncMap();
    };

    ///
    /// test suite
    ///
    class TestSuite : virtual public TestBase
    {
    public:
        TestSuite();
        ~TestSuite();

        typedef boost::ptr_list<DataModel::TestCase> TestCaseList;

        ///TestCase List operations

        const TestCaseList& testCases() const;

        void addTestCase(DataModel::TestCase*);
        void addTestCase(TestCaseList::iterator,
                         DataModel::TestCase*);
        void deleteTestCase(TestCaseList::iterator) throw (not_found);
//        void deleteTestCase(uuid_t) throw (not_found);
        void deleteTestCase(const std::string&) throw (not_found);


//        DataModel::TestCase* getTestCase(uuid_t) throw (not_found);
        DataModel::TestCase* getTestCase(const std::string&) throw (not_found);

//        DataModel::TestCase* existsTestCase(uuid_t);
        DataModel::TestCase* existsTestCase(const std::string&);

        size_t count() const;

        //own properties
        std::string appId() const;
        void appId(const std::string&);

        std::string name() const;
        void name(const std::string&);

#ifdef WANT_SERIALIZE
        //serialization
        friend class boost::serialization::access;
        template<class Archive>
                void serialize(Archive & ar, const unsigned int /*version*/)
        {
            ar & dataMap_;
            ar & metadataMap_;

            ar & appId_;
            ar & tcMap_;
            ar & testCases_;
        }
#endif

    protected:
        typedef std::map<std::string, TestCase*> TestCaseMap;

        TestCaseMap tcMap_;
        TestCaseList testCases_;
        std::string appId_;
        std::string name_;
    };

}

#endif // DATAMODEL_H
