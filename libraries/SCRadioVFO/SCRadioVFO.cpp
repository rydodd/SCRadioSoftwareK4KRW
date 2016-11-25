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
#include "SCRadioVFO.h"

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
    				int16_t mediumTuningThresholdms,
    				int16_t fastTuningThresholdms) : 
						_eventManager(eventManager),
    					_eventData(eventData),
    					_dds(dds),
    					_keyOutPin(keyOutPin),
    					_ddsTuningWord(ddsTuningWord),
    					_rxOffset(rxOffset),
    					_lowerFrequencyLimit(lowerFrequencyLimit),
						_upperFrequencyLimit(upperFrequencyLimit),
    					_ritMaxOffsetHz(ritMaxOffsetHz),
    					_slowTuningIncrement(slowTuningIncrement),
    					_mediumTuningIncrement(mediumTuningIncrement),
    					_fastTuningIncrement(fastTuningIncrement),
    					_mediumTuningThresholdms(mediumTuningThresholdms),
    					_fastTuningThresholdms(fastTuningThresholdms),
    					_lastTXFrequency(lowerFrequencyLimit) 
{
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
}

// public methods

void SCRadioVFO::begin()
{
	pinMode(_keyOutPin, OUTPUT);
	digitalWrite(_keyOutPin, LOW);
	_ritStatus = RitStatus::DISABLED;
	_rxTXStatus = RxTxStatus::RX;
	_currentTuningIncrement = _slowTuningIncrement;
	_currentTXFrequency = _initialFrequency;
	_eventData.setEventRelatedLong(0, EventLongField::RIT_OFFSET);
	_eventData.setEventRelatedBool(false, EventBoolField::RIT_IS_ENABLED);
	_eventData.setEventRelatedBool(false, EventBoolField::RX_OFFSET_IS_POSITIVE);
	calculateRXFrequency();
	_ritUpperLimitHz = _ritMaxOffsetHz;
	_ritLowerLimitHz = _ritMaxOffsetHz * -1;
}

void SCRadioVFO::keyLineChangedListener(int eventCode, int keyStatus)
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

void SCRadioVFO::ritKnobTurnListener(int eventCode, int turnDirection)
{
	if (_rxTXStatus == RxTxStatus::TX) 
	{
		return;
	}

	// calculateTuningIncrement();

	changeRITOffset(turnDirection);
}

void SCRadioVFO::ritStatusChangedListener(int eventCode, int whichMenuItem)
{
	ISCRadioReadOnlyMenuItem* menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);

	long menuItemValue = menuItem->getMenuItemValue();

	_ritStatus = (RitStatus)menuItemValue;

	_eventData.setEventRelatedBool((_ritStatus == RitStatus::ENABLED), EventBoolField::RIT_IS_ENABLED);

	calculateRXFrequency();

	_dds.sendFrequencyToDDS(_currentRXFrequency.asInt32());
}

void SCRadioVFO::rxOffsetDirectionChangedListener(int eventCode, int whichMenuItem)
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

	_eventData.setEventRelatedBool((_rxOffset > 0), EventBoolField::RX_OFFSET_IS_POSITIVE);

	calculateRXFrequency();

	_dds.sendFrequencyToDDS(_currentRXFrequency.asInt32());
}

void SCRadioVFO::setInitialFrequency(int32_t initialFrequency)
{
	_initialFrequency.replaceValue((int32_t)(initialFrequency - 10));
	_currentTXFrequency.replaceValue((int32_t)(initialFrequency - 10));
	_eventData.setEventRelatedFrequency(&_currentTXFrequency, EventFrequencyField::OPERATING_FREQUENCY);
	_eventManager.queueEvent(static_cast<int>(EventType::FREQUENCY_CHANGED), static_cast<int>(EventFrequencyField::OPERATING_FREQUENCY));
}

void SCRadioVFO::vfoKnobTurnListener(int eventCode, int turnDirection)
{
	if (_rxTXStatus == RxTxStatus::TX)
	{
		return;
	}

	calculateTuningIncrement();

	changeFrequency(turnDirection);
}

// private methods

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
	static int16_t lastCalculatedIncrement = 0;
	static uint32_t mSecsAtLastKnobTurn	= 0;

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
	
	if (calculatedIncrement == lastCalculatedIncrement) 
	{
		_currentTuningIncrement = calculatedIncrement;
	}
	
	lastCalculatedIncrement = calculatedIncrement;
	
	mSecsAtLastKnobTurn = currentMillis;
}
void SCRadioVFO::changeFrequency(int8_t turnDirection)
{
	_lastTXFrequency.replaceValue(_currentTXFrequency);

	_newTXFrequency.replaceValue(_currentTXFrequency);

	if (turnDirection == 1)
	{
		_newTXFrequency.addHertz(_currentTuningIncrement);
	}
	else
	{
		_newTXFrequency.addHertz(_currentTuningIncrement * -1);
	}

	if (_newTXFrequency.equals(_lastTXFrequency))
	{
		return;
	}

	checkFrequencyBounds();

	_currentTXFrequency.replaceValue(_newTXFrequency);

	calculateRXFrequency();

	_dds.sendFrequencyToDDS(_currentRXFrequency.asInt32());

	_eventData.setEventRelatedFrequency(&_currentTXFrequency, EventFrequencyField::OPERATING_FREQUENCY);

	_eventManager.queueEvent(static_cast<int>(EventType::FREQUENCY_CHANGED), static_cast<int>(EventFrequencyField::OPERATING_FREQUENCY));
}

void SCRadioVFO::changeRITOffset(int8_t turnDirection)
{
	int32_t newRITOffsetHz;
	long currentRITOffsetHz = _eventData.getEventRelatedLong(EventLongField::RIT_OFFSET);

	newRITOffsetHz = currentRITOffsetHz;

	if (turnDirection == 1)
	{
		newRITOffsetHz += RIT_ADJUST_INCREMENT;
	}
	else
	{
		newRITOffsetHz -= RIT_ADJUST_INCREMENT;
	}

	newRITOffsetHz = checkRITBoundaries(newRITOffsetHz);

	currentRITOffsetHz = newRITOffsetHz;

	_eventData.setEventRelatedLong(currentRITOffsetHz, EventLongField::RIT_OFFSET);

	calculateRXFrequency();

	_dds.sendFrequencyToDDS(_currentRXFrequency.asInt32());

	_eventManager.queueEvent(static_cast<int>(EventType::RIT_CHANGED), currentRITOffsetHz);
}

void SCRadioVFO::checkFrequencyBounds() 
{
	FrequencyCompareResult compareResult = _newTXFrequency.compare(_lowerFrequencyLimit);

	if (compareResult == FrequencyCompareResult::LESS_THAN)
	{
		_newTXFrequency.replaceValue(_lowerFrequencyLimit);
	}
	
	compareResult = _newTXFrequency.compare(_upperFrequencyLimit);

	if (compareResult == FrequencyCompareResult::GREATER_THAN)
	{
		_newTXFrequency.replaceValue(_upperFrequencyLimit);
	}
}

int16_t SCRadioVFO::checkRITBoundaries(int16_t newRITOffsetHz) 
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

