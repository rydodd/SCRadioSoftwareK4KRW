/**
 * SCRadioMainKnob.cpp - Library for constants and enums used by SCRadio software project by K4KRW
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

#ifndef SCRadioConstants_h
#define SCRadioConstants_h

// LCD Display Settings

/**
 * Address used to access the LCD display
 */
#define LCD_ADDRESS               0x27  

/**
 * Pin that controls backlight - I don't think I actually use this
 */
#define LCD_BACKLIGHT_PIN         3

/**
 * Number of columns for the display
 */
#define LCD_COLUMNS               16     

/**
 * Number of rows for the display
 */
#define LCD_ROWS                  2

/**
 * Text used to clear a line on the display
 */
#define BLANK_LCD_LINE "                "

/**
 * First column is zero
 */
#define LCD_FIRST_COLUMN_NUMBER 0

// Main Knob Rotary Encoder Settings
//
// Note:  You may have to reverse these two PINs value assignments if your main knob operates backwards
//

/**
 * Arduino pin listining for the rotary encoder
 */
#define MAIN_KNOB_PIN_1                3     

/**
 * Arduino pin listening for the rotary encoder
 */
#define MAIN_KNOB_PIN_2                2


// Used by switch for rotary encoder

/**
 * Arduino pin listening for switch
 */
#define MAIN_KNOB_SWITCH_PIN      4    

/**
 * Press duration below which we consider a press a bounce
 */
#define DEBOUNCE_THRESHOLD_MS     50

/**
 * Threshold between a short press and a long press
 */
#define LONG_PRESS_THRESHOLD_MS   300

// VFO band settings

/**
 * Lower limit of the band
 */
#define VFO_LIMIT_LOW             7000000L    

/**
 * Upper limit of the band
 */
#define VFO_LIMIT_HIGH            7300000L

/**
 * Initial starting frequency if none is stored in the EEPROM
 * or if EEPROM value is invalid
 */
#define INITIAL_FREQUENCY         7030000L

/** 
 * Receive offset.
 * if you want the received signal to have a different pitch when you are zero beat, change this setting.
 *
 * This is a direct conversion receiver.  So, it will hear a signal above and below the receive offset frequency
 *   as you tune past the signal.
 *
 * if the RX_OFFSET value is negative and you are tuning up the band, you will first hear a signal going down in
 *   pitch as you tune.  Then the pitch will become too low to hear.
 *   Then as you keep tuning, you will hear the same signal again and it will be going up in pitch as you tune.
 * if the RX_OFFSET is negative you will need to use the first signal (pitch going down as you tune up) in order
 *   for them to hear you when you reply.  If you tune to the wrong signal, you will be about 1kHz away when you
 *   transmit.
 * if the RX OFFSET is positive you will need to use the second signal (pitch going up as you tune up)
 * This software has a menu option to allow you to selected between a positive and negative offset at runtime.
 * This setting does not persist in the EEPROM at this time.  So, restarting the radio will set it back
 *   to this setting here.
 */
#define RX_OFFSET                 -520

/**
 * Tuning limit for the RIT.  
 * Rit will tune + and - the main frequency by this amount
 */
#define RIT_MAX_OFFSET_HZ         1500

/**
 * Inrement to use when knob is turning slow
 */
#define TUNING_INCREMENT_SLOW     10       

/**
 * Increment to use when knob is turning medium speed
 */
#define TUNING_INCREMENT_MEDIUM   100

/**
 * Increment to use when knob is turning fast
 */
#define TUNING_INCREMENT_FAST     1000

/**
 * Threshold between medium and slow knob turn event (milliseconds)
 */
#define TUNING_THRESHOLD_MEDIUM   190

/**
 * Threshold between medium and fast knob turn event (milliseconds)
 */
#define TUNING_THRESHOLD_FAST     90

/**
 * Increment to use when adjusting RIT
 */
#define RIT_ADJUST_INCREMENT      10

// DDS related defines

/**
 * DDS Tuning Word.
 * Use this to fine tune the frequency of your DDS.
 * Don't bother adding digits to the right.  They will be lost due to datatypes in use
 */
#define DDS_TUNING_WORD           34.359900

/**
 * Pin on the Arduino connected to the Word load clock pin on the DDS
 */
#define DDS_WORD_LOAD_CLOCK_PIN   8

/**
 * Pin on the Arduino connected to the frequency update pin on the DDS
 */
#define DDS_FREQUENCY_UPDATE_PIN  9

/**
 * Pin on the Arduino connected to the data pin on the DDS
 */
#define DDS_DATA_PIN              10

/** 
 * Pin on the Arduino connected to the reset pin on the DDS
 */
#define DDS_RESET_PIN             11

/**
 * Arduino pin listening for CW key presses.
 * You have to have done the rxOffset modification for this to be relevant
 */
#define KEY_IN_PIN                7

/**
 * Arduino pin directing the 49er to transmit.
 * You have to have done the rxOffset modification for this to be relevant
 */
#define KEY_OUT_PIN               13

/**
 * Text for splash screen (line 1)
 */
#define SPLASH_LINE_1             "40M Pieces-Parts"

/**
 * Text for splash screen (line 2)
 */
#define SPLASH_LINE_2             "   Transceiver  "

/**
 * Number of milliseconds to display the splash screen
 */
#define SPLASH_DELAY              4000

/**
 * Error text for cw key being depressed on rig startup
 */
#define STUCK_KEY_TEXT            "Err: Check Key  "

// This determines the minimul interval between writes of data to the EEPROM.
// This was initially set to 60 seconds when this source code was released.
// Be careful about reducing this number as there is a finite number of writes you
//   can do to EEPROM memory before it fails.
// This app only writes to the EEPROM if the data to be written actually changes 
//   from what is already there.
//

/**
 * Minimum milliseconds between writes to EEPROM.
 * App should only write if a value actually changes to preserve writes
 */
#define MIN_EPROM_WRITE_INTERVAL  60000

/**
 * Maximum length of text sent to the display
 */
#define TEXT_FOR_DISPLAY_MAX_LENGTH 16

/**
 * Number of frequencies in frequency array in the eventData class
 */
#define EVENT_DATA_FREQUENCY_FIELDS_COUNT 1

/**
 * Number of 32 bit integer fields in long array in the eventData class
 */
#define EVENT_DATA_LONG_FIELDS_COUNT 1

/**
 * Number of bool fields in bool array in the eventData class
 */
#define EVENT_DATA_BOOL_FIELDS_COUNT 2

/**
 * Maximum number of menu items.
 * If menu items are added, this number must be increased.
 */
#define MAX_MENU_ITEMS 3

// The following are enums (Enumerations)
// Rather than just having constants to represent the state of things, I am using enums.
// 
// This helps to prevent assigning invalid values to a variable representing the state of something.
// If the enum only has values 0 and 1 (most of these do) You cannot assign 12 to a variable of the enum's type
// It wont compile or it should throw an error if it happens at runtime.
//

/**
 * LCDDisplayLine enum.
 * Keeps us from trying to use a line that does not exist
 */
enum class LCDDisplayLine : int8_t
{
	FIRST_LINE = 0, /**< first line */
	SECOND_LINE = 1  /**< second line */
};

/**
 * RitStatus enum.
 */
enum class RitStatus : int8_t 
{
	DISABLED = 0,  /**< disabled */
	ENABLED        /**< enabled */
};
		
/**
 * Backlight status enum.
 */
enum class BacklightStatus : int8_t
{
	DISABLED = 0,   /**< disabled */
	ENABLED         /**< enabled */
};

/**
 * Position where an item is stored in the EEPROM memory
 */
enum class EEPROMValueIndex : uint8_t {
	OPERATING_FREQUENCY = 0
};

/** 
 * EventType enum.
 * All messages will have an eventCode that corresponds to a numerical value of this enum. 
 */
enum class EventType : int16_t 
{
	// Here, values start at 200 and increment by 1 for each following item.
	// So, next item is 201
	// 200 wasn't picked for a particular reason.  I could have used 1 instead.
	// I just followed the EventHandler author's example code
	//
	VFO_KNOB_TURNED= 200,
	RIT_KNOB_TURNED,
	MENU_KNOB_TURNED,
	MENU_ITEM_KNOB_TURNED,
	MAIN_KNOB_MODE_CHANGED,
	MENU_ITEM_SELECTED,
	MENU_ITEM_VALUE_CHANGED,
	FREQUENCY_CHANGED,
	RIT_CHANGED,
	RIT_STATUS_CHANGED,
	BACKLIGHT_STATUS_CHANGED,
	KEY_LINE_CHANGED,
	RX_OFFSET_DIRECTION_CHANGED,
	ERROR_OCCURRED
};

/**
 * Error type enum
 */
enum class ErrorType : int8_t
{
	STUCK_KEY=0
};
		
/**
 * ButtonPressType enum
 */
enum class ButtonPressType : int8_t 
{
	NONE = 0,
	SHORT,
	LONG
};

/**
 * FrequencyCompareResult enum
 */
enum class FrequencyCompareResult : int8_t
{
	LESS_THAN = -1,
	EQUAL,
	GREATER_THAN
};

/**
 * KnobTurnDirection enum
 */
enum class KnobTurnDirection : int8_t 
{
	CLOCKWISE = 1,
	NONE = 0,
	COUNTER_CLOCKWISE = -1
};

/**
 * MainKnobMode enum
 */
enum class MainKnobMode : int8_t 
{
	VFO = 1,
	RIT,
	MAIN_MENU,
	MENU_ITEM
};
	
/**
 * EventTextFieldEnum
 */
enum class EventTextField : int8_t 
{
	MESSAGE = 0,
	MENU_ITEM_NAME
};

/**
 * EventLongField enum
 */
enum class EventLongField : int8_t
{
	RIT_OFFSET = 0
};

/**
 * EventBoolField enum
 */
enum class EventBoolField : int8_t
{
	RIT_IS_ENABLED = 0,
	RX_OFFSET_IS_POSITIVE
};

/**
 * EventFrequencyField enum
 */
enum class EventFrequencyField : int8_t 
{
	OPERATING_FREQUENCY = 0
};

/**
 * RxOffsetDirection enum
 */
enum class RxOffsetDirection : int8_t
{
	BELOW = 0,
	ABOVE
};

/**
 * RxTxStatus enum
 */
enum class RxTxStatus : int8_t 
{
	RX = 0,
	TX = 1
};
	
/**
 * KeyStatus enum
 */
enum class KeyStatus : int8_t 
{
	PRESSED = 0,
	RELEASED = 1
};
#endif