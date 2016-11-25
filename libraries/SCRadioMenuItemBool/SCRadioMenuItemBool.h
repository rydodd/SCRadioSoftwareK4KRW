/*
 * SCRadioMenuItem.h - Class for radio menu item for boolean value type options
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

#ifndef SCRadioMenuItemBool_h
#define SCRadioMenuItemBool_h

// forwards for classes accessed via pointers and references only
class EventData;
class EventManager;

#include "SCradioConstants.h"
#include "SCRadioMenuItem.h"

class SCRadioMenuItemBool : public SCRadioMenuItem
{
private:
	// private member data

	/**
	 * Text to display if boolean value is true
	 */
	char _trueValueText[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];

	/**
	 * Text to display if boolean value is false
	 */
	char _falseValueText[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];

	/**
	 * Menu item value
	 */
	bool _menuItemValue;

public:
	// public methods

	/**
	 * SCRadioMenuItemBool
	 * 
	 * @detail
	 *   Creates a SCRadioMenuItemBool instance.  You must call begin() before using
	 *   You must also set up all of the required text for the menu item.
	 * 
	 * @param[in] eventManager Reference to event manager class with which we enqueue event messages
	 */
	SCRadioMenuItemBool(EventManager &eventManager,
		bool initialValue);
	
	/**
	 * ~SCRadioMenuItemBool 
	 * 
	 * @detail
	 *   Destructor
	 */
	virtual ~SCRadioMenuItemBool();

	/**
	 * begin
	 * 
	 * @detail
	 *   Method to setup object so it is ready to use - what would normally be constructor logic goes here.
	 */
	void begin();
	
	/**
	 * adjustMenuItemValue
	 * 
	 * @detail
	 *   Changes the menu item's value based on the direction of the turn of the knob
	 * 
	 * @param[in] turnDirection
	 */
	void virtual adjustMenuItemValue(KnobTurnDirection turnDirection);

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
	 * getMenuItemValue
	 * 
	 * @detail
	 *   Gets the actual menu item value
	 */
	int32_t getMenuItemValue();
	
	/**
	 * setTrueValueText
	 * 
	 * @detail
	 *   Sets the text to display for the menu item for true values
	 * 
	 * @param[in] trueValueText
	 */
	void setTrueValueText(char* trueValueText);

	/**
	 * setFalseValueText
	 * 
	 * @detail
	 *   Sets the text to display for the menu item for false values
	 *   
	 * @param[in] falseValueText
	 */
	void setFalseValueText(char* falseValueText);
    
private:
};

#endif