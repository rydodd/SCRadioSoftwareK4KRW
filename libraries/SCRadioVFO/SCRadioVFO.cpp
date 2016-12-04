/**
* SCRadioVFO.cpp - Class for controlling radio's VFO
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
#include "WString.h"

#include "ISCRadioReadOnlyMenuItem.h"
#include "SCRadioConstants.h"
#include "SCRadioDDS.h"
#include "SCRadioEventData.h"
#include "SCRadioFrequency.h"
#include "SCRadioKey.h"

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wreorder"
#include "SCRadioVFO.h"
//#pragma GCC diagnostic pop

// Constructor
// The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioVFO::SCRadioVFO(EventManager &eventManager,
	          SCRadioEventData &eventData,
	          SCRadioDDS &dds,
	          int8_t keyOutPin,
	          float ddsTuningWord,
	          int32_t rxOffset,
    				int32_t lowerFrequencyLimit,
    				int32_t upperFrequencyLimit,
    				int32_t ritMaxOffsetHz,
    				int16_t slowTuningIncrement,
    				int16_t mediumTuningIncrement,
    				int16_t fastTuningIncrement,
    				uint16_t mediumTuningThresholdms,
    				uint16_t fastTuningThresholdms) : 
						_eventManager(eventManager),
    					_eventData(eventData),
    					_dds(dds),
    					_ddsTuningWord(ddsTuningWord),
    					_lowerFrequencyLimit(lowerFrequencyLimit),
						_upperFrequencyLimit(upperFrequencyLimit),
    					_slowTuningIncrement(slowTuningIncrement),
    					_mediumTuningIncrement(mediumTuningIncrement),
    					_fastTuningIncrement(fastTuningIncrement),
    					_mediumTuningThresholdms(mediumTuningThresholdms),
    					_fastTuningThresholdms(fastTuningThresholdms),
						_keyOutPin(keyOutPin),
						_rxOffset(rxOffset),
						_ritMaxOffsetHz(ritMaxOffsetHz)
{
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
}

// public methods

// this initializes the instance to be ready to use.
// logic would normally be in a constructor.
void SCRadioVFO::begin()
{
	initializeKeyOutPin();

	// set some class status variables
	_ritStatus = RitStatus::DISABLED;
	_rxTXStatus = RxTxStatus::RX;
	_currentTuningIncrement = _slowTuningIncrement;
	_currentTXFrequency = _initialFrequency;
	_ritUpperLimitHz = _ritMaxOffsetHz;
	_ritLowerLimitHz = _ritMaxOffsetHz * -1;

	// initialize data in the eventData object which holds values needed by all 
	// event handling logic.  Since we can't pass an object with the event message
	// the _eventData class holds the data we would normally pass with the event message
	_eventData.setEventRelatedLong(0, EventLongField::RIT_OFFSET);
	_eventData.setEventRelatedBool(false, EventBoolField::RIT_IS_ENABLED);
	_eventData.setEventRelatedBool(false, EventBoolField::RX_OFFSET_IS_POSITIVE);

	calculateRXFrequency();
}

void SCRadioVFO::keyLineChangedListener(int eventCode, int keyStatus)
{
	// respond to CW key press
	sendToDDSTxRxFrequencyAndChangeTxRxStatus(keyStatus);
}

void SCRadioVFO::ritKnobTurnedListener(int eventCode, int turnDirection)
{
	changeRITOffset(turnDirection);
}

void SCRadioVFO::ritStatusChangedListener(int eventCode, int whichMenuItem)
{
	changeRITStatus(whichMenuItem);
}

void SCRadioVFO::rxOffsetDirectionChangedListener(int eventCode, int whichMenuItem)
{
	changeRxOffsetDirection(whichMenuItem);
}

void SCRadioVFO::setInitialFrequency(int32_t initialFrequency)
{
	// I set the initial frequency 10 Hz low here because my first action to kick things off
	// in the main app is to send a message that the knob turned CW.
	// That action will add the 10 Hz back
	// And everything (DDS, eventData, display) will have the correct frequency information
	_initialFrequency.replaceValue((int32_t)(initialFrequency - 10));
	_currentTXFrequency.replaceValue((int32_t)(initialFrequency - 10));
	_eventData.setEventRelatedFrequency(&_currentTXFrequency, EventFrequencyField::OPERATING_FREQUENCY);
}

void SCRadioVFO::vfoKnobTurnedListener(int eventCode, int turnDirection)
{
	// I don't want to change the frequency while transmitting.  So, I just bail
	if (_rxTXStatus == RxTxStatus::TX)
	{
		return;
	}

	calculateTuningIncrement();

	changeFrequency(turnDirection);
}

// private methods

// calculate RX frequency happens every time the tx frequency, rit or rx offset direction change.
// we use the calculated value every time we switch to rx status.
void SCRadioVFO::calculateRXFrequency()
{
	_currentRXFrequency.replaceValue(_currentTXFrequency);

	_currentRXFrequency.addHertz(_rxOffset);

	long currentRITOffsetHz = _eventData.getEventRelatedLong(EventLongField::RIT_OFFSET);

	if (_ritStatus == RitStatus::ENABLED)
	{
		_currentRXFrequency.addHertz(currentRITOffsetHz);
	}
}

void SCRadioVFO::calculateTuningIncrement() 
{
	uint32_t currentMillis = millis();
	int16_t calculatedIncrement;

	// Since these are static method variables, they will retain their value
	// between calls to this method.  They are only set to zero on the first call
	static int16_t lastCalculatedIncrement = 0;
	static uint32_t mSecsAtLastKnobTurn	= 0;

	// timediff is the number of milliseconds since the last knob turn event
	uint16_t timeDiff = currentMillis - mSecsAtLastKnobTurn;
  
	if (timeDiff > _mediumTuningThresholdms) 
	{
		calculatedIncrement = _slowTuningIncrement;
	} 
	else if (timeDiff > _fastTuningThresholdms) 
		{
			calculatedIncrement = _mediumTuningIncrement;
		} 
		else 
		{
			calculatedIncrement = _fastTuningIncrement;
		}
	
	// I had a problem with the tuning increment where I believe on board events
	// were causing the millis() to return abnormally low millisecond values 
	// (millis clock does not increment during interrupts)
	// So, to move to a larger tuning increment, 
	// I require two larger calculated tuning increments in a row.
	// to move to a smaller tuning increment, I don't
	if ((calculatedIncrement < _currentTuningIncrement)
	   || (calculatedIncrement == lastCalculatedIncrement))
	{
		_currentTuningIncrement = calculatedIncrement;
	}
	
	lastCalculatedIncrement = calculatedIncrement;
	
	mSecsAtLastKnobTurn = currentMillis;
}

void SCRadioVFO::changeFrequency(int8_t turnDirection)
{
	SCRadioFrequency lastTXFrequency(_currentTXFrequency);

	SCRadioFrequency newTXFrequency(_currentTXFrequency);

	if ((KnobTurnDirection)turnDirection == KnobTurnDirection::CLOCKWISE)
	{
		newTXFrequency.addHertz(_currentTuningIncrement);
	}
	else
	{
		newTXFrequency.addHertz(_currentTuningIncrement * -1);
	}

	if (newTXFrequency.equals(lastTXFrequency))
	{
		return;
	}

	checkBoundsAndCorrectIfNeeded(newTXFrequency);

	// all is good, use new value
	_currentTXFrequency.replaceValue(newTXFrequency);

	calculateRXFrequency();

	_dds.sendFrequencyToDDS(_currentRXFrequency.asInt32());

	// store off new value in eventData so display can pick it up when it gets frequency changed message
	_eventData.setEventRelatedFrequency(&_currentTXFrequency, EventFrequencyField::OPERATING_FREQUENCY);

	_eventManager.queueEvent(static_cast<int>(EventType::FREQUENCY_CHANGED), static_cast<int>(EventFrequencyField::OPERATING_FREQUENCY));
}

void SCRadioVFO::changeRITStatus(int8_t whichMenuItem)
{
	ISCRadioReadOnlyMenuItem* menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);

	long menuItemValue = menuItem->getMenuItemValue();

	_ritStatus = (RitStatus)menuItemValue;

	// this value is used by the display when it gets notified that RIT status changed
	_eventData.setEventRelatedBool((_ritStatus == RitStatus::ENABLED), EventBoolField::RIT_IS_ENABLED);

	calculateRXFrequency();

	_dds.sendFrequencyToDDS(_currentRXFrequency.asInt32());
}

void SCRadioVFO::changeRITOffset(int8_t turnDirection)
{
	int32_t newRITOffsetHz;

	// if we are transmitting, don't respond to RIT change requests
	if (_rxTXStatus == RxTxStatus::TX)
	{
		return;
	}

	long currentRITOffsetHz = _eventData.getEventRelatedLong(EventLongField::RIT_OFFSET);

	// if current rit setting is zero, we are changing it to a non-zero value.  So, turn rit on.
	if (currentRITOffsetHz == 0)
	{
		initiateRITStatusChange(RitStatus::ENABLED);
	}

	newRITOffsetHz = currentRITOffsetHz;

	if (turnDirection == 1)
	{
		newRITOffsetHz += RIT_ADJUST_INCREMENT;
	}
	else
	{
		newRITOffsetHz -= RIT_ADJUST_INCREMENT;
	}

	newRITOffsetHz = checkRITBoundariesAndCorrectIfNeeded(newRITOffsetHz);

	// value is good, use it
	currentRITOffsetHz = newRITOffsetHz;

	// if new rit offset is 0, turn off rit.
	if (currentRITOffsetHz == 0)
	{
		initiateRITStatusChange(RitStatus::DISABLED);
	}

	// store away in event data so Display can pick up the value to display
	_eventData.setEventRelatedLong(currentRITOffsetHz, EventLongField::RIT_OFFSET);

	// update the rx frequency to reflect the new RIT adjustment
	calculateRXFrequency();

	_dds.sendFrequencyToDDS(_currentRXFrequency.asInt32());

	// inform world is RIT is changed (display picks this up)
	_eventManager.queueEvent(static_cast<int>(EventType::RIT_CHANGED), currentRITOffsetHz);
}

void SCRadioVFO::initiateRITStatusChange(RitStatus ritStatus)
{
	_ritStatus = ritStatus;

	// this value is used by the display when it gets notified that RIT status changed
	_eventData.setEventRelatedBool((_ritStatus == RitStatus::ENABLED), EventBoolField::RIT_IS_ENABLED);

	// this tells the menu item to update so it shows the correct RIT status
	_eventManager.queueEvent(static_cast<int>(EventType::RIT_STATUS_EXTERNALLY_CHANGED), static_cast<int>(ritStatus));
}

void SCRadioVFO::changeRxOffsetDirection(int8_t whichMenuItem)
{
	ISCRadioReadOnlyMenuItem* menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);
	long menuItemValue = menuItem->getMenuItemValue();

	RxOffsetDirection rxOffsetDirection = (RxOffsetDirection)menuItemValue;

	if (((RxOffsetDirection::BELOW == rxOffsetDirection)
		&& (_rxOffset > 0))
		|| ((RxOffsetDirection::ABOVE == rxOffsetDirection)
			&& (_rxOffset < 0)))
	{
		_rxOffset *= -1;
	}

	// store off offset direction in eventData so display can pick it up
	_eventData.setEventRelatedBool((_rxOffset > 0), EventBoolField::RX_OFFSET_IS_POSITIVE);

	// recalculate RX frequency to reflect the new offset
	calculateRXFrequency();

	_dds.sendFrequencyToDDS(_currentRXFrequency.asInt32());
}

void SCRadioVFO::checkBoundsAndCorrectIfNeeded(SCRadioFrequency &newTXFrequency)
{
	FrequencyCompareResult compareResult = newTXFrequency.compare(_lowerFrequencyLimit);

	if (compareResult == FrequencyCompareResult::LESS_THAN)
	{
		newTXFrequency.replaceValue(_lowerFrequencyLimit);
	}
	
	compareResult = newTXFrequency.compare(_upperFrequencyLimit);

	if (compareResult == FrequencyCompareResult::GREATER_THAN)
	{
		newTXFrequency.replaceValue(_upperFrequencyLimit);
	}
}

int16_t SCRadioVFO::checkRITBoundariesAndCorrectIfNeeded(int16_t newRITOffsetHz)
{
	if (newRITOffsetHz > _ritUpperLimitHz) 
	{
		newRITOffsetHz = _ritUpperLimitHz;
	}

	if (newRITOffsetHz < _ritLowerLimitHz) 
	{
		newRITOffsetHz = _ritLowerLimitHz;
	}	

	return newRITOffsetHz;
}

void SCRadioVFO::initializeKeyOutPin()
{
	pinMode(_keyOutPin, OUTPUT);
	digitalWrite(_keyOutPin, LOW);
}

void SCRadioVFO::sendToDDSTxRxFrequencyAndChangeTxRxStatus(int16_t keyStatus)
{
	switch ((KeyStatus)keyStatus)
	{
	case KeyStatus::PRESSED:
		_rxTXStatus = RxTxStatus::TX;
		_dds.sendFrequencyToDDS(_currentTXFrequency.asInt32());
		digitalWrite(_keyOutPin, HIGH);
		break;
	default:
		digitalWrite(_keyOutPin, LOW);
		_rxTXStatus = RxTxStatus::RX;
		_dds.sendFrequencyToDDS(_currentRXFrequency.asInt32());
	}
}