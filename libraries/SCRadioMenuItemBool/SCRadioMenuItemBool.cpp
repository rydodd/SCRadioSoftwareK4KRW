/*
* SCRadioMenuItem.cpp - Class for radio menu item for boolean value type options
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
#include "SCRadioEventData.h"
#include "SCRadioMenuItem.h"
#include "SCRadioMenuItemBool.h"

SCRadioMenuItemBool::SCRadioMenuItemBool(EventManager &eventManager,
									bool initialValue) : SCRadioMenuItem(eventManager),
														_menuItemValue(initialValue)
{
}

SCRadioMenuItemBool::~SCRadioMenuItemBool()
{
}

// sets up object so it is ready for use.  Type of logic that is normally in a constructor
void SCRadioMenuItemBool::begin()
{
}

void SCRadioMenuItemBool::adjustMenuItemValue(KnobTurnDirection turnDirection)
{
	// Serial.println("In adjust menu item value");
	_menuItemValue = !_menuItemValue;
	_eventManager.queueEvent(static_cast<int>(EventType::MENU_ITEM_VALUE_CHANGED), _menuItemIndex);
	_eventManager.queueEvent(static_cast<int>(_menuItemEventType), _menuItemIndex);
}


void SCRadioMenuItemBool::getMenuItemDisplayValueCopy(char* destinationForCopy)
{
	if (_menuItemValue)
	{
		sprintf(destinationForCopy, _menuItemValueFormat, _trueValueText);
		return;
	}

	sprintf(destinationForCopy, _menuItemValueFormat, _falseValueText);
}

long SCRadioMenuItemBool::getMenuItemValue()
{
	if (_menuItemValue)
	{
		return 1;
	}

	return 0;
}

void SCRadioMenuItemBool::menuItemExternallyChangedListener(int eventCode, int menuItemValue)
{
	switch(menuItemValue)
	{
		case 0:
			{
			_menuItemValue = false;
			break;
			}
		case 1:
			{
			_menuItemValue = true;
			break;
			}
		default:
		{
			// nothing to do - ignore invalid value
		}
	}
}

void SCRadioMenuItemBool::setFalseValueText(const char* falseText)
{
	strncpy(_falseValueText, falseText, 9);
	_falseValueText[9] = 0;
}

void SCRadioMenuItemBool::setTrueValueText(const char* trueText)
{
	strncpy(_trueValueText, trueText, 9);
	_trueValueText[9] = 0;
}



