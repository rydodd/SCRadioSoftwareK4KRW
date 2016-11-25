/*
 * SCRadioMenuItem.h - Class for radio menu item
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

#ifndef SCRadioMenuItem_h
#define SCRadioMenuItem_h

// forwards for classes accessed via pointers and references only
class EventManager;

// includes
#include "SCRadioConstants.h"
#include "ISCRadioReadOnlyMenuItem.h"

class SCRadioMenuItem : public ISCRadioReadOnlyMenuItem
{
private:
	// private member variables

	/**
	 * Character array holding menu item name
	 */
	char _menuItemName[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];

protected:
	// protected member data

	/**
	 * Format string for formatting the menu item's value for display
	 */
	char _menuItemValueFormat[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];

	/**
	 * Indicates which menu item number this menu item is
	 */
	int8_t _menuItemIndex;

	/**
	 * Indicates the type of event enqueued when this menu item's value changes.
	 * 
	 * Actually two messages are sent.
	 * 1. EventType::MENU_ITEM_CHANGED which gets picked up by the display object so it shows the new value
	 * 2. Then the _menuItemEventType which gets picked up by the logic that responds to the menu item change (turn rit on off, change rx offset direction ...)
	 */
	EventType _menuItemEventType;

	/**
	 * Used to enqueue messages resulting from menu item value changes
	 */
	EventManager &_eventManager;

public:      
	// public methods

	/**
	 * SCRadioMenuItem
	 * 
	 * @detail
	 *   Creates a SCRadioMenuItem.  Call begin() after created and before using
	 * 
	 * @param[in] eventManager Reference to eventManager object that enqueues event messages for us
	 */
	SCRadioMenuItem(EventManager &eventManager);

	/**
	 * getMenuItemEventType
	 * 
	 * @detail
	 *   returns the event type that is enqueued when this menu item is changed
	 * 
	 * @returns EventType
	 */
	EventType getMenuItemEventType();

	/** 
	 * getMenuItemIndex
	 * 
	 * @detail
	 *   Returns the index of this menu item within the array of menu items
	 * 
	 * @returns index of menu item as integer
	 */
	int8_t getMenuItemIndex();

	/**
	 * setMenuItemEventType
	 * 
	 * @detail
	 *   Sets the event type that will be enqueued when this menu item changes
	 *   
	 * @param[in] eventType
	 */
	void setMenuItemEventType(EventType eventType);

	/**
	 * setMenuItemIndex
	 * 
	 * @detail
	 *   Used to hold the index number of this menu item within the menu items array.
	 * 
	 * @param[in] menuItemIndex
	 */
	void setMenuItemIndex(int8_t menuItemIndex);
	
	/**
	 * setMenuItemName
	 * 
	 * @detail
	 *   Sets the display name of the menu item
	 *   
	 * @param[in] origin Pointer to where to copy name from
	 */
	void setMenuItemName(char* origin);
	
	/**
	* setMenuItemValueFormat
	*
	* @detail
	*   Sets the format string used to make a text representation of a menu item's value
	*
	* @param[in] origin Pointer to where to copy format string from
	*/
	void setMenuItemValueFormat(char* origin);

	// public virtual functions.  These are implemented in this class but can be overriden in derived classes

	/**
	 * getMenuItemDisplayName
	 * 
	 * @detail
	 *   Gets the menu item's name for display in the menu
	 * 
	 * @param[out] destinationForCopy Where to copy display name
	 */
	void virtual getMenuItemDisplayNameCopy(char* destinationForCopy);
		
	// public pure virtual functions.  These are implemented only in derived classes

	/**
	 * adjustMenuItemValue
	 * 
	 * @detail
	 *   Adjusts this menu item's value based on the direction of the turn of the main knob
	 * 
	 * @param[in] turnDirection Directionknob was turned
	 */
	void virtual adjustMenuItemValue(KnobTurnDirection turnDirection) = 0;

	/**
	 * begin
	 * 
	 * @detail
	 *   Initializes this menu item to make it ready to use - replaces what would normally be constructor logic
	 *   This must be called before this object is used.
	 */
	void virtual begin() = 0;
	
	/**
	 * getMenuItemDisplayValueCopy
	 * 
	 * @detail
	 *   Provides a copy of the menu item's display value
	 * 
	 * @param[out] destinationForCopy Where to copy the value text
	 */
	void virtual getMenuItemDisplayValueCopy(char* destinationForCopy) = 0;
   
private:

	/** 
	 * copyDisplayTextValue
	 * 
	 * @detail
	 *   Used to copy text related to displaying menu items (length limited to 16 chars)
	 *
	 * @param[out] destination Where to copy text
	 * @param[in]  where to copy text from
	 */
	void copyDisplayTextValue(char* destination, char* origin);
};

#endif