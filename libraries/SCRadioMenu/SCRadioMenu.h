/*
 * SCRadioMenu.h - Class for controlling radio's menu system
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

#ifndef SCRadioMenu_h
#define SCRadioMenu_h

// forwards for classes accessed via pointers and references only
class EventManager;
class SCRadioEventData;

#include "SCRadioConstants.h"
#include "SCRadioMenuItem.h"

class SCRadioMenu
{
private:
	// private member variables

	/**
	 * The current number of menu items
	 */
	int8_t _numberOfMenuItems;

	/**
	 * Array of menu items under control of the menu
	 */
	SCRadioMenuItem* _menuItems[MAX_MENU_ITEMS];

	/**
	 * Used to send messages about menu related events
	 */
	EventManager &_eventManager;

	/**
	 * Used to access data related to events
	 */
	SCRadioEventData &_eventData;

	/**
	 * Indicates which menu item is currently selected
	 */
	int8_t _selectedMenuItem;
   
public:      
	// public methods

	/**
	 * SCRadioMenu
	 * 
	 * @detail
	 *   Creates an SCRadioMenu object.  Call begin() after creating and before using.
	 *   
	 * @param[in] eventManager Reference to eventManager object used for enqueueing messages
	 * @param[in] eventData Reference to eventData object having event related data
	 */
	SCRadioMenu(EventManager &eventManager,
    			SCRadioEventData &eventData);

	/**
	 * addMenuItem
	 * 
	 * @detail
	 *   Adds a menu item to the menu.  Items are displayed in order they are added.
	 * 
	 * @param[in] Reference to menu item to add
	 */
	void addMenuItem(SCRadioMenuItem &menuItem);
	
	/**
	 * begin
	 * 
	 * @detail
	 *   Initializes menu so it is ready to use (replaces what is normally constructor logic)
	 */
	void begin(); 
	
	/**
	 * changeSelectedMenuItem
	 * 
	 * @detail
	 *   Changes focus to the next or previous menu item depending on knob direction of turn
	 * 
	 * @param[in] turnDirection Direction knob was rotated
	 */
	void changeSelectedMenuItem(int turnDirection);

	/**
	 * menuItemKnobTurnedListener
	 * 
	 * @detail
	 *   Listens for menu item knob turn events
	 * 
	 * @param[in] eventCode Identifies what type of event message
	 * @param[in] turnDirection Identifies direction of knob turn
	 */
	void menuItemKnobTurnedListener(int eventCode, int turnDirection);
	
	/**
	* menuKnobTurnedListener
	*
	* @detail
	*   Listens for menu knob turn events
	*
	* @param[in] eventCode Identifies what type of event message
	* @param[in] turnDirection Identifies direction of knob turn
	*/
	void menuKnobTurnedListener(int eventCode, int turnDirection);
    
	private:
};

#endif