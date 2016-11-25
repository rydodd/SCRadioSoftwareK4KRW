/**
 * SCRadioKey.h - Class for capturing input from CW key
 * 
 * Copyright (c) 2016 - Richard Young Dodd
 *
 * Richard Young Dodd licenses this file to you under the MIT license.
 * See the LICENSE file in the project root for more information.
 * If you did not receive the 'LICENSE' file with this software
 * see <https://opensource.org/licenses/MIT>.
 * 
 * Note: this class does not enqueue messages in EventHandler for now.  It is instead called by the SCRadioMainKnob class which
 * enqueue's the messages
 * 
 * @author Richard Y. Dodd - K4KRW
 * @version 1.0  11/20/2016.
 */

#ifndef SCRadioKey_h
#define SCRadioKey_h

 // forwards for classes accessed via pointers and references only
class EventManager;

#include "SCRadioConstants.h"

class SCRadioKey
{
private:
	// private member data

	/**
	 * Arduino pin number listening for input from the key
	 */
	int8_t _keyInputPin;

	/**
	 * The state of the key when last checked
	 */
	KeyStatus _lastKeyStatus;

	/**
	 * Indicates that the rig has successfully passed the stuck CW key check
	 */
	bool _stuckKeyCheckPassed;

	/**
	 * Indicates that the stuck key state is currently being experienced
	 */
	bool _inStuckKeyErrorState;

	/**
	 * Reference to event manager used to send messages about state of CW key
	 */
	EventManager &_eventManager;
   
public: 
	// public methods

	/**
	 * SCRadioKey
	 * 
	 * @detail
	 *   Creates a SCRadioKey object.  Call begin() after creating and before using.
	 * 
	 * @param[in] eventManager Reference to an eventManager object used to enqueue event messages
	 * @param[in] keyInputPin Pin on the Arduino that is connected to the CW key
	 */
	SCRadioKey(EventManager &eventManager,
    				int8_t keyInputPin);

	/**
	 * begin
	 * 
	 * @detail
	 *   This should be called to initialize this object before it is used.
	 */
	void begin();
	
	/**
	 * loop
	 * 
	 * @detail
	 *   This should be called in apps main loop method to process CW key presses
	 */
	void loop();
    
	// private methods
private:
};

#endif