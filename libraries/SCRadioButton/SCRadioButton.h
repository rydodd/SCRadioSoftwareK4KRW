/**
 * SCRadioMainKnob.h - Class for controlling a button within a short and long press scheme
 *
 * This class watches the assigned pin for a button and indicates whether it was pressed for a short press
 * or a long press.  It sends a message to the message queue indicating a button press and it's duration.
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

#ifndef SCRadioButton_h
#define SCRadioButton_h

#include "SCRadioConstants.h"

class SCRadioButton
{
private:
	// private data members

	/**
	 * What pin on the Arduino this button is assigned to
	 */
	const int8_t _pin;
	
	/**
	 * The initial state of the pin for the button when the button is not pressed
	 */
	int8_t _state = HIGH;

	/**
	 * The millis() time the button was last noticed to change from not pressed to pressed
	 */
	uint32_t _buttonDownMillis = 0;

	/** 
	 * Durations less than this threshold in milliseconds are considered to be a bounce (repetative intermittent connections)
	 * longer durations are considered a legitimate button press
     */
	int16_t _debounceThresholdms;

	/**
	 * This is the boundary in milliseconds between a short and a long press.
	 */
	int16_t _longPressThresholdms;

	// public methods
public:

	/**
	 * SCRadioButton
	 * 
	 * @detail
	 *   Constructor that creates SCRadioButton and sets some initial values.
	 *   However, begin() must be called before using
	 * 
	 * @param[in] inputPin Arduino pin connected to the button
	 * @param[in] debounceThresholdms If time pressed is below this interval, consider it a bounce
	 * @param[in] longPressThresholdms Threshold between long and short press of button
	 */
	SCRadioButton(int8_t inputPin,
					int16_t debounceThresholdms,
					int16_t longPressThresholdms);

	/**
	 * begin
	 * 
	 * @detail
	 *   Configures SCRadioButton before use.  
	 */
	void begin();

	/**
	 * checkButton
	 * 
	 * @detail
	 *   Captures button presses
	 * 
	 * @returns the type of press for a button (none, short long)
	 */
	ButtonPressType checkButton();

private:
	// private methods
};

#endif