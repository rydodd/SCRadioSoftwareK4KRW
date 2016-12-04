/*
 * SCRadioDisplay.cpp - Class for controlling rig's display
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

#include "SCRadioConstants.h"
#include "SCRadioEventData.h"
#include "SCRadioFrequency.h"
#include "SCRadioMenuItem.h"

#include "SCRadioDisplay.h"

// public methods

// Constructor
// The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioDisplay::SCRadioDisplay( SCRadioEventData &eventData,
								int8_t lcdAddress,
								int8_t lcdColumns,
								int8_t lcdRows,
								int16_t splashDelay) : lcd(lcdAddress, 
															lcdColumns, 
															lcdRows),
														_eventData(eventData),
														_splashDelay(splashDelay) 
{
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
}

// used to setup the display object to be ready to use
void SCRadioDisplay::begin()
{
	lcd.init();
	lcd.backlight();
	_mainKnobMode = MainKnobMode::VFO;
	_lastMenuItemNumber = 0;
}

void SCRadioDisplay::backlightStatusChangedListener(int eventCode, int whichMenuItem)
{
	changeBacklight((int8_t)whichMenuItem);
}

void SCRadioDisplay::displaySplash()
{
	clearDisplayLine(LCDDisplayLine::FIRST_LINE);
	clearDisplayLine(LCDDisplayLine::SECOND_LINE);
	lcd.setCursor(LCD_FIRST_COLUMN_NUMBER, static_cast<uint8_t>(LCDDisplayLine::FIRST_LINE));
	lcd.print(_splashLine1);
	lcd.setCursor(LCD_FIRST_COLUMN_NUMBER, static_cast<uint8_t>(LCDDisplayLine::SECOND_LINE));
	lcd.print(_splashLine2);
	delay(_splashDelay);
	clearDisplayLine(LCDDisplayLine::FIRST_LINE);
	clearDisplayLine(LCDDisplayLine::SECOND_LINE);
}

void SCRadioDisplay::errorOccurredListener(int eventCode, int errorCode)
{
	ErrorType whichErrorType = (ErrorType)errorCode;
	displayErrorText(whichErrorType);
}

void SCRadioDisplay::frequencyChangedListener(int eventCode, int frequencyFlags)
{
	displayFrequency();
}

void SCRadioDisplay::mainKnobModeChangedListener(int eventCode, int newMode)
{
	changeDisplayForNewMainKnobMode((MainKnobMode)newMode);
}

void SCRadioDisplay::menuItemSelectedListener(int eventCode, int whichMenuItem)
{
//	Serial.print("Which Menu Item = ");
//	Serial.println(whichMenuItem);
	_lastMenuItemNumber = whichMenuItem;
	displayMenuItemName(whichMenuItem, false);
	displayMenuItemValue(whichMenuItem);
}

void SCRadioDisplay::menuItemValueChangedListener(int eventCode, int whichMenuItem)
{
	// Serial.println("In menu item value changed listener.");
	displayMenuItemValue(whichMenuItem);
}

void SCRadioDisplay::ritChangedListener(int eventCode, int ritOffset)
{
	if (_mainKnobMode == MainKnobMode::RIT)
	{
		displayRIT();
	}
}

// used to setup the splash text (first line)
void SCRadioDisplay::setSplashText(const char* line1Text, const char* line2Text)
{
	setTextField(_splashLine1, line1Text, TEXT_FOR_DISPLAY_MAX_LENGTH);
	setTextField(_splashLine2, line2Text, TEXT_FOR_DISPLAY_MAX_LENGTH);
}

// used to setup the error text for a stuck cw key
void SCRadioDisplay::setStuckKeyErrorText(const char* stuckKeyText)
{
	setTextField(_stuckKeyText, stuckKeyText, TEXT_FOR_DISPLAY_MAX_LENGTH);
}

// private object methods

void SCRadioDisplay::changeBacklight(int8_t whichMenuItem)
{
	ISCRadioReadOnlyMenuItem* menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);

	BacklightStatus backlightStatus = (BacklightStatus)menuItem->getMenuItemValue();

	if (backlightStatus == BacklightStatus::ENABLED)
	{
		lcd.backlight();
	}
	else
	{
		lcd.noBacklight();
	}
}

void SCRadioDisplay::changeDisplayForNewMainKnobMode(MainKnobMode newMode)
{
	_mainKnobMode = (MainKnobMode)newMode;

	switch (_mainKnobMode)
	{
	case MainKnobMode::VFO:
		clearDisplayLine(LCDDisplayLine::SECOND_LINE);
		displayFrequency();
		break;
	case MainKnobMode::RIT:
		displayRIT();
		break;
	case MainKnobMode::MAIN_MENU:
		// debug
		//			Serial.println("In Display - Displaying name and value");
		displayMenuItemName(_lastMenuItemNumber, false);
		//			Serial.println("Name displayed");
		displayMenuItemValue(_lastMenuItemNumber);
		//			Serial.println("Value displayed");
		break;
	case MainKnobMode::MENU_ITEM:
		displayMenuItemName(_lastMenuItemNumber, true);
		break;
	default:
		return;
	}
}

void SCRadioDisplay::displayRIT()
{
	char ritOffsetToDisplay[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];
	long ritOffset = _eventData.getEventRelatedLong(EventLongField::RIT_OFFSET);
	sprintf(ritOffsetToDisplay, "  RIT %5ld Hz  ", ritOffset);
	lcd.setCursor(LCD_FIRST_COLUMN_NUMBER, static_cast<uint8_t>(LCDDisplayLine::SECOND_LINE));
	lcd.print(ritOffsetToDisplay);
}

void SCRadioDisplay::displayMenuItemValue(int8_t whichMenuItem)
{
	// Serial.println("In displayMenuItemValue.");

	char textToDisplay[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];
	ISCRadioReadOnlyMenuItem * menuItem = _eventData.getReadOnlyMenuItem(whichMenuItem);
	menuItem->getMenuItemDisplayValueCopy(textToDisplay);
	clearDisplayLine(LCDDisplayLine::SECOND_LINE);
	lcd.setCursor(LCD_FIRST_COLUMN_NUMBER, static_cast<uint8_t>(LCDDisplayLine::SECOND_LINE));
	lcd.print(textToDisplay);
}

void SCRadioDisplay::displayFrequency()
{
	bool ritFlag = _eventData.getEventRelatedBool(EventBoolField::RIT_IS_ENABLED);
	bool offsetPositiveFlag = _eventData.getEventRelatedBool(EventBoolField::RX_OFFSET_IS_POSITIVE);

	char ritIndicator = ' ';
	char offsetDirectionIndicator = 'n';

	if (ritFlag)
	{
		ritIndicator = 'r';
	}

	if (offsetPositiveFlag)
	{
		offsetDirectionIndicator = 'p';
	}

	char frequencyToDisplay[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];
	SCRadioFrequency *currentFrequency = _eventData.getEventRelatedFrequency(EventFrequencyField::OPERATING_FREQUENCY);

	sprintf(frequencyToDisplay,
	"%d.%03d.%03d MHz %c%c",
	currentFrequency->megaHertz(),
	currentFrequency->kiloHertz(),
	currentFrequency->hertz(), 
	ritIndicator,
	offsetDirectionIndicator);

	lcd.setCursor(LCD_FIRST_COLUMN_NUMBER, static_cast<uint8_t>(LCDDisplayLine::FIRST_LINE));
	lcd.print(frequencyToDisplay);
}

void SCRadioDisplay::displayMenuItemName(int8_t whichMenuItem, bool isEditing)
{
	char menuItemToDisplay[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];
	char menuItemName[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];
//	Serial.print("Display menu item ");
//	Serial.println(whichMenuItem);
	ISCRadioReadOnlyMenuItem* menuItem = (ISCRadioReadOnlyMenuItem*)_eventData.getReadOnlyMenuItem(whichMenuItem);
	menuItem->getMenuItemDisplayNameCopy(menuItemName);
//	Serial.print("Menu Item Name");
//	Serial.println(menuItemName);
	
	if (isEditing)
	{
		sprintf(menuItemToDisplay, "Menu: %s *", menuItemName);
	}
	else
	{
		sprintf(menuItemToDisplay, "Menu: %s", menuItemName);
	}
	
	clearDisplayLine(LCDDisplayLine::FIRST_LINE);
	lcd.setCursor(LCD_FIRST_COLUMN_NUMBER, static_cast<uint8_t>(LCDDisplayLine::FIRST_LINE));
	lcd.print(menuItemToDisplay);
}

void SCRadioDisplay::displayErrorText(ErrorType errorType)
{
	clearDisplayLine(LCDDisplayLine::FIRST_LINE);
	lcd.setCursor(LCD_FIRST_COLUMN_NUMBER, static_cast<uint8_t>(LCDDisplayLine::FIRST_LINE));
	if (errorType == ErrorType::STUCK_KEY)
	{
		lcd.print(_stuckKeyText);
	}

	delay(1000);
}

void SCRadioDisplay::clearDisplayLine(LCDDisplayLine whichRow)
{
	lcd.setCursor(LCD_FIRST_COLUMN_NUMBER, static_cast<uint8_t>(whichRow));
	lcd.print(BLANK_LCD_LINE);
}

void SCRadioDisplay::setTextField(char* destination, const char* origin, int maxLength)
{
	int textLen = strlen(origin);

	if (textLen >= maxLength)
	{
		textLen = maxLength;
	}

	strncpy(destination, origin, textLen);

	destination[textLen] = 0;
}