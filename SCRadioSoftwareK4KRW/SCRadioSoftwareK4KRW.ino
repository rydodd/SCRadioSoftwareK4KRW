/**
 * K4KRWVFOSource.ino - Application to to control Software Controlled Radio via an Arduino Nano and a AD9850 DDS
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

 /**
 * Recognition
 *
 * The heart of the code that manages the AD9850 was originally written by
 * Richard Visokey AD7C - www.ad7c.com
 *
 * I have basically rewritten all of this.  I went back to the documentation for the DDS to understand
 * what was required to interact with it.  However, it was still made much easier by Richard's
 * example
 *
 * Also, Jack Purdum and Dennis Kidder wrote the original software for this rig.
 * Their application logic gave me a significant head start when writing this code.
 * Although my code may look very different, it is implementing much of the same logic.
 * Their work is greatly appreciated
 *
 * Also, a thank you to Igor Mikolic-Torreira for his EventManager library
 * It is what allowed me to write this using events and messages
 */

/* 
 * Regrets and compromises
 * 
 * The Arduino world is a bit limiting.  Not having sophisticated memory management like that on a PC forces you to 
 * allocate objects and variables once and reuse them.  Otherwise you run into memory fragmentation issues.
 * 
 * Because of this I have not taken advantage of the 'String' class.  It may be possible to use it with careful
 * programming anyway.  But, for now, I'm using traditional C strings which are null terminated character
 * arrays.  This means that this app uses pointers.  I was hoping to avoid that as much as possible, but ...
 * 
 * I would love it if the EventManager was enhanced to allow me to allocate handlers that were object instance methods.
 * I may contact the author to see if he has any interest. This would allow me to create instances of each object at 
 * whatever scope I wanted rather than making all of the objects globals.
 * 
 * Ideally I'd love it if you could pass a read only reference to the object sending the message to the listener.  
 * This is easy to do in C# as all classes are derived from 'Object'.  So that reference is passed as an 'Object' 
 * and then you can typecast it to what it actually is in your event handler.  I could create my own base 
 * class to allow this behavior.  But, what I have works for now.
 */

/*
 * IMPORTANT
 * 
 * Check all defines for settings in SCRadioConstants.h before using this application
 * Defines may need adjustment depending on your specific hardware and your preferences.
 * 
 * See notes below about EventManager.h defines
 */

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <EventManager.h>
#include <SCRadioConstants.h>
#include <SCRadioButton.h>
#include <SCRadioMainKnob.h>
#include <SCRadioDisplay.h>
#include <SCRadioEventData.h>
#include <SCRadioVFO.h>
#include <SCRadioDDS.h>
#include <SCRadioKeyer.h>
#include <SCRadioEEPROM.h>
#include <SCRadioMenu.h>
#include <SCRadioMenuItem.h>
#include <SCRadioMenuItemNameValue.h>
#include <SCRadioVoltageMonitor.h>

// Forwards definitions for functions in main .ino file.  This allows the actual 
// function definitions to fall below the main application logic (setup and loop) 
// but the app still compile

void retrieveInitialFrequency();
void setupInitialKeyerMode();
void setupInitialKeyerSpeed();
void setupInitialPaddlesOrientation();

int32_t checkInitialFrequency(int32_t initialFrequency);

// optional menu items forwards
void setupRITOnOffMenuItem();
void setupBacklightOnOffMenuItem();


void setupRxOffsetDirectionMenuItem();
void setupKeyerModeMenuItem();
void setupKeyerSpeedMenuItem();
void setupPaddlesOrientationMenuItem();

void vfoKnobTurnedListener(int eventCode, int turnDirection);
void ritKnobTurnedListener(int eventCode, int turnDirection);
void displayFrequencyChangedListener(int eventCode, int eventLongIndex);
void displayRitChangedListener(int eventCode, int eventLongIndex);
void eepromFrequencyChangedListener(int eventCode, int eventLongIndex);
void vfoKeyLineChangedListener(int eventCode, int keyStatus);
void displayMainKnobModeChangedListener(int eventCode, int newMode);
void displayMenuItemSelectedListener(int eventCode, int menuItemNumber);
void displayMenuItemValueChangedListener(int eventCode, int whichMenuItem);
void menuKnobTurnedListener(int eventCode, int turnDirection);
void menuItemKnobTurnedListener(int eventCode, int turnDirection);
void vfoRxOffsetDirectionChangedListener(int eventCode, int whichMenuItem);
void displayErrorOccurredListener(int eventCode, int whichErrorType);
void displayVoltageReadListener(int eventCode, int voltageX10);
void keyerModeChangedListener(int eventCode, int whichMenuItem);
void keyerSpeedChangedListener(int eventCode, int whichMenuItem);
void keyerPaddlesOrientationChangedListener(int eventCode, int whichMenuItem);
void eepromKeyerModeChangedListener(int eventCode, int whichMenuItem);
void eepromKeyerSpeedChangedListener(int eventCode, int whichMenuItem);
void eepromPaddlesOrientationChangedListener(int eventCode, int whichMenuItem);

// Forwards for listeners for optional settings
void vfoRitStatusChangedListener(int eventCode, int whichMenuItem);
void displayBacklightStatusChangedListener(int eventCode, int whichMenuItem);
void ritMenuItemExternallyChangedListener(int eventCode, int newValue);

// This is the library that implements the event queue
EventManager eventManager = EventManager();

// This is data that various components may need when firing or responding to events
SCRadioEventData eventData = SCRadioEventData();

// Main knob's button
SCRadioButton mainKnobButton = SCRadioButton(MAIN_KNOB_SWITCH_PIN, 
											DEBOUNCE_THRESHOLD_MS, 
											LONG_PRESS_THRESHOLD_MS);

// Main knob
//
// Note: If your main knob acts like it is tuning backwards (right does down, left goes up)
// reverse the pins in the SCRadioConstants.h file for the main knob and it will 
// reverse the behavior.  I had this problem with mine.
SCRadioMainKnob mainKnob = SCRadioMainKnob(eventManager, 
											MAIN_KNOB_PIN_1, 
											MAIN_KNOB_PIN_2, 
											mainKnobButton);

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(LCD_ADDRESS,
											LCD_COLUMNS,
											LCD_ROWS);

// Controls getting information to the LCD display
SCRadioDisplay lcdControl = SCRadioDisplay(eventData, 
											lcd,
											SPLASH_DELAY);

// Controls writing to and reading from EEPROM's persistent memory
SCRadioEEPROM eeprom = SCRadioEEPROM(eventData, 
									MIN_EPROM_WRITE_INTERVAL);

// Interaction logic for sending commands to the DDS
SCRadioDDS dds = SCRadioDDS(DDS_WORD_LOAD_CLOCK_PIN,
                            DDS_FREQUENCY_UPDATE_PIN,
                            DDS_DATA_PIN,
                            DDS_RESET_PIN,
                            DDS_TUNING_WORD);

// Handles input from the CW key
SCRadioKeyer keyer = SCRadioKeyer(eventManager, eventData);

// periodically checks the rig's voltage
SCRadioVoltageMonitor voltageMonitor = SCRadioVoltageMonitor(eventManager,
										RIG_VOLTAGE_READ_PIN,
										RIG_VOLTAGE_CALC_MULTIPLIER,
										EventType::RIG_VOLTAGE_CHANGED,
										LOOP_COUNT_BETWEEN_RIG_VOLTAGE_READS);

// This controls all having to do with frequency
// Changing frequency.  Calculating TX and RX frequency, RIT ...
SCRadioVFO vfo = SCRadioVFO(eventManager,
            eventData,
            dds,
            KEY_OUT_PIN,
            DDS_TUNING_WORD,
            RX_OFFSET,
            VFO_LIMIT_LOW,
            VFO_LIMIT_HIGH,
            RIT_MAX_OFFSET_HZ,
            TUNING_INCREMENT_SLOW,
            TUNING_INCREMENT_MEDIUM,
            TUNING_INCREMENT_FAST,
            TUNING_THRESHOLD_MEDIUM,
            TUNING_THRESHOLD_FAST);

// This controls the meny system
SCRadioMenu menu = SCRadioMenu(eventManager, eventData);

// The following are menu items used by the menu
SCRadioMenuItem keyerSpeedMenuItem = SCRadioMenuItem(eventManager, 12, 1, 0, 30);

SCRadioMenuItemNameValue keyerModeMenuItem = SCRadioMenuItemNameValue(eventManager, 0, 0, 2);

SCRadioMenuItemNameValue rxOffsetDirectionMenuItem = SCRadioMenuItemNameValue(eventManager, 0, 0, 1);

SCRadioMenuItemNameValue paddlesOrientationMenuItem = SCRadioMenuItemNameValue(eventManager, 0, 0, 1);

// Optional menu items.  IF you want to free up some data memory.  Removing these would 
// have little cost in functionality
SCRadioMenuItemNameValue ritOnOffMenuItem = SCRadioMenuItemNameValue(eventManager, 0, 0, 1);

SCRadioMenuItemNameValue backlightOnOffMenuItem = SCRadioMenuItemNameValue(eventManager, 1, 0, 1);

/**
 * setup
 * 
 * @detail
 *   This gets run at application starup.  Since there is no real 'Constructor' logic in Arduino world,
 *   all of this can be viewed as implementing logic that would normally be in a constructor.
 *   Basically, it is setting up each object so it is ready to do its job.
 */
void setup() 
{
	// Setting the baud rate for talking to the DDS
	Serial.begin(9600);

	mainKnob.begin();
	lcd.init();
	lcdControl.begin();
	lcdControl.setSplashText(SPLASH_LINE_1, SPLASH_LINE_2);
	lcdControl.setStuckKeyErrorText(STUCK_KEY_TEXT);
	keyer.begin();
	dds.begin();
	vfo.begin();

	// Setting up menu object
	menu.begin();

	// for each menu item, calling begin() to get it set up.
	// Then calling additional logic to do things like pass in
	// string information used by the menu item.
	// Then adding the menu item to the menu
	rxOffsetDirectionMenuItem.begin();
	setupRxOffsetDirectionMenuItem();
	keyerModeMenuItem.begin();
	setupKeyerModeMenuItem();
	keyerSpeedMenuItem.begin();
	setupKeyerSpeedMenuItem();
	paddlesOrientationMenuItem.begin();
	setupPaddlesOrientationMenuItem();

	// Option menu items setup
	ritOnOffMenuItem.begin();
	setupRITOnOffMenuItem();
	backlightOnOffMenuItem.begin();
	setupBacklightOnOffMenuItem();

	// Whatever order you place these addMenuItem statements is the order they will display
	// in the menu
	menu.addMenuItem(keyerSpeedMenuItem);
	menu.addMenuItem(keyerModeMenuItem);
	menu.addMenuItem(paddlesOrientationMenuItem);
	menu.addMenuItem(rxOffsetDirectionMenuItem);
	menu.addMenuItem(ritOnOffMenuItem);

	menu.addMenuItem(backlightOnOffMenuItem);

	// add optional menu items
	eeprom.begin();
	retrieveInitialFrequency();
	setupInitialKeyerMode();
	setupInitialKeyerSpeed();
	setupInitialPaddlesOrientation();

	// Here we are telling the eventManager all of the methods that will be listening for event messages
	//
	// IMPORTANT!!
	// There is a constant in EventManager.h that specifies how many listeners there can be.
	// EVENTMANAGER_LISTENER_LIST_SIZE - (determines size of an array)
	// You must ensure that you adjust that value to allow for all of the messages listeners
	// you will register.
	//
	// As of release date of this software, the setting needed to be at least 15.  However
	//  these do take up memory.  So don't specify more than you are going to use.
	//
	eventManager.addListener(static_cast<int>(EventType::VFO_KNOB_TURNED), &vfoKnobTurnedListener);  
	eventManager.addListener(static_cast<int>(EventType::RIT_KNOB_TURNED), &ritKnobTurnedListener);
	eventManager.addListener(static_cast<int>(EventType::RIT_CHANGED), &displayRitChangedListener);
	eventManager.addListener(static_cast<int>(EventType::FREQUENCY_CHANGED), &displayFrequencyChangedListener);  
	eventManager.addListener(static_cast<int>(EventType::FREQUENCY_CHANGED), &eepromFrequencyChangedListener);
	eventManager.addListener(static_cast<int>(EventType::KEY_LINE_CHANGED), &vfoKeyLineChangedListener);
	eventManager.addListener(static_cast<int>(EventType::MAIN_KNOB_MODE_CHANGED), &displayMainKnobModeChangedListener);
	eventManager.addListener(static_cast<int>(EventType::MENU_KNOB_TURNED), &menuKnobTurnedListener);
	eventManager.addListener(static_cast<int>(EventType::MENU_ITEM_KNOB_TURNED), &menuItemKnobTurnedListener);
	eventManager.addListener(static_cast<int>(EventType::MENU_ITEM_SELECTED), &displayMenuItemSelectedListener);
	eventManager.addListener(static_cast<int>(EventType::MENU_ITEM_VALUE_CHANGED), &displayMenuItemValueChangedListener);
	eventManager.addListener(static_cast<int>(EventType::RX_OFFSET_DIRECTION_MENU_ITEM_VALUE_CHANGED), &vfoRxOffsetDirectionChangedListener);
	eventManager.addListener(static_cast<int>(EventType::ERROR_OCCURRED), &displayErrorOccurredListener);
	eventManager.addListener(static_cast<int>(EventType::RIG_VOLTAGE_CHANGED), &displayVoltageReadListener);
	eventManager.addListener(static_cast<int>(EventType::KEYER_MODE_CHANGED), &keyerModeChangedListener);
	eventManager.addListener(static_cast<int>(EventType::KEYER_SPEED_CHANGED), &keyerSpeedChangedListener);
	eventManager.addListener(static_cast<int>(EventType::PADDLES_ORIENTATION_CHANGED), &keyerPaddlesOrientationChangedListener);
	eventManager.addListener(static_cast<int>(EventType::KEYER_MODE_CHANGED), &eepromKeyerModeChangedListener);
	eventManager.addListener(static_cast<int>(EventType::KEYER_SPEED_CHANGED), &eepromKeyerSpeedChangedListener);
	eventManager.addListener(static_cast<int>(EventType::PADDLES_ORIENTATION_CHANGED), &eepromPaddlesOrientationChangedListener);

	// add listeners for optional menu items
	eventManager.addListener(static_cast<int>(EventType::RIT_MENU_ITEM_VALUE_CHANGED), &vfoRitStatusChangedListener);
	eventManager.addListener(static_cast<int>(EventType::BACKLIGHT_MENU_ITEM_VALUE_CHANGED), &displayBacklightStatusChangedListener);
	eventManager.addListener(static_cast<int>(EventType::RIT_STATUS_EXTERNALLY_CHANGED), &ritMenuItemExternallyChangedListener);

	// The last thing we do before starting up is displaying the splash.
	lcdControl.displaySplash();

	// This message kicks off things for the VFO and ends up forcing the frequency to be displayed on the display
	eventManager.queueEvent(static_cast<int>(EventType::VFO_KNOB_TURNED), static_cast<int>(KnobTurnDirection::CLOCKWISE), EventManager::kHighPriority);
}


/**
 * loop
 * 
 * @detail
 *   Here is the main loop of the program
 *   Not much here is there?
 *   Any new hardware you add that will respond to user input or some outside action will need to
 *   have a loop function that is called here.
 *   All other actions and object interactions will be handled by the eventManager
 */
void loop() 
{
	// Handles checking status of the CW keyer.
	keyer.loop();

	// Handles checking status of the main knob (knob and button)
	mainKnob.loop();

	// Handles checking to see if items need to be persisted to the EEPROM memory.
	eeprom.loop();

	// periodically checks rig voltage
	voltageMonitor.loop();

	// The eventManager goes through the events message queue and calls handlers who have registered for each event it finds
	// in the queue.
	//
	// I have been calling processAllEvents().  This makes it process everything it finds during each loop
	// You can have it only process one message each time it is called.
	// I have not had time to test to see how far the event queue backs up.
	// Right now if more than 8 events queued up, it would blow through the event message array and start
	// corrupting application memory.  (Author may handle this.  I just haven't look at his code in that kind of detail)
	//
	// There are methods in eventManager to get the status of it.  I will incorporate checks at some point and build
	// it into my logic so the app displays an error if something goes past the limits.

	eventManager.processAllEvents();
//	eventManager.processEvent();
}

/**
 * retrieveInitialFrequency
 * 
 * @detail
 *   Get your last persisted operating frequency from the EEPROM memory
 */
void retrieveInitialFrequency() 
{
	uint32_t initialFrequency = eeprom.readStoredOperatingFrequency();

	initialFrequency = checkInitialFrequency(initialFrequency);
  
	vfo.setInitialFrequency(initialFrequency);
}

/**
 * setupInitialKeyerMode
 * 
 * @detail
 *   Gets last keyer mode used from EEPROM memory and sets up app
 *   to use that same mode
 */
void setupInitialKeyerMode()
{
	int8_t initialKeyerMode = eeprom.readStoredKeyerMode();
	
	initialKeyerMode = keyerModeMenuItem.rangeCheckValue(initialKeyerMode);

	keyer.setKeyerMode(KeyerMode(initialKeyerMode));

	keyerModeMenuItem.setMenuItemValue(initialKeyerMode);
}

/**
 * setupInitialKeyerSpeed
 * 
 * @detail
 *   Gets last keyer speed used from EEPROM memory and sets up app
 *   to use that keyer speed
 */
void setupInitialKeyerSpeed()
{
	int8_t initialKeyerWPM = eeprom.readStoredKeyerSpeed();

	initialKeyerWPM = keyerSpeedMenuItem.rangeCheckValue(initialKeyerWPM);

	keyer.setKeyerWPM(initialKeyerWPM);

	keyerSpeedMenuItem.setMenuItemValue(initialKeyerWPM);
}

/**
 * setupInitialPaddlesOrientation
 * 
 * @detail
 *   Gets last paddles orientation configuration from EEPROM memory
 *   and sets up app to use that orientation
 */
void setupInitialPaddlesOrientation()
{
	int8_t initialPaddlesOrientation = eeprom.readStoredPaddlesOrientation();

	initialPaddlesOrientation = paddlesOrientationMenuItem.rangeCheckValue(initialPaddlesOrientation);

	keyer.setPaddlesOrientation(PaddlesOrientation(initialPaddlesOrientation));

	paddlesOrientationMenuItem.setMenuItemValue(initialPaddlesOrientation);
}

/**
 * checkInitialFrequency
 * 
 * @detail
 *   Make sure the memory retrieved is within bounds.  If not, set to default frequency
 */
int32_t checkInitialFrequency(int32_t initialFrequency) 
{
	if ((initialFrequency < VFO_LIMIT_LOW) || (initialFrequency > VFO_LIMIT_HIGH)) {
		initialFrequency = INITIAL_FREQUENCY;
	};
  
	return initialFrequency;
}

/**
* setupRxOffsetDirectionMenuItem
*
* @detail
* Initializes the rx offset direction menu item
*/
void setupRxOffsetDirectionMenuItem()
{
	rxOffsetDirectionMenuItem.setMenuItemEventType(EventType::RX_OFFSET_DIRECTION_MENU_ITEM_VALUE_CHANGED);
	rxOffsetDirectionMenuItem.setMenuItemName("Ofst Dir");
	rxOffsetDirectionMenuItem.setMenuItemValueFormat("%s");
	rxOffsetDirectionMenuItem.setMenuItemDisplayValue(1, "Positive");
	rxOffsetDirectionMenuItem.setMenuItemDisplayValue(0, "Negative");
}

/**
 * setupKeyerModeMenuItem
 * 
 * @detail
 *   Sets up keyer mode menu item
 */
void setupKeyerModeMenuItem()
{
	keyerModeMenuItem.setMenuItemEventType(EventType::KEYER_MODE_CHANGED);
	keyerModeMenuItem.setMenuItemName("Kyr Mode");
	keyerModeMenuItem.setMenuItemValueFormat("%s");
	keyerModeMenuItem.setMenuItemDisplayValue(0, "Straight");
	keyerModeMenuItem.setMenuItemDisplayValue(1, "Iambic B");
	keyerModeMenuItem.setMenuItemDisplayValue(2, "Iambic A");
}

/**
 * setupKeyerSpeedMenuItem
 * 
 * @detail
 *   Sets up keyer speed menu item
 */
void setupKeyerSpeedMenuItem()
{
	keyerSpeedMenuItem.setMenuItemEventType(EventType::KEYER_SPEED_CHANGED);
	keyerSpeedMenuItem.setMenuItemName("Kyr WPM");
	keyerSpeedMenuItem.setMenuItemValueFormat("%ld WPM");
}

/**
 * setupPaddlesOrientationMenuItem
 * 
 * @detail
 *   Sets up paddles orientation menu item
 */
void setupPaddlesOrientationMenuItem()
{
	paddlesOrientationMenuItem.setMenuItemEventType(EventType::PADDLES_ORIENTATION_CHANGED);
	paddlesOrientationMenuItem.setMenuItemName("Paddles");
	paddlesOrientationMenuItem.setMenuItemValueFormat("%s");
	paddlesOrientationMenuItem.setMenuItemDisplayValue(0, "Standard");
	paddlesOrientationMenuItem.setMenuItemDisplayValue(1, "Reversed");
}


// setting up optional menu items

/**
* setupRITOnOffMenuItem
*
* @detail
* Initializes the RIT on off menu item
*/
void setupRITOnOffMenuItem()
{
	ritOnOffMenuItem.setMenuItemEventType(EventType::RIT_MENU_ITEM_VALUE_CHANGED);
	ritOnOffMenuItem.setMenuItemName("RIT");
	ritOnOffMenuItem.setMenuItemValueFormat("%s");
	ritOnOffMenuItem.setMenuItemDisplayValue(1, "On");
	ritOnOffMenuItem.setMenuItemDisplayValue(0, "Off");
}

/**
* setupBacklightOnOffMenuItem
*
* @detail
* Initializes the backlight on off menu item
*/
void setupBacklightOnOffMenuItem()
{
	backlightOnOffMenuItem.setMenuItemEventType(EventType::BACKLIGHT_MENU_ITEM_VALUE_CHANGED);
	backlightOnOffMenuItem.setMenuItemName("Bklight");
	backlightOnOffMenuItem.setMenuItemValueFormat("%s");
	backlightOnOffMenuItem.setMenuItemDisplayValue(1, "Yes");
	backlightOnOffMenuItem.setMenuItemDisplayValue(0, "No");
}


// Here I am creating global functions that in turn call methods on each class 
// to handle the various messages they need to respond to.
// First part of name identifies which object this refers to.
// Later part of name identifies what types of messages it responds to
//
// The event manager cannot register class methods directly
// This is why this is here
//
void vfoKnobTurnedListener(int eventCode, int turnDirection)
{
	vfo.vfoKnobTurnedListener(eventCode, turnDirection);
}

void ritKnobTurnedListener(int eventCode, int turnDirection)
{
	vfo.ritKnobTurnedListener(eventCode, turnDirection);
}

void displayFrequencyChangedListener(int eventCode, int eventLongIndex)
{
	lcdControl.frequencyChangedListener(eventCode, eventLongIndex);
}

void displayRitChangedListener(int eventCode, int eventLongIndex)
{
	lcdControl.ritChangedListener(eventCode, eventLongIndex);
}

void eepromFrequencyChangedListener(int eventCode, int eventLongIndex)
{
	eeprom.frequencyChangedListener(eventCode, eventLongIndex);
}

void vfoKeyLineChangedListener(int eventCode, int keyStatus)
{
	vfo.keyLineChangedListener(eventCode, keyStatus);
}

void displayMainKnobModeChangedListener(int eventCode, int newMode)
{
	lcdControl.mainKnobModeChangedListener(eventCode, newMode);
}

void displayMenuItemSelectedListener(int eventCode, int menuItemNumber)
{
	lcdControl.menuItemSelectedListener(eventCode, menuItemNumber);
}

void displayMenuItemValueChangedListener(int eventCode, int whichMenuItem)
{
	lcdControl.menuItemValueChangedListener(eventCode, whichMenuItem);
}

void menuKnobTurnedListener(int eventCode, int turnDirection)
{
	menu.menuKnobTurnedListener(eventCode, turnDirection);
}

void menuItemKnobTurnedListener(int eventCode, int turnDirection)
{
	menu.menuItemKnobTurnedListener(eventCode, turnDirection);
}

void vfoRxOffsetDirectionChangedListener(int eventCode, int whichMenuItem)
{
	vfo.rxOffsetDirectionChangedListener(eventCode, whichMenuItem);
}

void displayErrorOccurredListener(int eventCode, int whichErrorType)
{
	lcdControl.errorOccurredListener(eventCode, whichErrorType);
}

void displayVoltageReadListener(int eventCode, int voltageX10)
{
	lcdControl.voltageReadListener(eventCode, voltageX10);
}

void keyerModeChangedListener(int eventCode, int whichMenuItem)
{
	keyer.keyerModeChangedListener(eventCode, whichMenuItem);
}

void keyerSpeedChangedListener(int eventCode, int whichMenuItem)
{
	keyer.keyerSpeedChangedListener(eventCode, whichMenuItem);
}

void eepromKeyerModeChangedListener(int eventCode, int whichMenuItem)
{
	eeprom.keyerModeChangedListener(eventCode, whichMenuItem);
}

void eepromKeyerSpeedChangedListener(int eventCode, int whichMenuItem)
{
	eeprom.keyerSpeedChangedListener(eventCode, whichMenuItem);
}

void keyerPaddlesOrientationChangedListener(int eventCode, int whichMenuItem)
{
	keyer.keyerPaddlesOrientationChangedListener(eventCode, whichMenuItem);
}

void eepromPaddlesOrientationChangedListener(int eventCode, int whichMenuItem)
{
	eeprom.paddlesOrientationChangedListener(eventCode, whichMenuItem);
}

// optional menu item listeners
void vfoRitStatusChangedListener(int eventCode, int whichMenuItem)
{
	vfo.ritStatusChangedListener(eventCode, whichMenuItem);
}

void displayBacklightStatusChangedListener(int eventCode, int whichMenuItem)
{
	lcdControl.backlightStatusChangedListener(eventCode, whichMenuItem);
}

void ritMenuItemExternallyChangedListener(int eventCode, int newMenuItemValue)
{
	ritOnOffMenuItem.menuItemExternallyChangedListener(eventCode, newMenuItemValue);
}