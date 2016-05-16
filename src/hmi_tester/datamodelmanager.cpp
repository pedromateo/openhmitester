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

#include "datamodelmanager.h"


DataModelManager::DataModelManager()
{
}

DataModelManager::~DataModelManager()
{
}


///
/// datamodel adapters management
///

void DataModelManager::addDataModelAdapter(const std::string& key,
                                           DataModelAdapter* adapter)
{
    adapters_.insert(const_cast<std::string &>(key), adapter);
}

DataModelAdapter* DataModelManager::getDataModelAdapter(const std::string& key)
    throw (not_exists)
{
    AdapterMap::iterator it = adapters_.find(key);
    if (it == adapters_.end())
	throw not_exists();

    return it->second;
}

DataModelManager::StringList DataModelManager::getDataModelAdapterKeys() const
{
    StringList sl;
    for (AdapterMap::const_iterator it = adapters_.begin(); it != adapters_.end(); ++it)
	sl.push_back(it->first);
    return sl;
}

bool DataModelManager::setCurrentDataModelAdapter(const std::string& key) throw (not_exists)
{
    AdapterMap::iterator it = adapters_.find(key);
    if (it == adapters_.end())
	throw not_exists();

    currentAdapter_ = it->second;
    return true;
}

DataModelAdapter* DataModelManager::getCurrentDataModelAdapter() const
{
    return currentAdapter_;
}
