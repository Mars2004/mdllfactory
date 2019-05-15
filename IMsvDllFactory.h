/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Factory Interface
* @details		Contains definition of @ref IMsvDllFactory interface.
* @author		Martin Svoboda
* @date			06.04.2019
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


#ifndef MARSTECH_IDLLFACTORY_H
#define MARSTECH_IDLLFACTORY_H


#include "IMsvDll.h"

#include "merror/MsvErrorCodes.h"


/**************************************************************************************************//**
* @brief		MarsTech DLL Factory Interface.
* @details	Interface for dynamic/shared libraries factory. Contains methods to get dynamic/shared
*				libraries and theirs exported objects, functions, etc.
* @see		MsvDllFactory
******************************************************************************************************/
class IMsvDllFactory
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvDllFactory() {}

	/**************************************************************************************************//**
	* @brief			Get DLL.
	* @details		Loads dynamic/shared library and returns it. If library is already loaded returns it.
	*					It does not load same library twice.
	* @param[in]	id										DLL id.
	* @param[out]	spDll									Shared pointer to loaded dynamic/shared library.
	* @retval		MSV_ALLOCATION_ERROR				When memory allocation failed.
	* @retval		MSV_OPEN_ERROR						When load DLL library failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetDll(const char* id, std::shared_ptr<IMsvDll>& spDll) = 0;

	/**************************************************************************************************//**
	* @brief			Get DLL object.
	* @details		Loads dynamic/shared library (if not already loaded) and gets DLL object from it.
	* @param[in]	id										DLL (object) id.
	* @param[out]	spDllObject							Shared pointer to acquired DLL object.
	* @retval		MSV_ALLOCATION_ERROR				When memory allocation failed.
	* @retval		MSV_OPEN_ERROR						When load DLL library failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject) = 0;

	/**************************************************************************************************//**
	* @brief			Release DLL library.
	* @details		Releases dynamic/shared library and unloads it.
	* @retval		MSV_NOT_ALLOWED_ERROR			When someone else holds shared pointer to dynamic/shared library.
	* @retval		MSV_NOT_INITIALIZED_INFO		When DLL library has not been initialized (this is info, not error).
	* @retval		MSV_CLOSE_ERROR					When unload DLL library failed.
	* @retval		MSV_NOT_FOUND_INFO				When DLL library was not found in loaded DLLs(this is info, not error).
	* @retval		MSV_SUCCESS							On success.
	* @warning		All references to dynamic/shared library must be release. Other way fails with MSV_NOT_ALLOWED_ERROR.
	* @warning		Libraries with reference count greater then 0 should not be unitialized. It might lead
	*					to crash. On the other way, some DLLs might hold its acquired objects in shared_ptr
	*					too. Uninitialize just logs warning when uninitializing dynamic/shared library
	*					with references, but unloads it anyway.
	******************************************************************************************************/
	virtual MsvErrorCode ReleaseDll(const char* id) = 0;

	/**************************************************************************************************//**
	* @brief			Get DLL object and cast it to right type.
	* @details		Loads dynamic/shared library (if not already loaded) and gets DLL object from it. Internally
	*					calls @ref GetDllObject and cast returned object to T type.
	* @param[in]	id										DLL (object) id.
	* @param[out]	spDllObject							Shared pointer to acquired DLL object.
	* @retval		MSV_ALLOCATION_ERROR				When memory allocation failed.
	* @retval		MSV_OPEN_ERROR						When load DLL library failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	template<class T> inline MsvErrorCode GetDllObject(const char* id, std::shared_ptr<T>& spDllObject)
	{
		std::shared_ptr<IMsvDllObject> spInnerDllObject;
		MSV_RETURN_FAILED(GetDllObject(id, spInnerDllObject));
		
		spDllObject = std::static_pointer_cast<T, IMsvDllObject>(spInnerDllObject);
		
		return MSV_SUCCESS;
	}
};


#endif // MARSTECH_IDLLFACTORY_H

/** @} */	//End of group MDLLFACTORY.
