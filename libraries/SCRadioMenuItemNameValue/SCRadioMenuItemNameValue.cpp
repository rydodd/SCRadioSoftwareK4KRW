/*
* SCRadioMenuItemNameValue.cpp - Class for radio menu item of a name/value type
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
#include "SCRadioEventData.h"
#include "SCRadioMenuItem.h"
#include "SCRadioMenuItemNameValue.h"

SCRadioMenuItemNameValue::SCRadioMenuItemNameValue(EventManager &eventManager,
	int32_t initialValue,
	int8_t incrementValue,
	int32_t minimumValue,
	int32_t maximumValue) : SCRadioMenuItem(eventManager,
											initialValue,
											incrementValue,
											minimumValue,
											maximumValue)
{
}

SCRadioMenuItemNameValue::SCRadioMenuItemNameValue(EventManager &eventManager,
	int32_t initialValue,
	int32_t minimumValue,
	int32_t maximumValue) : SCRadioMenuItem(eventManager,
											initialValue,
											minimumValue,
											maximumValue)
{
}

SCRadioMenuItemNameValue::~SCRadioMenuItemNameValue()
{
}

void SCRadioMenuItemNameValue::getMenuItemDisplayValueCopy(char* destinationForCopy)
{
	sprintf(destinationForCopy, _menuItemValueFormat, _displayValues[_menuItemValue]);
	return;
}

void SCRadioMenuItemNameValue::setMenuItemDisplayValue(int whichMenuItem, const char* displayValue)
{
	_displayValues[whichMenuItem] = displayValue;
}

