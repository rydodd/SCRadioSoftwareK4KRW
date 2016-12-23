/*
 * SCRadioMenuItem.h - Base Class for radio menu item
 * 
 * This is an abstract class to be inherited by the actual menu item classes
 * which will implement menu items of different types
 * boolean, integer ...
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

#ifndef SCRadioMenuItem_h
#define SCRadioMenuItem_h

// forwards for classes accessed via pointers and references only
class EventManager;

// includes
#include "SCRadioConstants.h"
#include "ISCRadioReadOnlyMenuItem.h"

// Notice below that we are inheriting 
// ISCRadioReadOnlyMenuItem.
//
// ISCRadioReadOnlyMenuItem is an interface class.  It has no
// actual logic associated with it.  By inheriting it, we are
// required to implement methods that the interface provides
// definitions for.  This particular interface
// only has methods for getting menu item values.  It has no
// methods for changing them.  So By inheriting this interface
// we allow the menu item to be accessed in an readonly 
// fashion by parts of the application that should never change
// the menu item's values.
// 
// Because of the 'is a' relationship that inheritance gives us
// we can treat any menu item as if it 'is a'
// ISCRadioReadOnlyMenuItem by accessing it through pointer
// or reference that is of type ISCRadioReadOnlyMenuItem.
//
// So, even though the object we are referencing actually has
// methods to change internal values, we reference it as if 
// it does not and therefore those methods are not accessible.
// 
// Interfaces are a very powerful concept.  They allow you to
// dictate terms for interaction with other objects without
// requiring those objects' authors to inherit everything 
// from your classes including logic and data they don't 
// really need or want.
//
class SCRadioMenuItem : public ISCRadioReadOnlyMenuItem
{
private:
	// private member variables

	/**
	 * Character array holding menu item name
	 */
	const char* _menuItemName;

public:
	/**
	* Used to enqueue messages resulting from menu item value changes
	*/
	EventManager &_eventManager;

	// protected member data

	/**
	 * Format string for formatting the menu item's value for display
	 */
	const char* _menuItemValueFormat;

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
	 * Menu item value (integer representation)
	 */
	int32_t _menuItemValue;

	/**
	 * Menu item increment value
	 */
	int8_t _incrementValue = 1;

	/**
	 * Menu item minimum value
	 */
	int32_t _minimumValue;

	/**
	 * Menu item maximum value
	 */
	int32_t _maximumValue;

public:      
	// public methods

	/**
	* SCRadioMenuItem
	*
	* @detail
	*   Creates a SCRadioMenuItem.  Call begin() after created and before using
	*
	* @param[in] eventManager Reference to eventManager object that enqueues event messages for us
	* @param[in] initialValue Initial value for menu item
	* @param[in] incrementValue Increment value for menu item
	* @param[in] minimumValue minimum value
	* @param[in] maximumValue maximum value
	*/
	SCRadioMenuItem(EventManager &eventManager,
		int32_t initialValue,
		int8_t incrementValue,
		int32_t minimumValue,
		int32_t maximumValue);

	/**
	 * SCRadioMenuItem
	 * 
	 * @detail
	 *   Creates a SCRadioMenuItem.  Call begin() after created and before using
	 * 
	 * @param[in] eventManager Reference to eventManager object that enqueues event messages for us
	 * @param[in] initialValue Initial value for menu item
	 * @param[in] minimumValue minimum value
	 * @param[in] maximumValue maximum value
	 */
	SCRadioMenuItem(EventManager &eventManager,
		             int32_t initialValue,
		             	int32_t minimumValue,
		             		int32_t maximumValue);

	/**
	* adjustMenuItemValue
	*
	* @detail
	*   Adjusts this menu item's value based on the direction of the turn of the main knob
	*
	* @param[in] turnDirection Directionknob was turned
	*/
	void adjustMenuItemValue(KnobTurnDirection turnDirection);

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
	* getMenuItemValue
	*
	* @detail
	*   Gets the menu items current value (integer form)
	*/
	int32_t getMenuItemValue();

	/**
	 * menuItemExternallyChangedListener
	 * 
	 * @detail
	 *   Adjusts menu item value if an external process changes requests a change
	 *   
	 * @param[in] eventCode event type of message
	 * @param[in] menuItemValue new menu item value
	 */
	void menuItemExternallyChangedListener(int eventCode, int menuItemValue);

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
	void setMenuItemName(const char* origin);

	/**
	 * setMenuItemValue
	 * 
	 * @detail
	 *   Sets the menu items value
	 */
	void setMenuItemValue(int32_t newValue);

	/**
	* setMenuItemValueFormat
	*
	* @detail
	*   Sets the format string used to make a text representation of a menu item's value
	*
	* @param[in] origin Pointer to where to copy format string from
	*/
	void setMenuItemValueFormat(const char* origin);

	/**
	 * getMenuItemDisplayName
	 * 
	 * @detail
	 *   Gets the menu item's name for display in the menu
	 * 
	 * @param[out] destinationForCopy Where to copy display name
	 */
	void getMenuItemDisplayNameCopy(char* destinationForCopy);

	/**
	* rangeCheckValue
	*
	* @detail
	*   Returns the provided value or a corrected value if the provided value is
	*   out of range for this menu item.
	*
	* @param[in] valueToCheck Value to test
	*
	* @returns original or corrected value;
	*/
	int32_t rangeCheckValue(int32_t valueToCheck);

	// public virtual methods
	/**
	* begin
	*
	* @detail
	*   Initializes this menu item to make it ready to use - replaces what would normally be constructor logic
	*   This must be called before this object is used.
	*/
	void virtual begin();

	// public pure virtual functions.  These are implemented only in derived classes

	/**
	 * getMenuItemDisplayValueCopy
	 * 
	 * @detail
	 *   Provides a copy of the menu item's display value
	 * 
	 * @param[out] destinationForCopy Where to copy the value text
	 */
	void virtual getMenuItemDisplayValueCopy(char* destinationForCopy);

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
	void copyDisplayTextValue(char* destination, const char* origin);
};

#endif