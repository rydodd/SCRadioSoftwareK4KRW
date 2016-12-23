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
 * @version 1.0.3  12/22/2016.
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
 * Arduino pin listening for cw jack 'tip' signal
 * (This is usually the left 'dit' paddle or the cw key)
 * You have to have done the rxOffset modification for this to be relevant
 * 
 * IMPORTANT - You cannot reverse these pins if your paddle is not wired 
 * to this standard.  The straight key mode will not work if you do.
 * There are comments in the SCRadioKeyer.cpp file explaining what to change to
 * reverse the 'dit' and 'dah' paddles.
 */
#define CW_KEY_PADDLE_JACK_TIP_PIN 7

/**
 * Arduino pin listening for cw jack 'ring' signal
 * (This is usually the right 'dah' paddle)
 * This requires an additional key line to be added to the hardware
 * and routed to the specified pin.
 */
#define CW_KEY_PADDLE_JACK_RING_PIN 6

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
#define SPLASH_LINE_2             "Xcvr K4KRW 1.0.2"

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
#define MAX_MENU_ITEMS 6

/**
 * Maximum number of choices each name value menu item can have.
 */
#define MAX_NAME_VALUE_CHOICES 3

/**
 * Arduino Pin to use to read rig voltage
 */
#define RIG_VOLTAGE_READ_PIN A7

/**
 * VOLTAGE_CALC_MULTIPLIER
 *
 * Number multiplied by voltage read on analog pin 7 to get voltage being 
 * supplied to 49er.
 * 
 * So, where does this number come from?
 * 
 * The Nano can read the voltage on an analog pin.  But, it does not read it
 * directly and it can only read voltages from 0 to 5 Volts.
 * 
 * 0 Volts is 0 and 5 Volts is 1023.  So, if you are dealing with 0 to 5 Volts
 * you can multiple the value you read from the pin by 0.049 to get the
 * actual voltage.
 * 
 * In the rig, we are using 12 Volts.  This cannot be read directly by the
 * Arduino.  So the VFO circuit has an adjustable voltage divider circuit
 * that is used to bring the 12 Volts down to an acceptable range for
 * sampling by the Arduino.
 * 
 * So, as long as the voltage divider keeps the actual voltage being read under
 * 5V we can then compare the known voltage going into the rig with 
 * what is read by the Arduino and come up with a multiplier to give us
 * the actual voltage.
 * 
 * This number is a bit arbitrary as Hank mentions in his comments.  So, you can
 * use this or something close to it and can then use R8 to fine tune the
 * voltage displayed by the rig to match a known actual supply voltage.
 * 
 * In this app's logic, both the 5/1023 and the voltage divider reversal
 * are represented in this constant.
 * 
 * So, my Arduino was returning 739 when reading the analog pin.
 * 739 * 0.171070  becomes about 126.  This is 12.6 * 10.  
 * I handle the voltage as an integer value multiplied by 10 up until the 
 * time I display it because I can only handle an integer in my messaging 
 * scheme.  I then devide this value by 10 before I display it so we get 
 * 12.6V.
 * 
 * Again, the exact value displayed depends on how you adjust R8.  So, adjust
 * R8 until the display is accurate and you are done.
 */
#define RIG_VOLTAGE_CALC_MULTIPLIER 0.171070

/**
 * Check voltage after this many loops
 */
#define LOOP_COUNT_BETWEEN_RIG_VOLTAGE_READS 30000

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
	OPERATING_FREQUENCY = 0,
	KEYER_MODE = 1,
	KEYER_SPEED = 2,
	PADDLES_ORIENTATION = 3
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
	RIT_MENU_ITEM_VALUE_CHANGED,
	BACKLIGHT_MENU_ITEM_VALUE_CHANGED,
	KEY_LINE_CHANGED,
	RX_OFFSET_DIRECTION_MENU_ITEM_VALUE_CHANGED,
	ERROR_OCCURRED,
	RIT_STATUS_EXTERNALLY_CHANGED,
	RIG_VOLTAGE_CHANGED,
	KEYER_MODE_CHANGED,
	KEYER_SPEED_CHANGED,
	PADDLES_ORIENTATION_CHANGED
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
 * KeyerMode enum
 */
enum class KeyerMode : int8_t
{
	STRAIGHT_KEY = 0,
	IAMBICB = 1,
	IAMBICA = 2
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

enum class PaddlesOrientation : int8_t
{
	NORMAL = 0,
	REVERSED
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