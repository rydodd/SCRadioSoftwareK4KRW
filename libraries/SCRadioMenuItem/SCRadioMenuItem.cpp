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
 * @version 1.0.2  12/12/2016.
 */

#include "Arduino.h"
#include "EventManager.h"
#include "SCRadioConstants.h"
#include "SCRadioMenuItem.h"

 // Constructor
 // The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioMenuItem::SCRadioMenuItem(EventManager &eventManager) : _eventManager(eventManager)
{
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
}

// public member methods


void SCRadioMenuItem::getMenuItemDisplayNameCopy(char *destinationForCopy)
{
	strcpy(destinationForCopy, "                ");
	copyDisplayTextValue(destinationForCopy, _menuItemName);
}

EventType SCRadioMenuItem::getMenuItemEventType()
{
	return _menuItemEventType;
}

int8_t SCRadioMenuItem::getMenuItemIndex()
{
	return _menuItemIndex;
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
	copyDisplayTextValue(_menuItemName, origin);
}

void SCRadioMenuItem::setMenuItemValueFormat(const char *origin) 
{
	copyDisplayTextValue(_menuItemValueFormat, origin);
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