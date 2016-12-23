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
 * @version 1.0.3  12/22/2016.
 */

#include "Arduino.h"

#include "EEPROM.h"
#include "ISCRadioReadOnlyMenuItem.h"
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
	_keyerModeHasChanged = false;
	_keyerSpeedHasChanged = false;
}

void SCRadioEEPROM::frequencyChangedListener(int eventCode, int eventFrequencyIndex) 
{
	processFrequencyToPotentiallyArchive((EventFrequencyField)eventFrequencyIndex);
}

void SCRadioEEPROM::keyerModeChangedListener(int eventCode, int whichMenuItem)
{
	processKeyerModeToPotentiallyArchive(whichMenuItem);
}

void SCRadioEEPROM::keyerSpeedChangedListener(int eventCode, int whichMenuItem)
{
	processKeyerSpeedToPotentiallyArchive(whichMenuItem);
}

void SCRadioEEPROM::paddlesOrientationChangedListener(int eventCode, int whichMenuItem)
{
	processPaddlesOrientationToPotentiallyArchive(whichMenuItem);
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

	if (_txFrequencyHasChanged)
	{
		writeOperatingFrequency();
	}

	if (_keyerModeHasChanged)
	{
		writeKeyerMode();
	}

	if (_keyerSpeedHasChanged)
	{
		writeKeyerSpeed();
	}

	if (_paddlesOrientationHasChanged)
	{
		writePaddlesOrientation();
	}

	_itemsHaveChanged = false;
}

uint32_t SCRadioEEPROM::readStoredOperatingFrequency()
{
	uint32_t frequency = readStoredValue(EEPROMValueIndex::OPERATING_FREQUENCY);
	_lastTXFrequencyWritten = frequency;
	_txFrequencyHasChanged = false;
	return frequency;
}

int8_t SCRadioEEPROM::readStoredKeyerMode()
{
	int8_t mode = readStoredValue(EEPROMValueIndex::KEYER_MODE);
	_lastKeyerModeWritten = mode;
	_keyerModeHasChanged = false;
	return mode;
}

int8_t SCRadioEEPROM::readStoredKeyerSpeed()
{
	int8_t speed = readStoredValue(EEPROMValueIndex::KEYER_SPEED);
	_lastKeyerSpeedWritten = speed;
	_keyerSpeedHasChanged = false;
	return speed;
}

int8_t SCRadioEEPROM::readStoredPaddlesOrientation()
{
	int8_t orientation = readStoredValue(EEPROMValueIndex::PADDLES_ORIENTATION);
	_lastPaddlesOrientationWritten = orientation;
	_paddlesOrientationHasChanged = false;
	return orientation;
}

uint32_t SCRadioEEPROM::readStoredValue(EEPROMValueIndex whichValue) 
{
	int offset = static_cast<int8_t>(whichValue) * sizeof(uint32_t);

	myUnion.array[0] = EEPROM.read(offset);
	myUnion.array[1] = EEPROM.read(offset + 1);
	myUnion.array[2] = EEPROM.read(offset + 2);
	myUnion.array[3] = EEPROM.read(offset + 3);

	return (uint32_t)myUnion.val;
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

void SCRadioEEPROM::processKeyerModeToPotentiallyArchive(int whichMenuItem)
{
	ISCRadioReadOnlyMenuItem* menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);

	_keyerModeToWrite = menuItem->getMenuItemValue();

	if (_keyerModeToWrite != _lastKeyerModeWritten)
	{
		_itemsHaveChanged = true;
		_keyerModeHasChanged = true;
	}
}

void SCRadioEEPROM::processKeyerSpeedToPotentiallyArchive(int whichMenuItem)
{
	ISCRadioReadOnlyMenuItem* menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);

	_keyerSpeedToWrite = menuItem->getMenuItemValue();

	if (_keyerSpeedToWrite != _lastKeyerSpeedWritten)
	{
		_itemsHaveChanged = true;
		_keyerSpeedHasChanged = true;
	}
}


void SCRadioEEPROM::processPaddlesOrientationToPotentiallyArchive(int whichMenuItem)
{
	ISCRadioReadOnlyMenuItem* menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);

	_paddlesOrientationToWrite = menuItem->getMenuItemValue();

	if (_paddlesOrientationToWrite != _lastPaddlesOrientationWritten)
	{
		_itemsHaveChanged = true;
		_paddlesOrientationHasChanged = true;
	}
}

void SCRadioEEPROM::writeEEPROMValue(uint32_t valueToSet, uint8_t indexOfValue)
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
	writeEEPROMValue(_frequencyToWrite, static_cast<uint8_t>(EEPROMValueIndex::OPERATING_FREQUENCY));
	_lastTXFrequencyWritten = _frequencyToWrite;
	_lastWriteMillis = millis();
	_txFrequencyHasChanged = false;
}

void SCRadioEEPROM::writeKeyerMode()
{
	writeEEPROMValue(_keyerModeToWrite, static_cast<uint8_t>(EEPROMValueIndex::KEYER_MODE));
	_lastKeyerModeWritten = _keyerModeToWrite;
	_lastWriteMillis = millis();
	_keyerModeHasChanged = false;
}

void SCRadioEEPROM::writeKeyerSpeed()
{
	writeEEPROMValue(_keyerSpeedToWrite, static_cast<uint8_t>(EEPROMValueIndex::KEYER_SPEED));
	_lastKeyerSpeedWritten = _keyerSpeedToWrite;
	_lastWriteMillis = millis();
	_keyerSpeedHasChanged = false;
}

void SCRadioEEPROM::writePaddlesOrientation()
{
	writeEEPROMValue(_paddlesOrientationToWrite, static_cast<uint8_t>(EEPROMValueIndex::PADDLES_ORIENTATION));
	_lastPaddlesOrientationWritten = _paddlesOrientationToWrite;
	_lastWriteMillis = millis();
	_paddlesOrientationHasChanged = false;
}