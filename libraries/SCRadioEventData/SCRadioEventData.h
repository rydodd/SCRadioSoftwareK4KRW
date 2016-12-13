/*
 * SCRadioEventData.h - Class for common data shared by event handling logic
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

#ifndef SCRadioEventData_h
#define SCRadioEventData_h

// forwards for classes accessed via pointers and references only
class SCRadioFrequency;

// includes
#include "SCRadioConstants.h"
#include "SCRadioMenuItem.h"

/**
 * Class for accessing data related to events that could not be passed in the event message itself
 */
class SCRadioEventData
{
private:
	// private member variables

	/**
	 * Array of 32 bit integers used by event related logic
	 */
	int32_t _longValues[EVENT_DATA_LONG_FIELDS_COUNT];

	/**
	 * Array of boolean values used by event related logic
	 */
	bool _boolValues[EVENT_DATA_BOOL_FIELDS_COUNT];

	/**
	 * Array of menu items for use in event related logic
	 */
	SCRadioMenuItem* _menuItems[MAX_MENU_ITEMS];

	/**
	 * Array of frequencies used by event related logic
	 */
	SCRadioFrequency* _frequencies[EVENT_DATA_FREQUENCY_FIELDS_COUNT];

public:	

	// public methods

	/**
	 * begin
	 * 
	 * @detail
	 *   Gets event data object ready for use - logic that would normally be in a constructor.
	 *   Call before using object
	 */
	void begin(); 
	
	/**
	 * getEventRelatedBool
	 * 
	 * @detail
	 *   Returns the specified boolean value from the event data
	 * 
	 * @param[in] whichField Field to retrieve
	 * 
	 * @returns boolean value specified
	 */
	bool getEventRelatedBool(EventBoolField whichField);

	/**
	 * getEventRelatedFrequency
	 * 
	 * @detail
	 *   Returns the specified frequency value from the event data
	 * 
	 * @param[in] whichField Field to retrieve
	 * 
	 * @returns specified frequency
	 */
	SCRadioFrequency* getEventRelatedFrequency(EventFrequencyField whichField);

	/**
	 * getEventRelatedLong
	 * 
	 * @detail
	 *   Returns the specified 32 bit int from the event data
	 * 
	 * @param[in] whichField Field to retrieve
	 * 
	 * @returns 32 bit int specified
	 */
	int32_t getEventRelatedLong(EventLongField whichField);

	/**
	 * setEventRelatedLong
	 * 
	 * @detail
	 *   Sets an event related long field to the specified value
	 * 
	 * @param[in] valueToSet
	 * @param[in] whichField
	 */
	void setEventRelatedLong(long valueToSet, EventLongField whichField);

	/**
	 * setEventRelatedBool
	 * 
	 * @detail
	 *   Sets an event related long field to the specified value
	 * 
	 * @param[in] valueToSet
	 * @param[in] whichField
	 */
	void setEventRelatedBool(bool valueToSet, EventBoolField whichField);

	/**
	 * setEventRelatedFrequency
	 * 
	 * @detail
	 *   Sets an event related frequency field to the specified value
	 *   
	 * @param[in] valueToSet pointer to SCRadioFrequeny instance
	 * @param[in] whichField
	 */
	void setEventRelatedFrequency(SCRadioFrequency *valueToSet, EventFrequencyField whichField);

	/**
	 * setMenuItem
	 * 
	 * @detail
	 *   Stores a pointer to a menu item into the menu item array in the EventData object
	 */
	void setMenuItem(SCRadioMenuItem *menuItem, int8_t whichMenuItem);
  	
	/**
	* getReadOnlyMenuItem
	* 
	* @detail
	*   Returns a read only version of a menu item
	* 
	* @param[in] whichMenuItem Number of menu item to return
	* 
	* @returns read only version of specified menu item
	*/
	ISCRadioReadOnlyMenuItem* getReadOnlyMenuItem(int8_t whichMenuItem);

private:

	// private methods
};

#endif