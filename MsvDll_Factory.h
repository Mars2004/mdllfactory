/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech DLL Factory
* @details		Contains implementation of dependency injection factory @ref MsvDll_Factory
*					for @ref MsvDll.
* @author		Martin Svoboda
* @date			06.04.2019
* @copyright	GNU General Public License (GPLv3).
******************************************************************************************************/


/*
This file is part of MarsTech Threading.

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


#ifndef MARSTECH_DLL_FACTORY_H
#define MARSTECH_DLL_FACTORY_H


#include "mdi/MdiFactory.h"

#include "MsvDllAdapter.h"


/**************************************************************************************************//**
* @class		MsvDll_Factory
* @brief		MarsTech Dll Dependency Injection Factory.
* @details	Implementation of dependency injection factory for @ref MsvDll.
******************************************************************************************************/
MSV_FACTORY_START(MsvDll_Factory)
MSV_FACTORY_GET_1(IMsvDllAdapter, MsvDllAdapter, std::shared_ptr<MsvLogger>);
MSV_FACTORY_END


#endif // MARSTECH_DLL_FACTORY_H

/** @} */	//End of group MDLLFACTORY.
