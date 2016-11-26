/**'
 *
 * SCRadioMenu.cpp - Class for controlling radio's menu system
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

#include "Arduino.h"

#include "EventManager.h"

#include "SCRadioConstants.h"
#include "SCRadioEventData.h"
#include "SCRadioMenuItem.h"
#include "SCRadioMenu.h"

// Constructor
// The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioMenu::SCRadioMenu(EventManager &eventManager,
						SCRadioEventData &eventData) : _eventManager(eventManager), 
														_eventData(eventData)
{
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin()' instead and call it after instantiating your object.
}

void SCRadioMenu::addMenuItem(SCRadioMenuItem &menuItem)
{
	_menuItems[_numberOfMenuItems] = &menuItem;
	menuItem.setMenuItemIndex(_numberOfMenuItems);
	_eventData.setMenuItem(&menuItem, _numberOfMenuItems);
	_numberOfMenuItems++;
}

// call this to initialize the object so it is ready to use
void SCRadioMenu::begin()
{
	_selectedMenuItem = 0;
	_numberOfMenuItems = 0;
}

void SCRadioMenu::changeSelectedMenuItem(int turnDirection)
{
	int newMenuItemNumber = _selectedMenuItem + turnDirection;

	if (newMenuItemNumber < 0)
	{
		newMenuItemNumber = 0;
	}
	else if (newMenuItemNumber >= _numberOfMenuItems)
	{
		newMenuItemNumber = _numberOfMenuItems - 1;
	}
	//  Serial.print("Selected menu item is ");
	//  Serial.println(newMenuItemNumber);
	_selectedMenuItem = newMenuItemNumber;
}

void SCRadioMenu::menuKnobTurnedListener(int eventCode, int turnDirection)
{
	changeSelectedMenuItem(turnDirection);
	_eventManager.queueEvent(static_cast<int>(EventType::MENU_ITEM_SELECTED), _selectedMenuItem);
}

void SCRadioMenu::menuItemKnobTurnedListener(int eventCode, int turnDirection)
{
	// Serial.println("Menu item knob turn listener");
	KnobTurnDirection knobTurnDirection = (KnobTurnDirection)turnDirection;
	_menuItems[_selectedMenuItem]->adjustMenuItemValue(knobTurnDirection);
}
