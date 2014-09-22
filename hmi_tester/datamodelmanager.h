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
#ifndef DATAMODELMANAGER_H
#define DATAMODELMANAGER_H

#include "datamodel.h"
#include "datamodeladapter.h"

#include <boost/ptr_container/ptr_map.hpp>
#include <list>

class DataModelManager
{
public:
    class not_exists : public std::exception
    {
	virtual const char* what() const throw()
	{
	    return "conversion_error";
	}
    };

    typedef std::list<std::string> StringList;
    typedef boost::ptr_map<std::string, DataModelAdapter> AdapterMap;

public:
    DataModelManager();
    ~DataModelManager();

    //datamodel adapters management
    void addDataModelAdapter(const std::string& key,
                             DataModelAdapter*adapter);
    DataModelAdapter* getDataModelAdapter(const std::string& key)
            throw (not_exists);
    StringList getDataModelAdapterKeys() const;
    bool setCurrentDataModelAdapter(const std::string& key) throw (not_exists);
    DataModelAdapter* getCurrentDataModelAdapter() const;

protected:
    DataModelAdapter* currentAdapter_;
    AdapterMap adapters_;
};

#endif // DATAMODELMANAGER_H
