/*
 * SCRadioKeyer.h - Class for controlling radio's CW Keyer
 *
 * Class for implementing an iambic cw keyer.
 * 
 * Based on OpenQRP Blog Iambic Morse Code Keyer Sketch by Steven T. Elliott, 
 * K1EL
 * 
 * Modified by Glen Popiel, KW5GP
 * 
 * Further modified for the SCRadioSoftwareK4KRW project by Richard Y. Dodd, 
 * K4KRW
 * 12/19/2016
 * 
 * Based on OpenQRP Blog Iambic Morse Code Keyer Sketch by Steven T. Elliott, 
 * K1EL
 */

///////////////////////////////////////////////////////////////////////////////
//
//  Iambic Morse Code Keyer Sketch
//  Copyright (c) 2009 Steven T. Elliott
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details:
//
//  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
//  Boston, MA  02111-1307  USA
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SCRadioKeyer_h
#define SCRadioKeyer_h

///////////////////////////////////////////////////////////////////////////////
//  keyerControl bit definitions
//
#define     DIT_LATCH_BIT  0x01     // Dit latch
#define     DAH_LATCH_BIT  0x02     // Dah latch
#define     DIT_PROC_BIT   0x04     // Dit is being processed
#define     PDLSWAP_BIT    0x08     // 0 for normal, 1 for swap
#define     IAMBICB_BIT    0x10     // 0 for Iambic A, 1 for B   (Not Used)
#define     ULTIMATIC_BIT  0x20     // 1 for ultimatic           (Not used)
#define     STRAIGHT_BIT   0x80     // 1 for straight key mode   (Not Used)
//////////////////////////////////////////////////////////////////////////////

#define		DAH_TIMING_MULTIPLE 3

/**
 * CW Keyer states enum
 */
enum class KeyerState : int8_t
{
	IDLE = 0,
	CHK_DIT,
	CHK_DAH,
	KEYED_PREP,
	KEYED,
	INTER_ELEMENT
};


class SCRadioKeyer
{
private:
	/**
	 * Used to queue events to key and unkey transmitter
	 */
	EventManager	&_eventManager;

	/**
	 * Used to read event related data when an event message
	 * is received (like keyer speed changed).
	 */
	SCRadioEventData &_eventData;

	/**
	 * Holds keyer words per minute setting
	 */
	int8_t			_keyerWPM;	// variable for keying speed

	/**
	 * Holds keyer current mode (straight key, iambic a, iambic b)
	 */
	KeyerMode		_keyerMode; // variable for keying mode

	/**
	 * Number of milliseconds per dit
	 */
	uint32_t		_ditTime;

	/**
	 * additional keyer configuration data
	 */
	uint8_t			_keyerControl;

	/**
	 * Current keyer state
	 */
	KeyerState		_keyerState;

	/**
	 * keyer timer value for current element
	 */
	uint32_t		_keyerTimer;

	/**
	* Indicates whether the rig has successfully passed the stuck key check
	* on starup.
	*/
	bool			_stuckKeyCheckPassed;

	/**
	* Indicates whether the stuck key state is currently being experienced
	*/
	bool			_inStuckKeyErrorState;

public:
	/**
	 * SCRadioKeyer
	 * 
	 * @detail 
	 *   Constructor for class
	 * 
	 * @param[in] eventManager Reference to EventManager
	 * @param[in] eventData    Reference to EventData
	 */
	SCRadioKeyer(EventManager &eventManager, SCRadioEventData &eventData);

	/**
	 * begin
	 * 
	 * @detail
	 *   Initializes class so it is ready for use.
	 */
	void begin();

	/**
	 * loop
	 * 
	 * @detail
	 *   Should be called each time the main application loop executes.
	 */
	void loop();

	/**
	 * keyerModeChangedListener
	 * 
	 * @detail
	 *   Listens for changes in the keyer's mode setting
	 * 
	 * @param[in] eventCode Identifies event message type
	 * @param[in] whichMenuItem Identifies menu item where message originated
	 */
	void keyerModeChangedListener(int eventCode, int whichMenuItem);

	/**
	 * keyerSpeedChangedListener
	 * 
	 * @detail
	 *   Listens for changes in the keyer's speed setting
	 * 
	 * @param[in] eventCode Identifies event message type
	 * @param[in] whichMenuItem Identifies menu item where message originated
	 */
	void keyerSpeedChangedListener(int eventCode, int whichMenuItem);

	/**
	* keyerPaddlesOrientationChangedListener
	*
	* @detail
	*   Listens for changes in the keyer paddles orientation setting
	*
	* @param[in] eventCode Identifies event message type
	* @param[in] whichMenuItem Identifies menu item where message originated
	*/
	void keyerPaddlesOrientationChangedListener(int eventCode, int whichMenuItem);

	/**
	 * setKeyerMode
	 * 
	 * @detail
	 *   Sets the keyer mode
	 * 
	 * @param[in] keyerMode Keyer mode
	 */
	void setKeyerMode(KeyerMode keyerMode);

	/**
	 * setKeyerWPM
	 * 
	 * @detail
	 *   Sets the keyer speed (words per minute) setting
	 * 
	 * @param[in] keyerWPM Keyer words per minute setting
	 */
	void setKeyerWPM(int8_t keyerWPM);

	/**
	 * setPaddlesOrientation
	 * 
	 * @detail
	 *   Allows you to specify whether paddles are reversed.
	 * 
	 * @param orientation Paddle orientation (reversed, normal)
	 */
	void setPaddlesOrientation(PaddlesOrientation orientation);

private:
	/**
	 * In steps where a follow on dit or dah is required, this 
	 * method commits to sending the next element by setting
	 * the approprate latch bit in the keyer control variable.
	 * 
	 * Is cleared in later steps as items are sent.
	 * 
	 * ex:
	 * Between elements and paddle is still held down.
	 * In iambic B and paddles squeezed while an element i being sent.
	 */
	void updatePaddleLatch();

	/**
	 * Sets the timer setting to support the specified
	 * words per minute value.
	 */
	void loadWPM(int8_t wpm);
};

#endif