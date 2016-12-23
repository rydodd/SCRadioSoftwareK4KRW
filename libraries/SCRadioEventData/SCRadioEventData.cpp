/*
* SCRadioEventData.cpp - Class for common data shared by event handling logic
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
#include "SCRadioEventData.h"

// public methods
void SCRadioEventData::begin()
{
	//strcpy(_messageText, "        ");
	setEventRelatedLong(0, EventLongField::RIT_OFFSET);
}

ISCRadioReadOnlyMenuItem* SCRadioEventData::getReadOnlyMenuItem(int8_t whichMenuItem)
{
	return (ISCRadioReadOnlyMenuItem*)_menuItems[whichMenuItem];
}

bool SCRadioEventData::getEventRelatedBool(EventBoolField whichField)
{
	return _boolValues[static_cast<int>(whichField)];
}

SCRadioFrequency* SCRadioEventData::getEventRelatedFrequency(EventFrequencyField whichField)
{
	return _frequencies[static_cast<int>(whichField)];
}

long SCRadioEventData::getEventRelatedLong(EventLongField whichField)
{
	return _longValues[static_cast<int>(whichField)];
}

void SCRadioEventData::setEventRelatedBool(bool valueToSet, EventBoolField whichField)
{
	_boolValues[static_cast<int>(whichField)] = valueToSet;
}

void SCRadioEventData::setEventRelatedFrequency(SCRadioFrequency *frequency, EventFrequencyField whichField)
{
	_frequencies[static_cast<int>(whichField)] = frequency;
}

void SCRadioEventData::setEventRelatedLong(long longValue, EventLongField whichField)
{
	_longValues[static_cast<int>(whichField)] = longValue;
}

void SCRadioEventData::setMenuItem(SCRadioMenuItem * menuItem, int8_t whichMenuItem)
{
	_menuItems[whichMenuItem] = menuItem;
}
