/**
 * ISCRadioReadOnlyMenuItem.h - Interface to allowing readonly access to a menu item
 *
 * Notice there is no accompanying .cpp file.  There is no logic being
 * implemented.  This is an abstract class being treated as an interface.
 * 
 * Being abstract means you can't actually directly create one of these.
 * But an abstract class could still have logic associated with it.
 * 
 * Interfaces do not implement any logic.  They only contain method 
 * definitions.  Classes that inherit this class have to implement those
 * methods.  And then they can be treated as if they are one of these.
 * 
 * So, if you inherit ISCRadioReadOnlyMenuItem you can be accessed as if you 
 * are a ISCRadioReadOnlyMenuItem and you will only have access to the 
 * methods defined below.
 * 
 * Interfaces are a very powerful concept.  They allow you to
 * dictate terms for interaction with other objects without
 * requiring those objects' authors to inherit everything 
 * from your classes including logic and data they don't 
 * really need or want.
 * 
 * Notice that the methods below look like this:
 * void virtual myFunction() = 0;
 * 
 * Virtual means it can be overridden in classes that inherit this class.
 * But, the '= 0' at the end says there will be no implementation in this
 * class itself.  It is a 'pure virtual' method.
 * 
 * Copyright (c) 2016 - Richard Young Dodd
 *
 * Richard Young Dodd licenses this file to you under the MIT license.
 * See the LICENSE file in the project root for more information.
 * If you did not receive the 'LICENSE' file with this software
 * see <https://opensource.org/licenses/MIT>.
 *
 * @author Richard Y. Dodd - K4KRW
 * @version 1.0.2  12/12/2016.
 */

#ifndef ISCRadioReadOnlyMenuItem_h
#define ISCRadioReadOnlyMenuItem_h

class ISCRadioReadOnlyMenuItem
{
	public:
		// virtual ~ISCRadioReadOnlyMenuItem();

	/**
	 * getMenuItemDisplayNameCopy
	 * 
	 * @details
	 *   Copies display name of a menu item to the provided location
	 * 
	 * @param[out] destinationForCopy Where to place copy of name
	 */
	void virtual getMenuItemDisplayNameCopy(char* destinationForCopy) = 0;

	/**
	* getMenuItemDisplayNameCopy
	*
	* @details
	*   Copies text representation menu item's value to the provided location
	*
	* @param[out] destinationForCopy Where to place copy of value
	*/	void virtual getMenuItemDisplayValueCopy(char* destinationForCopy) = 0;

	/**
	 * getMenuItemValue
	 * 
	 * @details
	 *   Gets a 32 bit integer representation of the menu item's current value
	 */
	int32_t virtual getMenuItemValue() = 0;
};

#endif