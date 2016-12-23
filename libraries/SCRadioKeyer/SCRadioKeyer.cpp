/*
* SCRadioKeyer.cpp - Class for controlling radio's CW Keyer
*
* Based on OpenQRP Blog Iambic Morse Code Keyer Sketch by 
* Steven T. Elliott, K1EL
* 
* Modified by Glen Popiel, KW5GP
* 
* Further modified for the SCRadioSoftwareK4KRW project by Richard Y. Dodd, 
* K4KRW
* 
* Added back missing logic for selecting iambic a or b mode and also
* for reversing the paddles
* 
* 12/19/2016
* 
* Based on OpenQRP Blog Iambic Morse Code Keyer Sketch by Steven T. Elliott, 
* K1EL
*/

//////////////////////////////////////////////////////////////////////////////
//
//  Iambic Morse Code Keyer Sketch
//  Copyright (c) 2009 Steven T. Elliott
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details:
//
//  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
//  Boston, MA  02111-1307  USA
//
//////////////////////////////////////////////////////////////////////////////

#include "Arduino.h"
#include "EventManager.h"
#include "SCRadioConstants.h"
#include "SCRadioEventData.h"
#include "SCRadioKeyer.h"
#include "ISCRadioReadOnlyMenuItem.h"

SCRadioKeyer::SCRadioKeyer(EventManager &eventManager,
							SCRadioEventData &eventData) : 
								_eventManager(eventManager),
								_eventData(eventData)
{
	
}

void SCRadioKeyer::begin()
{
	_stuckKeyCheckPassed = false;

	_inStuckKeyErrorState = false;

	// Setup outputs
	pinMode(CW_KEY_PADDLE_JACK_TIP_PIN, INPUT);      // sets CW Key Jack tip
												     // digital pin as input

	pinMode(CW_KEY_PADDLE_JACK_RING_PIN, INPUT);     // sets CW key jack ring
	                                                 // digital pin as input

	digitalWrite(CW_KEY_PADDLE_JACK_TIP_PIN, HIGH);  // Enable pullup resistors
	digitalWrite(CW_KEY_PADDLE_JACK_RING_PIN, HIGH);     

	_keyerMode = KeyerMode::STRAIGHT_KEY;  // default mode straight key
	_keyerState = KeyerState::IDLE;  

	_keyerControl = 0;
	
	// Note: To reverse paddles, uncomment the following line
	// _keyerControl |= PDLSWAP_BIT;

	_keyerWPM = 12;                        // default speed 12 WPM

	loadWPM(_keyerWPM);                    // Set element timing to match
	                                       // selected WPM
}

void SCRadioKeyer::loop()
{
	if (_keyerMode == KeyerMode::STRAIGHT_KEY) 
	{
		// Straight Key Mode
		// Watch tip only
		if (digitalRead(CW_KEY_PADDLE_JACK_TIP_PIN) == LOW)
		{
			if (!_stuckKeyCheckPassed)
			{
				_eventManager.queueEvent(
					static_cast<int>(EventType::ERROR_OCCURRED), 
						static_cast<int>(ErrorType::STUCK_KEY));

				_inStuckKeyErrorState = true;

				return;
			}
			_eventManager.queueEvent(
				static_cast<int>(EventType::KEY_LINE_CHANGED), 
					static_cast<int>(KeyStatus::PRESSED));
		}
		else {
			if (_inStuckKeyErrorState)
			{
				_inStuckKeyErrorState = false;
				_eventManager.queueEvent(
					static_cast<int>(EventType::FREQUENCY_CHANGED),
					static_cast<int>(EventFrequencyField::OPERATING_FREQUENCY));
			}

			_stuckKeyCheckPassed = true;

			_eventManager.queueEvent(
				static_cast<int>(EventType::KEY_LINE_CHANGED), 
					static_cast<int>(KeyStatus::RELEASED));
		}

	}
	else {

		// _keyerControl contains processing flags and keyer mode bits
		// Supports Iambic A and B
		// State machine based, uses calls to millis() for timing.
		switch (_keyerState)
		{
		case KeyerState::IDLE:      // Wait for direct or latched paddle press
			if ((digitalRead(CW_KEY_PADDLE_JACK_TIP_PIN) == LOW) || 
					(digitalRead(CW_KEY_PADDLE_JACK_RING_PIN) == LOW) || 
						(_keyerControl & (DIT_LATCH_BIT + DAH_LATCH_BIT)))
			{
				if(!_stuckKeyCheckPassed)
				{
					_eventManager.queueEvent(
						static_cast<int>(EventType::ERROR_OCCURRED), 
							static_cast<int>(ErrorType::STUCK_KEY));

					_inStuckKeyErrorState = true;

					return;
				}

				updatePaddleLatch();
				_keyerState = KeyerState::CHK_DIT;
			}
			else
			{
				if (_inStuckKeyErrorState)
				{
					_inStuckKeyErrorState = false;

					// the following makes the display update and clear the error message
					_eventManager.queueEvent(
						static_cast<int>(EventType::FREQUENCY_CHANGED), 
							static_cast<int>(EventFrequencyField::OPERATING_FREQUENCY));
				}

				_stuckKeyCheckPassed = true;
			}
			break;

		case KeyerState::CHK_DIT:      // See if the dit paddle was pressed
			if (_keyerControl & DIT_LATCH_BIT)
			{
				_keyerControl |= DIT_PROC_BIT;
				_keyerTimer = _ditTime;
				_keyerState = KeyerState::KEYED_PREP;
			}
			else {
				_keyerState = KeyerState::CHK_DAH;
			}
			break;

		case KeyerState::CHK_DAH:      // See if dah paddle was pressed
			if (_keyerControl & DAH_LATCH_BIT)
			{
				_keyerTimer = _ditTime * DAH_TIMING_MULTIPLE;
				_keyerState = KeyerState::KEYED_PREP;
			}
			else {
				_keyerState = KeyerState::IDLE;
			}
			break;

		case KeyerState::KEYED_PREP:	// Assert key down, start timing, 
										// state shared for dit or dah
			_eventManager.queueEvent(
				static_cast<int>(EventType::KEY_LINE_CHANGED), 
					static_cast<int>(KeyStatus::PRESSED),
						EventManager::kHighPriority);       // tell rig to 
															// transmit

			_keyerTimer += millis();                // set _keyerTimer to  
													// interval end time

			_keyerControl &= ~(DIT_LATCH_BIT + DAH_LATCH_BIT);  // clear both 
																// paddle latch 
																// bits

			_keyerState = KeyerState::KEYED;                  // next state
			break;

		case KeyerState::KEYED:      // Wait for timer to expire
			if (millis() > _keyerTimer)  // are we at end of key down ?
			{
				_eventManager.queueEvent(
					static_cast<int>(EventType::KEY_LINE_CHANGED),
					static_cast<int>(KeyStatus::RELEASED),
					EventManager::kHighPriority);     // Stop transmit

				_keyerTimer = millis() + _ditTime;     // inter-element time

				_keyerState = KeyerState::INTER_ELEMENT;      // next state
			}
			else if (_keyerMode == KeyerMode::IAMBICB) 
			{
				updatePaddleLatch();     // early paddle latch in Iambic B mode
			}
			break;

		case KeyerState::INTER_ELEMENT:      // Insert time between dits/dahs
			updatePaddleLatch();               // latch paddle state
			if (millis() > _keyerTimer)  // are we at end of inter-space ? 
			{
				if (_keyerControl & DIT_PROC_BIT)      // was it a dit or dah?
				{
					_keyerControl &= ~(DIT_LATCH_BIT + DIT_PROC_BIT); // clear
																// latch bits

					_keyerState = KeyerState::CHK_DAH;		// dit done, 
															// check for dah
				}
				else {
					_keyerControl &= ~(DAH_LATCH_BIT);       // clear dah latch
					_keyerState = KeyerState::IDLE;          // go idle
				}
			}
			break;
		}
	}
}  // End Main Loop

void SCRadioKeyer::keyerModeChangedListener(int eventCode, int whichMenuItem)
{
	ISCRadioReadOnlyMenuItem* menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);

	long menuItemValue = menuItem->getMenuItemValue();

	_keyerMode = KeyerMode(menuItemValue);
}

void SCRadioKeyer::keyerSpeedChangedListener(int eventCode, int whichMenuItem)
{
	ISCRadioReadOnlyMenuItem* menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);

	_keyerWPM = menuItem->getMenuItemValue();

	loadWPM(_keyerWPM);
}

void SCRadioKeyer::keyerPaddlesOrientationChangedListener(int eventCode, int whichMenuItem)
{
	ISCRadioReadOnlyMenuItem* menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);

	long paddlesOrientation = menuItem->getMenuItemValue();

	setPaddlesOrientation(PaddlesOrientation(paddlesOrientation));
}

void SCRadioKeyer::setKeyerMode(KeyerMode newKeyerMode)
{
	_keyerMode = newKeyerMode;
}

void SCRadioKeyer::setKeyerWPM(int8_t keyerWPM)
{
	_keyerWPM = keyerWPM;
	loadWPM(_keyerWPM);
}

void SCRadioKeyer::setPaddlesOrientation(PaddlesOrientation orientation)
{
	if (orientation == PaddlesOrientation::NORMAL)
	{
		// clear paddle swap bit
		_keyerControl &= ~PDLSWAP_BIT;
	}
	else
	{
		_keyerControl |= PDLSWAP_BIT;
	}

}

void SCRadioKeyer::updatePaddleLatch()
{
	if (digitalRead(CW_KEY_PADDLE_JACK_RING_PIN) == LOW) {
		if (_keyerControl & PDLSWAP_BIT)
		{
			_keyerControl |= DIT_LATCH_BIT;
		}
		else
		{
			_keyerControl |= DAH_LATCH_BIT;
		}
	}
	if (digitalRead(CW_KEY_PADDLE_JACK_TIP_PIN) == LOW) {
		if (_keyerControl & PDLSWAP_BIT)
		{
			_keyerControl |= DAH_LATCH_BIT;
		}
		else
		{
			_keyerControl |= DIT_LATCH_BIT;
		}
	}

}

void SCRadioKeyer::loadWPM(int8_t wpm)
{
	_ditTime = 1200 / wpm;
}