/*
 * SCRadioMenuItem.cpp - Class for radio menu item
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

#include "Arduino.h"
#include "EventManager.h"
#include "SCRadioConstants.h"
#include "SCRadioMenuItem.h"

 // Constructor
 // The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioMenuItem::SCRadioMenuItem(EventManager &eventManager,
									int32_t initialValue,
										int8_t incrementValue,
										int32_t minimumValue,
											int32_t maximumValue) : 
								_eventManager(eventManager),
								_menuItemValue(initialValue),
								_incrementValue(incrementValue),
								_minimumValue(minimumValue),
								_maximumValue(maximumValue)
{
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
}

// Constructor
// The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioMenuItem::SCRadioMenuItem(EventManager &eventManager,
	int32_t initialValue,
	int32_t minimumValue,
	int32_t maximumValue) :
	_eventManager(eventManager),
	_menuItemValue(initialValue),
	_minimumValue(minimumValue),
	_maximumValue(maximumValue)
{
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
}

// public member methods

void SCRadioMenuItem::adjustMenuItemValue(KnobTurnDirection turnDirection)
{
	// Serial.println("In adjust menu item value");
	int32_t newValue = _menuItemValue + _incrementValue * static_cast<int32_t>(turnDirection);
	_menuItemValue = rangeCheckValue(newValue);
	_eventManager.queueEvent(static_cast<int>(EventType::MENU_ITEM_VALUE_CHANGED), _menuItemIndex);
	_eventManager.queueEvent(static_cast<int>(_menuItemEventType), _menuItemIndex);
}

// sets up object so it is ready for use.  Type of logic that is normally in a constructor
void SCRadioMenuItem::begin()
{
	_menuItemValue = rangeCheckValue(_menuItemValue);
}

void SCRadioMenuItem::getMenuItemDisplayNameCopy(char *destinationForCopy)
{
	strcpy(destinationForCopy, "                ");
	copyDisplayTextValue(destinationForCopy, _menuItemName);
}

void SCRadioMenuItem::getMenuItemDisplayValueCopy(char* destinationForCopy)
{
	sprintf(destinationForCopy, _menuItemValueFormat, _menuItemValue);
	return;
}

EventType SCRadioMenuItem::getMenuItemEventType()
{
	return _menuItemEventType;
}

int8_t SCRadioMenuItem::getMenuItemIndex()
{
	return _menuItemIndex;
}

int32_t SCRadioMenuItem::getMenuItemValue()
{
	return _menuItemValue;
}

void SCRadioMenuItem::menuItemExternallyChangedListener(int eventCode, int menuItemValue)
{
	setMenuItemValue(menuItemValue);
}

int32_t SCRadioMenuItem::rangeCheckValue(int32_t valueToCheck)
{
	if (valueToCheck < _minimumValue)
	{
		return _minimumValue;
	}

	if (valueToCheck > _maximumValue)
	{
		return _maximumValue;
	}

	return valueToCheck;
}

void SCRadioMenuItem::setMenuItemEventType(EventType eventType)
{
	_menuItemEventType = eventType;
}

void SCRadioMenuItem::setMenuItemIndex(int8_t menuItemIndex)
{
	_menuItemIndex = menuItemIndex;
}

void SCRadioMenuItem::setMenuItemName(const char *origin)
{
	_menuItemName = origin;
}

void SCRadioMenuItem::setMenuItemValue(int32_t newValue)
{
	_menuItemValue = rangeCheckValue(newValue);
}

void SCRadioMenuItem::setMenuItemValueFormat(const char *origin) 
{
	_menuItemValueFormat = origin;
}

void SCRadioMenuItem::copyDisplayTextValue(char *destination, const char *origin)
{
	int len = strlen(origin);
	if (len > TEXT_FOR_DISPLAY_MAX_LENGTH)
	{
		len = TEXT_FOR_DISPLAY_MAX_LENGTH;
	}
	strncpy(destination, origin, len);
	destination[len] = 0;
}