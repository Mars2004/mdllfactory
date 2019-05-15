/**************************************************************************************************//**
* @defgroup		MDLLFACTORY MarsTech Dll Factory
* @brief			MarsTech Dll Factory
* @details		Contains implementation and all definitions of MarsTech DLL Factory. Implements easy
*					access to DLLs and shared libraries.
* @copyright	GNU General Public License (GPLv3).
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Object Interface
* @details		Contains definition of @ref IMsvDllObject interface.
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


#ifndef MARSTECH_IDLLOBJECT_H
#define MARSTECH_IDLLOBJECT_H


/**************************************************************************************************//**
* @brief		MarsTech DLL Object Interface.
* @details	Interface for objects which can be acquired from DLL through @ref IMsvDll interface.
* @see		IMsvDll
* @note		Just inherit from this interface to be able to get your object through @ref IMsvDll or
*				@ref IMsvDllFactory.
******************************************************************************************************/
class IMsvDllObject
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvDllObject() {}
};


#endif // MARSTECH_IDLLOBJECT_H

/** @} */	//End of group MDLLFACTORY.
