/**
* SCRadioKey.cpp - Class for capturing input from CW key
*
* Copyright (c) 2016 - Richard Young Dodd
*
* Richard Young Dodd licenses this file to you under the MIT license.
* See the LICENSE file in the project root for more information.
* If you did not receive the 'LICENSE' file with this software
* see <https://opensource.org/licenses/MIT>.
*
*
* Note: this class does not enqueue messages in EventHandler for now.  It is instead called by the SCRadioMainKnob class which
* enqueue's the messages
*
* @author Richard Y. Dodd - K4KRW
* @version 1.0  11/20/2016.
*/

#include "Arduino.h"

#include "EventManager.h"

#include "SCRadioConstants.h"
#include "SCRadioKey.h"

// Constructor
// The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioKey::SCRadioKey(EventManager &eventManager,
    				int8_t keyInputPin) : _eventManager(eventManager), 
											_keyInputPin(keyInputPin)
{
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
}

void SCRadioKey::loop() 
{	
	int8_t keyPinState = digitalRead(_keyInputPin);
//	Serial.print("keyPinState : ");
//	Serial.println(keyPinState);
	
	KeyStatus keyStatus;
	
	if (keyPinState == LOW) 
	{
		keyStatus = KeyStatus::PRESSED;

		if (!_stuckKeyCheckPassed)
		{
			_eventManager.queueEvent(static_cast<int>(EventType::ERROR_OCCURRED), static_cast<int>(ErrorType::STUCK_KEY));
			_inStuckKeyErrorState = true;

			return;
		}

		if (_lastKeyStatus != keyStatus) 
		{
			_eventManager.queueEvent(static_cast<int>(EventType::KEY_LINE_CHANGED), static_cast<int>(KeyStatus::PRESSED ));
		}			
	} 
	else 
	{
		keyStatus = KeyStatus::RELEASED;

		if (_inStuckKeyErrorState)
		{
			_inStuckKeyErrorState = false;
			_eventManager.queueEvent(static_cast<int>(EventType::FREQUENCY_CHANGED), static_cast<int>(EventFrequencyField::OPERATING_FREQUENCY));
		}

		_stuckKeyCheckPassed = true;

		if (_lastKeyStatus != keyStatus) 
		{
			_eventManager.queueEvent(static_cast<int>(EventType::KEY_LINE_CHANGED), static_cast<int>(KeyStatus::RELEASED ));
		}		
	}

	_lastKeyStatus = keyStatus;
}

void SCRadioKey::begin()
{
	pinMode(_keyInputPin, INPUT_PULLUP);
	_lastKeyStatus = KeyStatus::PRESSED;
	_stuckKeyCheckPassed = false;
	_inStuckKeyErrorState = false;
}