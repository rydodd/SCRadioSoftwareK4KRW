/*
 * SCRadioMenuItem.h - Class for radio menu item of a name/value type
 *
 * Copyright (c) 2016 - Richard Young Dodd
 *
 * Richard Young Dodd licenses this file to you under the MIT license.
 * See the LICENSE file in the project root for more information.
 * If you did not receive the 'LICENSE' file with this software
 * see <https://opensource.org/licenses/MIT>.
 *
 * @author Richard Y. Dodd - K4KRW
 * @version 1.0.3  12/22/2016.
 */

#ifndef SCRadioMenuItemNameValue_h
#define SCRadioMenuItemNameValue_h

// forwards for classes accessed via pointers and references only
class EventData;
class EventManager;

#include "SCRadioConstants.h"
#include "SCRadioMenuItem.h"

class SCRadioMenuItemNameValue : public SCRadioMenuItem
{
private:
	// private member data

	const char* _displayValues[MAX_NAME_VALUE_CHOICES] = {0,0,0};

public:
	// public methods

	/**
	 * SCRadioMenuItemNameValue
	 * 
	 * @detail
	 *   Creates a SCRadioMenuItemNameValue instance.  You must call begin() before using
	 *   You must also set up all of the required text for the menu item.
	 * 
	 * @param[in] eventManager Reference to event manager class with which we enqueue event messages
	 * @param[in] initialValue Value to set for the initial value
	 * @param[in] incrementValue Value to increment menu item by as it is adjusted
	 * @param[in] minimumValue Minimum value for the menu item
	 * @param[in] maximumValue Maximum value for the minu item
	 */
	SCRadioMenuItemNameValue(EventManager &eventManager,
		int32_t initialValue,
			int8_t incrementValue,
				int32_t minimumValue,
					int32_t maximumValue);

	/**
	* SCRadioMenuItemNameValue
	*
	* @detail
	*   Creates a SCRadioMenuItemNameValue instance.  You must call begin() before using
	*   You must also set up all of the required text for the menu item.
	*
	* @param[in] eventManager Reference to event manager class with which we enqueue event messages
	* @param[in] incrementValue Value to increment menu item by as it is adjusted
	* @param[in] minimumValue Minimum value for the menu item
	* @param[in] maximumValue Maximum value for the minu item
	*/
	SCRadioMenuItemNameValue(EventManager &eventManager,
		int32_t initialValue,
		int32_t minimumValue,
		int32_t maximumValue);

	/**
	 * ~SCRadioMenuItemNameValue
	 * 
	 * @detail
	 *   Destructor
	 */
	virtual ~SCRadioMenuItemNameValue();

	/**
	 * getMenuItemDisplayValueCopy
	 * 
	 * @detail
	 *   Gets a copy of the menu item's current value formatted for display
	 * 
	 * @param[out] destination Where to copy string
	 */
	void virtual getMenuItemDisplayValueCopy(char* destination);
    
	/**
	 * setMenuItemDisplayValue
	 * 
	 * @detail
	 *   Sets a menu item display value.
	 * 
	 * @param[in] whichMenuItem Position of manu item (1, 2, 3 ...)
	 * 
	 * @param[in] displayValue Value to set.
	 */
	void setMenuItemDisplayValue(int whichMenuItem, const char* displayValue);
private:
};

#endif