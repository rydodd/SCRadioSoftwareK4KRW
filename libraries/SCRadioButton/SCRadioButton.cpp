/**
 * SCRadioMainKnob.cpp - Class for controlling a button within a short and long press scheme
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
 * @version 1.0.2  12/12/2016.
 */

#include "Arduino.h"

#include "SCRadioConstants.h"

#include "SCRadioButton.h"


// Public Methods
// The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioButton::SCRadioButton(	int8_t inputPin,
								int16_t debounceThresholdms,
								int16_t longPressThresholdms) : _pin(inputPin), 
																_debounceThresholdms(debounceThresholdms),
																_longPressThresholdms(longPressThresholdms) 
{ 
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
	//
	//Serial.begin(9600);
	//Serial.println("I will never run, I promise");
}

// Called in main setup() to initialize button object  
void SCRadioButton::begin() 
{
	// initialize arduino pin going to the button
	pinMode(_pin, INPUT_PULLUP);

	// starting state is HIGH (not pressed)
	_state = HIGH;
}

// Notes button presses.  Keeps track of how long and returns press type (short or long or none)
ButtonPressType SCRadioButton::checkButton() 
{
	int8_t prevState = _state;
	_state = digitalRead(_pin);

	//Serial.print("Button state: ");
	//Serial.println(_state);
	if (prevState == HIGH && _state == LOW) 
	{
		// Was not pressed but now is
		// Start keeping track of how long pressed
		_buttonDownMillis = millis();
		return ButtonPressType::NONE;
	}
	else if (prevState == LOW && _state == HIGH) 
	{
		// was pressed but is now not
		// Determine whether a bounce (intermittent connection when pressed)
		// or a legitimate short or long press.
		uint32_t currentMillis = millis();
		//  	Serial.print( " : currentMills ");
		//  	Serial.print( currentMillis );
  	
		if ((int32_t)(currentMillis - _buttonDownMillis) < _debounceThresholdms) 
		{
			// ignore this for debounce
			return ButtonPressType::NONE;
		}
		else if ((int32_t)(currentMillis - _buttonDownMillis) < _longPressThresholdms)
		{
			// short click
			return ButtonPressType::SHORT;
			//Serial.print(" short ");
		}
		else 
		{
			// long click
			return ButtonPressType::LONG;
			//Serial.print(" long ");
		}
	}
	else
	{
		return ButtonPressType::NONE;
	}
}