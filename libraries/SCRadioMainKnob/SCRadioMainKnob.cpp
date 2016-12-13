/**
  SCRadioMainKnob.cpp - Class for controlling rotary encoder knob for software controlled radio

  * Copyright (c) 2016 - Richard Young Dodd
  *
  * Richard Young Dodd licenses this file to you under the MIT license.
  * See the LICENSE file in the project root for more information.
  * If you did not receive the 'LICENSE' file with this software
  * see <https://opensource.org/licenses/MIT>.

  @author Richard Y. Dodd - K4KRW
  @version 1.0.2  12/12/2016.
*/

#include "EventManager.h"
#include "SCRadioButton.h"
#include "SCRadioMainKnob.h"

// Constructor
// The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioMainKnob::SCRadioMainKnob(EventManager &eventManager,
								byte rotaryPin1, 
								byte rotaryPin2, 
								SCRadioButton &button) : 
									_eventManager(eventManager), 
									_rotary(rotaryPin1, rotaryPin2), 
									_button(button) 
{      
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
}

// public methods
void SCRadioMainKnob::begin() 
{
	_mainKnobMode = MainKnobMode::VFO;
	_button.begin();
}

void SCRadioMainKnob::loop() 
{
	processButton();
	KnobTurnDirection knobTurnDirection = processRotaryEncoder();

	if (knobTurnDirection == KnobTurnDirection::NONE) 
	{
		return;
	}

	sendTurnEventMessage(knobTurnDirection);
}

// private methods
void SCRadioMainKnob::processButton()
{
	ButtonPressType buttonPressType = _button.checkButton();

	switch (_mainKnobMode)
	{
	case MainKnobMode::VFO:
		// Serial.println("Knob VFO");
		switch (buttonPressType)
		{
		case ButtonPressType::SHORT:

			// debug
			//					 Serial.println("Short press");
			//					 Serial.println("Goto Main Menu");

			_mainKnobMode = MainKnobMode::MAIN_MENU;
			break;
		case ButtonPressType::LONG:
			// Serial.println("Long press");
			_mainKnobMode = MainKnobMode::RIT;
			break;
		default:
			// Serial.println("Default no press");
			return;
		}
		break;
	case MainKnobMode::RIT:
		// Serial.println("Knob RIT");
		switch (buttonPressType)
		{
		case ButtonPressType::SHORT:
			// Serial.println("Short press");
			_mainKnobMode = MainKnobMode::VFO;
			break;
		case ButtonPressType::LONG:
			// Serial.println("Long press");
			_mainKnobMode = MainKnobMode::VFO;
			break;
		default:
			// Serial.println("Default no press");
			return;
		}
		break;
	case MainKnobMode::MAIN_MENU:
		// Serial.println("Knob Menu");
		switch (buttonPressType)
		{
		case ButtonPressType::SHORT:
			// Serial.println("Short press");
			_mainKnobMode = MainKnobMode::MENU_ITEM;
			break;
		case ButtonPressType::LONG:
			// Serial.println("Long press");
			_mainKnobMode = MainKnobMode::VFO;
			break;
		default:
			// Serial.println("Default no press");
			return;
		}
		break;
	case MainKnobMode::MENU_ITEM:
		switch (buttonPressType)
		{
		case ButtonPressType::SHORT:
			_mainKnobMode = MainKnobMode::MAIN_MENU;
			break;
		case ButtonPressType::LONG:
			_mainKnobMode = MainKnobMode::VFO;
			break;
		default:
			return;
		}
		break;
	default:
		return;
	}

	// debug
	//	Serial.print("Main Knob Mode Changed : ");
	//  Serial.println(static_cast<int>(_mainKnobMode));
	_eventManager.queueEvent(static_cast<int>(EventType::MAIN_KNOB_MODE_CHANGED), static_cast<int>(_mainKnobMode));
}

KnobTurnDirection SCRadioMainKnob::processRotaryEncoder() 
{
	unsigned char result = _rotary.process();

	KnobTurnDirection knobTurnDirection;

	switch (result) 
	{
		case DIR_CW:
			//Serial.println("Clockwise");
			knobTurnDirection = KnobTurnDirection::CLOCKWISE;
			break;

		case DIR_CCW:   		// Turning Counter-Clockwise, going to lower frequencies   
			//Serial.println("Counter-Clockwise");
			knobTurnDirection = KnobTurnDirection::COUNTER_CLOCKWISE;
			break;

		default:                                          // Should never be here
			knobTurnDirection = KnobTurnDirection::NONE;
			break;
	}

	return knobTurnDirection;
}

void SCRadioMainKnob::sendTurnEventMessage(KnobTurnDirection knobTurnDirection) 
{
	EventType eventTypeToSend;

	switch (_mainKnobMode) 
	{
		case MainKnobMode::VFO:
//			Serial.println("VFO Turned");
			eventTypeToSend = EventType::VFO_KNOB_TURNED;
			break;
		case MainKnobMode::RIT:
//			Serial.println("RIT Turned");
			eventTypeToSend = EventType::RIT_KNOB_TURNED;
			break;
		case MainKnobMode::MAIN_MENU:
//			Serial.println("Main Menu Turned");
			eventTypeToSend = EventType::MENU_KNOB_TURNED;
			break;
		case MainKnobMode::MENU_ITEM:
//			Serial.println("Menu Item Turned");
			eventTypeToSend = EventType::MENU_ITEM_KNOB_TURNED;
			break;
		default:
			return;
	}

	// Serial.println("Queueing turn event.");
	_eventManager.queueEvent(static_cast<int>(eventTypeToSend), static_cast<int>(knobTurnDirection), EventManager::kHighPriority);
}

