/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Implementation
* @details		Contains implementation @ref MsvDll of @ref IMsvDll interface.
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


#ifndef MARSTECH_DLL_H
#define MARSTECH_DLL_H


#include "IMsvDll.h"

#include "mlogging/mlogging.h"

MSV_DISABLE_ALL_WARNINGS

#include <map>
#include <mutex>

MSV_ENABLE_WARNINGS


//forward declaration of MarsTech Dll Dependency Injection Factory
class MsvDll_Factory;


/**************************************************************************************************//**
* @brief		MarsTech DLL Implementation.
* @details	Implementation for MarsTech dynamic/shared library.
* @see		IMsvDll
******************************************************************************************************/
class MsvDll:
	public IMsvDll
{
public:
	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	spLogger			Shared pointer to logger for logging.
	* @param[in]	spFactory		Shared pointer to dependency injection factory.
	* @see			MsvDll_Factory
	******************************************************************************************************/
	MsvDll(std::shared_ptr<MsvLogger> spLogger = nullptr, std::shared_ptr<MsvDll_Factory> spFactory = nullptr);

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvDll();

	/**************************************************************************************************//**
	* @brief			Deleted copy constructor.
	* @details		Copy constructor deleted -> copying is not allowed.
	* @param[in]	origin			Reference to copyied object.
	* @warning		Do not copy this object.
	******************************************************************************************************/
	MsvDll(const MsvDll& origin) = delete;

	/**************************************************************************************************//**
	* @brief			Deleted assign operator.
	* @details		Assign operator deleted -> assign is not allowed.
	* @param[in]	origin			Reference to assigned object.
	* @warning		Do not assign this object.
	******************************************************************************************************/
	MsvDll& operator= (const MsvDll& origin) = delete;

	/**************************************************************************************************//**
	* @copydoc IMsvDll::Initialize(const char* dllPath)
	******************************************************************************************************/
	virtual MsvErrorCode Initialize(const char* dllPath) override;

	/**************************************************************************************************//**
	* @copydoc IMsvDll::Uninitialize()
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() override;

	/**************************************************************************************************//**
	* @copydoc IMsvDll::Initialized()
	******************************************************************************************************/
	virtual bool Initialized() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDll::GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject, std::shared_ptr<IMsvDllDecorator> spDecorator = nullptr)
	******************************************************************************************************/
	virtual MsvErrorCode GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject, std::shared_ptr<IMsvDllDecorator> spDecorator = nullptr) override;

	/**************************************************************************************************//**
	* @copydoc IMsvDll::GetDllReferenceCount()
	******************************************************************************************************/
	virtual std::int64_t GetDllReferenceCount() const override;

protected:
	/**************************************************************************************************//**
	* @brief		Thread pool mutex.
	* @details	Locks this object for thread safety access.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

	/**************************************************************************************************//**
	* @brief		Objects acquired from DLL.
	* @details	Stored weak_ptrs to objects acquired from DLL.
	* @see		IMsvDllObject
	******************************************************************************************************/
	std::map<std::string, std::weak_ptr<IMsvDllObject>> m_dllObjects;

	/**************************************************************************************************//**
	* @brief		Initialize flag.
	* @details	Flag if DLL is initialized (true) or not (false).
	* @see		Initialize
	* @see		Uninitialize
	* @see		Initialized
	******************************************************************************************************/
	bool m_initialized;

	/**************************************************************************************************//**
	* @brief		Pointer to DLL entry point.
	* @details	Pointer to GetDllObject exported function from DLL. It is used to get objects which inherits
	*				from @ref IMsvDllObject.
	* @see		IMsvDllAdapter
	******************************************************************************************************/
	MsvErrorCode(*m_pGetDllObjectFunction)(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject);

	/**************************************************************************************************//**
	* @brief		DLL adapter.
	* @details	Adapter to real system dynamic/shared library.
	* @see		IMsvDllAdapter
	******************************************************************************************************/
	std::shared_ptr<IMsvDllAdapter> m_spDllAdapter;
	
	/**************************************************************************************************//**
	* @brief		Dependency injection factory.
	* @details	Contains get method for all injected objects.
	* @see		MsvDll_Factory
	******************************************************************************************************/
	std::shared_ptr<MsvDll_Factory> m_spFactory;

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;
};


#endif // MARSTECH_DLL_H

/** @} */	//End of group MDLLFACTORY.
