/**
SCRadioMainKnob.h - Class for controlling the main rotary encoder knob for software controlled radio

* Copyright (c) 2016 - Richard Young Dodd
*
* Richard Young Dodd licenses this file to you under the MIT license.
* See the LICENSE file in the project root for more information.
* If you did not receive the 'LICENSE' file with this software
* see <https://opensource.org/licenses/MIT>.

@author Richard Y. Dodd - K4KRW
@version 1.0  11/20/2016.
*/

#ifndef SCRadioMainKnob_h
#define SCRadioMainKnob_h

// forwards for class pointers and references
class SCRadioButton;
class EventManager;

// includes
#include "Rotary.h"
#include "SCRadioConstants.h"


class SCRadioMainKnob
{
private:
	// private member data

	/**
	 * Event manager is used to place knob and knob button events in the event queue
	 */
	EventManager &_eventManager;

	/**
	 * Rotary encoder object used to interact with encoder for knob
	 */
	Rotary _rotary;

	/**
	 * Button class handling main knob button input
	 */
	SCRadioButton &_button;

	/**
	 * The current main knob mode
	 */
	MainKnobMode _mainKnobMode;

public:
	// public methods

	/**
	 * SCRadioMainKnob
	 * 
	 * @detail
	 *   Creates a new main knob object
	 * 
	 * @param[in] eventManager Reference to event manager instance used to enqueue new messages
	 * @param[in] rotaryPin1 Arduino pin listening for rotary encoder input
	 * @param[in] rotaryPin2 Arduino pin listening for rotary encoder input
	 * @parma[in] button Reference to button representing knob's button
	 */
	SCRadioMainKnob(EventManager &eventManager, byte rotaryPin1, byte rotaryPin2, SCRadioButton &button);

	/**
	 * begin
	 * 
	 * @detail
	 *   Gets this object ready to use (replaces what would be constructor logic)
	 */
	void begin();

	/**
	 * loop
	 * 
	 * @detail
	 *   Call this once each time the main application loop runs to process main knob input
	 */
	void loop();

private:
	// private methods

	/**
	 * processRotaryEncoder
	 * 
	 * @detail
	 *   Checks to see if the rotary encoder (knob) turned
	 * 
	 * @returns KnobTurnDirection (CW or CCW)
	 */
	KnobTurnDirection processRotaryEncoder();

	/**
	 * processButton
	 * 
	 * @detail
	 *   Processes any input from the button
	 */
	void processButton();

	/**
	 * sendTurnEventMessage
	 * 
	 * @detail
	 *   Enqueues a message saying that the knob has turned
	 *   the exact message type sent depends on the current main knob mode (vfo, rit, menu, menu item)
	 */
	void sendTurnEventMessage(KnobTurnDirection knobTurnDirection);
};

#endif