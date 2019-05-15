/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Main Helper
* @details		Contains macros usefulls for creating DLL main.
* @author		Martin Svoboda
* @date			05.05.2019
* @copyright	GNU General Public License (GPLv3).
******************************************************************************************************/


/*
This file is part of MarsTech Dll Factory.

MarsTech Dependency Injection is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MarsTech Promise Like Syntax is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef MARSTECH_DLLMAINHELPER_H
#define MARSTECH_DLLMAINHELPER_H


#define MSV_GET_DLLOBJECT_WITH_ID(requestedId, id, objectToCreate, spOut) \
if (requestedId.compare(id) == 0) \
{ \
	spOut.reset(new (std::nothrow) objectToCreate()); \
	if (!spOut) \
	{ \
		return MSV_ALLOCATION_ERROR; \
	} \
	return MSV_SUCCESS; \
}

#define MSV_GETSHARED_DLLOBJECT_WITH_ID(requestedId, id, objectToCreate, spShared, spOut) \
if (requestedId.compare(id) == 0) \
{ \
	if (!spShared) \
	{ \
		spShared.reset(new (std::nothrow) objectToCreate()); \
		if (!spShared) \
		{ \
			return MSV_ALLOCATION_ERROR; \
		} \
	} \
	spOut = spShared; \
	return MSV_SUCCESS; \
}

#define MSV_GETWEAKSHARED_DLLOBJECT_WITH_ID(requestedId, id, objectToCreate, spShared, spOut) \
if (requestedId.compare(id) == 0) \
{ \
	if (!(spOut = spShared.lock())) \
	{ \
		spOut.reset(new (std::nothrow) objectToCreate()); \
		if (!spOut) \
		{ \
			return MSV_ALLOCATION_ERROR; \
		} \
		spShared = spOut; \
	} \
	return MSV_SUCCESS; \
}


#endif // MARSTECH_DLLMAINHELPER_H

/** @} */	//End of group MDLLFACTORY.
