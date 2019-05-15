/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Factory Implementation
* @details		Contains implementation @ref MsvDllFactory of @ref IMsvDllFactory interface.
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


#ifndef MARSTECH_DLLFACTORY_H
#define MARSTECH_DLLFACTORY_H


#include "IMsvDllFactory.h"

#include "IMsvDllList.h"
#include "mlogging/mlogging.h"

MSV_DISABLE_ALL_WARNINGS

#include <map>
#include <mutex>

MSV_ENABLE_WARNINGS


//forward declaration of MarsTech Dll Factory Dependency Injection Factory
class MsvDllFactory_Factory;


/**************************************************************************************************//**
* @brief		MarsTech DLL Factory Implementation.
* @details	Implementation for MarsTech dynamic/shared library factory. Loads and manage dynamic/shared
*				libraries.
* @see		IMsvDllFactory
******************************************************************************************************/
class MsvDllFactory:
	public IMsvDllFactory
{
public:
	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	spDllList		DLL list with dynamic/shared library data (path, decorator, etc.).
	* @param[in]	spLogger			Shared pointer to logger for logging.
	* @param[in]	spFactory		Shared pointer to dependency injection factory.
	* @see			MsvDllFactory_Factory
	* @see			IMsvDllList
	******************************************************************************************************/
	MsvDllFactory(const std::shared_ptr<IMsvDllList>& spDllList, std::shared_ptr<MsvLogger> spLogger = nullptr, std::shared_ptr<MsvDllFactory_Factory> spFactory = nullptr);

	/**************************************************************************************************//**
	* @brief			Deleted copy constructor.
	* @details		Copy constructor deleted -> copying is not allowed.
	* @param[in]	origin			Reference to copyied object.
	* @warning		Do not copy this object.
	******************************************************************************************************/
	MsvDllFactory(const MsvDllFactory& origin) = delete;

	/**************************************************************************************************//**
	* @brief			Deleted assign operator.
	* @details		Assign operator deleted -> assign is not allowed.
	* @param[in]	origin			Reference to assigned object.
	* @warning		Do not assign this object.
	******************************************************************************************************/
	MsvDllFactory& operator= (const MsvDllFactory& origin) = delete;

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvDllFactory public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvDllFactory::GetDll(const char* id, std::shared_ptr<IMsvDll>& spDll)
	******************************************************************************************************/
	virtual MsvErrorCode GetDll(const char* id, std::shared_ptr<IMsvDll>& spDll) override;

	/**************************************************************************************************//**
	* @copydoc IMsvDllFactory::GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject)
	******************************************************************************************************/
	virtual MsvErrorCode GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject) override;

	/**************************************************************************************************//**
	* @copydoc IMsvDllFactory::ReleaseDll(const char* id)
	******************************************************************************************************/
	virtual MsvErrorCode ReleaseDll(const char* id) override;

	/*-----------------------------------------------------------------------------------------------------
	**											MsvDllFactory protected methods
	**---------------------------------------------------------------------------------------------------*/
protected:
	/**************************************************************************************************//**
	* @brief			Get DLL.
	* @details		Loads dynamic/shared library and returns it. If library is already loaded returns it.
	*					It does not load same library twice.
	* @param[in]	id										DLL id.
	* @param[out]	spDll									Shared pointer to loaded dynamic/shared library.
	* @param[out]	spDecorator							Shared pointer to decorator (it might be nullptr if decorator is not needed).
	* @retval		MSV_ALLOCATION_ERROR				When memory allocation failed.
	* @retval		MSV_OPEN_ERROR						When load DLL library failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	MsvErrorCode GetDll(const char* id, std::shared_ptr<IMsvDll>& spDll, std::shared_ptr<IMsvDllDecorator>& spDecorator);

protected:
	/**************************************************************************************************//**
	* @brief		Thread pool mutex.
	* @details	Locks this object for thread safety access.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

	/**************************************************************************************************//**
	* @brief		Loaded DLLs.
	* @details	Stored shared pointers to loaded DLLs (each library is loaded only once).
	* @see		IMsvDll
	******************************************************************************************************/
	std::map<std::string, std::shared_ptr<IMsvDll>> m_loadedDlls;

	/**************************************************************************************************//**
	* @brief		DLL list.
	* @details	Contains dynamic/shared library data (path, decorator, etc.).
	* @see		IMsvDllList
	******************************************************************************************************/
	const std::shared_ptr<IMsvDllList> m_spDllList;	

	/**************************************************************************************************//**
	* @brief		Dependency injection factory.
	* @details	Contains get method for all injected objects.
	* @see		MsvDll_Factory
	******************************************************************************************************/
	std::shared_ptr<MsvDllFactory_Factory> m_spFactory;

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;
};


#endif // MARSTECH_DLLFACTORY_H

/** @} */	//End of group MDLLFACTORY.
