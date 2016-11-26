/**
 * ISCRadioReadOnlyMenuItem.h - Interface to allowing readonly access to a menu item
 *
 * Copyright (c) 2016 - Richard Young Dodd
 *
 * Richard Young Dodd licenses this file to you under the MIT license.
 * See the LICENSE file in the project root for more information.
 * If you did not receive the 'LICENSE' file with this software
 * see <https://opensource.org/licenses/MIT>.
 *
 * @author Richard Y. Dodd - K4KRW
 * @version 1.0  11/20/2016.
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