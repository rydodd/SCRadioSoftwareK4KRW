/*
 * SCRadioEEPROM.cpp - Class for controlling input from key
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

#include "EEPROM.h"

#include "SCRadioConstants.h"
#include "SCRadioEEPROM.h"
#include "SCRadioEventData.h"
#include "SCRadioFrequency.h"

// Constructor
// The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioEEPROM::SCRadioEEPROM(SCRadioEventData &eventData,
	          				int32_t minimumWriteIntervalMillis) :
	          						_eventData(eventData), 
	          						_minimumWriteIntervalMillis(minimumWriteIntervalMillis) 
{
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
}

// initializes object
void SCRadioEEPROM::begin()
{
	_lastWriteMillis = millis();
	_itemsHaveChanged = false;
	_txFrequencyHasChanged = false;
}

void SCRadioEEPROM::frequencyChangedListener(int eventCode, int eventFrequencyIndex) 
{
	processFrequencyToPotentiallyArchive((EventFrequencyField)eventFrequencyIndex);
}

// this should be called each time the app main loop executes
void SCRadioEEPROM::loop() 
{
	if (!_itemsHaveChanged)
	{
		return;
	}
	
	uint32_t currentMillis = millis();
	if ((currentMillis - _lastWriteMillis) < _minimumWriteIntervalMillis)
	{
		return;
	}
	
	writeOperatingFrequency();
	
	_itemsHaveChanged = false;
}

uint32_t SCRadioEEPROM::readStoredOperatingFrequency() 
{
	return readEEPROMValue(static_cast<int8_t>(EEPROMValueIndex::OPERATING_FREQUENCY));
}

void SCRadioEEPROM::processFrequencyToPotentiallyArchive(EventFrequencyField eventFrequencyIndex)
{
	if (eventFrequencyIndex == EventFrequencyField::OPERATING_FREQUENCY)
	{
		SCRadioFrequency* currentFrequency = _eventData.getEventRelatedFrequency(EventFrequencyField(eventFrequencyIndex));
		_frequencyToWrite = (uint32_t)(currentFrequency->asInt32());
		if (_frequencyToWrite != _lastTXFrequencyWritten)
		{
			_itemsHaveChanged = true;
			_txFrequencyHasChanged = true;
		}
	}
}

uint32_t SCRadioEEPROM::readEEPROMValue(uint8_t indexOfValue)
{
	int offset = indexOfValue * sizeof(uint32_t);

	myUnion.array[0] = EEPROM.read(offset);
	myUnion.array[1] = EEPROM.read(offset + 1);
	myUnion.array[2] = EEPROM.read(offset + 2);
	myUnion.array[3] = EEPROM.read(offset + 3);

	return (uint32_t)myUnion.val;
}

int32_t SCRadioEEPROM::writeEEPROMValue(uint32_t valueToSet, uint8_t indexOfValue)
{
	myUnion.val = valueToSet;

	int offset = indexOfValue * sizeof(uint32_t);

	EEPROM.write(offset, myUnion.array[0]);
	EEPROM.write(offset + 1, myUnion.array[1]);
	EEPROM.write(offset + 2, myUnion.array[2]);
	EEPROM.write(offset + 3, myUnion.array[3]);
}

void SCRadioEEPROM::writeOperatingFrequency()
{
	if (!_txFrequencyHasChanged)
	{
		return;
	}

	writeEEPROMValue(_frequencyToWrite, static_cast<uint8_t>(EEPROMValueIndex::OPERATING_FREQUENCY));
	_lastTXFrequencyWritten = _frequencyToWrite;
	_lastWriteMillis = millis();
	_txFrequencyHasChanged = false;
}