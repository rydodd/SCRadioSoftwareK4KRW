/*
 * SCRadioDisplay.h - Class for controlling radio's LCD display
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

// Get the LCD I2C Library here:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move all *_I2C files into a new folder named LiquidCrystal_I2C
// in the Arduino library folder
// #include <LiquidCrystal_I2C.h>

#ifndef SCRadioDisplay_h
#define SCRadioDisplay_h

// forwards for classes accessed via pointers and references only
class EventManager;
class SCRadioEventData;

// includes
#include "LiquidCrystal_I2C.h"

#include "SCRadioConstants.h"

/**
 * SCRadioDisplay class
 * 
 * This controls interaction between the application logic and the lcd display
 */
class SCRadioDisplay
{
private:

	// private members

	/**
	 * Instance of the lcd class which has the logic to interact with the LCD hardware
	 */
	LiquidCrystal_I2C lcd;

	/**
	 * Holds information needed when the application logic initiates and responds to messages
	 */
	SCRadioEventData &_eventData;  

	// the following char arrays are text needed for display operation
	// adding one to length to allow for char array's terminating null

	/**
	 * Text for first line of splash screen
	 */
	char _splashLine1[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];

	/**
	 * Text for second line of splash screen
	 */
	char _splashLine2[TEXT_FOR_DISPLAY_MAX_LENGTH + 1];

	/**
	 * Text for error message indicating key was already pressed when app started
	 */
	char _stuckKeyText[TEXT_FOR_DISPLAY_MAX_LENGTH + 1]; 

	/**
	 * How long in milliseconds to display the opening splash screen
	 */
	const int16_t _splashDelay;

	/**
	 * Holds the current mode of the main knob (VFO, RIT, Menu, MenuItem)
	 */
	MainKnobMode _mainKnobMode;

	/**
	 * Holds the last menu item accessed so it can be returned to if returning to menu
	 */
	int8_t _lastMenuItemNumber;
 
public:  
	// public methods

	/**
	* SCRadioDisplay
	*
	* @detail
	*   Creates a SCRadioDisplay object
	*   Note: You must call the begin() method before using the created object
	*
	* @param[in,out] eventData Reference to data needed by logic dealing with events
	* @param[in] lcdAddress 
	* @param[in] lcdColumns Number of columns on the LCD screen
	* @param[in] lcdRows Number of rows on the LCD screen
	* @param[in] splashDelay The number of milliseconds to display the splash screen
	*/
	SCRadioDisplay( SCRadioEventData &eventData,
    				int8_t lcdAddress,
    				int8_t lcdColumns,
    				int8_t lcdRows,
    				int16_t splashDelay);
   	
	/**
	 * begin
	 * 
	 * @detail
	 *   Initializes the object.  This must be called before using the object
	 */
	void begin();

	// logic to respond to requests to kill the backlight for the display
	
	/**
	* backlightStatusChangedListener
	*
	* @detail
	*   Handles requests for turning on or off the LCD display backlight
	*   
	* @param[in] eventCode Identifies the type of event (Corresponds to EventType enum)
	* @param[in] whichMenuItem The number of the menu item that initiated the event message
	*/void backlightStatusChangedListener(int eventCode, int whichMenuItem);

	/**
	* displaySplash
	*
	* @detail
	*   Displays the splash text
	*/
	void displaySplash();

	/**
	* errorOccurredListener
	*
	* @detail
	*   Listens for error occured messages
	*   
	* @param[in] eventCode Identifies the type of message (Corresponds to EventType enum)
	* @param[in] errorCode Number identifying type of error (Corresponds to ErrorType enum)
	*/
	void errorOccurredListener(int eventCode, int errorCode);

	/** 
	 * frequencyChangedListener
	 * 
	 * @detail
	 *   Listens for change of freqency messages
	 *   
	 *   frequencyFlags:
	 *   1 = rit enabed
	 *   2 = receive offset reversed enabled
	 *   3 = both enabled
	 * 
	 * @param[in] eventCode Identifies the type of message (Corresponds to EventType enum)
	 * @param[in] frequencyFlags Int containing bit position flags for frequency related statuses
	 * 
	 * logic to respond to frequency change events
	*/
	void frequencyChangedListener(int eventCode, int frequencyFlags);
	
	/**
	 * menuItemChangedListener
	 * 
	 * @detail
	 *   Listens for menu item value change messages
	 *   
	 * @param[in] eventCode Identifies the type of message (Corresponds to EventType enum)
	 * @param[in] whichMenuItem Specifies the numerical id of the menu item that changed
	 */
	void menuItemValueChangedListener(int eventCode, int whichMenuItem);

	/**
	 * menuItemSelectedListener
	 * 
	 * @detail
	 *   Listens for messages indicating the selected menu item changed
	 * 
	 * @param[in] eventCode Identifies the type of message (Corresponds to EventType enum)
	 * @param[in] whichMenuItem Specifies the numerical id of the menu item that was selected
	 */
	void menuItemSelectedListener(int eventCode, int whichMenuItem);

	/**
	 * mainKnobModeChangedListener
	 * 
	 * @detail
	 *   Listens for messages indicating that the main knob mode has changed
	 * 
	 * @param[in] eventCode Identifies the type of message (Corresponds to EventType enum)
	 * @param[in] newModeCode Identifies the new knob mode (Corresponds to MainKnobMode enum)
	 */
	void mainKnobModeChangedListener(int eventCode, int newModeCode);

	/**
	 * ritChangedListener
	 * 
	 * @detail
	 *   Listens for rit changed messages
	 * 
	 * @param[in] eventCode Identifies the type of message (Corresponds to EventType enum)
	 * @param[in] ritOffset Specifies the ritOffset value
	 */
	void ritChangedListener(int eventType, int ritOffset);

	// the following methods set up text used by the display

	/**
	 * setSplashText
	 * 
	 * @detail
	 *   Sets text to be displayed for the splash screen
	 * 
	 * @param[in] line1Text points to text to be copied into splash text line 1
	 * @param[in] line2Text points to text to be copied into splash text line 2
	 */
	void setSplashText(char* line1Text, char*line2Text);

	/**
	 * setStuckKeyErrorText
	 * 
	 * @detail
	 *   Sets text to be displayed for key closed on startup error
	 * 
	 * @param[in] stuckKeyText points to text to be copied into the error text string
	 */
	void setStuckKeyErrorText(char* stuckKeyText);
    
private:
	// private methods

	/**
	 * changeBacklight
	 * 
	 * @detail
	 *   Directs the turn on or off of the LCD backlight
	 * 
	 * @param[in] whichMenuItem The menu item that requested the change (holds the desired status)
	 */
	void changeBacklight(int8_t whichMenuItem);

	/**
	 * changeDisplayForNewMainKnobMode
	 * 
	 * @detail
	 *   Changes the display to correspond to the requested VFO knob mode
	 * 
	 * @param[in] newMode Mode to change to (Corresponds to MainKnobMode enum)
	 */
	void changeDisplayForNewMainKnobMode(MainKnobMode newMode);

	/**
	 * clearDisplayLine
	 * 
	 * @detail
	 *   Clears the specified line on the display
	 * 
	 * @param[in] whichLine Which line of the LCD to clear
	 */
	void clearDisplayLine(LCDDisplayLine whichLine);	
	
	/**
	 * displayErrorText
	 * 
	 * @detail
	 *   Sends the specified error text to the LCD display
	 * 
	 * @param[in] errorType Type of error to display
	 */
	void displayErrorText(ErrorType errorType);

	/**
	 * displayFrequency
	 * 
	 * @detail
	 *   Displays the current operating frequency
	 */
	void displayFrequency();

	/**
	 * displayMenuItemName
	 * 
	 * @detail
	 *   Displays the specified menu item name
	 * 
	 * @param[in] whichMenuItem Menu item number we wish to display
	 * @param[in] isEditing true if the item is currently being edited (shows asterisk)
	 */
	void displayMenuItemName(int8_t whichMenuItem, bool isEditing);

	/**
	 * displayMenuItemValue
	 * 
	 * @detail
	 *   Displays the current value of the specified menu item
	 * 
	 * @param[in] whichMenuItem menu item number of the desired menu item for display
	 */
	void displayMenuItemValue(int8_t whichMenuItem);

	/**
	 * displayRIT
	 * 
	 * @detail
	 *   Displays the current RIT offset
	 */
	void displayRIT();

	/**
	 * setTextField
	 * 
	 * @detail
	 *   Sets the value of a text array used for displaying text to the LCD display - Limits length
	 *   
	 * @param[in-out] destination Where to copy string
	 * @param[in] origin Where to copy from
	 * @param[in] max length to copy
	 */
	void setTextField(char* destination, char* origin, int maxLength);
};

#endif