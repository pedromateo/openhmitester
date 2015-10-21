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
#ifndef DATAMODELSERIALIZER_H
#define DATAMODELSERIALIZER_H

#include <datamodel.h>

#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>

namespace DataModel {
namespace S10e {

    // template <typename T>
    // class serializer: public T
    // {
    //     const T& t_;
    // public:
    //     serializer (const T& t)
    //         : t_ (t)
    //     {
    //     }

    //     friend class boost::serialization::access;
    //     template<class Archive>
    //     void serialize(Archive & ar, const unsigned int version)
    //     {
    //         // ar & dataMap_;
    //         // ar & metadataMap_;
    //     }
    // };


    // // Partial specialization for TestItem.
    // template <>
    // class serializer<DataModel::TestItem>: public DataModel::TestItem
    // {
    //     const DataModel::TestItem& t_;
    // public:
    //     serializer (const DataModel::TestItem& t)
    //         : t_ (t)
    //     {}

    //     friend class boost::serialization::access;
    //     template<class Archive>
    //     void serialize(Archive & ar, const unsigned int version)
    //     {
    //         ar & t_.type_;
    //         ar & t_.subtype_;
    //     }
    // };

    ///
    /// test item
    ///
    class TestItem : public DataModel::TestItem
    {
        // TODO: UUID needed.

    public:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & dataMap_;
            ar & metadataMap_;

            ar & type_;
            ar & subtype_;
        }
    };

    ///
    /// test case
    ///
    class TestCase : public DataModel::TestCase
    {
        // TODO: TestCase name and other data?

    public:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & dataMap_;
            ar & metadataMap_;

            ar & testItems_;
        }
    };

    ///
    /// test suite
    ///
    class TestSuite : virtual public DataModel::TestSuite
    {
        // TODO: TestSuite name and other data?

    public:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & dataMap_;
            ar & metadataMap_;

            ar & testCases_;
        }
    };

}
}

#endif // DATAMODELSERIALIZER_H
